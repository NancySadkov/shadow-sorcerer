/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"

#include <dos.h>
#include "shadow.h"
#include "structs.h"
#include "game.h"

extern int16 g_hw_config;
extern uint16 g_mem_next;
extern volatile uint16 g_shadow_counters[4];   /* 0xA8A4..0xA8AA */
#define g_shadow_event_count g_shadow_counters[0]

/* 0xA9C6 */

extern uint16 g_rng_seed;       /* 0xA9C6 */
uint8 g_pixel_order[256];      /* 0x9E16 */

static uint8 reverse_bits(uint8 v)
{
	int16 i;
	uint16 r = 0;

	for (i = 0; i < 8; i++) {
		r = (r << 1) | (v & 1);
		v >>= 1;
	}
	return (uint8)r;
}

static uint8 reverse_fields(uint8 v)
{
	return (uint8)(((v & 3) << 6) |
	                       (((v >> 2) & 3) << 4) |
	                       (((v >> 4) & 3) << 2) |
	                       ((v >> 6) & 3));
}

/* 0x9AD8 */
int16 startup_checks(void)
{
	union REGS r;
	uint16 i;

	r.h.ah = 0x2C;
	int86(0x21, &r, &r);
	g_rng_seed = r.x.dx;

	r.h.ah = 0x19;
	int86(0x21, &r, &r);
	if (r.h.al < 2)
		g_disk_class = 0;

	r.x.bx = ALLOC_PROBE_PARAGRAPHS;
	r.h.ah = 0x48;
	int86(0x21, &r, &r);
	if (r.x.cflag) {
		r.h.ah = 0x48;
		int86(0x21, &r, &r);
	}
	g_mem_next = r.x.ax;

	if (g_hw_config != HW_CONFIG_20) {
		r.x.ax = 1;
		int86(SHADOW_VECTOR, &r, &r);
	}

	r.x.si = (uint16)&g_shadow_counters[0];
	r.x.ax = 0xD;
	int86(SHADOW_VECTOR, &r, &r);

	if (g_hw_config == HW_CONFIG_20) {
		for (i = 0; i < 256; i++)
			g_pixel_order[i] = reverse_fields((uint8)i);
	} else {
		for (i = 0; i < 256; i++)
			g_pixel_order[i] = reverse_bits((uint8)i);
	}

	return 0;
}
