/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"

#include <dos.h>
#include <stdio.h>
#include "shadow.h"
#include "game.h"
#include "structs.h"

/* 0x3A4 */
/* DGROUP 0x2468 */

struct site_rec g_site_table[SITE_COUNT] = {
	{  5134,     4,      0,      0,     0,     4,     0,   130 },
	{  5134,     6,      0,      0,     0,     4,     0,   130 },
	{  5136,     5,      0,      0,     0,     4,     0,   130 },
	{    39,    81,     80,     80,     0,     4,     0,   642 },
	{    18,     7,      0,      0,     0,     5,     0,   130 },
	{  5134,    31,     20,     65,     0,     4,     0,   129 },
	{  5134,    35,     76,     20,     0,     4,     0,   129 },
	{  5134,    55,     76,     20,     0,     4,     0,   129 },
	{     3,    11,     92,     65,     0,     8,     0,   129 },
	{  5135,    12,     92,     65,     0,     4,     0,   641 },
	{     3,    13,     92,     65,     0,     8,     0,   129 },
	{  5134,    13,     12,     14,     0,     4,     0,   129 },
	{    39,    14,     12,     14,     0,     4,     0,   641 },
	{    12,    15,     76,     20,     0,     8,     0,   641 },
	{     4,    19,     12,     21,     0,     8,     0,   129 },
	{    54,    19,     81,     11,     0,     8,     0,   641 },
	{     3,    19,     81,     11,     0,     4,     0,   129 },
	{  5135,    19,     81,     11,     0,     4,     0,   641 },
	{    12,    19,     10,     20,     0,     4,     0,   129 },
	{    43,    22,    128,     72,     0,   150,     0,    97 },
	{    44,     0,     13,     33,     0,     1,     0,   132 },
	{    25,     0,     13,     33,     0,     4,     0,   132 },
	{    51,     0,     13,     33,     0,     4,     0,   644 },
	{  5134,     0,     45,     19,     0,     4,     0,   132 },
	{     8,    23,     15,     44,     0,     4,     0,   641 },
	{    12,    25,     15,     51,     0,     8,     0,   129 },
	{    43,    68,     80,     64,     0,   150,     0,    97 },
	{    53,     0,     83,     12,     0,     1,     0,   516 },
	{     3,     0,     83,     12,     0,     8,     0,   132 },
	{  5134,     0,     83,     12,     0,     4,     0,     4 },
	{    45,    29,     21,     19,     0,     1,     0,   129 },
	{    39,    33,     62,     15,     0,     4,     0,   641 },
	{    51,    36,     62,     15,     0,     4,     0,   641 },
	{    37,    39,    136,     24,     0,   150,     0,     1 },
	{    40,    41,     70,     15,     0,     8,     0,   641 },
	{    39,    42,     39,     29,     0,     4,     0,   641 },
	{  5135,    42,     22,     13,     0,     4,     0,   641 },
	{    43,    47,     23,     10,     0,   150,     0,    97 },
	{    46,     0,      0,      0,     0,     1,     0,   644 },
	{    47,    49,     30,     57,     0,     1,     0,   129 },
	{    49,    49,     80,     80,     0,     2,     0,   641 },
	{    43,    52,     24,     26,     0,   150,     0,    97 },
	{    10,     0,      0,      0,     0,    12,     0,   132 },
	{    39,     0,     13,     10,     0,     4,     0,   132 },
	{    54,    54,     13,     10,     0,     4,     0,   641 },
	{     3,    54,     10,     64,     0,     8,     0,   129 },
	{     8,    55,     16,     64,     0,     4,     0,   897 },
	{    48,    54,     27,     16,     0,     1,     0,   129 },
	{    76,    21,    152,      8,     0,   150,     0,     1 },
	{    34,    44,    152,     72,     0,     8,     0,     1 },
	{    37,    44,    152,      8,     0,   150,     0,     1 },
	{    37,    18,    112,     50,     0,   150,     0,     1 },
	{    59,     0,     30,     57,     0,     4,     0,    33 },
	{    37,    50,    100,     60,     0,   150,     0,     1 },
	{     4,    72,    112,    112,     0,     8,     0,   129 },
	{    12,    82,     96,     64,     0,     8,     0,   129 },
	{    58,     0,     17,     31,     0,     4,     0,   129 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
	{     0,     0,      0,      0,     0,     0,     0,     0 },
};

typedef char site_table_size_check[(sizeof(g_site_table) == 1280) ? 1 : -1];

/* 0x3A4 */

/* 0x03A4 */
void tile_event_clear(void)
{
	struct site_rec *r = SITE(g_site_table);
	int16 i;

	for (i = 0; i != SITE_COUNT; i++, ++r) {			/* 3BD-3C1 */
		if ((SITE_FLAG_LO(r) & 1) != 0 && SITE_MAP_BYTE(r) <= 0x0a)	/* 3C6-3D0 */
			r->id = 0;
	}
}

/* 0x60DE */

void tile_set_position(void)
{
	int16 si;

	if (g_map_mode_char == MAP_START_WEST) {
		PARTY_REC(0)->grid_x = 4;
		PARTY_REC(0)->grid_y = 8;
	} else if (g_map_mode_char == MAP_START_EAST) {
		PARTY_REC(0)->grid_x = 5;
		PARTY_REC(0)->grid_y = 8;
	}

	si = TERRAIN_WIDTH * PARTY_REC(0)->grid_y;                                    /* 610A-6111 */
	if (g_terrain_grid[PARTY_REC(0)->grid_x + si] == (uint8)g_map_mode_char) {
		tile_event_clear();
		g_pane_base_y = 4;
		g_pane_base_x = 4;
		draw_map_view();
		g_overworld_phase = 1;
		driver_play_sequence(SOUND_SEQUENCE_OVERWORLD);
	}
}

extern uint8 g_map_tile[];                /* DGROUP 0x2c92 */
extern struct party_rec g_party_table[];                /* DGROUP 0x2b48 */
extern int16 rnd(int16 n);                          /* 0x0B88 */
extern int16 event_raise(int16 v);
/* 0x13a6 */

