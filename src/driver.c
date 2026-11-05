/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"

#define SERVICE_INIT		0x06b8
#define SERVICE_REGISTER	0x09b9
#define SERVICE_LOOKUP		0x0a3d
#define SERVICE_REPORT5		0x0a49
#define SERVICE_REPORT_LONG	0x0b28
#define SERVICE_REPORT3		0x0b34
#define SERVICE_FINISH		0x0b40

#include "shadow.h"
#ifdef SS_SDL
#include "ss_music.h"
#endif

extern int16 g_hw_config;                     /* 0x9E14 */
extern uint16 g_mem_next;                 /* 0xA9C8 */

extern char g_play_mode;
extern uint16 g_driver_stopped;          /* 0x9E04 */

static char *const g_mode_names[4] = { 0, 0, "adlib.adv", "mt32mpu.adv" };  /* 0x98EE */

static uint32 g_fat_dest;            /* 0xA8AE */
static uint32 g_ssi_dest;            /* 0x9F1C */
static uint32 g_mode_dest;           /* 0xA986 */

/* 0xD11E */
void load_driver_assets(void)
{
	char saved_prefix;
	int32 n;
	uint16 dest;

	/* 0xD124 */
	g_driver_stopped = 0;

	saved_prefix = g_shadow_prefix;

	if (g_mode_names[g_play_mode] != 0) {
		g_shadow_prefix = g_mode_names[g_play_mode][0];
		dest = (uint16)g_mem_next;
		*(uint16 *)&g_mode_dest = 0;
		*((uint16 *)&g_mode_dest + 1) = dest;
		n = shadow_load(g_mode_names[g_play_mode], dest);
		g_mem_next += (((uint16)n >> 4) + 1);
	}

	if (g_play_mode == 2) {
		g_shadow_prefix = 'f';
		dest = (uint16)g_mem_next;
		*(uint16 *)&g_fat_dest = 0;
		*((uint16 *)&g_fat_dest + 1) = dest;
		n = shadow_load("fat.adb", dest);
		g_mem_next += (((uint16)n >> 4) + 1);
	}

	g_shadow_prefix = 's';
	dest = (uint16)g_mem_next;
		*(uint16 *)&g_ssi_dest = 0;
		*((uint16 *)&g_ssi_dest + 1) = dest;
	n = shadow_load("ssi.dat", dest);
	g_mem_next += (((uint16)n >> 4) + 1);

	g_shadow_prefix = saved_prefix;
}

#define SERVICE_CS 0x1F1Bu

void  tactical_command_panel(int16 slot, int16 command);

uint16 g_overworld_hold;          /* 0x943a */

uint16 g_elapsed_accum;
uint16 g_elapsed_step = 10;   /* 0x228c */

/* 0xA8A4..0xA8AA */

volatile uint16 g_shadow_counters[4];   /* 0xA8A4 */

/* 0xD1DC */

extern char g_play_mode;                              /* 0x9E0C */
extern void driver_stop(void);                 /* 0x9A9D */

uint16 g_drv_handle;

/* 0xD1DC */
int16 driver_play_sequence(int16 sequence)
{
#ifdef SS_SDL
	/* No MIDI driver in the port; the Ogg music stings are selected by the
	   original sequence ids. */
	ss_music_play_sequence(sequence);
	return 0;
#else
	union { void (far *fn)(void); struct { uint16 off, seg; } r; } open_drv;
	union { int16 (far *fn)(int16, int16); struct { uint16 off, seg; } r; } open_it;
	union { int32 (far *fn)(int16); struct { uint16 off, seg; } r; } name_it;
	union { void (far *fn)(int16, int16, int16, int16, int16);
	        struct { uint16 off, seg; } r; } report5;
	union { void (far *fn)(int16, int32); struct { uint16 off, seg; } r; } report_l;
	union { void (far *fn)(int16, int16, int16); struct { uint16 off, seg; } r; } report3;
	union { int16 (far *fn)(int16); struct { uint16 off, seg; } r; } finish;
	union { uint16 far *pw; struct { uint16 off, seg; } r; } rec;
	union { int32 l; struct { uint16 lo, hi; } w; } sum;

	uint16 off, seg, carry, vlo, sum_lo, sum_hi, rv;

	if (g_play_mode < 2) {
		return 0;
	}

	if (g_driver_stopped == 0) {
		driver_stop();
		g_driver_stopped = 1;

		open_drv.r.seg = SERVICE_CS;  open_drv.r.off = SERVICE_INIT;
		open_drv.fn();

		open_it.r.seg = SERVICE_CS;   open_it.r.off = SERVICE_REGISTER;
		g_drv_handle = (uint16)open_it.fn((int16)g_mode_dest,
		                                  ((uint16 *)&g_mode_dest)[1]);

		name_it.r.seg = SERVICE_CS;   name_it.r.off = SERVICE_LOOKUP;
		sum.l = name_it.fn((int16)g_drv_handle);
		rec.r.off = sum.w.lo;
		rec.r.seg = sum.w.hi;

		report5.r.seg = SERVICE_CS; report5.r.off = SERVICE_REPORT5;
		report5.fn((int16)g_drv_handle, rec.pw[4], rec.pw[5],
		             rec.pw[6], rec.pw[7]);
	}

	sum_lo = ((uint16 *)&g_ssi_dest)[0];
	sum_hi = ((uint16 *)&g_ssi_dest)[1];

	vlo = (uint16)sequence << 2;
	off = sum_lo + vlo;
	carry = (off < sum_lo) ? 1u : 0u;
	seg = (carry << 12) + sum_hi;

	rec.r.off = off;
	rec.r.seg = seg;

	sum_lo = (uint16)(sum_lo + rec.pw[0]);
	carry = (sum_lo < rec.pw[0]) ? 1u : 0u;
	sum_hi = (uint16)(sum_hi + rec.pw[1] + carry);

	sum.w.lo = sum_lo;
	sum.w.hi = sum_hi;

	report_l.r.seg = SERVICE_CS;  report_l.r.off = SERVICE_REPORT_LONG;
	report_l.fn((int16)g_drv_handle, sum.l);

	if (g_play_mode == 2) {
		report3.r.seg = SERVICE_CS; report3.r.off = SERVICE_REPORT3;
		report3.fn((int16)g_drv_handle, ((uint16 *)&g_fat_dest)[0],
		             ((uint16 *)&g_fat_dest)[1]);
	}

	finish.r.seg = SERVICE_CS;  finish.r.off = SERVICE_FINISH;
	rv = (uint16)finish.fn((int16)g_drv_handle);
	return (int16)rv;
#endif
}
