/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"

#include <dos.h>

#include "shadow.h"
#include "game.h"
#include "structs.h"

char g_shadow_prefix = 'e';  /* 0xA61A */

/* 0xD388 */

static char shadow_path[32];

int32 shadow_load(const char *name, uint16 dest_para)
{
    union REGS r;
    int16 i;

    for (i = 0; name[i] != 0 && i < (int16)sizeof(shadow_path) - 1; i++)
        shadow_path[i] = name[i];
    shadow_path[i] = 0;
    shadow_path[0] = g_shadow_prefix;
    r.x.dx = (uint16)shadow_path;
    r.x.bx = dest_para;          /* 0xD399 */
    r.x.ax = SHADOW_SEL_LOAD;             /* 0xD39C */
    int86(SHADOW_VECTOR, &r, &r);
    return (int32)r.x.ax;
}

/* 0xD374 */
int16 shadow_poll(void)
{
    union REGS r;

    r.x.ax = SHADOW_SEL_KEY_POLL;             /* 0xD375 */
    int86(SHADOW_VECTOR, &r, &r);
    return r.x.ax;
}

/* 0xD3C8 */

int16 shadow_config(void)
{
    union REGS r;

    r.x.ax = SHADOW_SEL_CONFIG;             /* 0xD3C9 */
    int86(SHADOW_VECTOR, &r, &r);
    return r.x.ax;
}

/* 0xD340 */
void shadow_text(int16 colour, int16 col, int16 row, uint16 str)
{
    union REGS r;

    r.x.bx = (uint16)colour;
    /* 0xD350 */

    r.x.dx = (uint16)((row & LOW_BYTE_MASK) | ((uint16)col << 8));
    r.x.si = (uint16)str;
    r.x.ax = SHADOW_SEL_TEXT;             /* 0xD355 */
    int86(SHADOW_VECTOR, &r, &r);
}

uint16 g_svc_pos;   /* 0x9f1a */

uint16 g_mouse_x;   /* 0xa9c2 */
uint16 g_mouse_y;   /* 0xa9c4 */
uint16 g_mouse_button; /* 0xa990 */

/* 0xa9c8 */

uint16 g_overlay_segment;			/* 0x9f2a */

/* 0xD360 */

/* 0xD340 */