                /* DGROUP 0x13a6 */
uint8 g_walk_grid[TERRAIN_CELLS] = {	/* DGROUP 0x1662 */
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,   18,    2,    2,    2,    2,    2,    2,   11,    3,    3,    1,
	   3,   45,   45,   45,   45,   45,   45,    0,    0,   13,   13,   14,   13,   13,   13,   12,
	  10,    3,    4,    3,    3,    3,   45,    3,   45,   51,    0,    0,    0,   14,   13,   14,
	  17,   13,   14,   15,    3,   10,    3,    4,    4,   46,   46,    3,    3,    3,   51,   45,
	   0,   13,   13,   13,    0,   13,   13,   15,    3,   10,    3,    3,    4,   46,    3,   49,
	  48,    0,   51,    0,    0,   22,   16,   13,   13,   14,   13,   15,   15,   15,   10,   10,
	   6,   46,   46,   46,   46,    0,   51,    0,    0,   22,   16,   13,   13,   13,   13,   13,
	  13,   15,   15,   62,    4,   46,   46,   47,   46,    0,   51,    0,    0,   23,    0,   13,
	   0,   13,   16,   16,   13,   15,   25,   62,    4,    4,    8,   46,   46,    0,   51,    0,
	  24,    0,   43,    0,   27,   38,   25,   16,   15,   25,   25,   62,   55,   55,    4,   56,
	  54,   52,   51,    0,    0,   40,   43,   43,    0,    0,   25,   25,   25,   25,   25,   62,
	  55,   55,   55,   56,   56,   54,   53,    0,    0,   40,    0,   43,   44,   25,   25,   26,
	  26,   25,   25,   62,   55,   56,   56,   56,   56,   56,   57,    0,    0,   40,   42,   43,
	  43,   63,    0,   25,   62,   62,   62,   62,   61,   61,   59,   59,   59,   59,   58,    0,
	   0,   40,   42,   63,   63,   64,   65,   62,   61,   61,   61,   61,   59,   59,   59,   59,
	  60,   60,    0,    0,    0,   40,   42,   63,    0,   63,   63,   65,   62,   61,   59,   59,
	  59,   59,   59,   59,   59,   60,   66,    0,    0,   40,   42,   63,    0,   63,   63,   62,
	  61,   66,   66,   66,   60,   60,   60,   60,   60,   66,   67,    0,    0,   40,   40,   40,
	  40,   40,   61,   61,   61,   66,   66,   66,   66,   66,   60,   66,   66,   66,   66,   67,
	   0,   72,   72,   72,   72,   74,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,
	  66,   66,   67,    0,    0,   73,   73,   73,   74,   66,   66,   66,   66,   66,   66,   66,
	  66,   66,   66,   66,   66,   66,   68,   69,    0,   73,   73,   74,   75,   66,   66,   66,
	  66,   66,   66,   66,   66,   66,   66,   66,   66,   70,   70,    0,    0,   73,   73,   74,
	  75,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   66,   70,   70,
	   0,   73,   74,   76,   66,   66,   66,   66,   81,   92,   82,   66,   66,   66,   66,   66,
	  66,   66,   66,   66,    0,   73,   74,   77,   66,   66,   66,   66,   66,   81,   92,   82,
	  92,   83,   66,   66,   66,   66,   71,   71,    0,   73,   74,   74,   74,   81,   81,   81,
	  81,   92,   82,   92,   83,   92,   66,   66,   66,   66,   66,    0,    0,   73,   73,   73,
	  73,   92,   92,   92,   92,   81,   92,   92,   83,   92,   88,   84,   84,   66,   66,   66,
	   0,   79,   79,   79,   90,   90,   90,   90,   92,   92,   88,   92,   92,   88,   84,   84,
	  84,   66,   66,    0,    0,   80,   80,   80,   80,   90,   90,   90,   90,   90,   92,   88,
	  88,   88,   87,   84,   84,   84,   84,    0,    0,    0,   80,   80,   80,   90,   90,   90,
	  90,   93,   88,   88,   88,   88,   87,   84,   84,   84,    0,    0,    0,    0,   80,   80,
	   0,   80,   90,   90,   91,   92,   88,   88,   88,   88,   87,   85,   84,   84,   84,    0,
	   0,   80,    0,    0,   80,   97,   97,    0,    0,   94,  109,  109,  111,   87,   84,   86,
	  86,   86,   84,    0,    0,   80,    0,    0,   96,   96,   96,   96,   96,   95,    0,    0,
	 110,  112,   87,   87,    0,    0,    0,    0,    0,   80,   80,   98,   99,  100,   99,  101,
	 102,  105,  108,  108,  112,  114,  114,  114,  114,  114,  115,    0,    0,    0,   80,   98,
	  99,  100,   99,  102,  102,  104,  105,  107,  108,    0,    0,    0,  115,  115,  114,    0,
	   0,    0,   98,    0,    0,    0,  103,    0,    0,    0,  106,    0,    0,    0,  113,    0,
	   0,    0,  117,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};                   /* DGROUP 0x1662 */

/* 0xB0B8 */

void rebuild_map_tail(void)
{
	int16 i;

	for (i = TERRAIN_CELLS - 2 * TERRAIN_WIDTH; i != TERRAIN_CELLS; i++) {
		if ((g_map_tile[i] & TERRAIN_MASK) == TERRAIN_DOLLAR) {
			g_terrain_grid[i] = MAP_DRAGON_ATTACK;
			g_walk_grid[i] = 0x75;
		} else {
			g_terrain_grid[i] = 0x07;
			g_walk_grid[i] = 0x00;
		}
	}
}

/* 0xB072 */

void hide_one_dollar(void)
{
	int16 n = rnd(5) + 1;
	int16 i;

	for (i = 0; i != TERRAIN_CELLS; i++) {
		if (g_map_tile[i] == TERRAIN_DOLLAR) {
			if (--n == 0)
				continue;
			g_map_tile[i] = 0x08;
		}
	}
	rebuild_map_tail();
}

void creature_behavior_round(void);					/* 0x7D48 */
/* 0x7EE5 */

void party_upkeep_charge(void)
{
	struct party_rec *rec;
	int16 i;

	for (i = PARTY_REFUGEES, rec = PARTY_REC(PARTY_REFUGEES); i != PARTY_FOLLOWER_LEADER; i++, rec++) {
		int16 cnt = 0;
		int16 j;

		rec->attr &= 0xfb;
		rec->attr &= 0xef;
		if (i == PARTY_REFUGEES) {
			for (j = 4; j != HERO_SLOT_COUNT; j++) {
				if ((g_hero_slots[j].state & RECT_STATE_RETIRING_MASK) == 0)
					cnt++;
			}
		}
		rec->score = (uint16)((int16)rec->score
			- (int16)(rec->refugees + cnt));	/* 7F4A-7F50 */
		if ((int16)rec->score < 0) {
			rec->score = 0;
			rec->cooldown = 0;
			rec->attr |= 0x04;
		}
	}
}

/* 0x4771 */

uint16 g_hold_length;					/* 0x9f26 */
/* 0x9f26 */
void terrain_change(void);					/* 0x5F32 */
void hero_slot_select(int16 idx);					/* 0x1FA4 */
/* 0x0010 */

int16 region_loaded(int16 a)
{
	int16 i;

	for (i = 0; i != 4; i++)
		if ((int16)(uint8)g_hero_slots[i].state == a)
			return 1;
	return 0;
}

/* 0x628C */

int16 tile_first_entry(void)
{
	int16 cell;
	int16 v;
	int16 other;

	cell = TERRAIN_WIDTH * PARTY_REC(0)->grid_y + PARTY_REC(0)->grid_x;
	if (g_map_tile[cell] & 0x40)
		return 0;
	g_map_tile[cell] |= 0x40;
	v = (int8)g_walk_grid[cell];
	if (v == 4) {
		other = region_loaded(5);
		v = (region_loaded(7) | other) ? 4 : 5;		/* 62D0-62E4 */
	}
	if (v == 6) {
		other = region_loaded(8);
		v = (region_loaded(4) | other) ? 6 : 7;		/* 62FB-630F */
	}
	shadow_video_page(SHADOW_PAGE_DEFAULT);
	show_text_panel(v);
	shadow_video_page(SHADOW_PAGE_PREVIOUS);
	return 1;
}

uint16 g_cursor_tile_x;				/* 0xa9cc */
uint16 g_cursor_tile_y;				/* 0xa9ce */

/* 0x5B50 */

/* 0x5B50 */
void camera_publish_cells(int16 a)
{
	struct actor_rec *w;
	int16 mx2, my, si, col, row;

	if (g_mouse_y < 8)				/* 5B57-5B5E */
		return;

	RECT_STATE_HI(&g_hero_slot_boxes[g_selected_hero_slot]) &= 0x7f;		/* 5B61-5B6F */

	w = &g_actor_table[g_selected_hero_slot];			/* 5B74-5B7E */
	mx2 = (int16)g_mouse_x;
	my = (int16)g_mouse_y - 0x28;			/* 5B87-5B8D */

	si = mx2 / 2;
	col = (si - my + 0x4f) >> 4;			/* 5B9A-5BA7 */
	g_cursor_tile_x = col;
	row = (my + si - 0x4f) >> 4;			/* 5BAA-5BB7 */
	g_cursor_tile_y = row;

	if (w->state != 0 && a != 0)			/* 5BBA-5BC6 */
	{
		w->pixel_x = (col << 4) + 8;		/* 5BC8-5BD0 */
		w->pixel_y = (row << 4) + 8;		/* 5BD3-5BDE */
		w->mode = 1;
		w->impulse = 0;
		w->motion_state = ACTOR_MOTION_GOTO;
	}

	if (col < 0 || col > 9 || row < 0 || row > 9)	/* 5BF6-5C0C */
		return;

	sprite_draw_dispatch((col + row) << 4,
	         ((row - col) << 3) + 0x70, SPRITE_CELL_CURSOR);
}

/* 0x0FA8 */
const uint8 g_reveal_cells_n_event[26] = {
	9, 20, 10, 21, 12, 21, 9, 22, 11, 22, 10, 23, 11, 23,
	9, 24, 10, 25, 9, 26, 9, 27, 9, 28, 0, 0,
};

/* 0x0FC2 */
const uint8 g_reveal_cells_key_ring[8] = {
	18, 9, 3, 21, 15, 27, 0, 0,
};

/* 0x706E */

void reveal_cells(const uint8 *cells)
{
	const uint8 *p = cells;
	int16 x, y;

	for (;;) {
		x = (int16)(int8)*p++;
		y = (int16)(int8)*p++;
		if (x == 0)
			break;
		g_map_tile[x + TERRAIN_WIDTH * y] |= TILE_VISIBLE;
	}
}

/* 0x5321 */

int16 setting_apply(int16 a)
{
	int16 n = a;
	int16 saved[20];
	int16 *slot = saved;
	int16 retired = 0;
	int16 moved = 0;
	int16 i;
	uint8 *src;
	uint8 *dst;

	src = (uint8 *)(g_hero_slots + a);
	dst = src;

	for (i = a; i != HERO_SLOT_COUNT; i++, src += 10)		/* 5359-53A7 */
	{
		if (RECT(src)->state & RECT_STATE_RETIRING_MASK)
		{
			*slot++ = RECT(src)->state;
			retired++;
			RECT(src)->state = 0x17;
		}
		if (RECT(src)->state != HERO_SLOT_NONE)
		{
			moved = 1;
			RECT(dst)->state = RECT(src)->state;
			dst += 10;
			n++;
		}
	}

	slot = saved;
	for (i = 0; i != retired; i++, dst += 10)
		RECT(dst)->state = (uint16)*slot++;

	n += retired;

	for (i = n; i != HERO_SLOT_COUNT; i++, dst += 10)
		RECT(dst)->state = 0x17;

	return moved;
}

/* 0x63DB */

int16 move_view(void)
{
	int16 i;

	g_pending_terrain = 0;
	for (i = 0; i != HERO_SLOT_COUNT; i++) {
		if (g_hero_slots[i].state > 0x16)
			continue;
		(void)event_raise(EVENT_VIEW_MOVED);
		PARTY_REC(0)->grid_x = PARTY_REC(PARTY_REFUGEES)->grid_x;              /* 6416-6419 */
		PARTY_REC(0)->grid_y = PARTY_REC(PARTY_REFUGEES)->grid_y;              /* 641C-641F */
		g_map_mode_char = g_terrain_grid[PARTY_REC(0)->grid_x + TERRAIN_WIDTH * PARTY_REC(0)->grid_y];   /* 6422-6433 */
		(void)tile_set_position();
		(void)setting_apply(0);
		load_pane_maps();
		return 1;
	}
	return 0;
}

uint16 g_stale_clear_done;					/* 0x9e08 */
uint16 g_terrain_change_done;					/* 0x9f16 */
uint16 g_window_stamp;					/* 0xa9d0 */
extern uint16 g_move_speed_shift;				/* 0x2272 */

/* 0x5F32 */

/* 0x5F32 */
void terrain_change(void)
{
	int16 i;
	struct actor_rec *qw;
	struct rect_rec *rq;
	int16 xbase, ybase, xoff;

	effect_clear_flags();

	if ((int16)g_pending_terrain > MAP_LAST_RECORD)
		g_pending_terrain = 0;

	if (g_pending_terrain != 0) {
		for (i = 0; i != 4; i++) {					/* 5F50-5F5E */
			if ((int8)g_actor_table[i].timer >= 30)	/* 5F60-5F68 */
				g_actor_table[i].timer -= 30;
			else
				g_actor_table[i].timer = 0;
			g_frame_debt += 30;
		}
		g_map_mode_char = (uint8)g_pending_terrain;			/* 5F8A-5F8D */
		(void)clock_bank_timers();
	} else if (g_overworld_phase == 1) {					/* 5F95-5F9A */
		g_map_mode_char = g_terrain_grid[PARTY_REC(0)->grid_x + TERRAIN_WIDTH * PARTY_REC(0)->grid_y];	/* 5F9C-5FAD */
	}

	g_pending_terrain = 0;
	g_move_speed_shift = 0;
	g_detect_magic_ticks = 0;
	(void)map_record_show((int16)(int8)g_map_mode_char, 1);		/* 5FBB-5FC7 */
	g_stale_clear_done = 0;
	g_terrain_change_done = 0;

	if (g_pane_base_x == 4)
		g_window_stamp = 5;						/* 5FDC-5FE3 */
	xbase = ((int16)g_pane_base_x << 4) + 8;				/* 5FE9-5FF0 */
	ybase = ((int16)g_pane_base_y << 4) + 8;				/* 5FF6-5FFE */

	for (i = 0; i != 4; i++) {					/* 6006-601F */
		qw = &g_actor_table[i];
		rq = g_hero_slots + i;

		if (i == 2)
			ybase += 0x10;
		xoff = (i & 1) ? 0x10 : 0;					/* 602E-6039 */

		qw->x = xbase + xoff;				/* 603E-6047 */
		qw->y = ybase;					/* 604A-6050 */
		qw->health = 0;					/* 6053-6056 */

		if ((rq->state & HIGH_BYTE_MASK) != 0)			/* 605A-6065 */
			qw->state = 0;
		else
			qw->state = (uint8)(i + 1);		/* 6067-606F */

		if (rq->state == HERO_SLOT_NONE)					/* 6071-6077 */
			qw->state = 0;				/* 6079-607C */

		qw->direction = (uint8)g_window_stamp;		/* 607F-6085 */
		qw->motion_state = ACTOR_MOTION_STOP;				/* 6088-608B */
		RECT_STATE_HI(&g_hero_slot_boxes[i]) &= 0x7f;				/* 608F-609C */
	}

	if (g_map_mode_char == MAP_WINDOW_BOOST) {
		qw = &g_actor_table[4];
		qw->direction = 2;
		qw->vel_x = 2;					/* 60B5-60B8 */
		qw->vel_y = 0;					/* 60BD-60C0 */
		qw->impulse = 0x28;				/* 60C5-60C8 */
		qw->mode = 1;					/* 60CC-60CF */
	}

	g_overworld_phase = 2;
}

/* 0xA17D */

uint8 g_panel_kind_hi;
uint8 g_panel_kind_lo;

/* 0x1A17 */

/* 0x1A17 */
void panel_kind_draw(void)
{
	int16 kind;
	int16 pos = (int16)PARTY_REC(0)->grid_x + TERRAIN_WIDTH * (int16)PARTY_REC(0)->grid_y;	/* 1A2F-1A3A */

	kind = ((int16)g_panel_kind_hi << 8) | (int16)g_panel_kind_lo;

	if (kind == 0) {					/* 1A3D-1A41 */
		kind = (int16)(int8)g_walk_grid[pos];
		g_map_tile[pos] |= 0x40;
	}

	if (kind == 4) {					/* 1A52-1A56 */
		int16 other = region_loaded(5);			/* 1A58-1A5F */
		kind = (region_loaded(7) | other) ? 4 : 5;		/* 1A62-1A7A */
	}

	if (kind == 6) {					/* 1A7D-1A81 */
		int16 other = region_loaded(8);			/* 1A83-1A8A */
		kind = (region_loaded(4) | other) ? 6 : 7;		/* 1A8D-1AA5 */
	}

	show_text_panel(kind);					/* 1AA8-1AAE */
}

/* 0xB3BB */

int16 spawn_descriptor(int16 request, int16 w, int16 h)
{
	struct actor_rec *slot;
	int16 i = 4;
	int16 kind = request + MONSTER_STATE_OFFSET;

	for (i = 4; i != 15; ++i) {					/* B3CB-B3E4 */
		slot = &g_actor_table[i];
		if (slot->state == 0)
			break;
	}
	if (i == 15)
		slot = &g_actor_table[15];

	slot->state = (uint8)kind;
	slot->mode = 0;
	slot->status = 0;
	slot->direction = 0;
	slot->impulse = 0;
	slot->motion_state = ACTOR_MOTION_STOP;
	slot->action_timer = 0;
	slot->action_phase = 0;
	slot->pad_08 = 0;
	slot->timer = 0;
	slot->anchor_index = 0;

	slot->countdown = (kind == ACTOR_KIND_BOZAK) ? (uint16)(rnd(10) + 10) : 0u;	/* B436-B452 */
	slot->x = (uint16)w;					/* B455-B45B */
	slot->y = (uint16)h;					/* B45E-B464 */

	slot->health = (uint8)roll_dice_amount(request - 1);		/* B467-B475 */

	if (kind == ACTOR_KIND_BOZAK)
		slot->timer = (uint8)(rnd(4) + 3);		/* B47B-B48F */
	if (kind >= ACTOR_KIND_RDRAGON)
		slot->timer = 4;
	if (kind >= ACTOR_KIND_GDRAGON)
		slot->timer = 4;
	if (kind >= ACTOR_KIND_DWARVES)
		slot->timer = 5;

	return 0;
}

/* 0x191E */

uint8 g_cell_grid[TERRAIN_CELLS] = {
	39,39,39,39,39,39,39,39,39,39,39,39,
	39,39,39,39,39,39,39,0,39,0,18,18,
	18,18,18,18,18,18,18,0,26,18,18,26,
	34,42,42,39,39,17,18,18,19,19,19,0,
	19,19,0,18,27,27,27,35,43,51,39,0,
	39,19,20,20,0,27,27,28,19,20,29,26,
	27,28,19,36,36,58,35,39,39,29,28,28,
	0,19,27,28,21,29,28,20,29,28,28,20,
	28,28,39,0,39,35,35,28,28,29,20,28,
	20,28,29,28,28,20,19,30,21,22,28,39,
	39,42,35,36,35,36,45,45,37,43,36,44,
	45,37,37,0,35,28,39,0,39,37,0,45,
	37,37,37,37,37,37,38,38,45,38,0,36,
	29,36,36,39,39,45,45,46,0,0,34,37,
	44,42,34,34,34,34,35,28,28,35,39,0,
	39,0,58,50,44,44,44,37,37,37,44,37,
	35,43,35,28,28,77,0,0,39,44,37,59,
	47,46,45,36,36,37,36,44,36,28,28,28,
	28,30,0,0,39,61,33,53,54,45,44,44,
	37,37,37,53,28,28,50,54,52,50,50,0,
	39,42,51,58,51,19,36,36,28,28,28,28,
	51,51,51,51,36,60,0,0,39,44,61,46,
	0,52,37,46,36,28,50,51,52,53,54,55,
	55,60,61,0,39,45,45,45,0,19,35,44,
	28,59,60,61,36,60,36,60,36,60,39,39,
	54,53,52,61,0,28,28,28,60,19,60,61,
	62,60,61,62,63,79,39,39,63,62,61,53,
	60,52,51,61,60,61,61,68,68,60,61,61,
	71,39,39,9,74,59,60,68,61,61,68,61,
	61,69,60,60,68,60,60,19,68,39,39,80,
	70,63,62,61,61,69,71,63,62,70,70,61,
	61,69,61,69,39,0,39,72,70,71,54,53,
	69,77,71,71,70,70,70,69,69,69,77,69,
	76,39,39,77,60,61,62,77,68,19,76,68,
	60,71,79,19,76,69,70,68,39,0,39,76,
	0,68,68,68,69,69,77,76,68,68,68,68,
	77,69,78,70,71,39,39,79,78,77,79,77,
	76,77,77,79,76,76,77,77,78,78,77,77,
	39,0,39,20,12,12,19,10,9,73,10,76,
	12,13,77,13,20,12,29,18,25,39,39,20,
	19,11,11,12,20,13,13,13,12,13,12,18,
	12,20,12,12,39,0,39,23,15,14,13,12,
	11,10,9,10,11,12,13,14,15,15,21,20,
	28,0,39,28,20,29,20,27,20,10,12,12,
	11,12,11,11,11,0,20,28,0,0,39,28,
	18,12,10,11,12,12,0,12,18,12,12,13,
	13,13,13,20,31,0,39,20,21,13,21,21,
	13,20,12,12,19,18,12,20,17,25,23,23,
	39,0,39,31,30,21,33,21,20,22,20,20,
	0,0,17,21,20,21,20,17,20,39,39,20,
	28,20,20,28,20,20,21,21,28,28,20,20,
	28,21,28,28,39,0,39,39,31,23,23,31,
	31,23,31,23,23,31,23,31,23,23,31,31,
	31,39,39,39,0,39,39,39,0,39,39,39,
	0,39,39,39,0,39,39,39,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,
};

/* 0x1DFA */

union spawn_table g_spawn_table = { {
	8,0,3,0,9,0,6,0,7,0,7,0,
	9,0,4,0,8,0,6,0,8,0,8,0,
	9,0,3,0,9,0,7,0,0,1,0,1,
	0,1,0,1,5,6,0,6,0,6,1,6,
	2,5,1,6,0,1,0,1,6,42,0,1,
	6,1,2,4,0,1,0,1,0,1,0,1,
	2,4,1,6,8,16,0,1,16,66,0,1,
	0,1,2,0,8,1,4,5,3,4,0,4,
	1,2,5,4,3,0,6,1,4,5,3,8,
	0,8,3,0,1,3,6,0,13,2,
} };

extern uint16 g_monster_bank_segments[];
int16 shadow_asset_load(char *name, uint16 seg);

/* 0x01bc */

uint16 g_special_monster_bank_loaded;				/* 0x01bc */
/* 0x0B02 */

/* 0x8B53 */

void spawn_monsters(int16 which)
{
	uint8 kind = 0;
	int16 count = 0;
	const uint16 *p = g_spawn_table.rec.position;
	int16 i, x, y;

	switch (which) {			/* 8BC5-8BD3 */
	case SPAWN_FROM_TERRAIN: {
		uint8 t = g_cell_grid[(int16)PARTY_REC(0)->grid_x + TERRAIN_WIDTH * (int16)PARTY_REC(0)->grid_y];	/* 8B64-8B75 */
		kind = t;
		count = t & 7;
		kind = (uint8)((int16)(int8)kind >> 3);
		if (count == 0) {							/* 8B83-8B8D */
			count = 1;
			kind = 0;
		}
		break;
	}
	case SPAWN_SCENE_MONSTER:									/* 8B92-8BAF */
		g_special_monster_bank_loaded = 1;
		g_monster_bank_suffix = VMON_BANK_RDRAGON;
		(void)shadow_asset_load(g_name_vmon, g_monster_bank_segments[0]);
		kind = MONSTER_REQUEST_RDRAGON;
		count = 1;
		break;
	case SPAWN_EIGHT_MONSTERS:									/* 8BB6-8BBA */
		kind = MONSTER_REQUEST_BOZAK;
		count = 8;
		break;
	default:
		break;
	}

	for (i = 0; i != count; ++i) {					/* 8BD5-8C1A */
		x = (int16)(*p++ << 4) + 8;
		y = (int16)(*p++ << 4) + 8;
		(void)spawn_descriptor((int16)kind, x, y);
	}
}

extern int16 sprite_draw_block(int16, int16, int16, int16, uint16);	/* 0x9CD3 */

/* DGROUP 0x8AB4 */

const uint8 g_presentation_scripts[479] = {

	SCRIPT_NEAR_OFFSET(10),
	SCRIPT_NEAR_OFFSET(102),
	SCRIPT_NEAR_OFFSET(202),
	SCRIPT_NEAR_OFFSET(262),
	SCRIPT_NEAR_OFFSET(442),

	SCRIPT_CLEAR(0, 0, 8, 40, 192),
	SCRIPT_BOX(4, 32, 32, 20),
	SCRIPT_VMPL(VMPL_DWARF_SCENE, 10, 40),
	SCRIPT_TEXT(TEXT_DWARF_RESCUE, 5, 144),
	SCRIPT_WAIT(3),
	SCRIPT_VMPL(VMPL_DWARF_WAKE_1, 14, 50),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DWARF_WAKE_2, 14, 50),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DWARF_WAKE_3, 14, 50),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DWARF_WAKE_4, 14, 50),
	SCRIPT_WAIT(1),
	SCRIPT_DRAIN(),
	SCRIPT_SKIP(-26),
	SCRIPT_VMPL(VMPL_DWARF_SIT_1, 16, 55),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DWARF_SIT_2, 16, 55),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DWARF_SIT_3, 16, 55),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DWARF_SIT_4, 16, 55),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DWARF_SIT_5, 16, 55),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DWARF_SIT_6, 16, 55),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DWARF_SIT_7, 16, 55),
	SCRIPT_WAIT(4),
	SCRIPT_WAIT_KEY(),
	SCRIPT_STOP(),

	SCRIPT_CLEAR(0, 0, 8, 40, 192),
	SCRIPT_BOX(4, 32, 32, 20),
	SCRIPT_VMPL(VMPL_FEAST_SCENE, 10, 40),
	SCRIPT_TEXT(TEXT_NEIDAR_JOURNAL9, 5, 144),
	SCRIPT_WAIT(3),
	SCRIPT_VMPL(VMPL_FEAST_DISH_1, 22, 50),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_FEAST_DISH_4, 22, 50),
	SCRIPT_WAIT(1),
	SCRIPT_VMPL(VMPL_FEAST_DISH_2, 22, 50),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_FEAST_DISH_3, 22, 50),
	SCRIPT_WAIT(4),
	SCRIPT_DRAIN(),
	SCRIPT_SKIP(-26),
	SCRIPT_VMPL(VMPL_FEAST_TABLE_1, 16, 72),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_FEAST_TABLE_2, 16, 72),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_FEAST_TABLE_3, 16, 72),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_FEAST_TABLE_4, 16, 72),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_FEAST_TABLE_5, 16, 72),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_FEAST_TABLE_6, 16, 72),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_FEAST_TABLE_7, 16, 72),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_FEAST_TABLE_8, 16, 72),
	SCRIPT_WAIT(4),
	SCRIPT_DRAIN(),
	SCRIPT_SKIP(-14),
	SCRIPT_STOP(),

	SCRIPT_CLEAR(0, 0, 8, 40, 192),
	SCRIPT_BOX(4, 32, 32, 20),
	SCRIPT_VMPL(VMPL_DOOR_SCENE, 10, 40),
	SCRIPT_TEXT(TEXT_FIZBAN_KEY, 5, 144),
	SCRIPT_WAIT(3),
	SCRIPT_WAIT_KEY(),
	SCRIPT_VMPL(VMPL_DOOR_OPEN_1, 16, 52),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DOOR_OPEN_2, 16, 52),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DOOR_OPEN_3, 16, 52),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DOOR_OPEN_4, 16, 57),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DOOR_OPEN_5, 16, 62),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_DOOR_OPEN_6, 16, 62),
	SCRIPT_WAIT(4),
	SCRIPT_WAIT_KEY(),
	SCRIPT_STOP(),

	SCRIPT_CLEAR(0, 0, 8, 40, 192),
	SCRIPT_BOX(4, 32, 32, 20),
	SCRIPT_VMPL(VMPL_GATE_SCENE, 10, 40),
	SCRIPT_TEXT(TEXT_QUEST_END, 5, 144),
	SCRIPT_WAIT(3),
	SCRIPT_WAIT_KEY(),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_1, 24, 96),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_2, 22, 96),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_3, 22, 93),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_4, 20, 93),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_5, 20, 89),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_6, 20, 89),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_SCENE, 10, 40),
	SCRIPT_WAIT(3),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_1, 24, 96),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_2, 22, 96),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_3, 22, 93),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_4, 20, 93),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_5, 20, 89),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_6, 20, 89),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_SCENE, 10, 40),
	SCRIPT_WAIT(3),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_1, 24, 96),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_2, 22, 96),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_3, 22, 93),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_4, 20, 93),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_5, 20, 89),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_FLOOR_6, 20, 89),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_SCENE, 10, 40),
	SCRIPT_WAIT(3),
	SCRIPT_VMPL(VMPL_GATE_DOOR_1, 18, 67),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_DOOR_2, 20, 67),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_DOOR_3, 20, 67),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_DOOR_4, 20, 67),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_GATE_DOOR_5, 20, 67),
	SCRIPT_WAIT(4),
	SCRIPT_WAIT_KEY(),
	SCRIPT_STOP(),

	SCRIPT_CLEAR(0, 0, 8, 40, 192),
	SCRIPT_BOX(4, 32, 32, 20),
	SCRIPT_TEXT(TEXT_COUNCIL_FORMED, 5, 144),
	SCRIPT_VMPL(VMPL_COUNCIL_FRAME_1, 10, 40),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_COUNCIL_FRAME_2, 10, 40),
	SCRIPT_WAIT(4),
	SCRIPT_VMPL(VMPL_COUNCIL_FRAME_3, 10, 40),
	SCRIPT_WAIT(4),
	SCRIPT_DRAIN(),
	SCRIPT_SKIP(-20),
	SCRIPT_STOP(),
};

