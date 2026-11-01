/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include <dos.h>

#include "types.h"
#include "shadow.h"

char g_shadow_prefix = 'e';	/* 0x0000 */

static uint16 g_mouse_x;
static uint16 g_mouse_y;
static uint16 g_mouse_button;
static uint16 g_svc_pos;

/* 0x030A */
int16 shadow_key_poll(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_KEY_POLL;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x0302 */
int16 shadow_getkey(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_GETKEY;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x0312 */
int16 shadow_direction_read(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_DIRECTION_READ;
	int86(SHADOW_VECTOR, &r, &r);
	return (int16)(r.h.al);
}

/* 0x0366 */
int16 shadow_page_flip(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_PAGE_FLIP;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x0399 */
int16 shadow_screen_refresh(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_SCREEN_REFRESH;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x03A9 */
int16 shadow_save(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_SAVE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x03D9 */
int16 shadow_video_page(int16 page)
{
	union REGS r;

	r.x.bx = (uint16)page;
	r.x.ax = SHADOW_SEL_VIDEO_PAGE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x03E6 */
int16 shadow_screen_restore(uint16 value)
{
	union REGS r;

	r.x.bx = value;
	r.x.ax = SHADOW_SEL_SCREEN_RESTORE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x031E */

int32 shadow_load(char *name, uint16 dest_para)
{
	static char path[32];
	union REGS r;
	int16 i;

	for (i = 0; name[i] != 0 && i < (int16)sizeof(path) - 1; i++)
		path[i] = name[i];
	path[i] = 0;
	path[0] = g_shadow_prefix;
	r.x.dx = (uint16)path;
	r.x.bx = dest_para;
	r.x.ax = SHADOW_SEL_LOAD;
	int86(SHADOW_VECTOR, &r, &r);
	return (int32)r.x.ax;
}

/* 0x033D */

int16 shadow_stream_read(uint16 offset)
{
	union REGS r;

	r.x.dx = offset;
	r.x.cx = g_svc_pos;
	r.x.bx = 0;
	r.x.ax = SHADOW_SEL_STREAM_READ;
	int86(SHADOW_VECTOR, &r, &r);
	g_svc_pos += r.x.ax;
	return r.x.ax;
}

/* 0x0379 */
int16 shadow_rect_fill(int16 x, int16 y, int16 w, int16 h)
{
	union REGS r;

	r.x.cx = (uint16)x;
	r.x.dx = (uint16)y;
	r.x.bx = (uint16)w;
	r.h.dh = (uint8)h;
	r.x.ax = SHADOW_SEL_RECT_FILL;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x03F3 */
int16 shadow_region_clear(int16 mask, int16 column, int16 row,
			  int16 width_bytes, int16 height_rows)
{
	union REGS r;

	r.x.bx = (uint16)mask;
	r.x.dx = (uint16)(((uint8)column << 8) | (uint8)row);
	r.x.cx = (uint16)(((uint8)width_bytes << 8) | (uint8)height_rows);
	r.x.ax = SHADOW_SEL_REGION_CLEAR;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x02D6 */
void shadow_text(int16 colour, int16 col, int16 row, uint16 str)
{
	union REGS r;

	r.x.bx = (uint16)colour;

	r.x.dx = (uint16)(((uint8)row) | ((uint16)(uint8)col << 8));
	r.x.si = str;
	r.x.ax = SHADOW_SEL_TEXT;
	int86(SHADOW_VECTOR, &r, &r);
}

/* 0x03AF */
void shadow_mouse_read(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_MOUSE_READ;
	int86(SHADOW_VECTOR, &r, &r);
	g_mouse_y = r.x.dx;
	g_mouse_x = r.x.cx;
	g_mouse_button = r.x.bx;
}

/* 0x03C1 */
void shadow_mouse_draw(int16 x, int16 y)
{
	union REGS r;

	r.x.dx = (uint16)y;
	r.x.cx = (uint16)x;
	r.x.ax = SHADOW_SEL_MOUSE_DRAW;
	int86(SHADOW_VECTOR, &r, &r);
}
