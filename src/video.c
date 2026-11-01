/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"

#include <dos.h>
#include <stdio.h>
#include "shadow.h"
#include "game.h"
#include "structs.h"

/* 0x1144 */

const uint8 g_palette_registers[17] = {
	0x00, 0x3b, 0x3d, 0x3f, 0x00, 0x04, 0x3c, 0x07, 0x38,
	0x03, 0x39, 0x3c, 0x06, 0x3a, 0x02, 0x3e, 0x00,
};

uint16 g_pal_rgb[16] = {           /* DGROUP 0x1122 */
	0x000, 0x410, 0x630, 0x777, 0x000, 0x222, 0x047, 0x555,
	0x333, 0x030, 0x040, 0x150, 0x500, 0x741, 0x753, 0x770
};

/* 0x9B81 */

void set_dac_palette(void)
{
	union REGS in, out;
	int16 i;

	for (i = 0; i != 16; i++) {
		uint16 w = g_pal_rgb[i];

		in.h.dh = (uint8)((uint16)w >> 8 << 3);   /* 9B90-9B96 */
		in.h.cl = (uint8)((w & 0x0f) << 3);         /* 9B98-9BA1 */
		in.h.ch = (uint8)((w & 0xf0) >> 1);         /* 9BA3-9BA8 */
		in.x.ax = BIOS_SET_DAC_REGISTER;
		in.x.bx = (uint16)i;
		int86(0x10, &in, &out);
		in.x.ax = BIOS_SET_BORDER_COLOR;
		in.x.bx = (uint16)((i << 8) | i);
		int86(0x10, &in, &out);
	}
}

/* 0x4693 */

char g_line_buffer[50];			/* 0xad40 - 0x32 */

const char g_tmpl_horiz[41] = { 0x23, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x00 };

const char g_tmpl_vert[48]  = { 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x0d, 0x7c, 0x00 };

/* 0x00C0 */

struct video_mode_config g_video_mode_config = {
	1,
	8,
	TEXT_PAGE_DEFAULT,
	PAGE_RECT_DEFAULT,
	TEXT_WINDOW_DEFAULT,
	TEXT_ATTR_DEFAULT,
	GAUGE_FILL_SHORT_DEFAULT,
	GAUGE_FILL_LONG_DEFAULT,
};

/* 0x4693 */

int16 draw_text_window(int16 x, int16 y, int16 w, int16 h)
{
	int16 height_px = h << 3;					/* 4699-46A0 */

	shadow_region_clear(g_page_rect, x, y, w, height_px);		/* 46A3-46B4 */
	x--;
	y -= 8;
	w++;

	text_copy(g_line_buffer, g_tmpl_horiz);			/* 46C1-46C9 */
	g_line_buffer[w] = 0x24;
	g_line_buffer[w + 1] = 0;
	if (y >= 8)
		shadow_text_draw(g_text_page, x, y, g_line_buffer);	/* 46E5-46F6 */

	g_line_buffer[w] = 0x26;
	g_line_buffer[0] = 0x25;
	shadow_text_draw(g_text_page, x, y + height_px + 8,
	         g_line_buffer);				/* 4706-471E */

	text_copy(g_line_buffer, g_tmpl_vert);			/* 4721-472C */
	g_line_buffer[h * 2] = 0;
	shadow_text_draw(g_text_page, x, y + 8, g_line_buffer);
	return shadow_text_draw(g_text_page, x + w, y + 8,
	                g_line_buffer);
}

/* 0xAE6E */

/* 0xAE6E */
void mark_cells_redraw(int16 xbase, int16 ybase, int16 count, uint8 *ofs)
{
	int16 i;
	int16 x, y;
	int16 keep;

	for (i = 0; i != count; i++) {
		x = xbase + (int8)*ofs++;		/* AE87-AE93 */
		y = ybase + (int8)*ofs++;		/* AE96-AEA2 */
		if (x < 0 || y < 0)
			continue;
		keep = 1;
		if (y > TERRAIN_HEIGHT)
			keep = 0;
		else if ((y & 1) != 0) {
			if (x >= TERRAIN_WIDTH)
				keep = 0;
		} else {
			if (x >= TERRAIN_WIDTH - 1)
				keep = 0;
		}
		if (keep)
			g_map_tile[x + TERRAIN_WIDTH * y] |= TILE_VISIBLE;		/* AEE2-AEF0 */
	}
}

/* 0xAEFC */

void draw_map_view(void)
{
	int16 ystart;
	int16 px, py;
	int16 step, col;
	int16 terrain, count;
	int16 sprite;
	uint8 *tp;

	shadow_video_page(SHADOW_PAGE_MAP);				/* AF03-AF0A */
	shadow_region_clear(MAP_VIEW_CLEAR_MASK, 0, SCREEN_TOP_MARGIN, SCREEN_WIDTH_BYTES, MAP_VIEW_HEIGHT);		/* AF10-AF23 */

	ystart = (int16)PARTY_REC(0)->grid_y - MAP_VIEW_ROWS_ABOVE;			/* AF26-AF2C */
	if (ystart < 0)
		ystart = 0;				/* AF2F-AF33 */
	if (ystart > TERRAIN_HEIGHT - MAP_VIEW_ROWS)
		ystart = TERRAIN_HEIGHT - MAP_VIEW_ROWS;				/* AF38-AF3E */

	px = (int16)PARTY_REC(0)->grid_x;
	py = (int16)PARTY_REC(0)->grid_y;
	terrain = g_map_tile[px + TERRAIN_WIDTH * py] & TERRAIN_MASK;	/* AF4F-AF61 */
	count = (terrain == 7) ? 0x13 : 7;		/* AF64-AF71 */
	if ((py & 1) != 0)
		mark_cells_redraw(px, py, count, (uint8 *)g_ofs_odd_ring);
							/* AF7A-AF88 */
	else
		mark_cells_redraw(px, py, count, (uint8 *)g_ofs_even_ring);
							/* AF8D-AF9A */

	tp = &g_map_tile[TERRAIN_WIDTH * ystart];			/* AFA0-AFA9 */
	py = 8;
	for (step = 0; step != MAP_VIEW_ROWS; step++, py += 0xc) {	/* AFAC-AFBF */
		if (ystart + step == TERRAIN_HEIGHT - 1)
			break;				/* AFC8-AFD3 */
		px = (((ystart + step) & 1) != 0) ? 0 : 8;	/* AFD6-AFE7 */
		for (col = 0; col != TERRAIN_WIDTH; col++, tp++, px += 0x10) {
							/* AFF1-AFFE */
			sprite = MAP_SPRITE_UNSEEN;
			terrain = *tp & TERRAIN_MASK;		/* B009-B011 */
			if (terrain == TERRAIN_DOLLAR && g_clock_hour != HOUR_20)
				terrain = 7;		/* B014-B020 */
			if ((*tp & TILE_VISIBLE) != 0) {
				sprite = MAP_SPRITE_TERRAIN_BASE + terrain;	/* B025-B030 */
			} else {
				if (terrain == 7 || terrain == 8)
					sprite = MAP_SPRITE_UNSEEN_WATER;	/* B035-B041 */
			}
			if (terrain != 0)
				sprite_draw_anchored(px, py, sprite);	/* B046-B055 */
		}
	}

	shadow_video_page(SHADOW_PAGE_PREVIOUS);				/* B060-B067 */
	draw_menubar();
}