/* 0x98AB */

void draw_presentation(int16 presentation)
{
	const uint8 *p;
	int16 quit = 0;
	int16 ev = 0;
	int16 op;

	fclose(g_text_stream);
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	load_vcin_clip(presentation);

	p = g_presentation_scripts
	  + (*(const uint16 *)(g_presentation_scripts + 2 * presentation)
	     - PRESENTATION_SCRIPT_BASE);

	g_text_stream = fopen("text.dat", "r+b");	/* 98F0-98FE */

	while (!quit) {
		op = *p++;
		if (op >= SCRIPT_OP_COUNT)
			continue;
		switch (op) {
		case SCRIPT_OP_DRAW_VMPL: {
			int16 row = *p++, column = *p++, sprite = *p++;
			(void)sprite_draw_block(sprite, 1, column, row, g_vmpl_para[0]);
			break;
		}
		case SCRIPT_OP_CLEAR_REGION: {
			int16 height = *p++, width = *p++, row = *p++, column = *p++, and_mask = *p++;
			(void)shadow_region_clear(and_mask, column, row, width, height);
			break;
		}
		case SCRIPT_OP_STOP:
			quit = 1;
			break;
		case SCRIPT_OP_WAIT_TICKS: {
			uint16 ticks = (uint16)*p++;
			g_shadow_tick_count = 0;
			while (ticks > g_shadow_tick_count)
				;
			break;
		}
		case SCRIPT_OP_WAIT_EVENT:
			while (shadow_key_read() == 0 && mouse_button_read() == 0)
				;
			break;
		case SCRIPT_OP_DRAIN_EVENT: {
			ev = shadow_key_read();
			if (ev == 0) {
				ev = mouse_button_read();
				while (mouse_button_read() != 0)
					;
			}
			break;
		}
		case SCRIPT_OP_SKIP_IF_EVENT: {
			int8 span = (int8)*p;
			if (ev == 0)
				p += span;
			else
				p += 1;
			break;
		}
		case SCRIPT_OP_DRAW_SPRITE: {
			int16 id = *p++, y = *p++, x = *p++;
			(void)sprite_draw_segment(x, y, id, g_objpic_at);
			break;
		}
		case SCRIPT_OP_DRAW_BOX: {

			int16 h = *p++, w = *p++, y = *p++, x = *p++;
			(void)draw_text_window(x, y, w, h);
			break;
		}
		case SCRIPT_OP_DRAW_TEXT: {
			int16 text_id = *p++, y = *p++, x = *p++;
			if (text_id == TEXT_NEIDAR_JOURNAL9 && g_map_mode_char != MAP_NEIDAR_VILLAGE)
				text_id = TEXT_AGHAR_FOOD_RECIPE;	/* 9A04-9A10 */
			text_resource_read(text_id);
			(void)shadow_text_draw(TEXT_COLOUR_NORMAL, x, y, (char *)&g_credits);
			break;
		}
		default:
			break;
		}
	}

	load_pane_maps();
	while (mouse_button_read() != 0)
		;
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
}

