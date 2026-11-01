/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"
#ifndef SHADOW_H
#define SHADOW_H

extern char g_shadow_prefix;

#define SHADOW_VECTOR	0xF8

#define SHADOW_SEL_VIDEO_COPY		0x0000
#define SHADOW_SEL_PAGE_FLIP		0x0001
#define SHADOW_SEL_SET_PALETTE		0x0002
#define SHADOW_SEL_SPRITE		0x0008
#define SHADOW_SEL_LOAD			0x0009
#define SHADOW_SEL_KEY_POLL		0x000a
#define SHADOW_SEL_TEXT			0x000b
#define SHADOW_SEL_DIRECTION_READ	0x000c
#define SHADOW_SEL_SCREEN_REFRESH	0x0010
#define SHADOW_SEL_SAVE			0x0011
#define SHADOW_SEL_GETKEY		0x0015
#define SHADOW_SEL_CONFIG		0x0017
#define SHADOW_SEL_STREAM_READ		0x0018
#define SHADOW_SEL_REGION_CLEAR		0x0019
#define SHADOW_SEL_RECT_FILL		0x001a
#define SHADOW_SEL_MOUSE_READ		0x001c
#define SHADOW_SEL_VIDEO_FLAGS		0x001d
#define SHADOW_SEL_VIDEO_PAGE		0x001e
#define SHADOW_SEL_SCREEN_RESTORE	0x001f
#define SHADOW_SEL_MOUSE_DRAW		0x0022

int16 shadow_poll(void);

int16 shadow_config(void);

int32 shadow_load(const char *name, uint16 dest_para);

#define SHADOW_PAGE_DEFAULT	0
#define SHADOW_PAGE_PREVIOUS	(-1)
#define SHADOW_PAGE_MAP		7

#define SHADOW_MASK_CLEAR_ALL	0x00
int16 shadow_video_page(int16 page);

int16 shadow_region_clear(int16 and_mask, int16 column, int16 row,
                          int16 width_bytes, int16 height_rows);

int16 shadow_video_copy(uint16 lines);

int16 shadow_video_flags(int16 flags);

int16 shadow_key_read(void);
void shadow_mouse_read(void);
void shadow_mouse_draw(uint16 x, uint16 y);
int16 shadow_screen_refresh(void);
int16 shadow_screen_restore(uint16 selector);
int16 shadow_page_flip(void);
int16 shadow_rect_fill(uint16 x, uint16 y, uint16 w, uint16 h);
int16 shadow_getkey_noecho(void);
int16 shadow_save(void);
int16 shadow_set_palette(uint16 palette_offset);
int16 shadow_text_draw(int16 attr, int16 col, int16 row, const char *text);
int16 shadow_asset_load(char *name, uint16 paragraph);
int16 shadow_stream_read(uint16 offset);
int16 shadow_direction_read(void);
void shadow_sprite(int16 selector, int16 kind, int16 x, int16 y,
                   int16 data_off, uint16 segment);

void shadow_text(int16 colour, int16 col, int16 row, uint16 str);

#define TEXT_COLOUR_NORMAL	0xFF04
#define TEXT_COLOUR_HIGHLIGHT	0xFF03
#define TEXT_COLOUR_HEADING	0xFF0F
#define TEXT_COLOUR_DIM		0xFF06
#define TEXT_COLOUR_CHOSEN	0xFF02

#define SHADOW_BLIT_FLUSH	0x4000
#define SHADOW_BLIT_RESET	0x8000

int16 rnd(int16 n);

int16 wander_step(int16 kind, uint8 *p);

extern struct party_rec g_party_table[];
#define PARTY_REC(i)	(&g_party_table[i])

#endif