int16   companion_react_tile(int16 ent, int16 ask, int16 kind);
uint16 g_state_countdown, g_overworld_phase;
uint8 g_map_mode_char;
uint16 g_pane_base_y, g_pane_base_x, g_companion_react_skip;
uint16 g_elistan_menu_choice, g_golmoon_menu_choice, g_raistlin_menu_choice, g_panel_busy, g_selected_hero_slot;
uint16 g_timer_debt_d, g_timer_debt_c, g_timer_debt_b, g_timer_debt_a;
uint16 g_sound_request, g_game_end_flag, g_reserved_b;

/* 0x5E1A */
/* 0x7807 */

/* 0x738E */
/* 0x74B6 */
/* 0x7501 */
/* 0x7670 */
/* 0x733D */
/* 0xA206 */

extern uint16 g_objpic_at;
void shadow_sprite(int16, int16, int16, int16, int16, uint16);

struct sprite_descriptor {
	uint16 width;
	uint16 height;

};

typedef char sprite_descriptor_size_check[(sizeof(struct sprite_descriptor) == 4) ? 1 : -1];

static struct sprite_descriptor far *sprite_descriptor_at(uint16 seg, uint16 id)
{
	uint16 far *index;
	struct sprite_descriptor far *desc;

	FP_SEG(index) = seg;
	FP_OFF(index) = 0;
	FP_SEG(desc) = seg;
	FP_OFF(desc) = index[id];
	return desc;
}

/* 0xA206 */

/* 0xA206 */
void sprite_draw_anchored(int16 px, int16 py, int16 kind)
{
	struct sprite_descriptor far *desc;
	uint16 id = (uint16)kind;
	int16 adj;

	if ((uint16)(id + id) == 0)
		return;

	desc = sprite_descriptor_at(g_objpic_at, id);
	adj = (int16)(desc->height - 0xf);
	if (adj >= 0)
		py = (int16)((uint16)py - (uint16)adj);

	shadow_sprite(3, (int16)(desc->height & 0xff), px, py,
	              (int16)(FP_OFF(desc) + 4), (uint16)g_objpic_at);	/* A22A-22D */
}

void map_record_show(int16 rec_no, int16 transition);
int16 save_load_panel(int16 mode);
extern int16 shadow_video_flags(int16);				/* 0x9CFA */
void text_resource_read(int16 a);					/* 0x4771 */
void text_measure(char *s, int16 *w, int16 *h);		/* 0x0E37 */
int16 shadow_text_draw(int16 attr, int16 col, int16 row, const char *text);	/* 0xD340 */
extern uint16 g_hold_length;				/* 0x9f26 */

void sprite_draw_dispatch(int16 a, int16 b, int16 c);		/* 0xA097 */
extern uint16 g_objpic_at;				/* 0xa9a8 */

void sprite_blit_pair(int16 pair, int16 kind, int16 x, int16 y, int16 data_off);

/* 0xA097 */

/* 0xA097 */
void sprite_draw_dispatch(int16 x, int16 y, int16 id)
{
	struct sprite_descriptor far *desc = sprite_descriptor_at(g_objpic_at, (uint16)id);

	sprite_blit_pair((((uint16)desc->width & 0xff) << 8) | ((uint16)desc->width >> 8),
	         (int16)(desc->height & 0xff), x, y, (int16)(FP_OFF(desc) + 4));
}

void shadow_sprite(int16 sel, int16 kind, int16 x, int16 y, int16 data_off, uint16 seg)
{
	union REGS r, noop;
	struct SREGS s, back;
	uint16 ds_here;

	segread(&s);
	ds_here = (uint16)s.ds;

	s.ds = seg;
	r.x.ax = (uint16)sel;
	r.x.bx = (uint16)kind;
	r.x.cx = (uint16)x;
	r.x.dx = (uint16)y;
	r.x.si = (uint16)data_off;

	r.x.di = (uint16)(0x28 * (uint16)(y & WORD_MASK) + (uint16)((x & PIXEL_COLUMN_MASK) >> 3));
	int86x(SHADOW_VECTOR, &r, &r, &s);

	segread(&back);
	back.ds = ds_here;
	noop.x.ax = 0; noop.x.bx = 0; noop.x.cx = 0; noop.x.dx = 0;
	noop.x.si = 0; noop.x.di = 0;
	int86x(0x28, &noop, &noop, &back);
}

extern uint8 g_pixel_order[256];		/* 0x9e16 */
extern int16 g_hw_config;				/* 0x9e14 */

void remap_group(int16 kind, int16 data_off, int16 grp)
{
	uint8 far *dst;
	uint8 far *src;
	uint16 n, i, j;

	FP_SEG(dst) = (uint16)g_mem_next;
	FP_OFF(dst) = 0;
	FP_SEG(src) = g_objpic_at;
	FP_OFF(src) = (uint16)data_off;

	n = (uint16)kind * 4;
	for (i = 0; i != n; i++)
		for (j = 0; j != (uint16)grp; j++)
			*(dst + i * (uint16)grp + j) = g_pixel_order[
				*(src + i * (uint16)grp + ((uint16)grp - 1 - j))];
}

/* 0x9EFD */
void sprite_expand_group4(int16 kind, int16 x, int16 y, int16 data_off)
{
	(void)x; (void)y;
	remap_group(kind, data_off, g_hw_config == HW_CONFIG_20 ? 8 : 4);
}

/* 0x9FAF */

void sprite_expand_group2(int16 kind, int16 x, int16 y, int16 data_off)
{
	(void)x; (void)y;
	remap_group(kind, data_off, g_hw_config == HW_CONFIG_20 ? 4 : 2);
}

/* 0x9E3B */

/* 0x9E3B */
void sprite_blit_pair(int16 pair, int16 kind, int16 x, int16 y, int16 data_off)
{
	int16 al = pair & 0xff;
	int16 idx = ((pair >> 8) & 0xff) - 1;

	if (al == 0)
	{
		switch (idx)
		{
		case SPRITE_PAIR_SINGLE_3:
			shadow_sprite(3, kind, x, y, data_off, (uint16)g_objpic_at);
			break;
		case SPRITE_PAIR_SINGLE_4:
			shadow_sprite(4, kind, x, y, data_off, (uint16)g_objpic_at);
			break;
		case SPRITE_PAIR_DRAW_ERASE:
			shadow_sprite(4, kind, x, y, data_off, (uint16)g_objpic_at);
			shadow_sprite(3, kind, x, y, data_off, (uint16)g_objpic_at);
			break;
		case SPRITE_PAIR_TWO_PART:
			shadow_sprite(4, kind, x, y, data_off, (uint16)g_objpic_at);
			shadow_sprite(4, kind, x + 0x20, y, data_off + kind * 16, (uint16)g_objpic_at);
			break;
		}
		return;
	}

	switch (idx)
	{
	case SPRITE_PAIR_EXPAND_2:
		sprite_expand_group2(kind, x, y, data_off);
		shadow_sprite(3, kind, x, y, data_off, (uint16)g_mem_next);
		break;
	case SPRITE_PAIR_EXPAND_4:
		sprite_expand_group4(kind, x, y, data_off);
		shadow_sprite(4, kind, x, y, data_off, (uint16)g_mem_next);
		break;
	default:
		sprite_expand_group4(kind, x, y, data_off);
		shadow_sprite(4, kind, x + 0x20, y, data_off, (uint16)g_mem_next);
		sprite_expand_group4(kind, x, y, data_off + kind * 16);
		shadow_sprite(4, kind, x, y, data_off + kind * 16, (uint16)g_mem_next);
		break;
	}
}

