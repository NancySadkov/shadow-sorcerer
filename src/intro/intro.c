/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "types.h"
#include "intro.h"
#include "lzhuf.h"
#include "shadow.h"

struct title_page g_page;		/* 0x4FA6 */
uint8 *g_decode_dst;			/* 0x4FA0 */
uint8 g_compressed[COMPRESSED_BUF_BYTES];	/* 0x0A36 */

int16 g_intro_mode;			/* 0x0830 */
int16 g_vga_flag;			/* 0x4FA2 */
int16 g_shadow_config;			/* 0x0832 */

/* 0x00A8 */

uint16 g_pixel_mask[16] = {
	0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100,
	0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001
};

/* 0x0096 */

uint8 g_ega_palette[17] = {	/* 0x0096 */
	0x00, 0x3B, 0x3D, 0x3F, 0x00, 0x04, 0x3C, 0x07, 0x38,
	0x03, 0x39, 0x3C, 0x06, 0x3A, 0x02, 0x3E, 0x00
};

void load_page(const char *name)
{
	g_decode_dst = (uint8 *)&g_page;
	decode_file(name);
}

void decode_file(const char *name)
{
	FILE *fp;

	fp = fopen(name, "rb");
	fread(g_compressed, 1, COMPRESSED_BUF_BYTES, fp);
	fclose(fp);
	g_lz_in = g_compressed;
	lzhuf_decode();
}

int16 page_pixel(uint16 *planes, uint16 mask)
{
	uint16 m;
	int16 colour;

	m = (uint16)((mask >> 8) | (mask << 8));
	colour = 0;
	if (planes[0] & m)
		colour += 1;
	if (planes[1] & m)
		colour += 2;
	if (planes[2] & m)
		colour += 4;
	if (planes[3] & m)
		colour += 8;
	return colour;
}

void plot_pixel(int16 x, int16 y, int16 colour)
{
	uint16 *planes;
	uint16 off;

	if (colour == -1) {
		off = (uint16)(((x & 0xFFF0) >> 1) + y * PAGE_ROW_BYTES);
		planes = (uint16 *)(g_page.image + off);
		colour = page_pixel(planes, g_pixel_mask[x & 0x0F]);
	}
	shadow_pixel(x, y, colour);
}

void dissolve_page(int16 colour)
{
	uint16 lo, hi;
	int16 x, y;

	lo = 1;
	hi = 0;
	do {
		x = (int16)(lo & 0x01FF);
		y = (int16)((lo >> 9) | (hi << 7));
		if (x < PAGE_WIDTH && y < PAGE_HEIGHT)
			plot_pixel(x, y, colour);

		if (lo & 1) {
			lo = (uint16)(((lo >> 1) | (hi << 15)) ^ 0x2000);
			hi = 1;
		} else {
			lo = (uint16)((lo >> 1) | (hi << 15));
			hi = 0;
		}
	} while (lo != 1 || hi != 0);

	plot_pixel(0, 0, colour);
	shadow_command21(2);
	if (kbhit())
		quit_intro();
}

void quit_intro(void)
{
	sound_play(0);
	shadow_command21(1);
	while (sound_channel_active(0) != 0)
		;
	sound_stop();
	exit(0);
}

void set_vga_palette(uint16 *palette)
{
	union REGS r;
	int16 i;
	uint16 v;

	for (i = 0; i < 16; i++) {
		v = palette[i];

		r.h.ah = 0x10;
		r.h.al = 0x10;
		r.x.bx = (uint16)i;
		r.h.dh = (uint8)(v << 3);
		r.h.cl = (uint8)(((v >> 8) & 0x0F) << 3);
		r.h.ch = (uint8)(((v >> 8) & 0xF0) >> 1);
		int86(0x10, &r, &r);

		r.h.ah = 0x10;
		r.h.al = 0x00;
		r.h.bh = (uint8)i;
		r.h.bl = (uint8)i;
		int86(0x10, &r, &r);
	}
}

void bios_write_pixel(int16 x, int16 y, int16 colour)
{
	union REGS r;

	r.x.bx = 0;
	r.x.cx = (uint16)x;
	r.x.dx = (uint16)y;
	r.h.ah = 0x0C;
	r.h.al = (uint8)colour;
	int86(0x10, &r, &r);
}

int main(int argc, char **argv)
{
	int16 i;
	int16 song;
	int16 cfg;

	g_intro_mode = 0;
	g_vga_flag = 0;
	for (i = 1; i != argc; i++) {
		switch (argv[i][0]) {
		case 'T':
			g_intro_mode = 1;
			break;
		case 'A':
			g_intro_mode = 2;
			break;
		case 'R':
			g_intro_mode = 3;
			break;
		case 'V':
			g_vga_flag = 1;
			break;
		default:
			break;
		}
	}

	sound_init(g_intro_mode);
	song = (g_intro_mode == 0) ? 4 : 2;

	do {
		sound_play(song);
		signal(SIGINT, quit_intro);		/* 0x01E7 */
		cfg = shadow_config();			/* 0x01F5 */
		g_shadow_config = cfg;
		shadow_video_copy(PAGE_HEIGHT);		/* 0x01FF */

		load_page("etitle1.dat");		/* 0x0205 */
		if (g_vga_flag)
			set_vga_palette(g_page.palette);
		else
			shadow_set_palette((uint16)g_ega_palette);	/* 0x0226 */
		dissolve_page(-1);

		load_page("etitle2.dat");		/* 0x0236 */
		dissolve_page(-1);
		load_page("etitle3.dat");		/* 0x024A */
		dissolve_page(-1);
		load_page("etitle4.dat");		/* 0x025E */
		dissolve_page(-1);
		load_page("etitle5.dat");		/* 0x0272 */
		dissolve_page(-1);

		if (g_vga_flag) {
			dissolve_page(0);		/* 0x0290 - fade */
			load_page("vtitle6.dat");	/* 0x0296 */
			set_vga_palette(g_page.palette);
			dissolve_page(-1);
		} else {
			load_page("etitle6.dat");	/* 0x02B6 */
			dissolve_page(-1);
		}
	} while (1);

	return 0;
}