/* 0xAE3B */

/* 0xAE3B */
void cursor_set(int16 idx)
{
	uint8 *p = (uint8 *)g_actor_table + (uint16)(0x1eu * (uint16)idx);	/* AE41-AE4A */
	int16 i = idx;									/* AE4D-AE50 */

	while (i != ACTOR_COUNT) {
		ACTOR(p)->state = 0;						/* AE62-AE65 */
		i++;
		p += 0x1e;
	}
}

extern int16 abs_w(int16);						/* 0x9BC4 */

uint16 g_screen_hold;					/* 0x2264 */

#include "structs.h"

/* 0x3CED */

/* 0x3CED */
int16 choose_target(struct actor_rec *rec, int16 tag, int16 start, int16 end)
{
	int16 best = TARGET_SCORE_NONE;
	int16 best_i = 0;
	struct actor_rec *win = &g_actor_table[start];
	int16 i;

	for (i = start; i != end; i++, win++) {		/* 3D0F-3D1E */
		int16 b = win->state;			/* 3D23-3D28 */
		int16 dx, dy, d;

		if (b == 0)
			continue;				/* 3D2B-3D2F */
		if ((b & ACTOR_STATE_INACTIVE) != 0)
			continue;
		if ((rec->x | rec->y) == 0)
			continue;				/* 3D3B-3D46 */

		dx = abs_w((int16)rec->x
			- (int16)(uint16)win->x) >> 4;	/* 3D49-3D5D */
		dy = abs_w((int16)rec->y
			- (int16)(uint16)win->y) >> 4;		/* 3D60-3D77 */
		d = (int16)(((uint16)dx * (uint16)dx)
			+ ((uint16)dy * (uint16)dy));

		if (g_screen_hold != 0
		    && (int16)(int16)(uint8)g_hero_slots[b - 1].state == CHAR_TASSLEHOFF) {
			rec->anchor_index = (uint8)i;		/* 3DA9-3DB2 */
			return i;				/* 3DB2-3DB5 */
		}
		if (d < best) {
			best = d;
			best_i = i;				/* 3DBF-3DC8 */
		}
	}

	if (best != TARGET_SCORE_NONE) {					/* 3DCE-3DD3 */
		g_actor_table[best_i].anchor_index = (uint8)tag;
						   		/* 3DD5-3DE9 */
	} else {
		best_i = -1;
	}
	rec->anchor_index = (uint8)best_i;			/* 3DF0-3DF9 */
	return best_i;
}