/* 0x4866 */

void wait_ticks(int16 a)
{
	int16 w, h;
	int16 x, y;

	shadow_video_page(SHADOW_PAGE_DEFAULT);
	text_resource_read(a);
	shadow_video_flags(SHADOW_BLIT_FLUSH);
	text_measure((char *)&g_credits, &w, &h);
	x = (((SCREEN_WIDTH - (w << 3)) >> 1) / 8);	/* 48A1-48B5 */
	y = ((SCREEN_HEIGHT - (h << 3)) >> 1);		/* 48BF-48CA */
	draw_text_window(x, y, w, h);
	shadow_text_draw(TEXT_COLOUR_NORMAL, x, y, (char *)&g_credits);
	g_shadow_tick_count = 0;
	while (g_hold_length * 5 > g_shadow_tick_count)		 			;
	shadow_video_page(SHADOW_PAGE_PREVIOUS);
}

/* 0x5EC9 */
/* 0xA061 */

/* 0xA02D */

/* 0xA02D */
void sprite_blit_center(int16 x, int16 y, int16 id)
{
	struct sprite_descriptor far *desc = sprite_descriptor_at(g_objpic_at, (uint16)id);

	sprite_blit_pair((((uint16)desc->width & 0xff) << 8) | ((uint16)desc->width >> 8),
	         (int16)(desc->height & 0xff),
	         (int16)((uint16)x - ((uint16)desc->width << 3)),
	         (int16)((uint16)y - (uint16)desc->height),
	         (int16)(FP_OFF(desc) + 4));
}

/* 0xA061 */
void sprite_blit_offset(int16 x, int16 y, int16 id)
{
	struct sprite_descriptor far *desc = sprite_descriptor_at(g_objpic_at, (uint16)id);

	sprite_blit_pair((((uint16)desc->width & 0xff) << 8) | ((uint16)desc->width >> 8),
	         (int16)(desc->height & 0xff),
	         (int16)((uint16)x - ((uint16)desc->width << 3)),
	         (int16)((uint16)y - ((uint16)desc->height >> 1)),
	         (int16)(FP_OFF(desc) + 4));
}  /* 0xA061 */

/* DGROUP 0x23a0 */

struct rect_rec g_hero_slots[20] = {
	{  3,    0,   8,  32,  32 },
	{  1,  288,   8,  32,  32 },
	{  0,    0, 168,  32,  32 },
	{  4,  288, 168,  32,  32 },
	{  2,    0,   0,   0,   0 },
	{  5,    0,   0,   0,   0 },
	{  6,    0,   0,   0,   0 },
	{  7,    0,   0,   0,   0 },
	{  8,    0,   0,   0,   0 },
	{  9,    0,   0,   0,   0 },
	{ 13,    0,   0,   0,   0 },
	{ 10,    0,   0,   0,   0 },
	{ 11,    0,   0,   0,   0 },
	{ 12,    0,   0,   0,   0 },
	{ 14,    0,   0,   0,   0 },
	{ 15,    0,   0,   0,   0 },
	{ 23,    0,   0,   0,   0 },
	{ 23,    0,   0,   0,   0 },
	{ 23,    0,   0,   0,   0 },
	{ 23,    0,   0,   0,   0 },
};
typedef char hero_slot_table_size_check[(sizeof(g_hero_slots) == 200) ? 1 : -1];

/* DGROUP 0x0bb2 */
struct rect_rec g_rect_panel_boxes[4] = {
	{ 31,   36,   8, 16, 16 },
	{ 31,  268,   8, 16, 16 },
	{ 31,   36, 184, 16, 16 },
	{ 31,  268, 184, 16, 16 },
};
typedef char rect_panel_boxes_size_check[(sizeof(g_rect_panel_boxes) == 40) ? 1 : -1];

/* DGROUP 0x0bda */

struct rect_rec g_hero_slot_boxes[4] = {
	{ 25,    0,  39, 16, 16 },
	{ 25,  304,  39, 16, 16 },
	{ 25,    0, 153, 16, 16 },
	{ 25,  304, 153, 16, 16 },
};
typedef char hero_slot_boxes_size_check[(sizeof(g_hero_slot_boxes) == 40) ? 1 : -1];

/* DGROUP 0x0c02 */

