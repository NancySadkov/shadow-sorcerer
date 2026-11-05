/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Display pages and palette (see ss_ega.h).
*/

#include "ss_ega.h"
#include "ss_platform.h"

static uint8 ss_pages[SS_VRAM_PAGES][SS_SCREEN_W * SS_SCREEN_H];

/* EGA palette registers (identity: the game writes the DAC directly) and the
   DAC itself, held as 6-bit components exactly like the BIOS calls. */
static uint8 attr_pal[16] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};
static uint8 dac[256][3];

void ss_ega_reset(void)
{
	int i;

	memset(ss_pages, 0, sizeof(ss_pages));
	for (i = 0; i < 256; i++) {
		int v = i & 63;
		dac[i][0] = (uint8)(((v >> 4) & 3) * 21);
		dac[i][1] = (uint8)(((v >> 2) & 3) * 21);
		dac[i][2] = (uint8)((v & 3) * 21);
	}
}

void ss_ega_set_palette_identity(void)
{
	int i;

	for (i = 0; i < 16; i++) {
		attr_pal[i] = (uint8)i;
	}
}

uint8 *ss_page(uint16 seg)
{
	uint32 n = ((uint32)seg - 0xA000u) >> 9;   /* 0x200 paragraphs per page */

	if (n >= SS_VRAM_PAGES) {
		n = 0;
	}
	return ss_pages[n];
}

void ss_ega_compose(uint16 seg)
{
	memcpy(ss_screen, ss_page(seg), SS_SCREEN_W * SS_SCREEN_H);
}

void ss_vram_copy(uint16 dst_seg, uint16 src_seg, uint16 bytes)
{
	uint32 rows = (uint32)bytes / 40u;

	if (rows > SS_SCREEN_H) {
		rows = SS_SCREEN_H;
	}
	memcpy(ss_page(dst_seg), ss_page(src_seg), (size_t)rows * SS_SCREEN_W);
}

void ss_ega_set_dac(int index, int r, int g, int b)
{
	if (index < 0 || index > 255) {
		return;
	}
	dac[index][0] = (uint8)(r & 0x3F);
	dac[index][1] = (uint8)(g & 0x3F);
	dac[index][2] = (uint8)(b & 0x3F);
}

void ss_ega_color(int index, int *r, int *g, int *b)
{
	uint8 *c;

	if (index < 0) {
		index = 0;
	}
	index &= 0x0F;
	c = dac[attr_pal[index]];
	*r = c[0];
	*g = c[1];
	*b = c[2];
}
