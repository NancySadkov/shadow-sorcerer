/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"

#include <dos.h>

#include "hw.h"
#include "tick.h"

extern void pit_stop(void);      /* 0xD0A4 */
extern void pit_reload(int16 div); /* 0xD0C9 */
extern void speaker_toggle(void); /* 0xD0C2 */

volatile uint16 g_tick_enabled;           /* 0x98CA */

volatile uint16 g_disp_last;              /* 0xCE7E */
volatile uint16 g_disp_index;             /* 0xCE80 */
volatile uint16 g_disp_flag;              /* 0xCE7C */
volatile uint16 g_disp_state;             /* 0xCE76 */
volatile uint16 g_disp_control;           /* 0xCE6E */
volatile uint16 g_disp_clear;             /* 0xCE66 */
#ifdef SS_SDL
volatile const uint8 *g_disp_ptr;         /* 0xCE78 */
volatile const uint8 *g_disp_ptr2;        /* 0xCE7A */
#define G_DISP_SET(p) (g_disp_ptr = (const uint8 *)(p))
#else
volatile uint16 g_disp_ptr;               /* 0xCE78 */
volatile uint16 g_disp_ptr2;              /* 0xCE7A */
#define G_DISP_SET(p) (g_disp_ptr = (uint16)(p))
#endif
volatile uint16 g_noise_shift;            /* 0xCE68 */
volatile uint16 g_noise_mask;             /* 0xCE70 */
volatile uint16 g_noise_or;               /* 0xCE72 */
volatile uint16 g_noise_period;           /* 0xCE74 */
void (*volatile g_disp_handler)(void);      /* 0xCE82 */

/* 0xD07F */

/* 0xD07F */
void tick_handler_type1(void)
{
	const struct tone_event *ev =
		(const struct tone_event *)(uint8 *)g_disp_ptr;
	uint16 w = ev->divisor;

	G_DISP_SET(ev + 1);

	if (w == 0) {
		pit_stop();
		return;
	}

	w |= SOUND_DIVISOR_FLAG;
	pit_reload((int16)w);
	speaker_toggle();
}

/* 0xD003 */
void tick_handler_type0(void)
{
	uint16 ax;

	ax = g_disp_clear;

	if (ax == 0) {
		const struct noise_event *ev =
			(const struct noise_event *)(uint8 *)g_disp_ptr;
		g_noise_mask = ev->mask;
		g_noise_or   = ev->or_value;
		g_noise_period = ev->period;
		G_DISP_SET(ev + 1);
		ax = 0;
	}

	ax = ax + 1;

	if (ax != g_noise_period) {
		uint16 sr;

		g_disp_clear = ax;
		sr = g_noise_shift + NOISE_LFSR_FEEDBACK;
		sr = (uint16)((sr >> 3) | (sr << 13));
		g_noise_shift = sr;
		pit_reload((int16)((sr & g_noise_mask) | g_noise_or));
		if (g_noise_mask != NOISE_MASK_ALL)
			speaker_toggle();
	} else {
		g_disp_control = g_disp_control - 1;
		if (g_disp_control == 0)
			pit_stop();
		g_disp_clear = 0;
	}
}

/* 0xCFA4 */
void tick_handler_type2(void)
{
	const struct note_event *ev;
	uint16 ax;

top:
	g_disp_state = g_disp_state - 1;
	if (g_disp_state != 0)
		return;

	ev = (const struct note_event *)(uint8 *)g_disp_ptr;
	ax = ev->duration;
	g_disp_state = ax;

	if (ax == 0) {
		if (g_disp_flag != 0) {
			g_disp_ptr = g_disp_ptr2;
			g_disp_state = 1;
			goto top;
		}
		pit_stop();
		return;
	}

	outp(0x43, 0xB6);

	ax = ev->frequency;
	G_DISP_SET(ev + 1);

	if (ax == 0) {
		uint8 v = (uint8)(inp(0x61) & 0xFC);
		outp(0x61, v);
		v = (uint8)(v - 1);
		outp(0x42, v);
		outp(0x42, v);
	} else {
		outp(0x42, (uint8)(ax & 0xFF));
		outp(0x42, (uint8)(ax >> 8));
		outp(0x61, (uint8)(inp(0x61) | 0x03));
	}
}