/* 0x10EA */

uint8 g_tile_flags[56] = {
	0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01,
};

/* 0x2E98 */

/* 0x2E98 */
int16 tile_property(int16 x, int16 y)
{
	int16 cell;
	int16 cy = (int16)(int16)y >> 4;
	int16 cx = (int16)(int16)x >> 4;

	if ((int16)x < 0)
		return 0;							/* 2E9E-2EA2 */
	if ((int16)y < 0)
		return 0;							/* 2EA4-2EA8 */

	cell = (int16)(uint16)((uint16)cy * 10u + (uint16)cx);

	return (int16)(int8)g_tile_flags[
		g_map_working.grid[cell].cell.value];			/* 2ED0-2EE2 */
}

/* 0x1BDA */

int8 g_noise_table[250] = {
	0x0a, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb,
	0x0a, 0x0a, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xfb,
	0x0f, 0x0f, 0x0f, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf1,
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xfb, 0xfb, 0xf6,
	0x0a, 0x0a, 0x0a, 0x0a, 0x05, 0x05, 0x00, 0x00, 0xfb, 0xfb,
	0x0a, 0x0a, 0x0a, 0x0a, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00,
	0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb,
	0x0a, 0x0a, 0x0a, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0xf1,
	0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xfb,
	0x0a, 0x0a, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0xf6, 0xf6,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x05, 0x05, 0x05, 0x05, 0x00,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0xfb, 0xfb,
	0x0a, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xfb, 0xf1,
	0x0a, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0xf6,
	0x0f, 0x0f, 0x05, 0x05, 0xfb, 0xfb, 0xfb, 0xf6, 0xf6, 0xf1,
	0x0a, 0x05, 0x05, 0x00, 0x00, 0xfb, 0xfb, 0xf6, 0xf6, 0xf1,
	0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfb, 0xfb,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x00, 0xf1,
	0xfb, 0xfb, 0xfb, 0xfb, 0xfb, 0xfb, 0xfb, 0xfb, 0xfb, 0xfb,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x05, 0x05, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xfb, 0xfb, 0xf6, 0xf6, 0xf1, 0xf1, 0xec,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0xec,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0xf1,
	0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf1, 0xf1,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0xfb,
};

/* 0x81EC */

int16 random_cell(int16 col, int16 row)
{
	int16 jitter = rnd(10);					/* 81EE-81F4 */
	int16 off = (int16)(uint16)((uint16)row * 50u
		+ (uint16)col * 10u + (uint16)jitter - 110u);	/* 81F6-8215 */
	int16 spread = ROLL_D20();					/* 8218-821E */

	return (int16)g_noise_table[off] + spread;			/* 8220-8233 */
}

/* 0x0AC7 */

/* 0x0AC7 */
int16 state_query(int16 slot)
{
	struct character_rec *rec;
	int16 w;
	int16 held;

	w = (uint16)g_hero_slots[slot].state;	/* 0ACD-0ADE */
	rec = CHAR_REC(character_at(g_record_addr[w & 0xff]));		/* 0AE1-0AEC */

	if ((int16)rec->ranged_weapon.icon == 3) {
		held = (int16)(int8)rec->action_code;
		if (held != ACTION_PAIR_IF_MODE3_0E && held != ACTION_PAIR_IF_MODE3_0F)
			return 0;				/* 0B25-0B27 */
		if (--(rec->action_timer) == 0)	/* 0B29-0B2C */
			rec->action_code = 0;			/* 0B2E-0B31 */
		return (held == ACTION_PAIR_IF_MODE3_0E) ? EFFECT_KIND_DEACTIVATE_6 : EFFECT_KIND_DEACTIVATE_7;			/* 0B35-0B43 */
	}
	if ((int16)rec->ranged_weapon.icon == 6)
		return EFFECT_KIND_DEACTIVATE_8;			/* 0B60-0B63 */
	if ((int16)rec->ranged_weapon.icon == WEAPON_RECORD_9) {
		rec->ranged_weapon.icon = 0;			/* 0AF6-0AF9 */
		award_claim(1, WEAPON_RECORD_9);				/* 0AFE-0B09 */
		return EFFECT_KIND_RECOVER_WEAPON_9;		/* 0B0C-0B0F */
	}
	if ((int16)rec->ranged_weapon.icon == WEAPON_RECORD_17) {
		rec->ranged_weapon.icon = 0;			/* 0B45-0B48 */
		award_claim(7, WEAPON_RECORD_17);				/* 0B4D-0B58 */
		return EFFECT_KIND_RECOVER_WEAPON_17;	/* 0B5B-0B5E */
	}
	return 0;							/* 0B65-0B67 */
}

/* 0x8C21 */

/* 0x8C21 */
void mark_map_unseen(void)
{
	uint8 *p = g_map_tile;
	int16 i;

	for (i = 0; i != TERRAIN_CELLS; i++)				/* 8C2C-8C3B */
		p[i] |= 0x80;					/* 8C3D-8C43 */
}

extern uint8 g_map_mode_char;			/* 0x227A */

/* 0xC82C */

/* 0xC82C */
void load_current_map(void)
{
	g_map_records[MAP_SWAMP_GATE_RECORD].exit_right_guard =
		MAP_SWAMP_GATE_EXIT_ROW;
	g_map_records[MAP_SWAMP_GATE_RECORD].exit_right_map =
		TERRAIN_SWAMP;
	g_map_records[MAP_SWAMP_GATE_RECORD].icons[MAP_ICON_SLOT_EXIT] =
		MAP_ICON_EXIT;
	map_record_show((int16)(int8)g_map_mode_char, 0);	/* C83E-C849 */
}

/* 0x09A6 */

