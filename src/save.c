/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"

#include <dos.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "shadow.h"
#include "game.h"
#include "structs.h"

/* 0x9C04 */

struct save_block {
	uint16 offset;
	uint8 *data;
	uint16 size;
};

#ifdef SS_SDL
static void save_image_copy(uint8 *block, int16 to_block)
#else
static void save_image_copy(uint8 far *block, int16 to_block)
#endif
{
	static const struct save_block parts[] = {
		{ SAVE_OFFSET(SAVE_IMAGE_BASE), (uint8 *)(g_game_state.kill_count + 5), SAVE_SIZE_GAME_STATE },
		{ SAVE_OFFSET(TRAIL_TABLE_ADDR), (uint8 *)g_trail, SAVE_SIZE_TRAIL },
		{ SAVE_OFFSET(TRAIL_HEAD_ADDR), (uint8 *)&g_trail_head, SAVE_SIZE_TRAIL_HEAD },
		{ SAVE_OFFSET(HERO_SLOT_TABLE_ADDR), (uint8 *)g_hero_slots, SAVE_SIZE_HERO_SLOTS },
		{ SAVE_OFFSET(SITE_TABLE_ADDR), (uint8 *)g_site_table, SAVE_SIZE_SITES },
		{ SAVE_OFFSET(ACTOR_TABLE_ADDR), (uint8 *)g_actor_table, SAVE_SIZE_ACTORS },
		{ SAVE_OFFSET(PARTY_TABLE_ADDR), (uint8 *)g_party_table, SAVE_SIZE_PARTIES },
		{ SAVE_OFFSET(MAP_TILE_ADDR), g_map_tile, SAVE_SIZE_MAP_TILE },
		{ SAVE_OFFSET(MAP_RECORDS_ADDR), (uint8 *)g_map_records, SAVE_SIZE_MAP_RECORDS },
		{ SAVE_OFFSET(SAVE_IMAGE_SPLIT), (uint8 *)g_character_table, SAVE_SIZE_CHARACTERS },
	};
	uint16 i, j, n = sizeof(parts) / sizeof(parts[0]);

	for (i = 0; i < n; i++) {
		uint8 *b = block + parts[i].offset;
		uint8 *p = parts[i].data;

		if (to_block)
			for (j = 0; j < parts[i].size; j++)
				b[j] = p[j];
		else
			for (j = 0; j < parts[i].size; j++)
				p[j] = b[j];
	}
}

/* The DOS build lays the live game-state globals over the copy fields inside
   g_game_state at the same DGROUP addresses, so the save block transfers both
   at once.  In the port they are separate C objects; copy them explicitly. */
#ifdef SS_SDL
extern uint16 g_screen_hold, g_state_countdown, g_timer_debt_a, g_timer_debt_b;
extern uint16 g_timer_debt_c, g_timer_debt_d, g_frame_debt, g_move_speed_shift;
extern uint16 g_map_N_event_done, g_seek_suspended, g_hour_is_20, g_day_counter;
extern uint16 g_clock_hour, g_hour_ticks, g_elapsed_accum, g_elapsed_step;
extern uint16 g_overworld_phase;
extern uint8 g_map_mode_char;

#define SAVE_SYNC(field, live) \
	do { if (to_image) g_game_state.field = (live); \
	     else (live) = g_game_state.field; } while (0)

static void save_state_sync(int16 to_image)
{
	SAVE_SYNC(overworld_phase_image, g_overworld_phase);
	SAVE_SYNC(screen_hold_image, g_screen_hold);
	SAVE_SYNC(state_countdown_image, g_state_countdown);
	SAVE_SYNC(timer_debt_image[0], g_timer_debt_a);
	SAVE_SYNC(timer_debt_image[1], g_timer_debt_b);
	SAVE_SYNC(timer_debt_image[2], g_timer_debt_c);
	SAVE_SYNC(timer_debt_image[3], g_timer_debt_d);
	SAVE_SYNC(frame_debt_image, g_frame_debt);
	SAVE_SYNC(move_speed_shift_image, g_move_speed_shift);
	SAVE_SYNC(map_N_event_done_image, g_map_N_event_done);
	SAVE_SYNC(map_mode_char_image, g_map_mode_char);
	SAVE_SYNC(seek_suspended_image, g_seek_suspended);
	SAVE_SYNC(hour_is_20_image, g_hour_is_20);
	SAVE_SYNC(day_counter_image, g_day_counter);
	SAVE_SYNC(clock_hour_image, g_clock_hour);
	SAVE_SYNC(hour_ticks_image, g_hour_ticks);
	SAVE_SYNC(elapsed_accum_image, g_elapsed_accum);
	SAVE_SYNC(elapsed_step_image, g_elapsed_step);
}

#undef SAVE_SYNC
#else
#define save_state_sync(to_image) ((void)0)
#endif

static void save_image_transfer(int16 to_block)
{
#ifdef SS_SDL
	uint8 *block = SS_PTR(uint8, g_mem_next, 0);
#else
	union {
		uint8 far *p;
		struct { uint16 off, seg; } r;
	} block;
#endif
	uint16 i;

#ifdef SS_SDL
	if (to_block)
		for (i = 0; i < SAVE_IMAGE_SIZE; i++)
			block[i] = 0;

	save_image_copy(block, to_block);
#else
	block.r.off = 0;
	block.r.seg = (uint16)g_mem_next;

	if (to_block)
		for (i = 0; i < SAVE_IMAGE_SIZE; i++)
			block.p[i] = 0;

	save_image_copy(block.p, to_block);
#endif
}

/* 0x9C6A */

int16 save_restore(char *pattern)
{
#ifdef SS_SDL
	/* SEL 0xLOAD takes the name as-is; only the game's own loader rewrites
	   the first letter with the asset prefix. */
	if (ss_load_asset(pattern, g_mem_next) == 0)
		return 0;
	save_image_transfer(0);
	save_state_sync(0);
	return -1;
#else
	union REGS r;

	r.x.dx = FP_OFF(pattern);
	r.x.bx = (uint16)g_mem_next;
	r.x.ax = SHADOW_SEL_LOAD;
	int86(SHADOW_VECTOR, &r, &r);				/* 9C79-9C7C */
	if (r.x.ax == 0)					/* 9C7E-9C80 */
		return 0;
	save_image_transfer(0);				/* 9C82-9C9E */
	save_state_sync(0);
	return -1;
#endif
}

/* 0x9C04 */

int16 save_pack(char *pattern)
{
#ifdef SS_SDL
	char path[512];
	FILE *f;

	save_state_sync(1);
	save_image_transfer(1);
	ss_path_for(pattern, path, sizeof(path));
	f = fopen(path, "wb");
	if (f == NULL)
		return -1;
	if (fwrite(SS_PTR(uint8, g_mem_next, 0), 1, SAVE_IMAGE_SIZE, f) != SAVE_IMAGE_SIZE) {
		fclose(f);
		return -1;
	}
	fclose(f);
#ifdef __EMSCRIPTEN__
	EM_ASM({ FS.syncfs(false, function (err) { if (err) console.error('save sync failed', err); }); });
#endif
	return 0;
#else
	union REGS r;

	r.x.dx = FP_OFF(pattern);
	save_state_sync(1);
	save_image_transfer(1);				/* 9C0D-9C27 */
	r.x.bx = (uint16)g_mem_next;
	r.x.cx = SAVE_IMAGE_SIZE;
	r.x.ax = SHADOW_SEL_SAVE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
#endif
}
