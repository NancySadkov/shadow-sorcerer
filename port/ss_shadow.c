/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Native replacement for SHADOW.COM's INT F8h services.

  The service bodies follow src/shadow/services.c, but the EGA register
  sequences are resolved into their visible effect: the planar sprite banks
  are decoded and blitted with transparency, text is drawn from the driver's
  own 8x8 font, and the page services keep their segment semantics.
*/

#include "names.h"
#include "ids.h"
#include "shadow.h"
#include "game.h"
#include "structs.h"

#include "ss_platform.h"
#include "ss_ega.h"
#include "ss_shadow_table.h"

#include <SDL3/SDL.h>

char g_shadow_prefix = 'v';                 /* VGA asset set, like SS.BAT */
uint16 g_svc_pos;                           /* 0x9F1A */
uint16 g_mouse_x;                           /* 0xA9C2 */
uint16 g_mouse_y;                           /* 0xA9C4 */
uint16 g_mouse_button;                      /* 0xA990 */
uint16 g_overlay_segment;                   /* 0x9F2A */

static void cursor_invalidate(void);
static void cursor_erase(void);

/* ---- resident driver state (was CS:0330..) ---- */
static uint8  sd_page_toggle;               /* 0x330 */
static uint16 sd_saved_display = 0xA000;    /* 0x332 */
static uint16 sd_display_seg = 0xA000;      /* 0x334 */
static uint16 sd_back_seg = 0xA000;         /* 0x336 */
static uint16 sd_page0_seg = 0xA000;        /* 0x338 */
static uint16 sd_page1_seg = 0xA200;        /* 0x33A */
static uint16 sd_frame_count;               /* 0x33E */
static uint16 sd_sprite_w, sd_sprite_h;     /* 0x36B/0x36D */
static uint16 sd_defer_cx, sd_defer_dy;     /* 0x382/0x384 */
static uint16 sd_defer_pending;             /* 0x386 */
static uint8  sd_defer_h;                   /* 0x388 */
static const uint8 *sd_defer_src;           /* 0x37E/0x380 (ds:si) */

/* Set by a SHADOW blit flush, serviced at the next input poll. */
static uint8 ss_present_pending;

/* Saved strip for the deferred cursor: 5 EGA bytes per row = 40 pixels. */
static uint8 ss_defer_save[SS_SCREEN_H][40];

static uint8 *disp_page(void)
{
	return ss_page(sd_display_seg);
}

void ss_blit_indexed_screen(const unsigned char *pixels)
{
	if (pixels != NULL) {
		memcpy(disp_page(), pixels, SS_SCREEN_W * SS_SCREEN_H);
	}
}

/* ------------------------------------------------------------------ */
/* sprites                                                            */
/* ------------------------------------------------------------------ */

/* Planar sprite: `units` 16-pixel columns (1 = selectors 3, 2 = selector 4). */
static void blit_planar(int16 x, int16 y, int16 h, const uint8 *src, int16 units)
{
	int16 row_plane = (int16)(units * 2);
	int16 row_bytes = (int16)(units * 8);
	int16 w = (int16)(units * 16);
	int16 row;
	uint8 *dst = disp_page();

	if (h <= 0) {
		return;
	}
	for (row = 0; row < h; row++) {
		int16 py = (int16)(y + row);
		const uint8 *rp = src + (int32)row * row_bytes;
		int16 px;

		if (py < 0 || py >= SS_SCREEN_H) {
			continue;
		}
		for (px = 0; px < w; px++) {
			int16 x0 = (int16)(x + px);
			uint8 v = 0;
			int16 pl;
			int16 byte, bit;

			if (x0 < 0 || x0 >= SS_SCREEN_W) {
				continue;
			}
			byte = (int16)(px >> 3);
			bit = (int16)(7 - (px & 7));
			for (pl = 0; pl < 4; pl++) {
				if (((rp[pl * row_plane + byte] >> bit) & 1) != 0) {
					v = (uint8)(v | (1 << pl));
				}
			}
			if (v != 0) {
				dst[py * SS_SCREEN_W + x0] = v;
			}
		}
	}
}

static void svc03(int16 x, int16 y, int16 h, const uint8 *src)
{
	blit_planar(x, y, h, src, 1);
}

static void svc04(int16 x, int16 y, int16 h, const uint8 *src)
{
	blit_planar(x, y, h, src, 2);
}