struct rect_rec g_panel_boxes[4] = {
	{ 0, 0, 0, 0x20, 0x10 },
	{ 1, 0, 0, 0x10, 0x08 },
	{ 2, 0, 0, 0x10, 0x08 },
	{ 3, 0, 0, 0x10, 0x08 },
};
typedef char panel_boxes_size_check[(sizeof(g_panel_boxes) == 40) ? 1 : -1];
uint8 g_terrain_grid[TERRAIN_CELLS] = {	/* DGROUP 0x13a6 */
	   6,    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
	   6,    6,    6,    0,    6,   15,    5,    5,    5,    5,    5,    5,    5,    6,    6,    7,
	   6,    1,    1,    1,    1,    1,    1,    6,    6,    1,    1,    4,    1,    1,    1,   11,
	   7,    6,   12,    6,    6,    6,    1,    6,    1,    8,    6,    0,    6,    4,    1,    4,
	  17,    1,    4,    5,    6,    7,    6,    7,    7,    1,    1,    6,    6,    6,    8,    1,
	   6,    8,    1,    1,    9,    1,    1,    5,    6,    7,    6,    6,    7,    1,    6,   18,
	   4,    6,    8,    0,    6,    8,    6,    1,    1,    4,    1,    5,    5,    5,    7,    7,
	  13,    1,    1,    1,    1,    6,    8,    6,    6,    8,    6,    1,    1,    1,    1,    1,
	   1,    5,    5,    6,    7,    1,    1,   19,    1,    6,    8,    0,    6,    1,    9,    1,
	   6,    1,    6,    6,    4,    5,    4,    6,    7,    7,   16,    1,    1,    6,    8,    6,
	   6,    6,    5,    6,   14,   20,    4,    6,    5,    4,    4,    6,    1,    1,    7,    5,
	   1,    6,    8,    0,    6,   21,    5,    5,    6,    6,    4,    4,    4,    4,    4,    6,
	   1,    1,    1,    5,    5,    1,   22,    9,    6,   10,    6,    5,    6,    4,    4,    5,
	   5,    4,    4,    6,    1,    5,    5,    5,    5,    5,   23,    0,    6,   10,    6,    5,
	   5,    4,    4,    4,    6,    6,    6,    6,    5,    5,    1,    1,    1,    1,    1,    9,
	   6,   10,    6,    4,    4,   18,    5,    6,    5,    5,    5,    5,    1,    1,    1,    1,
	   5,    5,    9,    0,    6,   10,    6,    4,    9,    4,    4,    5,    6,    5,    1,    1,
	   1,    1,    1,    1,    1,    5,    1,    9,    6,   10,    6,    4,    9,   18,    4,    6,
	   5,    1,    1,    1,    5,    5,    5,    5,    5,    1,    8,    0,    6,   10,   10,   10,
	  10,   24,    5,    5,    5,    1,    1,    1,    1,   49,    5,    1,    1,    1,    1,    8,
	   6,    6,    6,    6,    8,    6,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
	   1,    1,    8,    0,    6,    1,    1,    1,    8,    1,    1,    1,    1,    1,    1,    1,
	   1,    1,    1,    1,    1,    1,   25,    1,    6,    1,    1,    8,    6,    1,    1,    1,
	   1,    1,    1,    1,    1,    1,    1,    1,    1,    5,    5,    0,    6,    1,    1,    8,
	   6,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    5,    5,
	   6,    1,    8,    5,    1,    1,    1,    1,    3,   48,    3,    1,    1,    1,    1,    1,
	   1,    1,    1,    0,    6,    1,    8,   26,    1,    1,    1,    1,    1,    3,    1,    3,
	  48,    3,    1,    1,    1,    1,    5,    5,    6,    1,    8,    8,    8,   28,    3,    3,
	   3,    1,    3,    1,    3,   48,    1,    1,    1,    1,    1,    0,    6,    1,    1,    1,
	   1,    1,    1,    1,    1,    3,    1,    1,    3,    1,    3,    5,    5,    1,    1,    1,
	   6,    5,    5,    5,    3,    3,    3,    3,    1,    1,    3,    1,    1,    3,    5,    5,
	   5,    1,    1,    0,    6,    6,    6,    6,    6,    3,    3,    3,    3,    3,    7,    3,
	   3,    3,    6,    5,    5,    5,    5,    9,    6,    6,    6,    6,    6,    3,    3,    3,
	   3,   27,    3,    3,    3,    3,    6,    5,    5,    5,    9,    0,    6,    6,    6,    6,
	   6,    6,    3,    3,    3,    7,    3,    3,    3,    3,    6,   29,    5,    5,    5,    9,
	   6,    6,    6,    6,    6,    5,    5,    1,    1,    7,    7,    7,    5,    5,    5,    6,
	   6,    6,    5,    0,    6,    6,    6,    6,    7,    7,    7,    7,    7,    7,    9,    9,
	   7,    7,    6,    6,    6,    6,    6,    6,    6,    6,    6,    7,    6,    5,   50,    5,
	   7,    7,    5,    5,    7,    7,    7,    7,    7,    7,    5,    0,    6,    6,    6,    7,
	   6,    5,    6,    7,    7,    5,    7,    6,    5,    7,    7,    6,    5,    5,    7,    6,
	   6,    6,    1,    6,    6,    6,    1,    6,    6,    6,    1,    6,    6,    6,    1,    6,
	   6,    6,    1,    0,    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,    6,
	   6,    6,    6,    6,    6,    6,    6,    6,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
uint16 g_reserved_a, g_overworld_ready, g_pending_actor_event, g_pending_terrain;
uint16 g_map_N_event_done;

/* 0x0B88 */

extern void draw_hero_gauges(void);					/* 0x01D2 */
extern void draw_hero_overlay(void);					/* 0x032E */
void sprite_draw_selector(int16 x, int16 y, int16 v);			/* 0xA192 */

void sprite_draw_selector(int16 x, int16 y, int16 id);			/* 0xA192 */

/* 0x9753 */

int16 draw_map_record(int16 rec_no)
{
	struct map_record *rec;
	uint8 *ptr;
	int16 x, y, i;

	shadow_video_page(SHADOW_PAGE_MAP);
	rec = &g_map_records[rec_no - 1];
	g_sprite_record_off = rec->free_list;
	ptr = rec->icons;
	x = 0;
	y = 0x5c;

	for (i = 0; i != 10; i++)
	{
		if (*ptr++ == '3')
			sprite_draw_selector(x, y, rnd(3) + 0x46);
		x += 0x10;
		y -= 8;
	}
	y += 8;

	for (i = 0; i != 10; i++)
	{
		if (*ptr++ == '4')
			sprite_draw_selector(x, y, rnd(3) + 0x49);
		x += 0x10;
		y += 8;
	}

	shadow_video_page(SHADOW_PAGE_PREVIOUS);
	draw_hero_gauges();
	draw_hero_overlay();
	return 0;
}

/* 0x27A */

struct actor_rec g_actor_table[ACTOR_COUNT] = {	/* DGROUP 0x2968 */

	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30, 0, 0x90 },
	{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30, 0, 0x90 },
	{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30, 0, 0x90 },
	{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30, 0, 0x90 },
	{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x30, 0, 0x90 },
};

/* 0x27A */

int16 hero_cursor_rotate(int16 a)
{
	uint16 p = (uint16)g_selected_hero_slot;

	shadow_video_page(SHADOW_PAGE_MAP);                                 /* 0x284 */
	(void)sprite_draw_current(a + SPRITE_HERO_CURSOR, SPRITE_DRAW_SOLID,
	               (int16)((int16)g_hero_slots[p].x >> 3),   /* 2A5-2A7 */
	               (int16)g_hero_slots[p].y);
	if (g_actor_table[g_selected_hero_slot].state != 0) {
		g_selected_hero_slot++;
		g_selected_hero_slot &= 3;
	}
	shadow_video_page(SHADOW_PAGE_PREVIOUS);                                /* 0x2D8 */
	return 0;
}

/* 0x32E */

/* 0x032E */
void draw_hero_overlay(void)
{
	int16 i;
	struct rect_rec *r;

	shadow_video_page(SHADOW_PAGE_MAP);				/* 334-33B */

	for (i = 0, r = g_hero_slot_boxes; i != 4; i++, r++)	/* 33E-355 */
	{
		(void)sprite_draw_current((int16)(uint8)r->state, SPRITE_DRAW_MASKED,
		                 (int16)(int16)r->x >> 3,
		                 (int16)r->y);

		if ((r->state & HIGH_BYTE_MASK) != 0) {
			(void)sprite_draw_current(SPRITE_CONTROL_ICON, SPRITE_DRAW_MASKED,
			                 (int16)(int16)r->x >> 3,
			                 (int16)r->y);
		}
	}

	shadow_video_page(SHADOW_PAGE_PREVIOUS);				/* 396-39D */
}

/* 0x0C0 */

/* 0x1D2 */

void draw_hero_gauges(void)
{
	int16 i;

	shadow_video_page(SHADOW_PAGE_MAP);
	for (i = 0; i != 4; i++)
	{
		struct rect_rec *rec = (&g_hero_slots[i]);
		int16 adj = (i & 1) ? -5 : 31;			/* 1FB-209 */

		sprite_draw_dispatch((int16)rec->x + adj,
		         (int16)rec->y, SPRITE_HERO_GAUGE_FRAME);
		draw_hero_gauge(i);					/* 223-229 */
		sprite_draw_current((int16)(uint8)rec->state, SPRITE_DRAW_MASKED, (int16)(int16)RECT(rec)->x >> 3,
		         (int16)rec->y);
		if (rec->state & HIGH_BYTE_MASK)
			sprite_draw_current(SPRITE_CONTROL_OVERLAY, SPRITE_DRAW_SOLID, (int16)(int16)RECT(rec)->x >> 3,
			         (int16)rec->y);		/* 251-264 */
	}
	shadow_video_page(SHADOW_PAGE_PREVIOUS);
}

/* 0x9D07 */