int16 shadow_text_draw(int16 attr, int16 col, int16 row, const char *text)
{
	union REGS r;

	r.x.bx = (uint16)attr;
	r.x.dx = (uint16)(((col & LOW_BYTE_MASK) << 8) | (row & LOW_BYTE_MASK));  /* 0xD350 */
	r.x.si = (uint16)text;
	r.x.ax = SHADOW_SEL_TEXT;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0xD360 */
int16 shadow_video_copy(uint16 lines)
{
	union REGS r;

	r.x.cx = lines;
	r.x.ax = SHADOW_SEL_VIDEO_COPY;                 /* 0xD366 */
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0xD36C */

int16 shadow_getkey_noecho(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_GETKEY;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0xD37C */

int16 shadow_direction_read(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_DIRECTION_READ;
	int86(SHADOW_VECTOR, &r, &r);
	return r.h.al;
}

/* 0xD3A7 */

int16 shadow_stream_read(uint16 offset)
{
	union REGS r;

	r.x.dx = offset;
	r.x.cx = g_svc_pos;
	r.x.bx = (uint16)g_mem_next;       /* 0xD3B5 */
	r.x.ax = SHADOW_SEL_STREAM_READ;
	int86(SHADOW_VECTOR, &r, &r);
	g_svc_pos += r.x.ax;          /* 0xD3C2 */
	return r.x.ax;
}

/* 0xD3D0 */

int16 shadow_page_flip(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_PAGE_FLIP;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x9CA9 */

int16 sprite_draw_current(int16 sprite, int16 mode, int16 column, int16 row)
{
	uint16 seg = g_overlay_segment;
	uint16 far *tbl;
	union REGS r;

	FP_SEG(tbl) = (uint16)seg;
	FP_OFF(tbl) = (uint16)(sprite * 2);

	r.x.bx = (uint16)(sprite * 2);
	r.x.si = *tbl;
	r.x.cx = (uint16)mode;
	r.x.dx = (uint16)(((column & LOW_BYTE_MASK) << 8) | (row & LOW_BYTE_MASK));
	r.x.ax = SHADOW_SEL_SPRITE;
	{
		struct SREGS s2;
		segread(&s2);
		s2.ds = (uint16)seg;
		int86x(SHADOW_VECTOR, &r, &r, &s2);
	}
	return r.x.ax;
}

/* 0xD3D6 */

int16 shadow_set_palette(uint16 palette_offset)
{
	union REGS r;

	r.x.dx = palette_offset;
	r.x.ax = SHADOW_SEL_SET_PALETTE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0xD3E3 */

int16 shadow_rect_fill(uint16 a, uint16 b, uint16 c, uint16 d)
{
	union REGS r;

	r.x.cx = a;
	r.x.dx = (uint16)((b & LOW_BYTE_MASK) | ((c & LOW_BYTE_MASK) << 8));   /* 0xD3F3 */
	r.x.bx = d;
	r.x.ax = SHADOW_SEL_RECT_FILL;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0xD403 */
int16 shadow_screen_refresh(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_SCREEN_REFRESH;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0xD413 */

int16 shadow_save(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_SAVE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0xD419 */

void shadow_mouse_read(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_MOUSE_READ;
	int86(SHADOW_VECTOR, &r, &r);
	g_mouse_y = r.x.dx;           /* 0xD41E */
	g_mouse_x = r.x.cx;           /* 0xD422 */
	g_mouse_button = r.x.bx;         /* 0xD426 */
}

/* 0xD42B */

void shadow_mouse_draw(uint16 a, uint16 b)
{
	union REGS r;

	g_mouse_x = a;                /* 0xD438 */
	g_mouse_y = b;                /* 0xD434 */
	r.x.cx = a;
	r.x.dx = b;
	r.x.ax = SHADOW_SEL_MOUSE_DRAW;
	int86(SHADOW_VECTOR, &r, &r);
}

/* 0xD443 */

int16 shadow_video_page(int16 page)
{
	union REGS r;

	r.x.bx = (uint16)page;
	r.x.ax = SHADOW_SEL_VIDEO_PAGE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

int16 shadow_key_read(void)
{
	return shadow_poll();
}

/* 0xD450 */

int16 shadow_screen_restore(uint16 v)
{
	union REGS r;

	r.x.bx = v;
	r.x.ax = SHADOW_SEL_SCREEN_RESTORE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0xD45D */

int16 shadow_region_clear(int16 and_mask, int16 column, int16 row,
                          int16 width_bytes, int16 height_rows)
{
	union REGS r;

	r.x.bx = (uint16)and_mask;
	r.x.dx = (uint16)(((column & LOW_BYTE_MASK) << 8) | (row & LOW_BYTE_MASK));
	r.x.cx = (uint16)((height_rows & LOW_BYTE_MASK) | ((width_bytes & LOW_BYTE_MASK) << 8));
	r.x.ax = SHADOW_SEL_REGION_CLEAR;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x9CFA */

int16 shadow_video_flags(int16 v)
{
	union REGS r;

	r.x.bx = (uint16)v;
	r.x.ax = SHADOW_SEL_VIDEO_FLAGS;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0xD388 */

extern char g_shadow_prefix;

/* 0xD388 */
int16 shadow_asset_load(char *name, uint16 seg)
{
	union REGS r;

	*name = g_shadow_prefix;
	r.x.bx = seg;
	r.x.ax = SHADOW_SEL_LOAD;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x9CD3 */

int16 sprite_draw_block(int16 a, int16 b, int16 c, int16 d, uint16 seg)
{
	uint16 far *tbl;
	union REGS r;

	FP_SEG(tbl) = (uint16)seg;
	FP_OFF(tbl) = (uint16)(a * 2);

	r.x.bx = (uint16)(a * 2);
	r.x.si = *tbl;
	r.x.cx = (uint16)b;
	r.x.dx = (uint16)(((c & LOW_BYTE_MASK) << 8) | (d & LOW_BYTE_MASK));  /* 9CE3-9CE9 */
	r.x.ax = SHADOW_SEL_SPRITE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}