/* Block sprite: width units of 8 bytes per row, 16 pixels per unit. */
static void blit_block_impl(int16 col, int16 row, const uint8 *src, uint8 mode)
{
	uint16 width, height;
	uint8 *dst = disp_page();
	uint16 y;

	width = (uint16)(src[0] | ((uint16)src[1] << 8));
	height = (uint16)(src[2] | ((uint16)src[3] << 8));
	src += 4;
	sd_sprite_w = width;
	sd_sprite_h = height;

	for (y = 0; y < height; y++) {
		int16 py = (int16)(row + y);
		uint16 u;

		for (u = 0; u < width; u++) {
			const uint8 *unit = src + ((uint32)y * width + u) * 8;
			uint16 words[4];
			int16 pl;

			for (pl = 0; pl < 4; pl++) {
				words[pl] = (uint16)(unit[pl * 2] | ((uint16)unit[pl * 2 + 1] << 8));
			}
			{
				int16 px;
				for (px = 0; px < 16; px++) {
					int16 x0 = (int16)(col + u * 16 + px);
					int16 bit = (int16)(15 - px);
					uint8 v = 0;

					if (py < 0 || py >= SS_SCREEN_H)
						continue;
					if (x0 < 0 || x0 >= SS_SCREEN_W)
						continue;
					for (pl = 0; pl < 4; pl++) {
						if (((words[pl] >> bit) & 1) != 0) {
							v = (uint8)(v | (1 << pl));
						}
					}
					/* mode 1 is the masked (transparent) blit; mode 0
					   writes the whole 16-pixel cell. */
					if ((mode & 1) != 0) {
						if (v != 0) {
							dst[py * SS_SCREEN_W + x0] = v;
						}
					} else {
						dst[py * SS_SCREEN_W + x0] = v;
					}
				}
			}
		}
	}
}

int16 sprite_draw_block(int16 sprite, int16 mode, int16 column, int16 row,
                        const uint8 *src)
{
	(void)sprite;
	/* `column` is an EGA byte column (8 pixels), `row` a scan line. */
	blit_block_impl((int16)((column & 0x00FF) * 8), (int16)(row & 0x00FF), src,
	                (uint8)(mode & 0xFF));
	return 0;
}

int16 sprite_draw_current(int16 sprite, int16 mode, int16 column, int16 row)
{
	uint16 seg = g_overlay_segment;
	uint16 *tbl = SS_PTR(uint16, seg, (uint16)(sprite * 2));
	uint16 off = *tbl;

	return sprite_draw_block(sprite, mode, column, row,
	                         SS_PTR(uint8, seg, off));
}

/* ------------------------------------------------------------------ */
/* text                                                               */
/* ------------------------------------------------------------------ */

static void svc_text(uint16 colour, int16 col, int16 row, const char *s)
{
	int16 px = (int16)((col & 0xFF) * 8);
	int16 py = (int16)(row & 0xFF);
	int16 start_x = px;
	uint8 fg = (uint8)(colour & 0xFF);
	uint8 bg = (uint8)(colour >> 8);
	uint8 *dst = disp_page();
	int32 limit;

	if (fg > 0x0F) {
		fg = 0x0F;
	}
	if (s == 0) {
		return;
	}
	for (limit = 0; limit < 4096; limit++) {
		uint8 ch = (uint8)*s++;
		int16 i;

		if (ch == 0)
			break;
		if (ch == 0x0D) {
			py = (int16)(py + 8);
			px = start_x;
			continue;
		}
		{
			int16 gi;
			const uint8 *glyph;

			if (ch >= 0x20 && ch < 0x80) {
				gi = (int16)(ch - 0x20);
			} else if (ch < 0x20) {
				gi = 0;                       /* blank for control codes */
			} else {
				gi = '?' - 0x20;              /* no CP437 glyphs in the font */
			}
			glyph = ss_font + gi * 8;
			for (i = 0; i < 8; i++) {
				uint8 bits = glyph[i];
				int16 yy = (int16)(py + i);
				int16 b;

				if (yy < 0 || yy >= SS_SCREEN_H)
					continue;
				for (b = 0; b < 8; b++) {
					int16 xx = (int16)(px + b);
					if (xx < 0 || xx >= SS_SCREEN_W)
						continue;
					if ((bits & (0x80 >> b)) != 0) {
						dst[yy * SS_SCREEN_W + xx] = fg;
					} else if (bg != 0xFF) {
						dst[yy * SS_SCREEN_W + xx] = (uint8)(bg & 0x0F);
					}
				}
			}
		}
		px = (int16)(px + 8);
	}
}