/* 0x9D07 */
int16 sprite_draw_far(int16 id, int16 x, int16 y)
{
	struct sprite_descriptor far *desc = sprite_descriptor_at(g_objpic_at, (uint16)id);
	int16 bh;

	if ((uint8)id == SPRITE_TOPLEFT_3B || (uint8)id == SPRITE_TOPLEFT_3D ||
	    (uint8)id == SPRITE_TOPLEFT_3E || (uint8)id == SPRITE_TOPLEFT_3F)	/* 9D23-9D35 */
	{
		bh = (int16)((uint16)desc->width & 0xff);
	}
	else
	{
		x -= (int16)(((uint16)desc->width << 3) & WORD_MASK);		/* 9D39-9D3F */
		if (x < 0)
			x = 0;
		y -= (int16)((uint16)desc->height >> 1);			/* 9D45-9D49 */
		if (y < 0)
			y = 0;
		bh = (int16)(((uint16)desc->height >> 1) & 0xff);
	}

	shadow_sprite(0x1d, (bh << 8) | ((int16)(uint16)desc->height & 0xff),	/* 9D4F-9D52 */
	              x, y, (int16)(FP_OFF(desc) + 4), (uint16)g_objpic_at);	/* 9D54-9D57 */
	return 0;
}

/* 0xA192 */

extern uint16 g_mappic_at;					/* 0xa9a2 */

/* 0xA192 */
void sprite_draw_selector(int16 x, int16 y, int16 id)
{
	struct sprite_descriptor far *desc = sprite_descriptor_at(g_mappic_at, (uint16)id);

	shadow_sprite(3, (int16)(uint16)desc->height & 0xff,	/* A1AB-A1AD */
	              x,
	              (int16)((uint16)y - (uint16)desc->height),
	              (int16)(FP_OFF(desc) + 4), (uint16)g_mappic_at);
}

extern uint16 g_mouse_button;				/* 0xa990 */
extern int16 path_find_distance(int16 gx, int16 gy, int16 tx, int16 ty, int16 *dist);	/* 0x7501 */
extern int16 shadow_screen_restore(uint16 v);
extern int16 shadow_page_flip(void);
extern void clock_draw_time(void);					/* 0x5EC9 */
/* 0x8659 */

/* 0x8659 */
void world_map_click_view(void)
{
	int16 col, row, row_saved, found, out_idx;
	int16 dest_x, dest_y, src_x, src_y;
	int16 cell, tile, saved_char, i;

	if ((int16)g_mouse_y <= 8)					/* 8660-8667 */
		return;

	col = (int16)g_mouse_x;
	row = (int16)PARTY_REC(0)->grid_y - 6;					/* 8670-8676 */
	if (row < 0)
		row = 0;							/* 8679-867D */
	if (row > 19)
		row = 19;							/* 8682-8688 */
	row_saved = row;							/* 868D-8690 */
	row += ((int16)g_mouse_y - 8) / 12;

	if ((row & 1) == 0)							/* 86A2-86A8 */
		col -= 8;
	col >>= 4;
	if (col < 0)								/* 86B1-86B7 */
		return;

	found = path_find_distance((int16)PARTY_REC(0)->grid_x, (int16)PARTY_REC(0)->grid_y, col, row, &out_idx);	/* 86BA-86D2 */

	dest_x = (row & 1) ? 0 : 8;						/* 86D5-86E2 */
	dest_x += col << 4;							/* 86E5-86EC */
	dest_y = row * 12 + 8;							/* 86EF-86FF */
	src_x = ((int16)PARTY_REC(0)->grid_x << 4) + (((int16)PARTY_REC(0)->grid_y & 1) ? 0 : 8);	/* 8702-871A */
	src_y = (int16)PARTY_REC(0)->grid_y * 12 + 8;					/* 871D-872D */
	(void)src_x; (void)src_y;

	if ((uint16)col == PARTY_REC(0)->grid_x && row == (int16)PARTY_REC(0)->grid_y) {	/* 8730-873D */
		if (g_mouse_button == 1) {
			tile_first_entry();
			terrain_change();
		} else {
			shadow_video_page(SHADOW_PAGE_DEFAULT);
			panel_kind_draw();
			while (mouse_button_read() != 0)					/* 875A-8761 */
				;
			shadow_video_page(SHADOW_PAGE_PREVIOUS);
		}
	} else if (g_mouse_button == 1) {						/* 8770-8775 */
		cell = col + TERRAIN_WIDTH * row;						/* 877A-8785 */
		tile = (int16)(int8)g_map_tile[cell];
		if ((tile & TILE_VISIBLE) == 0)							/* 878D-8793 */
			return;
		tile &= TERRAIN_MASK;
		if (tile == 6 || tile == 8)						/* 879A-87A4 */
			return;
		g_move_speed_shift = 0;
		PARTY_REC(0)->cooldown = (uint16)out_idx;
		PARTY_REC(0)->target_x = (uint16)col;
		PARTY_REC(0)->target_y = (uint16)row;
		shadow_screen_restore(7);
		sprite_draw_anchored(dest_x, dest_y - row_saved * 12, 0x39);	/* 87CB-87E5 */
		party_update_all(0, 1);
		clock_draw_time();
		shadow_page_flip();
	} else if (g_mouse_button == 2) {						/* 8801-8806 */
		for (i = 0; i != 10; ++i) {						/* 880B-8819 */
			shadow_screen_restore(7);
			sprite_draw_anchored(dest_x, dest_y - row_saved * 12, 0x39);	/* 8825-883F */
			clock_advance(1);
			party_update_all(1, 1);
			clock_draw_time();
			shadow_page_flip();
		}
		if (found != 0) {								/* 8862-8866 */
			cell = col + TERRAIN_WIDTH * row;						/* 8868-8874 */
			g_map_tile[cell] |= TILE_VISIBLE;
			saved_char = (int16)(int8)g_map_mode_char;		/* 887C-8880 */
			g_map_mode_char = g_terrain_grid[cell];					/* 888A-8896 */
			map_record_show(g_terrain_grid[cell], 1);
			shadow_screen_restore(7);
			free_list_clear_nodes();
			actor_sweep();
			draw_terrain_sprites();
			clock_draw_time();
			shadow_page_flip();
			while (mouse_button_read() == 0)
				;
			g_map_mode_char = (uint8)saved_char;			/* 88C3-88C6 */
			draw_map_view();
		}
	}

	while (mouse_button_read() != 0)							/* 88CC-88D3 */
		;
}

/* 0xA1B5 */

void sprite_draw_segment(int16 x, int16 y, int16 id, uint16 seg)
{
	struct sprite_descriptor far *desc = sprite_descriptor_at(seg, (uint16)id);

	if (desc->width == 3)					/* A1CC-A1D4 */
		x -= 8;
	else if (desc->width == 4)				/* A1D6-A1DB */
		x -= 0x10;

	sprite_blit_pair((((uint16)desc->width & 0xff) << 8) | ((uint16)desc->width >> 8),
	         (int16)(desc->height & 0xff), x, y, (int16)(FP_OFF(desc) + 4));
}

extern uint16 g_missile_at;

struct missile_descriptor {
	uint16 frame_table;
	uint16 width;
	uint16 height;
	uint16 frame_limit;
};

typedef char missile_descriptor_size_check[(sizeof(struct missile_descriptor) == 8) ? 1 : -1];

/* 0xA0ED */

