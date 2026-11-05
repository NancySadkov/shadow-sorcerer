/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"

#include "shadow.h"
#include "structs.h"

#define TIMER_STRIDE	30
#define TIMER_N		4

uint8 g_timers[TIMER_N * TIMER_STRIDE] = {
	  0,   0,   0,   0,  48,   0,   0,   0, 144,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,  48,   0,   0,   0, 144,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	 48,   0,   0,   0, 144,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  48,   0,
	  0,   0, 144,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  5,   0,   0,   0,   0,   0,   0,   0,
};	/* DGROUP 0x2970 */

uint16 g_hour_is_20;			/* DGROUP 0x2282 */
uint16 g_day_counter;			/* DGROUP 0x2284 */
uint16 g_clock_hour = 4;		/* DGROUP 0x2286 */
uint16 g_hour_ticks;			/* DGROUP 0x2288 */

extern uint16 g_seek_suspended;

/* 0xA061 */

extern void sprite_blit_offset(int16 x, int16 y, int16 table_index);

/* 0x5E1A */

int16 clock_advance(int16 delta)
{
	int16 i;
	int16 was_twenty = 0;

	for (i = 0; i != TIMER_N; i++) {
		uint8 *t = g_timers + i * TIMER_STRIDE;
		if (t[1] != 0)
			t[1]--;
	}

	g_seek_suspended = (g_clock_hour >= HOUR_20 || g_clock_hour <= 3) ? 1 : 0;

	if (g_clock_hour == HOUR_20)
		was_twenty = 1;

	g_hour_ticks = (uint16)(g_hour_ticks + delta);
	if ((int16)g_hour_ticks >= 60) {
		g_clock_hour = (uint16)(g_clock_hour + (int16)g_hour_ticks / 60);
		g_hour_ticks = (uint16)((int16)g_hour_ticks % 60);
		if ((int16)g_clock_hour >= HOURS_PER_DAY) {
			g_day_counter++;
			g_clock_hour = (uint16)(g_clock_hour - HOURS_PER_DAY);
		}
	}

	if (g_clock_hour == HOUR_20 && was_twenty == 0)
		g_hour_is_20 = 1;

	return (g_clock_hour == HOUR_20) ? 1 : 0;
}

/* 0x5EC9 */

void clock_draw_time(void)
{
	int16 x = 224 + ((int16)g_clock_hour << 2) + ((int16)g_hour_ticks / 15);

	sprite_blit_offset(x, 4, 0x53);
}

#include "game.h"

uint16 g_frame_debt;					/* 0x2270 - acc */
extern uint16 g_overworld_phase, g_timer_debt_a, g_timer_debt_b;
extern uint16 g_timer_debt_c, g_timer_debt_d, g_overworld_hold;
extern uint16 g_overworld_ready, g_pending_terrain;
extern int16 shadow_video_flags(int16);				/* 0x9CFA */
extern int16 tile_first_entry(void);				/* 0x628C */
extern int16 clock_advance(int16);				/* 0x5E1A */
extern int16 party_update_all(int16 advance, int16 draw);				/* 0x7807 */

/* 0x5EFF */

/* 0x5EFF */
void clock_advance_frames(int16 n)
{
	int16 i;

	for (i = 0; i != n; i++) {				/* 5F05-5F15 */
		(void)clock_advance(1);				/* 5F17-5F1E */
		(void)party_update_all(1, 1);				/* 5F21-5F29 */
	}
}

/* 0x6246 */

/* 0x6246 */
void clock_bank_timers(void)
{
	uint16 m = g_timer_debt_a;				/* 624C-624F */

	if (g_timer_debt_b > m) m = g_timer_debt_b;			/* 6252-625A */
	if (g_timer_debt_c > m) m = g_timer_debt_c;			/* 625D-6265 */
	if (g_timer_debt_d > m) m = g_timer_debt_d;			/* 6268-6270 */

	g_frame_debt += m;						/* 6273-6276 */
	g_timer_debt_d = g_timer_debt_c = g_timer_debt_b = g_timer_debt_a = 0;	/* 627A-6285 */
}

/* 0x633C */

/* 0x633C */
void clock_tick_overworld(void)
{
	int16 i;

	if (g_overworld_hold != 0)
		return;							/* 6343-634A */

	if (g_overworld_phase == 1) {
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);					/* 6354-635B */
		(void)tile_first_entry();
		effect_clear_flags();
		terrain_change();
		return;
	}

	if (g_overworld_ready == 0 && g_map_mode_char == MAP_OVERWORLD_GATE && g_map_N_event_done == 0)	/* 6369-637C */
		(void)event_raise(EVENT_OVERWORLD_READY);					/* 637E-6385 */

	(void)clock_bank_timers();
	tile_set_position();
	g_pending_terrain = 0;

	if (g_overworld_phase != 1)
		return;

	for (i = 0; i != 4; i++) {						/* 639B-63A9 */
		if (g_actor_table[i].timer != 0)
			g_actor_table[i].timer +=
				(uint8)g_frame_debt;
	}

	(void)clock_advance_frames(g_frame_debt + 30);					/* 63C3-63CD */
	g_frame_debt = 0;
}

uint16 g_find_traps_span;				/* 0xa9b8 */

/* 0x2653 */

/* 0x2653 */
void clock_hold_span(void)
{
	g_find_traps_span = g_hold_length << 1;					/* 2654-2659 */
}