void shadow_text(int16 colour, int16 col, int16 row, const char *text)
{
	cursor_invalidate();
	svc_text((uint16)colour, col, row, text);
}

int16 shadow_text_draw(int16 attr, int16 col, int16 row, const char *text)
{
	cursor_invalidate();
	svc_text((uint16)attr, col, row, text);
	return 0;
}

/* ------------------------------------------------------------------ */
/* pages                                                              */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* pages                                                              */
/* ------------------------------------------------------------------ */

static void render_deferred(void);
static void cursor_erase(void);

int16 shadow_video_copy(uint16 lines)
{
	/* c_svc00 is not a copy: with EGA write mode 1 the overlapping movsw
	   propagates page0[0] (just zeroed) across the page, then page0 ->
	   page1 spreads the same zeros.  Net effect: clear both buffers. */
	uint32 rows = (uint32)(lines & 0xFFu);

	if (rows > SS_SCREEN_H) {
		rows = SS_SCREEN_H;
	}
	memset(ss_page(sd_page0_seg), 0, rows * SS_SCREEN_W);
	memset(ss_page(sd_page1_seg), 0, rows * SS_SCREEN_W);
	return 0;
}

static void do_page_flip(void)
{
	sd_page_toggle ^= 1;
	if (sd_page_toggle != 0) {
		sd_display_seg = sd_page0_seg;
		sd_back_seg = sd_page1_seg;
	} else {
		sd_display_seg = sd_page1_seg;
		sd_back_seg = sd_page0_seg;
	}
	sd_frame_count++;
	render_deferred();
}

int16 shadow_page_flip(void)
{
	do_page_flip();
	/* sd_A000.. is the draw target; sd_back_seg is the page the CRTC shows
	   after the flip, and the one the deferred cursor is drawn into. */
	ss_ega_compose(sd_back_seg);
	ss_present();
	ss_present_pending = 0;
	ss_wait_frame();
	return 0;
}

int16 shadow_video_page(int16 page)
{
	if (page < 0) {
		sd_display_seg = sd_saved_display;
		sd_page0_seg = 0xA000;
		sd_page1_seg = 0xA200;
		sd_defer_cx = g_mouse_x;
		sd_defer_dy = g_mouse_y;
		return 0;
	}
	sd_saved_display = sd_display_seg;
	if (page == 0) {
		sd_display_seg = sd_page0_seg;
		if ((sd_page_toggle & 1) != 0)
			sd_display_seg = sd_page1_seg;
		sd_back_seg = sd_display_seg;
		return 0;
	}
	sd_display_seg = (uint16)(0xA000 + page * 0x200);
	sd_page0_seg = sd_display_seg;
	sd_page1_seg = sd_display_seg;
	return 0;
}

int16 shadow_screen_restore(uint16 selector)
{
	/* c_svc1F copies the saved page into the display page. */
	ss_vram_copy(sd_display_seg, (uint16)(0xA000 + selector * 0x200), 8000);
	return 0;
}

void ss_restore_static_rect(int16 x, int16 y, int16 w, int16 h)
{
	uint8 *dst;
	const uint8 *src;
	int16 i;

	cursor_erase();
	dst = ss_page(sd_display_seg);
	src = ss_page(0xAE00);		/* page 7: saved static scene */
	if (x < 0) {
		w = (int16)(w + x);
		x = 0;
	}
	if (y < 0) {
		h = (int16)(h + y);
		y = 0;
	}
	if (x + w > SS_SCREEN_W)
		w = (int16)(SS_SCREEN_W - x);
	if (y + h > SS_SCREEN_H)
		h = (int16)(SS_SCREEN_H - y);
	if (w <= 0 || h <= 0)
		return;
	for (i = 0; i < h; i++) {
		int32 row = (int32)(y + i) * SS_SCREEN_W + x;
		memcpy(dst + row, src + row, (size_t)w);
	}
	ss_present_pending = 1;
}

/* Copy a rectangle out of the static map page into a buffer with the given
   row pitch (used to remember what the corner tiles look like before a
   ground-item icon is drawn over them). */
void ss_read_static_rect(uint8 *dst, int16 pitch,
                         int16 x, int16 y, int16 w, int16 h)
{
	const uint8 *src = ss_page(0xAE00);
	int16 i;

	for (i = 0; i < h; i++) {
		int16 yy = (int16)(y + i);
		if (yy < 0 || yy >= SS_SCREEN_H)
			continue;
		memcpy(dst + (int32)i * pitch,
		       src + (int32)yy * SS_SCREEN_W + x, (size_t)w);
	}
}

