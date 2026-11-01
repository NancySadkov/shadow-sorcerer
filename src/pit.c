/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"

#include <dos.h>

#include "hw.h"

extern volatile uint16 g_disp_last;
extern volatile uint16 g_disp_clear;
extern volatile uint16 g_disp_flag;

volatile uint16 g_pit_divisor;            /* 0xCE6C */

/* 0xD0C9 */
void pit_reload(int16 divisor)
{
	uint16 v = (uint16)divisor;

	if (v == 0)
		v = PIT_DIVISOR_MAX;

	g_pit_divisor = v;
	outp(0x43, 0x36);
	outp(0x40, (uint8)(v & 0xFF));
	outp(0x40, (uint8)(v >> 8));
}

/* 0xD0A4 */
void pit_stop(void)
{
	outp(0x61, (uint8)(inp(0x61) & 0xFC));

	g_disp_last = 0;
	g_disp_clear = 0;
	g_disp_flag = 0;

	pit_reload(-1);
}

/* 0xD0C2 */
void speaker_toggle(void)
{
	outp(0x61, (uint8)(inp(0x61) ^ 0x02));
}
