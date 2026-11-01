/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#ifndef SHADOW_H
#define SHADOW_H

#include "types.h"

#define SHADOW_VECTOR	0xF8

#define SHADOW_SEL_VIDEO_COPY		0x00
#define SHADOW_SEL_PAGE_FLIP		0x01
#define SHADOW_SEL_SET_PALETTE		0x02
#define SHADOW_SEL_LOAD			0x09
#define SHADOW_SEL_KEY_POLL		0x0A
#define SHADOW_SEL_TEXT			0x0B
#define SHADOW_SEL_DIRECTION_READ	0x0C
#define SHADOW_SEL_SCREEN_REFRESH	0x10
#define SHADOW_SEL_SAVE			0x11
#define SHADOW_SEL_GETKEY		0x15
#define SHADOW_SEL_CONFIG		0x17
#define SHADOW_SEL_STREAM_READ		0x18
#define SHADOW_SEL_REGION_CLEAR		0x19
#define SHADOW_SEL_RECT_FILL		0x1A
#define SHADOW_SEL_MOUSE_READ		0x1C
#define SHADOW_SEL_VIDEO_PAGE		0x1E
#define SHADOW_SEL_SCREEN_RESTORE	0x1F
#define SHADOW_SEL_PIXEL		0x20
#define SHADOW_SEL_QUEUE		0x21
#define SHADOW_SEL_MOUSE_DRAW		0x22

extern char g_shadow_prefix;	/* 0x0000 */

int16  shadow_video_copy(uint16 lines);
int16  shadow_set_palette(uint16 palette_offset);
int16  shadow_config(void);
int16  shadow_key_poll(void);
int16  shadow_getkey(void);
int16  shadow_direction_read(void);
int16  shadow_page_flip(void);
int16  shadow_screen_refresh(void);
int16  shadow_save(void);
int16  shadow_video_page(int16 page);
int16  shadow_screen_restore(uint16 value);

int32  shadow_load(char *name, uint16 dest_para);
int16  shadow_stream_read(uint16 offset);
int16  shadow_rect_fill(int16 x, int16 y, int16 w, int16 h);
int16  shadow_region_clear(int16 mask, int16 column, int16 row,
                           int16 width_bytes, int16 height_rows);
void   shadow_text(int16 colour, int16 col, int16 row, uint16 str);

void   shadow_mouse_read(void);
void   shadow_mouse_draw(int16 x, int16 y);

void   shadow_pixel(int16 x, int16 y, int16 colour);	/* 0x0A95 */
void   shadow_command21(uint16 value);			/* 0x0AAA */

#endif