/* Put a saved rectangle back into the static map page, erasing whatever
   icon was drawn there. */
void ss_write_static_rect(const uint8 *src, int16 pitch,
                          int16 sx, int16 sy,
                          int16 x, int16 y, int16 w, int16 h)
{
	uint8 *dst = ss_page(0xAE00);
	int16 i;

	cursor_invalidate();
	for (i = 0; i < h; i++) {
		int16 yy = (int16)(y + i);
		if (yy < 0 || yy >= SS_SCREEN_H)
			continue;
		memcpy(dst + (int32)yy * SS_SCREEN_W + x,
		       src + (int32)(sy + i) * pitch + sx, (size_t)w);
	}
}

int16 shadow_screen_refresh(void)
{
	ss_ega_compose(sd_back_seg);
	ss_present();
	return 0;
}

void ss_force_present(void)
{
	ss_ega_compose(sd_back_seg);
	ss_present();
}

void ss_present_current_page(void)
{
	ss_ega_compose(sd_display_seg);
	ss_present();
}

/* ------------------------------------------------------------------ */
/* fills                                                              */
/* ------------------------------------------------------------------ */

int16 shadow_region_clear(int16 and_mask, int16 column, int16 row,
                          int16 width_bytes, int16 height_rows)
{
	int16 x0 = (int16)((column & 0xFF) * 8);
	int16 y0 = (int16)(row & 0xFF);
	int16 w = (int16)((width_bytes & 0xFF) * 8);
	int16 h = (int16)(height_rows & 0xFF);
	uint8 colour = (uint8)(and_mask & 0x0F);
	uint8 *dst;
	int16 y, x;

	cursor_invalidate();
	dst = disp_page();

	for (y = 0; y < h; y++) {
		int16 py = (int16)(y0 + y);
		if (py < 0 || py >= SS_SCREEN_H)
			continue;
		for (x = 0; x < w; x++) {
			int16 px = (int16)(x0 + x);
			if (px < 0 || px >= SS_SCREEN_W)
				continue;
			dst[py * SS_SCREEN_W + px] = colour;
		}
	}
	return 0;
}

int16 shadow_rect_fill(uint16 x, uint16 y, uint16 colour, uint16 rows)
{
	int16 px = (int16)x;
	int16 py = (int16)(y & 0xFF);
	int16 n = (int16)rows;
	uint8 c = (uint8)(colour & 0x0F);
	uint8 *dst;
	int16 i;

	cursor_invalidate();
	dst = disp_page();

	if (px < 0 || px >= SS_SCREEN_W)
		return 0;
	for (i = 0; i < n; i++) {
		int16 yy = (int16)(py + i);
		if (yy < 0 || yy >= SS_SCREEN_H)
			continue;
		dst[yy * SS_SCREEN_W + px] = c;
	}
	return 0;
}

/* The deferred sprite is the mouse cursor.  SHADOW saves the pixels it covers
   and redraws it at the latest pointer position on every page flip, so the
   cursor follows the mouse even when the game itself is not polling. */
static int ss_cursor_on;                    /* drawn, at ss_cursor_* */
static uint16 ss_cursor_seg;
static int16 ss_cursor_x, ss_cursor_y;
static uint8 ss_defer_rows;

static void cursor_invalidate(void)
{
	/* The game is about to write over the cursor's saved background.  Put
	   the old pixels back first (so the cursor does not leave a ghost) and
	   clear the saved-strip state; the next cursor draw captures the new
	   background.  The following draw services then paint over a clean
	   area, and a later SHADOW_BLIT_FLUSH cannot restore the stale strip
	   over a freshly drawn menu. */
	cursor_erase();
}

static void cursor_erase(void)
{
	int16 i;
	uint8 *page;

	if (!ss_cursor_on)
		return;
	page = ss_page(ss_cursor_seg);
	for (i = 0; i < ss_defer_rows; i++) {
		int32 row = (int32)(ss_cursor_y + i) * SS_SCREEN_W + ss_cursor_x;
		memcpy(page + row, ss_defer_save[i], 40);
	}
	ss_cursor_on = 0;
}

