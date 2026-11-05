/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"

#include <dos.h>
#include <memory.h>

#include "hw.h"
#ifdef SS_SDL
#include "ss_platform.h"
#include <time.h>
#endif

extern void pit_stop(void);             /* 0xD0A4 */
extern volatile uint16 g_pit_divisor;
extern volatile uint16 g_disp_last;
extern void (*volatile g_disp_handler)(void);

uint16 g_tick_prev_off;                          /* 0xCE84 */
uint16 g_tick_prev_seg;                          /* 0xCE86 */

#ifndef SS_SDL
extern void interrupt far tick_handler(void);  /* 0xCF55 */
#endif

/* 0xD0DF */
void install_tick_hook(void)
{
#ifdef SS_SDL
	/* Nothing to hook: the audio callback clocks the PIT handlers. */
	pit_stop();
#else
	void (interrupt far *ph)(void);
	union REGS r;
	struct SREGS s;

	pit_stop();

	r.h.ah = 0x35;
	r.h.al = 0x08;
	int86x(0x21, &r, &r, &s);
	g_tick_prev_off = r.x.bx;
	g_tick_prev_seg = s.es;

	ph = tick_handler;

	r.h.ah = 0x25;
	r.h.al = 0x08;
	r.x.dx = FP_OFF(ph);
	s.ds = FP_SEG(ph);
	int86x(0x21, &r, &r, &s);
#endif
}

uint16 g_tick_accum;                          /* 0xCE6A */
static volatile uint16 g_tick_in_handler;     /* 0xCF53 */

#ifndef SS_SDL
/* 0xCF55 */
void interrupt far tick_handler(void)
{
	union chain {
		void (interrupt far *fn)(void);
		struct { uint16 off, seg; } r;
	} prev;
	uint16 sum;

	sum = g_tick_accum + g_pit_divisor;
	g_tick_accum = sum;

	if (sum < g_pit_divisor || sum == 0) {
		prev.r.off = g_tick_prev_off;
		prev.r.seg = g_tick_prev_seg;
		prev.fn();
	} else {
		outp(0x20, 0x20);
	}

	if (g_tick_in_handler != 0)
		return;
	g_tick_in_handler = 1;

	if (g_disp_last != 0) {
		g_disp_handler();
	}

	g_tick_in_handler = 0;
}
#endif

/* 0xD108 */
void tick_uninstall(void)
{
#ifdef SS_SDL
	pit_stop();
#else
	union REGS r;
	struct SREGS s;

	pit_stop();

	r.h.ah = 0x25;
	r.h.al = 0x08;
	r.x.dx = g_tick_prev_off;
	s.ds = g_tick_prev_seg;
	int86x(0x21, &r, &r, &s);
#endif
}