void sprite_draw_field(int16 x, int16 y, struct sprite_node_rec *rec, int16 sel)
{
	struct missile_descriptor far *desc;
	uint16 far *frames;
	uint16 idx = (uint16)rec->id;
	uint16 frame, data;

	FP_SEG(desc) = g_missile_at;
	FP_OFF(desc) = idx * sizeof(struct missile_descriptor);

	frame = ((uint16)rec->stride_index >= desc->frame_limit) ? 0u : (uint16)rec->stride_index;	/* A130-A137 */
	SPRITE_FRAME_WORD(rec) = (uint16)frame;

	FP_SEG(frames) = g_missile_at;
	FP_OFF(frames) = desc->frame_table;
	data = frames[frame];					/* A13D-A143 */

	sprite_blit_pair((int16)(((desc->width & 0xff) << 8) | ((uint16)sel & 0xff)),	/* A146-A148 */
	         (int16)(desc->height & 0xff),
	         x - (int16)(desc->width << 3),			/* A11B-A121 */
	         y - (int16)(desc->height >> 1),			/* A128-A12A */
	         (int16)data);
}

uint16 g_sprite_draw_x;                    /* 0xa9aa */
uint16 g_sprite_draw_y;                    /* 0xa9ae */
uint8 g_sprite_stride_index;               /* 0xa61b */
uint8 g_node_selector;               /* 0xa174 */
uint8 g_sprite_step_x;               /* 0xad34 */
uint8 g_sprite_step_y;               /* 0xad3a */
uint8 g_sprite_frame_count;               /* 0xad72 */
uint8 g_sprite_frame_index;               /* 0xad73 */
uint8 g_sprite_last_frame;               /* 0xa992 */

struct sprite_bank_header {
	uint16 unknown_00;
	uint16 unknown_02;
	uint16 x_offset;
	uint16 y_offset;
	uint16 state_table;
};

struct sprite_state_entry {
	uint16 frame_table;
	uint8  frame_count;
	uint8  step_x;
	uint8  step_y;
	uint8  frame_limit;
};

struct sprite_direction_table {
	uint16 frame_offset[8];
};

typedef char sprite_bank_header_size_check[(sizeof(struct sprite_bank_header) == 10) ? 1 : -1];
typedef char sprite_state_entry_size_check[(sizeof(struct sprite_state_entry) == 6) ? 1 : -1];
typedef char sprite_dir_table_size_check[(sizeof(struct sprite_direction_table) == 16) ? 1 : -1];

/* 0x9D67 */

void sprite_draw_state_frame(uint16 seg)
{
	struct sprite_bank_header far *bank;
	struct sprite_state_entry far *entry;
	struct sprite_direction_table far *dirs;
	uint16 far *frames;
	uint16 x, y, sel, cnt, off;

	FP_SEG(bank) = seg;
	FP_OFF(bank) = 0;

	x = g_sprite_draw_x - (bank->x_offset << 3);				/* 9D78-9D81 */
	if ((int16)x < 0)
		return;
	if (x >= SCREEN_WIDTH)
		return;								/* 9D97-9D9B */

	y = g_sprite_draw_y - bank->y_offset;					/* 9D9D-9DA5 */
	if ((int16)y < 0)
		return;
	if (y >= SCREEN_MAX_Y)
		return;								/* 9DB6-9DBA */

	FP_SEG(entry) = seg;
	FP_OFF(entry) = bank->state_table + (uint16)g_sprite_stride_index * 6u;	/* 9DBD-9DCA */

	g_sprite_step_x = entry->step_x;					/* 9DCC-9DCD */
	g_sprite_step_y = entry->step_y;					/* 9DD3-9DD6 */
	if (g_sprite_frame_index == entry->frame_limit)					/* 9DDA-9DE1 */
		g_sprite_last_frame = g_sprite_frame_index;
	g_sprite_frame_count = entry->frame_count;						/* 9DE7-9DEA */

	if (g_sprite_frame_index != 0 && g_sprite_frame_index >= entry->frame_count)
		g_sprite_frame_index = 0;						/* 9DF7-9DF9 */
	cnt = g_sprite_frame_index;

	FP_SEG(dirs) = seg;
	FP_OFF(dirs) = entry->frame_table;
	sel = ((uint16)g_node_selector - 1u) & 7u;				/* 9E02-9E08 */
	FP_SEG(frames) = seg;
	FP_OFF(frames) = dirs->frame_offset[sel];				/* 9E0C-9E10 */
	off = frames[cnt];							/* 9E13-9E17 */

	sel = ((uint16)g_node_selector - 1u) & 3u;				/* 9E1C-9E2E */
	if ((((uint16)g_node_selector - 1u) & 4u) == 0)
		sel = 0;

	sprite_blit_pair((int16)((2u << 8) | sel), (int16)(bank->y_offset & 0xff),
	                 (int16)x, (int16)y, (int16)off);
}

extern int16 shadow_screen_restore(uint16 sel);	/* 0xD450 */
extern int16 shadow_page_flip(void);			/* 0xD3D0 */

/* 0x10BD */

void screen_refresh(void)
{
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);					/* 10C0-10C7 */
	(void)shadow_video_flags(SHADOW_BLIT_RESET);					/* 10CA-10D1 */
	(void)shadow_screen_restore(7);					/* 10D4-10DB */
	(void)draw_terrain_sprites();
	(void)shadow_page_flip();
	(void)shadow_screen_restore(7);					/* 10E4-10EB */
	(void)draw_terrain_sprites();
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);						/* 10F1-10F7 */
}

/* 0x0C79 */

void draw_glyph_wait(const struct rect_rec *rec, int16 selected)
{
	(void)sprite_draw_dispatch((int16)rec->x - 5,	/* C89-CAF */
	               (int16)rec->y,
	               selected != 0 ? SPRITE_BUTTON_MARK_SET : SPRITE_BUTTON_MARK);
	while (mouse_button_read() != 0)
		;
}

/* 0xA1EB */

uint16 g_panel_stride_mode;					/* 0xa61c */
uint16 g_row_height;					/* 0xab2c */

/* 0xA1EB */
void far_pair_load(int16 index, uint16 seg)
{
	uint16 far *tab;
	uint16 far *p;
	uint16 off;

	FP_SEG(tab) = (uint16)seg;
	FP_OFF(tab) = (uint16)(index * 2);
	off = *tab;
	FP_SEG(p) = (uint16)seg;
	FP_OFF(p) = (uint16)off;
	g_panel_stride_mode = p[0];
	g_row_height = p[1];						/* 1FE-1FF */
}

/* 0x0C31 */

void record_set_mode(struct site_rec *o, int16 x, int16 y, int16 mode_value)
{
	int16 mask = (int16)WORD_MASK;

	SITE_FLAG_LO(o) &= 0xf8;
	SITE_FLAG_LO(o) |= 0x01;
	o->map = (uint16)(int16)(int8)g_map_mode_char;
	o->x = (int16)((uint16)x & (uint16)mask);			/* C54-C5D */
	o->y = (int16)((uint16)y & (uint16)mask);			/* C60-C69 */
	o->mode_value = (uint16)mode_value;
}

/* 0x02E2 */

/* 0x02E2 */
void draw_glyph_corners(void)
{
	struct rect_rec *p = g_rect_panel_boxes;
	int16 i = 0;

	(void)shadow_video_page(SHADOW_PAGE_MAP);							/* 2E8-2EF */
	while (i != 4) {							/* 305-309 */
		(void)sprite_draw_dispatch((int16)p->x,
		               (int16)p->y,
		               SPRITE_PANEL_CORNER);
		i++;
		p++;
	}
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);						/* 320-327 */
}

/* 0xF94 */