int16 scan_pair_window(uint8 *p1, uint8 *p2)
{
	int16 n;

	p1 += 9;					/* 09AC-09B0 */
	p2 += 9;
	n = 9;

	for (;;) {
		if (n < 0)					/* 09C7-09CB */
			return -1;
		if (*p1 == 0) {				/* 09CD-09D3 */
			--n; --p1; --p2;
			continue;
		}
		if (*p2 == 0) {				/* 09D5-09DB */
			--n; --p1; --p2;
			continue;
		}
		--*p2;
		return n;					/* 09DF-09E2 */
	}
}

extern int16 tile_property(int16 x, int16 y);

/* 0x2EEB */

/* 0x2EEB */
void probe_north(int16 *a, int16 *b, struct actor_rec *c)
{
	int16 x = *a;					/* 2EF1-2EF6 */
	int16 y = *b - 7;					/* 2EF9-2F01 */
	int16 r1 = tile_property(x - 7, y);
	int16 r2 = tile_property(x + 7, y);

	if (r1 == 1 || r2 == 1)
		c->vel_y = 0;		/* 2F33-2F36 */
}

/* 0x2F92 */

void probe_south(int16 *a, int16 *b, struct actor_rec *c)
{
	int16 x = *a;					/* 2F98-2F9D */
	int16 y = *b + 7;					/* 2FA0-2FA8 */
	int16 r1 = tile_property(x - 7, y);
	int16 r2 = tile_property(x + 7, y);

	if (r1 == 1 || r2 == 1)
		c->vel_y = 0;		/* 2FDA-2FDD */
}

/* 0x2F3F */

void probe_east(int16 *a, int16 *b, struct actor_rec *c)
{
	int16 x = *a + 7;					/* 2F45-2F4D */
	int16 y = *b;					/* 2F50-2F55 */
	int16 r1 = tile_property(x, y - 7);
	int16 r2 = tile_property(x, y + 7);

	if (r1 == 1 || r2 == 1)
		c->vel_x = 0;		/* 2F86-2F89 */
}

/* 0x2FE6 */
void probe_west(int16 *a, int16 *b, struct actor_rec *c)
{
	int16 x = *a - 7;					/* 2FEC-2FF4 */
	int16 y = *b;					/* 2FF7-2FFC */
	int16 r1 = tile_property(x, y - 7);
	int16 r2 = tile_property(x, y + 7);

	if (r1 == 1 || r2 == 1)
		c->vel_x = 0;		/* 302D-3030 */
}

extern struct site_rec g_site_table[];
extern uint8 g_map_mode_char;

/* 0x0BD8 */

int16 probe_record_kind(int16 id, int16 x, int16 y, int16 mode_value, int16 flags)
{
	struct site_rec *rec = SITE(g_site_table);
	int16 i;

	for (i = 0; i != SITE_COUNT; i++, ++rec) {		/* 0BEA-0BF5 */
		if (rec->id != 0)
			continue;

		rec->id = (uint16)id;			/* 0BFF-0C02 */
		rec->flags = (uint16)flags;
		rec->weight = 5;				/* 0C0D-0C10 */

		record_set_mode(SITE(rec), x, y, mode_value);	/* 0C15-0C21 */
		return 0;
	}

	return 0;
}

/* 0x19BC */

void record_flag_rebuild(struct site_rec *p)
{
	int16 sx = SITE(p)->x;				/* 19C2-19C8 */
	int16 sy = SITE(p)->y;				/* 19CB-19CE */

	SITE(p)->id = 0;
	p++;

	while (SITE_FLAG_LO(p) & 4) {
		SITE_FLAG_LO(p) &= 0x80;
		SITE_FLAG_LO(p) |= 1;
		SITE(p)->x = (int16)sx;			/* 19F8-19FB */
		SITE(p)->y = (int16)sy;			/* 19FE-1A04 */
		SITE(p)->map = (uint16)(int16)(int8)g_map_mode_char;	/* 1A07-1A0E */
		p++;
	}
}

extern struct rect_rec g_hero_slot_boxes[4];
extern int16 shadow_getkey_noecho(void);

/* 0xAD8C */

void hero_window_suppress(int16 i)
{
	int16 si = i;						/* 0xAD90-0xAD9F */

	if ((g_hero_slot_boxes[si].state & RECT_STATE_CONTROL) == 0) {	/* 0xADA3-0xADA7 */
		if (g_actor_table[i].mode == 0)			/* 0xADAC-0xADB5 */
			RECT_STATE_HI(&g_hero_slot_boxes[si]) |= 0x80;			/* 0xADB7 */
	}
}

/* 0x03DC */

uint8 *walk_table(int16 off)
{
	if (off >= SITE_TABLE_ADDR && off < SITE_TABLE_END)
		return (uint8 *)g_site_table + (off - SITE_TABLE_ADDR);
	if (off >= ACTOR_TABLE_ADDR && off < ACTOR_TABLE_END)
		return (uint8 *)g_actor_table + (off - ACTOR_TABLE_ADDR);
	if (off >= PARTY_TABLE_ADDR && off < PARTY_TABLE_END)
		return (uint8 *)g_party_table + (off - PARTY_TABLE_ADDR);
	if (off >= MAP_TILE_ADDR && off < MAP_TILE_END)
		return g_map_tile + (off - MAP_TILE_ADDR);
	if (off >= MAP_RECORDS_ADDR)
		return (uint8 *)g_map_records + (off - MAP_RECORDS_ADDR);
	return 0;
}

extern int16 shadow_video_flags(int16 v);				/* 0x9CFA */

/* 0x1FA4 */
void hero_slot_select(int16 idx)
{
	struct rect_rec *base = g_hero_slots;
	struct rect_rec *rec;
	struct character_rec *srec;
	int16 v;

	if (idx == -1) {				/* 1FB0-1FB4 */
		screen_refresh();
		g_cursor_mode = CURSOR_MODE_TARGET;
		while (mouse_button_read() != 0)
			;
		while (mouse_button_read() == 0)
			(void)mouse_poll_moved(1);
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);			/* 1FDB-1FE2 */
		while (mouse_button_read() != 0)			/* 1FE5-1FEC */
			;
		g_cursor_mode = CURSOR_MODE_DEFAULT;
		idx = rect_scan(4, (uint8 *)g_hero_slots);

		if (idx < 0)				/* 2005-2009 */
			return;
	}

	rec = base + (uint16)idx * RECT_STRIDE;		/* 200B-201B */
	v = (int16)(uint16)RECT(rec)->state;
	if (v < 0)					/* 201B-2020 */
		g_sticky_click_flag = 1;

	rec = base + (uint16)idx * RECT_STRIDE;
	v = (int16)(uint16)RECT(rec)->state;
	if (v > 0x16)					/* 2036-203B */
		g_sticky_click_flag = 1;

	if ((int16)g_sticky_click_flag != 0)
		return;

	srec = CHAR_REC(character_at(g_record_addr[(int16)RECT(base + (uint16)idx)->state]));			/* 204A-2062 */
	record_counter_advance(srec, 0xf, 2);
	draw_hero_gauge(idx);					/* 2076-207C */
}

/* 0x2573 */
void hero_slot_select_spread(int16 idx)
{
	struct rect_rec *base = g_hero_slots;
	struct rect_rec *rec;
	struct character_rec *srec;
	int16 v;

	if (idx == -1) {				/* 257F-2583 */
		screen_refresh();
		g_cursor_mode = CURSOR_MODE_TARGET;
		while (mouse_button_read() != 0)			/* 258E-2595 */
			;
		while (mouse_button_read() == 0)			/* 2597-25A8 */
			(void)mouse_poll_moved(1);
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);			/* 25AA-25B1 */
		while (mouse_button_read() != 0)			/* 25B4-25BB */
			;
		g_cursor_mode = CURSOR_MODE_DEFAULT;
		idx = rect_scan(4, (uint8 *)g_hero_slots);	/* 25C3-25D1 */
		if (idx < 0)				/* 25D4-25D8 */
			return;
	}

	rec = base + (uint16)idx * RECT_STRIDE;		/* 25DA-25EA */
	v = (int16)(uint16)RECT(rec)->state;
	if (v < 0)					/* 25EA-25EF */
		g_sticky_click_flag = 1;

	rec = base + (uint16)idx * RECT_STRIDE;
	v = (int16)(uint16)RECT(rec)->state;
	if (v > 0x16)					/* 2605-260A */
		g_sticky_click_flag = 1;

	if ((int16)g_sticky_click_flag != 0)
		return;

	srec = CHAR_REC(character_at(g_record_addr[(int16)RECT(base + (uint16)idx)->state]));			/* 2619-2631 */
	record_counter_advance(srec, 9, 1);

	draw_hero_gauge(idx);					/* 2645-264B */
}

/* 0x9662 */

const int16 g_neighbour_table[8][3] = {
	{ -1, -1, 3 }, {  0, -1, 4 }, {  1, -1, 5 },
	{ -1,  0, 2 },		       {  1,  0, 6 },
	{ -1,  1, 1 }, {  0,  1, 0 }, {  1,  1, 7 },
};

uint8 g_path_qx[64];		/* 0xa622 */
uint8 g_path_qy[64];		/* 0xa864 */
uint8 g_path_grid[100];		/* 0xa8b2 */

uint16 g_path_read;				/* 0x9f18 */
uint16 g_path_write;				/* 0xa16c */

