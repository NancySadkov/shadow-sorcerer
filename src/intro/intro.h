/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#ifndef INTRO_H
#define INTRO_H

#include "types.h"

#define PAGE_WIDTH	320
#define PAGE_HEIGHT	200
#define PAGE_ROW_BYTES	160
#define PAGE_IMAGE_BYTES (PAGE_ROW_BYTES * PAGE_HEIGHT)
#define PAGE_MAX_BYTES	0x8300

struct title_page {
	uint16	reserved;
	uint16	palette[16];
	uint8	image[PAGE_IMAGE_BYTES];
	uint8	tail[32];
};

extern struct title_page g_page;	/* 0x4FA6 - dec */

/* DGROUP 0x4FA0 */
extern uint8 *g_decode_dst;

/* DGROUP 0x04D0 */
extern uint16 g_lz_bitbuf;
extern uint8  g_lz_bitcnt;

/* DGROUP 0xF0F4 */

extern uint8 *g_lz_in;

/* DGROUP 0x00C8 */
extern uint32 g_lz_size;

/* DGROUP 0x0A36 - 16000 */

#define COMPRESSED_BUF_BYTES 16000
extern uint8 g_compressed[COMPRESSED_BUF_BYTES];	/* 0x0A36 */

/* DGROUP 0x00A8 */

extern uint16 g_pixel_mask[16];	/* 0x00A8 */

/* DGROUP 0x0096 */

extern uint8 g_ega_palette[17];

/* DGROUP 0x0830 */

extern int16 g_intro_mode;		/* 0x0830 */
/* DGROUP 0x04FA2 */
extern int16 g_vga_flag;		/* 0x4FA2 */
/* DGROUP 0x0832 */
extern int16 g_shadow_config;		/* 0x0832 */

/* 0x0010 */
void load_page(const char *name);

/* 0x0026 */

void plot_pixel(int16 x, int16 y, int16 colour);

/* 0x007e */

void dissolve_page(int16 colour);

/* 0x0119 */

void quit_intro(void);

/* 0x09be - dec */

void decode_file(const char *name);

/* 0x0a1a */

int16 page_pixel(uint16 *planes, uint16 mask);

/* 0x0a54 */

void set_vga_palette(uint16 *palette);

/* 0x0a95 */

void shadow_pixel(int16 x, int16 y, int16 colour);
void shadow_command21(uint16 value);

/* 0x0a00 */

void bios_write_pixel(int16 x, int16 y, int16 colour);

#include "sound.h"

#endif