const uint8 g_walk_delta[20] = {
	  0,   1,   0,   1,   0,   1,   0,   1,   0,   1,
	  0,   1,   1,   0,   1,   0,   1,   0,   1,   0,
};

/* 0x713C */

/* 0x713C */
void walk_steps(void)
{
	const uint8 *p = g_walk_delta;
	int16 i = 0;

	while (i != 10) {							/* 7151-7155 */
		PARTY_REC(0)->grid_x += (int16)(int8)*p++;			/* 7157-7160 */
		PARTY_REC(0)->grid_y += (int16)(int8)*p++;			/* 7164-716D */
		draw_map_view();
		(void)shadow_screen_restore(7);					/* 7174-717B */
		(void)party_update_all(0, 1);						/* 717E-7188 */
		(void)shadow_page_flip();
		i++;
	}
}

/* 0xA153 */

/* 0xA153 */
void sprite_draw_index(int16 x, int16 y, int16 idx)
{
	struct sprite_descriptor far *desc;
	uint16 si;

	si = (uint16)idx & 0xffu;
	if (si & 0xe0u) {
		desc = sprite_descriptor_at(g_mappic_at, si);
		g_panel_stride_mode = desc->width;			/* A171-A172 */
		g_row_height = desc->height;				/* A176-A177 */
		si = g_sprite_record_off;
	}

	desc = sprite_descriptor_at(g_mappic_at, si);
	shadow_sprite(4, (int16)(desc->height & 0xffu), x, y,
	              (int16)(FP_OFF(desc) + 4), (uint16)g_mappic_at);	/* A188-A18D */
}

/* 0xA0C1 */

/* 0xA0C1 */
void sprite_draw_forced(int16 x, int16 y, int16 id)
{
	struct sprite_descriptor far *desc = sprite_descriptor_at(g_objpic_at, (uint16)id);

	sprite_blit_pair((((uint16)desc->width & 0xffu) << 8) | 1u,
	         (int16)(desc->height & 0xffu), x, y, (int16)(FP_OFF(desc) + 4));
}

/* 0x1BE */

const uint16 g_map_ring[80] = {		/* DGROUP 0x01BE */
	0x0004, 0x0008, 0x0008, 0x0008, 0x000C, 0x0008, 0x0010, 0x0008,
	0x0014, 0x0008, 0x0018, 0x0008, 0x001C, 0x0008, 0x0020, 0x0008,
	0x0000, 0x0028, 0x0004, 0x0028, 0x0008, 0x0028, 0x000C, 0x0028,
	0x0010, 0x0028, 0x0014, 0x0028, 0x0018, 0x0028, 0x001C, 0x0028,
	0x0020, 0x0028, 0x0024, 0x0028, 0x0000, 0x0048, 0x0004, 0x0048,
	0x0008, 0x0048, 0x001C, 0x0048, 0x0020, 0x0048, 0x0024, 0x0048,
	0x0000, 0x0068, 0x0024, 0x0068, 0x0000, 0x0088, 0x0004, 0x0088,
	0x0008, 0x0088, 0x001C, 0x0088, 0x0020, 0x0088, 0x0024, 0x0088,
	0x0004, 0x00A8, 0x0008, 0x00A8, 0x000C, 0x00A8, 0x0010, 0x00A8,
	0x0014, 0x00A8, 0x0018, 0x00A8, 0x001C, 0x00A8, 0x0020, 0x00A8
};

/* 0x914D */

/* 0xA0C1 */

#define FRAME_RIGHT_X		0x130
#define FRAME_MARK_LEFT_X	0x100
#define FRAME_MARK_RIGHT_X	0x120

/* 0x914D */
void draw_frame_marks(void)
{
	const uint16 *p = g_map_ring;
	int16 i, x, y;

	for (i = 0; i != FRAME_RING_COUNT; i++) {				/* 915D-9191 */
		x = (int16)*p++;
		y = (int16)*p++;
		(void)sprite_draw_current(SPRITE_FRAME_RING, SPRITE_DRAW_MASKED, x, y);
	}

	for (i = 0; i != FRAME_BORDER_COUNT; i++)					/* 9193-91BC */
		sprite_draw_dispatch((i << 4) + 0x30, 8, SPRITE_FRAME_BORDER);

	for (i = 0; i != FRAME_PAIR_COUNT; i++) {					/* 91BE-9248 */
		sprite_draw_dispatch(i << 4, 0x69 - (i << 3), SPRITE_FRAME_CORNER);
		sprite_draw_forced(FRAME_RIGHT_X - (i << 4), 0x69 - (i << 3), SPRITE_FRAME_CORNER);
		sprite_draw_dispatch(i << 4, (i << 3) + 0x7a, SPRITE_FRAME_CORNER2);
		sprite_draw_forced(FRAME_RIGHT_X - (i << 4), (i << 3) + 0x7a, SPRITE_FRAME_CORNER2);
	}

	for (i = 0; i != FRAME_EDGE_COUNT; i++) {					/* 924B-9289 */
		sprite_draw_dispatch(0, (i << 4) + 0x2b, SPRITE_FRAME_EDGE);
		sprite_draw_forced(FRAME_RIGHT_X, (i << 4) + 0x2b, SPRITE_FRAME_EDGE);
	}

	sprite_draw_dispatch(0, 0x7d, SPRITE_FRAME_EDGE);		sprite_draw_forced(FRAME_RIGHT_X, 0x7d, SPRITE_FRAME_EDGE);	/* 928B-92AB */
	sprite_draw_dispatch(0, 0x86, SPRITE_FRAME_EDGE);		sprite_draw_forced(FRAME_RIGHT_X, 0x86, SPRITE_FRAME_EDGE);	/* 92AE-92CE */
	sprite_draw_dispatch(0x20, 8, SPRITE_FRAME_MARK_A);		sprite_draw_dispatch(0, 0x28, SPRITE_FRAME_MARK_B);	/* 92D1-92F1 */
	sprite_draw_forced(FRAME_MARK_LEFT_X, 8, SPRITE_FRAME_MARK_A);		sprite_draw_forced(FRAME_MARK_RIGHT_X, 0x28, SPRITE_FRAME_MARK_B);	/* 92F4-9315 */
	sprite_draw_dispatch(0x20, 0xa4, SPRITE_FRAME_MARK_C);		sprite_draw_dispatch(0, 0x95, SPRITE_FRAME_MARK_D);	/* 9318-9335 */
	sprite_draw_forced(FRAME_MARK_LEFT_X, 0xa4, SPRITE_FRAME_MARK_C);		sprite_draw_forced(FRAME_MARK_RIGHT_X, 0x95, SPRITE_FRAME_MARK_D);	/* 933B-9359 */
	sprite_draw_dispatch(0x30, 0xb4, SPRITE_FRAME_COUNTER_L);		sprite_draw_dispatch(0x50, 0xb4, SPRITE_FRAME_COUNTER_R);	/* 935F-937D */
	sprite_draw_forced(0xf0, 0xb4, SPRITE_FRAME_COUNTER_L);		sprite_draw_forced(0xd0, 0xb4, SPRITE_FRAME_COUNTER_R);	/* 9383-93A1 */
	sprite_draw_dispatch(0x65, 0xc4, SPRITE_FRAME_PIP);		sprite_draw_dispatch(0x71, 0xc4, SPRITE_FRAME_PIP);	/* 93AB-93C5 */
	sprite_draw_dispatch(0xaf, 0xc4, SPRITE_FRAME_PIP);		sprite_draw_dispatch(0xbb, 0xc4, SPRITE_FRAME_PIP);	/* 93CB-93EC */
}