/* 0xA6C4 */
int16 queue_expand(void)
{
	int16 sx, sy;
	int16 i;
	int16 x, y, d, cell, idx;
	uint16 rd;

	if ((int16)g_path_write == (int16)g_path_read)
		return -1;

	rd = g_path_read;
	sx = (int16)(int8)g_path_qx[rd];		/* A6DD-A6E2 */
	++g_path_read;

	sy = (int16)(int8)g_path_qy[rd];
	g_path_read &= PATH_QUEUE_MASK;

	for (i = 0; i != 8; ++i) {
		x = g_neighbour_table[i][0] + sx;			/* A70E-A71A */
		y = g_neighbour_table[i][1] + sy;			/* A71D-A729 */
		d = g_neighbour_table[i][2];			/* A72C-A735 */

		if (x < 0 || x >= 10)			/* A738-A742 */
			continue;
		if (y < 0 || y >= 10)			/* A744-A74E */
			continue;

		idx = y * 10 + x;				/* A750-A760 */
		cell = (int16)(int8)g_path_grid[idx];	/* A763-A768 */

		if (cell == -1) {
			g_path_qx[g_path_write] = (uint8)x;
			g_path_qy[g_path_write] = (uint8)y;
			++g_path_write;
			g_path_write &= PATH_QUEUE_MASK;
			g_path_grid[idx] = (uint8)d;

			continue;
		}
		if (cell == -3)
			return d;
	}

	return 0;
}

/* 0xA7AF */
int16 path_find(int16 start_x, int16 start_y, int16 goal_x, int16 goal_y)
{
	int16 i;
	int16 r;
	int16 v;
	struct actor_rec *o;

	g_path_write = 1;
	g_path_read = 0;
	g_path_qx[0] = (uint8)start_x;
	g_path_qy[0] = (uint8)start_y;	/* A7C8-A7CB */

	for (i = 0; i != 100; ++i) {			/* A7DD-A80C */
		v = (int16)g_map_working.grid[i].cell.value;
		g_path_grid[i] = (uint8)((v >= 0x20) ? 0xfe : 0xff);	/* A7F9-A80A */
	}

	for (i = 0; i != 16; ++i) {			/* A80E-A848 */
		o = &g_actor_table[i];
		if (o->state == 0)
			continue;
		g_path_grid[((int16)o->x >> 4)
			+ ((int16)o->y >> 4) * 10] = 0xfe;

	}

	g_path_grid[goal_y * 10 + goal_x] = 0xfd;

	do {
		r = queue_expand();
	} while (r == 0);

	return r;
}

extern uint8 g_map_mode_char;		/* 0x227a */
extern struct map_record g_map_records[];	/* 0x3060 */
extern uint16 g_detect_invisible_ticks;			/* 0x9e0a */
void map_record_show(int16 rec_no, int16 transition);		/* 0x93F3 */

/* 0x2896 */
void tile_event_apply(void)
{
	int16 n;

	if ((int16)g_detect_invisible_ticks == 0)			/* 289A-28A1 */
		return;

	switch ((uint8)g_map_mode_char) {		/* 28A3-28CB */
	case MAP_BLEAK_HILLS:
	case MAP_DRY_PLAINS:
	case MAP_KHAROLIS_FOOTHILLS:
	case MAP_KHAROLIS_MOUNTAINS:
	case MAP_MOUNTAIN_LAKE:
	case MAP_FRUIT_FOREST:
		break;
	default:
		return;
	}

	n = (int16)(int8)g_map_mode_char;		/* 28CD-28D1 */

	g_map_records[n - 1].free_list = 3;

	map_record_show(n, 0);					/* 28E0-28E7 */
}

uint16 g_find_traps_gate;			/* 0xa9ba */

/* 0xBCD0 */
void draw_map_items(void)
{
	struct site_rec *p;
	int16 i;
	int16 x, y;

	p = SITE(g_site_table);
	for (i = 0; i != SITE_COUNT; ++i, ++p) {
		uint16 w;

		if (p->id == 0)
			break;
		if ((SITE_FLAG_LO(p) & 1) == 0)
			continue;
		if ((int16)p->map != (int16)(int8)g_map_mode_char)
			continue;

		w = g_overworld_phase;
		if (w < 1u || w > 2u)
			continue;

		if ((p->flags & SITE_FLAG_HI_BIT0) == 0) {

			x = (int16)p->x;
			y = (int16)p->y;
			if (x > 0 && y > 0 && x < 0xa0 && y < 0xa0)
				(void)free_list_insert(1, 0xa0 - x, y, 2, (int16)p, 0, 0);	/* BD3C-BD56 */
		} else if ((int16)g_detect_invisible_ticks != 0) {		/* BD5B-BD65 */
			SITE_FLAG_HI(p) &= 0xfe;
		}

		if ((SITE_FLAG_LO(p) & 0x20) != 0 && (int16)g_find_traps_gate != 0) {	/* BD6C-BD77 */
			(void)effect_publish(EFFECT_KIND_CREATURE, 0, 4,
			          (int16)p->x, (int16)p->y,
			          0x10, 0, 0, 0, 0);		/* BD79-BD97 */
			SITE_FLAG_LO(p) &= 0xdf;
		}
	}

	if ((int16)g_find_traps_gate != 0)			/* BDB5-BDBC */
		--g_find_traps_gate;
	if ((int16)g_detect_invisible_ticks != 0)			/* BDC0-BDC7 */
		--g_detect_invisible_ticks;
}

uint16 g_bless_duration;				/* 0x9e06 */
uint16 g_spell_purge_pending;			/* 0x9f28 */
uint16 g_protection_duration;			/* 0xa99a */
uint16 g_paralysis_duration;			/* 0xa9ca */

/* 0x265E */
int16 setting_apply_choice(int16 sel)
{
	switch (sel) {
	case CLERIC_CURE_LIGHT_WOUNDS:					/* 2667-2671 */
		hero_slot_select_spread(-1);
		return 0;
	case CLERIC_PROTECTION_FROM_EVIL:					/* 2676-267F */
		g_protection_duration = 0x78 * g_hold_length;
		sound_request_max(SOUND_REQUEST_MENU);
		return 0;
	case CLERIC_BLESS:
		g_bless_duration = g_hold_length * 15;
		sound_request_max(SOUND_REQUEST_MENU);
		return 0;
	case CLERIC_DETECT_MAGIC:					/* 26B0-26C4 */
		g_detect_magic_ticks = 0x14 * g_hold_length;
		sound_request_max(SOUND_REQUEST_MENU);
		return 0;
	case CLERIC_FIND_TRAPS:					/* 26C9-26E2 */
		clock_hold_span();
		g_find_traps_gate = g_hold_length * 3;
		sound_request_max(SOUND_REQUEST_MENU);
		return 0;
	case CLERIC_HOLD_PERSON:

		g_paralysis_duration = 0x50 * g_hold_length;
		return 10;
	case CLERIC_RESIST_FIRE:					/* 26F5-26FB */
		hero_slot_click_arm();
		return -1;
	case CLERIC_PRAYER:					/* 26FD-2713 */
		g_protection_duration = TICKS_PER_SECOND * g_hold_length;
		sound_request_max(SOUND_REQUEST_MENU);
		return 0;
	case CLERIC_DISPEL_MAGIC:					/* 2715-271D */
		g_spell_purge_pending = 2;
		return 0;
	case CLERIC_CURE_SERIOUS_WOUNDS:					/* 271F-2729 */
		hero_slot_select(-1);
		return 0;
	case CLERIC_TURN_UNDEAD:				/* 272D-2730 */
		return -1;
	default:
		return 0;
	}
}

int16 muldiv100(int16 a, int16 b)
{
	uint16 lo, hi;
	int16 neg = 0;
	uint16 ua;

	if (a < 0) {
		neg = 1;
		ua = (uint16)(0 - a);

	} else {
		ua = (uint16)a;
	}

	mul16(ua, (uint16)b, &lo, &hi);
	(void)div32_by_10(&lo, &hi);
	(void)div32_by_10(&lo, &hi);

	return neg ? (int16)-lo : (int16)lo;
}

/* 0x0873 */
void hero_member_retire(int16 idx)
{
	int16 no_move;
	struct actor_rec *win;
	int16 kind;
	struct character_rec *rp;
	struct site_rec *q;
	int16 i;

	no_move = 0;

	win = (struct actor_rec *)((uint8 *)g_actor_table + (uint16)(0x1e * idx));
	win->state = 0;
	(void)effect_publish(EFFECT_KIND_CREATURE, 0, 4, (int16)win->x,
	               (int16)win->y, 0x10, 0, 0, 0, 0);	/* 0892-08B2 */

	if (idx < 0)
		no_move = 1;
	idx &= LOW15_MASK;

	kind = (int16)(uint8)g_hero_slots[(uint16)idx].state;	/* 08C9-08DA */
	rp = character_at(g_record_addr[kind & 0xff]);
	--rp->base_stats.attributes[ATTR_CON];

	rp->hp_max = (uint16)((int16)(int16)rp->hp_max
		- (int16)muldiv100((int16)(int16)rp->hp_max, rnd(25)));

	if ((int16)(int16)(int16)rp->hp_max < 10)
		rp->hp_max = 10;

	if ((int16)(int16)(int16)rp->hp > -10) {
		RECT_STATE_LO(&g_hero_slots[(uint16)idx + 16]) = (uint8)kind;
	}
	rp->hp = 1;

	if (no_move == 0) {				/* 0943-0988 */
		q = SITE(g_site_table);
		for (i = 0; i != SITE_COUNT; ++i, ++q) {
			if ((SITE_FLAG_LO(q) & 2) == 0)
				continue;
			if ((int16)q->map != kind)
				continue;
			record_set_mode(SITE(q), (int16)win->x,
			                (int16)win->y, 0);

		}
	}

	g_hero_slots[(uint16)idx].state = 0x17;	/* 098A-099A */
	draw_hero_gauges();
}

/* 0xADC0 */