static void cursor_draw(void)
{
	int16 h, x, y, i, px0;
	uint8 *page;

	if (sd_defer_pending == 0)
		return;
	h = (int16)sd_defer_h;
	x = (int16)sd_defer_cx;
	y = (int16)sd_defer_dy;
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (y >= SS_SCREEN_H || h <= 0)
		return;
	if (SS_SCREEN_H - y < h)
		h = (int16)(SS_SCREEN_H - y);
	px0 = (int16)(x & ~7);
	if (px0 + 40 > SS_SCREEN_W)
		px0 = (int16)(SS_SCREEN_W - 40);
	if (px0 < 0)
		px0 = 0;

	page = ss_page(sd_back_seg);
	for (i = 0; i < h; i++) {
		int32 row = (int32)(y + i) * SS_SCREEN_W + px0;
		memcpy(ss_defer_save[i], page + row, 40);
	}
	ss_defer_rows = (uint8)h;
	ss_cursor_x = px0;
	ss_cursor_y = y;
	ss_cursor_seg = sd_back_seg;
	ss_cursor_on = 1;

	{
		uint16 saved = sd_display_seg;
		sd_display_seg = sd_back_seg;
		svc03(x, y, h, sd_defer_src);
		sd_display_seg = saved;
	}
}

static void render_deferred(void)
{
	cursor_draw();
}

/* Re-stamp the deferred pointer after the caller repainted the whole page.
   The saved strip from the previous frame is stale by then, so discarding it
   (rather than restoring it) avoids putting old pixels over the new content. */
void ss_cursor_restamp(void)
{
	if (sd_defer_pending == 0) {
		return;
	}
	ss_mouse_peek(&sd_defer_cx, &sd_defer_dy);
	cursor_draw();
	if (sd_back_seg == sd_display_seg) {
		ss_present_pending = 1;
	}
}

/* Put the page state back to the values main() has at startup.  The title
   screen leaves its own display/saved-page stack (and pointer strip) behind;
   a game loaded from the title needs the same starting page layout as a new
   game or its first page flips publish stale title art. */
void ss_reset_pages(void)
{
	sd_page_toggle = 0;
	sd_saved_display = 0xA000;
	sd_display_seg = 0xA000;
	sd_back_seg = 0xA000;
	sd_page0_seg = 0xA000;
	sd_page1_seg = 0xA200;
	sd_frame_count = 0;
	sd_defer_pending = 0;
	sd_defer_cx = 0;
	sd_defer_dy = 0;
	ss_present_pending = 0;
	ss_cursor_on = 0;
	ss_cursor_seg = 0;
	ss_cursor_x = 0;
	ss_cursor_y = 0;
	ss_defer_rows = 0;
}

/* Advance the mouse cursor independently of the game's 24 Hz logic tick.
   The game only polls the mouse on its own loop, so without this the pointer
   moves in 41 ms steps; the release's cursor was a hardware sprite and stayed
   real-time.  Called from the frame wait, where the game is not drawing into
   the page being displayed. */
void ss_cursor_tick(void)
{
	static Uint64 next;
	uint16 mx, my;
	Uint64 now = SDL_GetTicks();

	if (now < next) {
		return;
	}
	next = now + 16u;   /* 60 Hz pointer refresh */
	if (sd_defer_pending == 0) {
		return;
	}
	ss_mouse_peek(&mx, &my);
	if (!ss_cursor_on || (int16)(mx & ~7) != ss_cursor_x ||
	    (int16)my != ss_cursor_y) {
		cursor_erase();
		sd_defer_cx = mx;
		sd_defer_dy = my;
		cursor_draw();
	}
	/* svc1d draws the cursor but, in the double-buffered loop, only the
	   next page flip presents it.  Present here on every 16 ms tick so the
	   pointer is visible while the game is spinning on its counters. */
	ss_ega_compose(sd_back_seg);
	ss_present();
}

static void svc1d(int16 bx, int16 cx, int16 dy, const uint8 *src)
{
	if (bx < 0) {
		cursor_erase();
		sd_defer_pending = 0;
		sd_defer_cx = 0;
		sd_defer_dy = 0;
		return;
	}
	if ((bx & 0x4000) != 0) {
		/* c_svc1D restores the strip the cursor covers before the game
		   redraws that area.  Without this, the next cursor move restores
		   stale map pixels over a freshly drawn menu or text window. */
		cursor_erase();
		return;
	}
	cursor_erase();
	sd_defer_pending = 1;
	sd_defer_h = (uint8)bx;
	sd_defer_src = src;
	sd_defer_cx = (uint16)cx;
	sd_defer_dy = (uint16)dy;
	cursor_draw();
	/* The cursor was drawn into the displayed page.  When the game is
	   drawing directly to that page (menus and other modal screens),
	   publish it on the next input poll: those loops do not flip, so the
	   flush's present would otherwise land before the redraw.  In the
	   double-buffered main loop the next flip does this. */
	if (sd_back_seg == sd_display_seg)
		ss_present_pending = 1;
}