struct map_record g_map_working;		/* 0xa178 */
uint16 g_detect_invisible_ticks;				/* 0x9e0a */
uint16 g_map_grid_ptr;					/* 0xa9b4 */
uint16 g_map_load_gate;					/* 0xa9ac */
uint8 g_map_cell_kind;					/* 0xa9d2 */

/* 0x93F3 */

/* 0x93F3 */
void map_record_show(int16 rec_no, int16 transition)
{
	struct map_cell *pw;
	uint8 *bytp;
	int16 i, j, x, y, val, kind, last_kind = 0;

	g_detect_invisible_ticks = 0;						/* 93F9-93FB */
	if (transition != 0)							/* 9401-9404 */
		cursor_set(0);						/* 9406-940A */
	free_list_reset();
	(void)shadow_video_page(SHADOW_PAGE_MAP);						/* 9410-9417 */
	(void)shadow_region_clear(SHADOW_MASK_CLEAR_ALL, 0, SCREEN_TOP_MARGIN, SCREEN_WIDTH_BYTES, MAP_VIEW_HEIGHT);			/* 941A-942D */
	draw_frame_marks();

	if ((int16)(int8)g_map_mode_char > 10 || transition != 0 || g_panel_busy != 0)	/* 9433-9445 */
		mem_copy_bytes((uint8 *)&g_map_working, (uint8 *)&g_map_records[rec_no - 1], MAP_RECORD_SIZE);	/* 9447-9459 */

	g_sprite_record_off = g_map_working.free_list;			/* 9461-9464 */
	g_map_cell_kind = g_map_working.cell_kind;			/* 9467-946A */

	x = 0x90;
	y = 0x28;

	if (rec_no == MAP_DRAGON_ATTACK && g_clock_hour == HOUR_20) {			/* 9477-9482 */
		(void)driver_play_sequence(SOUND_SEQUENCE_DRAGON_ATTACK);					/* 9484-948B */
		g_map_working.exit_right_guard = MAP_DRAGON_ATTACK_EXIT_GUARD;	g_map_master_record->exit_right_guard = MAP_DRAGON_ATTACK_EXIT_GUARD;	/* 948E-9493 */
		g_map_working.exit_right_map = TERRAIN_RIVER_VALLEY;	g_map_master_record->exit_right_map = TERRAIN_RIVER_VALLEY;		/* 9496-949B */
		g_map_working.icons[MAP_ICON_SLOT_EXIT] = MAP_ICON_EXIT;	g_map_master_record->icons[MAP_ICON_SLOT_EXIT] = MAP_ICON_EXIT;	/* 949E-94A3 */
	}

	pw = g_map_working.grid + 9;					/* 94A6-94AC */
	g_map_grid_ptr = (uint16)pw;

	for (i = 0; i != 10; i++) {					/* 94B2-94BC */
		for (j = 9; j >= 0; j--) {				/* 94C5-94CF */
			val = (int16)pw->cell.value;			/* 94D8-94DD */
			sprite_draw_index(x, y, val);				/* 94E0-94EA */
			pw->cell.part.kind = 0;

			if (transition != 0) {					/* 94F4-94F8 */
				if ((val & 0xff00) != 0) {			/* 94FA-94FF */
					kind = (int16)(int16)val >> 8;
					if (kind >= MONSTER_REQUEST_DEATH_KNIGHT)
						last_kind = kind;
					(void)spawn_descriptor(kind, (i << 4) + 8, (j << 4) + 8);	/* 9513-952D */
				}
				if (g_map_load_gate == 0 && rec_no <= 10 && val >= 0x22	/* 9534-9545 */
				    && ROLL_D100() > 25) {			/* 9547-9554 */
					pw->cell.value = g_sprite_record_off;		/* 9556-955C */
					val = 0;
				}
			}

			if (val >= 0x20)					/* 9569-956D */
				(void)free_list_insert(0, (9 - j) << 4, i << 4, 1,
				         (int16)pw, x, y - (int16)g_row_height + 0xf);	/* 956F-959E */

			pw -= 1;	x -= 0x10;	y += 8;			/* 95A1-95A9 */
		}
		pw += 0x14;	x += 0xb0;	y -= 0x48;			/* 95B0-95B9 */
	}

	x = 0;	y = 0x78;							/* 95C0-95C5 */
	bytp = g_map_working.icons;					/* 95CA-95D0 */
	for (i = 0; i != 10; i++) {					/* 95D3-95E8 */
		val = (int16)(int8)*bytp;				/* 95EE-95F4 */
		if (val > 0x45) {						/* 95F7-95FA */
			val = 0;
			*bytp = 0;
		}
		if (val != 0)
			sprite_draw_selector(x, y, val);				/* 960A-9616 */
		x += 0x10;	y -= 8;	bytp++;				/* 95DA-95E5 */
	}

	y += 8;
	for (i = 0; i != 10; i++) {					/* 961F-9638 */
		val = (int16)(int8)*bytp;				/* 963A-9640 */
		if (val > 0x45) {						/* 9643-9646 */
			val = 0;
			*bytp = 0;
		}
		if (val != 0)
			sprite_draw_selector(x, y, val);				/* 9656-9662 */
		x += 0x10;	y += 8;	bytp++;				/* 9626-9631 */
	}

	(void)sprite_draw_current(SPRITE_MAP_SCROLL_ARROW, SPRITE_DRAW_MASKED, 7, 0xb8);				/* 9667-9679 */
	(void)sprite_draw_current(SPRITE_MAP_SCROLL_ARROW, SPRITE_DRAW_MASKED, 0x1b, 0xb8);				/* 967C-968E */

	if (rec_no == MAP_DUSK_EYE && g_clock_hour < HOUR_20 && g_clock_hour > 3)	/* 9691-96A3 */
		cursor_set(6);						/* 96A5-96AC */

	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);						/* 96AF-96B6 */
	draw_menubar();
	(void)shadow_video_page(SHADOW_PAGE_MAP);						/* 96BC-96C3 */
	clock_draw_time();
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);						/* 96C9-96D0 */
	draw_hero_gauges();
	draw_glyph_corners();
	draw_hero_overlay();

	if (transition != 0) {							/* 96DC-96E0 */
		if (last_kind != 0) {					/* 96E2-96E6 */
			g_special_monster_bank_loaded = 1;
			g_monster_bank_suffix = (uint8)((uint8)last_kind
			          + (last_kind > MONSTER_REQUEST_RDRAGON ? VMON_BANK_DIGIT_BASE : VMON_BANK_LETTER_BASE));	/* 96EE-9700 */
			(void)shadow_asset_load(g_name_vmon, g_monster_bank_segments[0]);		/* 9703-9711 */
		} else if (g_special_monster_bank_loaded != 0) {				/* 9716-971B */
			g_monster_bank_suffix = VMON_BANK_BAAZ;
			(void)shadow_asset_load(g_name_vmon, g_monster_bank_segments[0]);		/* 9722-9730 */
			g_monster_bank_suffix = VMON_BANK_BOZAK;
			(void)shadow_asset_load(g_name_vmon, g_monster_bank_segments[1]);		/* 9738-9746 */
			g_special_monster_bank_loaded = 0;
		}
	}
}