int16 hero_member_damage(int16 idx, int16 amount)
{
	struct character_rec *rp;
	int16 kind;

	kind = (int16)(uint8)g_hero_slots[(uint16)idx].state;	/* ADC6-ADFB */
	if (kind > 0x16)

		return 0;

	rp = character_at(g_record_addr[kind & 0xff]);

	rp->hp = (uint16)((int16)rp->hp - (int16)amount);

	if ((int16)(int16)(int16)rp->hp > 0) {			/* AE05-AE09 */
		draw_hero_gauge(idx);
		hero_window_suppress(idx);					/* AE24-AE2A */
	} else {
		rp->hp = 0;

		hero_member_retire(idx);					/* AE10-AE13 */
	}

	sound_request_max(SOUND_REQUEST_DAMAGE);						/* AE2D-AE34 */
	return 0;

}

/* 0x28EF */
int16 setting_apply_checked(int16 sel)
{
	switch (sel) {
	case SPELL_SLEEP:				/* 28F8-2905 */
		g_paralysis_duration = SLEEP_HOLD_FACTOR * g_hold_length;
		return SPELL_EFFECT_SLEEP;
	case SPELL_MAGIC_MISSILE:			/* 2908-290B */
		return SPELL_EFFECT_MAGIC_MISSILE;
	case SPELL_DETECT_MAGIC:			/* 290E-2927 */
		g_detect_magic_ticks = DETECT_MAGIC_ANIM_FACTOR * g_hold_length;
		tile_event_apply();
		sound_request_max(SOUND_REQUEST_MENU);
		return 0;
	case SPELL_CHARM:

		g_paralysis_duration = g_hold_length * CHARM_HOLD_FACTOR;
		return SPELL_EFFECT_CHARM;
	case SPELL_BURNING_HANDS:			/* 293F-2942 */
		return SPELL_EFFECT_BURNING_HANDS;
	case SPELL_WEB:					/* 2944-2947 */
		return SPELL_EFFECT_WEB;
	case SPELL_DETECT_INVISIBLE:			/* 2949-2986 */
		if ((uint8)g_map_mode_char == MAP_SETTINGS_SCENE) {
			g_map_record_11->exit_top_guard = MAP_HIDDEN_PASSAGE_EXIT_GUARD;
			g_map_record_11->exit_top_map = TERRAIN_DIFFICULT_CLIMB;
			g_map_record_11->icons[MAP_ICON_SLOT_HIDDEN_PASSAGE] = MAP_ICON_HIDDEN_PASSAGE;
			map_record_show((int16)(int8)g_map_mode_char, 0);	/* 295F-296A */
		}
		g_detect_invisible_ticks = DETECT_INVIS_REDRAW_FACTOR * g_hold_length;

		tile_event_apply();
		sound_request_max(SOUND_REQUEST_MENU);
		return 0;
	case SPELL_FIREBALL:				/* 2988-298B */
		return SPELL_EFFECT_FIREBALL;
	default:

		return sel;
	}
}

uint8 g_mask[48] = {
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,		/* 0x0F1C */
	0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,		/* 0x0F28 */
	1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,		/* 0x0F34 */
	1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,		/* 0x0F40 */
};

/* 0x09EF */
int16 spell_spend_charge(int16 idx)
{
	int16 state;
	int16 r;

	state = (int16)(int16)g_hero_slots[(uint16)idx].state;	/* 09F5-0A06 */

	switch (state) {
	case CHAR_GOLMOON:					/* 0A2F-0A40 */
		r = scan_pair_window(g_mask + MASK_RECORD_GOLMOON, g_game_state.golmoon_spell_count);
		break;
	case CHAR_RAISTLIN:					/* 0A0C-0A20 */
		r = scan_pair_window(g_mask + MASK_RECORD_RAISTLIN, g_game_state.raistlin_spell_count);
		if (r == -1)
			return 0;
		return setting_apply_checked(r);
	case CHAR_GILTHANAS:					/* 0A76-0A9F */
		r = scan_pair_window(g_mask + MASK_RECORD_GILTHANAS, g_game_state.gilthanas_spell_count);
		if (r == -1)
			return 0;
		if (r == SPELL_MENU_WEB_INDEX)				/* 0A8C-0A91 */
			r = SPELL_WEB;
		return setting_apply_checked(r);
	case CHAR_ELISTAN:					/* 0A42-0A50 */
		r = scan_pair_window(g_mask + MASK_RECORD_ELISTAN, g_game_state.elistan_spell_count);
		break;
	default:						/* 0AA3-0ABF */
		return 0;
	}

	if (r == -1)
		return 0;
	r = setting_apply_choice(r);
	if (r == -1)						/* 0A65-0A6A */
		r = 0;
	return r;
}

/* 0xAC5F */
int16 actor_attack_character(struct actor_rec *actor)
{
	struct character_rec *rp;
	int16 idx;
	int16 kind;
	int16 amt;
	int16 r;

	idx = (int16)(int8)actor->anchor_index;			/* AC69-AC6D */
	kind = (int16)(int8)actor->state - 5;			/* AC70-AC76 */

	if ((int16)(int16)g_hero_slots[(uint16)idx].state > 0x16)	/* AC79-AC8B */
		return 0;

	rp = character_at(g_record_addr[				/* AC92-ACAB */
		(int16)(uint8)g_hero_slots[(uint16)idx].state & 0xff]);

	amt = (int16)g_roll_table.attack_threshold[
		  (int16)g_roll_table.rolls[kind].attack_row
		      + (int16)(int8)rp->stats.armor_class] - 1;

	if (g_bless_duration != 0)					/* ACD3-ACDA */
		++amt;

	r = ROLL_D20();						/* ACDD-ACE4 */
	if (r < amt)
		return r;

	r = rnd((int16)g_roll_table.rolls[kind].charge_dice);	/* ACEF-AD0A */
	amt = (int16)g_roll_table.rolls[kind].charge_bonus + r;	/* AD0C-AD13 */

	if (g_protection_duration != 0					/* AD16-AD37 */
	 && (g_roll_table.rolls[kind].flags & 1) != 0
	 && amt != 0)
		--amt;

	if (ROLL_D100() > 50)
		rp->hp = (uint16)((int16)rp->hp - amt);

	if ((int16)(int16)(int16)rp->hp > 0) {			/* AD55-AD59 */
		draw_hero_gauge(idx);
		hero_window_suppress(idx);					/* AD74-AD7A */
	} else {
		rp->hp = 0;
		hero_member_retire(idx);					/* AD60-AD63 */
	}

	sound_request_max(SOUND_REQUEST_MONSTER_HIT);
	return 0;
}

/* 0x1136 */
int16 hero_window_point_draw(int16 slot, int16 kind)
{
	struct actor_rec *win;
	int16 dx, dy;
	int16 fdx, fdy;
	int16 qx, qy;
	int16 ax, ay;
	uint16 sqx, sqy, hix, hiy;
	int16 dist, vx, vy, step, code, r, x, y;

	win = &g_actor_table[slot];					/* 113E-1147 */

	g_cursor_tile_x = (g_cursor_tile_x << 4) + 8;
	g_cursor_tile_y = (g_cursor_tile_y << 4) + 8;				/* 1155-1159 */

	dx = (int16)win->x - (int16)g_cursor_tile_x;			/* 1160-1167 */
	dy = (int16)win->y - (int16)g_cursor_tile_y;			/* 116D-1174 */
	fdx = dx;
	fdy = dy;

	qx = -(dx >> 2);							/* 117A-1183 */
	qy = -(dy >> 2);							/* 1186-118F */

	mul16((uint16)qx, (uint16)qx, &sqx, &hix);				/* 1192-1197 */
	mul16((uint16)qy, (uint16)qy, &sqy, &hiy);				/* 1199-119E */
	dist = isqrt_word((uint16)(sqx + sqy), 0);				/* 11A0-11A7 */

	vy = (int16)((uint16)((uint16)qy << 3) / (uint16)dist);		/* 11AA-11B7 */
	vx = (int16)((uint16)((uint16)qx << 3) / (uint16)dist);		/* 11BA-11C7 */

	ax = abs_w(dx);							/* 11CA-11D3 */
	ay = abs_w(dy);							/* 11D6-11DF */

	if ((int16)((uint16)ax << 1) <= ay)
		fdx = 0;
	if ((int16)((uint16)ay << 1) <= ax)
		fdy = 0;

	code = 0;									/* 1200-1229 */
	if (fdx < 0) code |= 8;
	if (fdx > 0) code |= 4;
	if (fdy < 0) code |= 2;
	if (fdy > 0) code |= 1;

	if (kind == EFFECT_KIND_LARGE_EFFECT || kind == EFFECT_KIND_FIREBALL) {					/* 122D-1280 */
		ax = abs_w(dx);
		ay = abs_w(dy);
		if (ay > ax)
			step = ay / abs_w(vy);
		else
			step = ax / abs_w(vx);
	} else {
		step = 0x1e;
	}

	win->direction = g_dir_code[code];						/* 1287-1291 */

	x = (int16)win->x;						/* 1294-12A7 */
	y = (int16)win->y;
	if (kind == SPELL_EFFECT_MAGIC_MISSILE || kind == EFFECT_KIND_HIT_BOZAK) {					/* 12AA-12C2 */
		x = (int16)g_cursor_tile_x;
		y = (int16)g_cursor_tile_y;
		step = 3;
	}

	r = effect_publish(kind, 1, step, x - vx, y - vy, 0x10,				/* 12C7-12F4 */
	             vx, vy, 0, (int16)(int8)win->direction);

	win->mode = 4;							/* 12FA-12FD */
	win->status = 0;						/* 1301-1304 */
	return r;
}