int16 shadow_video_flags(int16 flags)
{
	/* SHADOW's EGA drawing is visible immediately; the port renders into a
	   private page, so a flush becomes "present what has been drawn".  Defer
	   it to the next input poll, which is where the modal wait loops spend
	   their time, so the batch that follows the flush is on screen by then. */
	if ((flags & SHADOW_BLIT_FLUSH) != 0)
		ss_present_pending = 1;
	svc1d(flags, 0, 0, 0);
	return 0;
}

void shadow_sprite(int16 sel, int16 kind, int16 x, int16 y, const uint8 *src)
{
	cursor_invalidate();
	switch (sel) {
	case 3:
		svc03(x, y, (int16)(kind & 0xFF), src);
		break;
	case 4:
		svc04(x, y, (int16)(kind & 0xFF), src);
		break;
	case 0x1D:
		svc1d(kind, x, y, src);
		break;
	default:
		break;
	}
}

/* ------------------------------------------------------------------ */
/* public driver API                                                  */
/* ------------------------------------------------------------------ */

int32 shadow_load(const char *name, uint16 dest_para)
{
	char path[16];
	int i;

	/* The game's loader substitutes the asset prefix for the first letter. */
	for (i = 0; i < (int)sizeof(path) - 1 && name[i] != 0; i++) {
		path[i] = name[i];
	}
	path[i] = 0;
	path[0] = g_shadow_prefix;
	return ss_load_asset(path, dest_para);
}

int16 shadow_asset_load(char *name, uint16 seg)
{
	*name = g_shadow_prefix;
	return (int16)ss_load_asset(name, seg);
}

int16 shadow_poll(void)
{
	ss_pump();
	return ss_key_poll();
}

int16 shadow_key_read(void)
{
	return shadow_poll();
}

int16 shadow_getkey_noecho(void)
{
	int16 k;

	for (;;) {
		k = shadow_poll();
		if (k != 0)
			return (int16)(k & 0xFF);
	}
}

int16 shadow_config(void)
{
	return 0x21;                 /* VGA colour set / planar pixel order */
}

int16 shadow_set_palette(const uint8 *table)
{
	/* The palette-register remap has no meaning on a true-colour display;
	   the game's palette arrives through set_dac_palette(). */
	(void)table;
	ss_ega_set_palette_identity();
	return 0;
}

int16 shadow_direction_read(void)
{
	/* Same bit layout as the release driver: 1 = up, 2 = down, 4 = left,
	   8 = right (diagonals combine them).  GAME.EXE itself never asks for
	   this; the port uses ss_direction_step() for its keyboard walking. */
	int16 d = ss_direction_step();
	int16 v = 0;

	if (d < 0) {
		return 0;
	}
	switch (d) {
	case 0: v = 0x01; break;              /* up */
	case 1: v = 0x09; break;              /* up-right */
	case 2: v = 0x08; break;              /* right */
	case 3: v = 0x0A; break;              /* down-right */
	case 4: v = 0x02; break;              /* down */
	case 5: v = 0x06; break;              /* down-left */
	case 6: v = 0x04; break;              /* left */
	case 7: v = 0x05; break;              /* up-left */
	default: break;
	}
	return v;
}

int16 shadow_stream_read(uint16 offset)
{
	(void)offset;
	return 0;
}

int16 shadow_save(void)
{
	return 0;
}

void shadow_mouse_read(void)
{
	ss_mouse_read(&g_mouse_x, &g_mouse_y, &g_mouse_button);

	/* SEL 1Ch also publishes the position to the deferred sprite, which is
	   redrawn at every page flip; that is what keeps the cursor on the
	   pointer while the game is idle. */
	if (sd_defer_pending != 0) {
		sd_defer_cx = g_mouse_x;
		sd_defer_dy = g_mouse_y;
	}
	/* Site WAIT loops spend most of their time in the counter spin; keep
	   the hardware-style cursor moving there instead of waiting for the
	   next page flip. */
	ss_cursor_tick();
	if (ss_present_pending != 0) {
		ss_present_pending = 0;
		ss_ega_compose(sd_back_seg);
		ss_present();
	}
}

void shadow_mouse_draw(uint16 x, uint16 y)
{
	g_mouse_x = x;
	g_mouse_y = y;
	ss_set_mouse((int16)x, (int16)y);
}
