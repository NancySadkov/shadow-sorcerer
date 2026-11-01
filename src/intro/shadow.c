/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include <dos.h>

#include "types.h"
#include "shadow.h"

/* 0x02F6 */
int16 shadow_video_copy(uint16 lines)
{
	union REGS r;

	r.x.cx = lines;
	r.x.ax = SHADOW_SEL_VIDEO_COPY;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x036C */
int16 shadow_set_palette(uint16 palette_offset)
{
	union REGS r;

	r.x.dx = palette_offset;
	r.x.ax = SHADOW_SEL_SET_PALETTE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x035E */
int16 shadow_config(void)
{
	union REGS r;

	r.x.ax = SHADOW_SEL_CONFIG;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}

/* 0x0A95 */

void shadow_pixel(int16 x, int16 y, int16 colour)
{
	union REGS r;

	r.x.dx = (uint16)y;
	r.x.bx = (uint16)x;
	r.h.cl = (uint8)colour;
	r.x.ax = SHADOW_SEL_PIXEL;
	int86(SHADOW_VECTOR, &r, &r);
}

/* 0x0AAA */

void shadow_command21(uint16 value)
{
	union REGS r;

	r.x.cx = (uint16)(value << 4);
	r.x.ax = SHADOW_SEL_QUEUE;
	int86(SHADOW_VECTOR, &r, &r);
}
