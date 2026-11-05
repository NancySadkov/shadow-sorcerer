/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"
#include "structs.h"
/* 0x7807 */

#include "shadow.h"


#define REC	22
#define PARTY(i)	(&g_party_table[i])
#define CELL(p)		((p)->grid_x + TERRAIN_WIDTH * (p)->grid_y)

/* DGROUP 0x2c92 */

uint8 g_map_tile[TERRAIN_CELLS] = {
	   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,
	   8,    8,    8,    0,    8,   47,    5,    5,    5,    5,    5,    5,   24,    8,    8,   14,
	   8,    1,    1,    1,    1,    1,    1,    8,    8,    1,    1,    4,    1,    1,    1,   44,
	  11,    8,   17,    8,    8,    8,    1,    7,    1,   29,    8,    0,    8,    4,    1,    4,
	  48,    1,    4,    5,    7,   11,    8,   10,   15,    1,    1,    7,    7,    7,   29,    1,
	   9,    1,    1,    1,    6,    1,    1,    5,    7,   11,    8,    8,   14,    1,    7,   48,
	   4,    8,   30,    0,    8,   29,    7,    1,    1,    4,    1,    5,    5,    5,   10,   28,
	  18,    1,    1,    1,    1,    8,   31,    8,    8,   29,    7,    1,    1,    1,    1,    1,
	   1,    5,    5,    7,   14,    1,    1,   43,    1,    8,   30,    0,    8,    1,    6,    1,
	   8,    1,    7,    7,    1,    5,    4,    7,   20,   28,   44,    1,    1,    8,   31,    8,
	   7,    8,    5,    8,   43,   48,    4,    7,    5,    4,    4,    7,    1,    1,   12,    5,
	   1,    7,   29,    0,    8,   35,    5,    5,    8,    8,    4,    4,    4,    4,    4,    7,
	   1,    1,    1,    5,    5,    1,   46,    6,    8,   35,    7,    5,    7,    4,    4,    5,
	   5,    4,    4,    7,    1,    5,    5,    5,    5,    5,    1,    0,    8,   35,    7,    5,
	   5,    4,    8,    4,    7,    7,    7,    7,    5,    5,    1,    1,    1,    1,    1,    6,
	   8,   35,    7,    4,    4,   48,    5,    7,    5,    5,    5,    5,    1,    1,    1,    1,
	   5,    5,    6,    0,    8,   35,    7,    4,    6,    4,    4,    5,    7,    5,    1,    1,
	   1,    1,    1,    1,    1,    5,    1,    6,    8,   35,    7,    4,    6,   48,    4,    7,
	   5,    1,    1,    1,    5,    5,    5,    5,    5,    1,   31,    0,    8,   35,   35,   35,
	  35,   38,    5,    5,    5,    1,    1,    1,    1,   43,    5,    1,    1,    1,    1,   30,
	   8,    7,    7,    7,   30,    7,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
	   1,    1,   33,    0,    8,    1,    1,    1,   33,    1,    1,    1,    1,    1,    1,    1,
	   1,    1,    1,    1,    1,    1,   42,    1,    8,    1,    1,   33,    7,    1,    1,    1,
	   1,    1,    1,    1,    1,    1,    1,    1,    1,    5,    5,    0,    8,    1,    1,   33,
	   7,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,    5,    5,
	   8,    1,   33,    5,    1,    1,    1,    1,    3,   43,    3,    1,    1,    1,    1,    1,
	   1,    1,    1,    0,    8,    1,   31,   47,    1,    1,    1,    1,    1,    3,    1,    3,
	  43,    3,    1,    1,    1,    1,    5,    5,    8,    1,   32,   22,   22,   41,    3,    3,
	   3,    1,    3,    1,    3,   43,    1,    1,    1,    1,    1,    0,    8,    1,    1,    1,
	   1,    1,    1,    1,    1,    3,    1,    1,    3,    1,    3,    5,    5,    1,    1,    1,
	   8,    5,    5,    5,    3,    3,    3,    3,    1,    1,    3,    1,    1,    3,    5,    5,
	   5,    1,    1,    0,    8,    7,    7,    7,    7,    3,    3,    3,    3,    3,    1,    3,
	   3,    3,    7,    5,    5,    5,    5,    6,    8,    8,    7,    7,    7,    3,    3,    3,
	   3,   45,    3,    3,    3,    3,    7,    5,    5,    5,    6,    0,    8,    8,    7,    7,
	   8,    7,    3,    3,    3,   17,    3,    3,    3,    3,    7,   39,    5,    5,    5,    6,
	   8,    7,    8,    8,    7,    5,    5,    1,    1,   25,   28,   15,    5,    7,    5,    7,
	   7,    7,    5,    0,    8,    7,    8,    8,   27,   28,   28,   28,   28,   24,    6,    6,
	  21,   15,    7,    7,    8,    8,    8,    8,    8,    7,    7,   23,    7,    5,   40,    5,
	  23,   11,    5,    5,   11,   10,   28,   28,   28,   15,    5,    0,    8,    8,    7,   23,
	   7,    5,    7,   27,   26,    5,   11,    7,    5,   10,   15,    8,    5,    5,   11,    8,
	   8,    8,    1,    8,    8,    8,    1,    8,    8,    8,    1,    8,    8,    8,    1,    8,
	   8,    8,    1,    0,    8,    8,   36,    8,    8,    8,   36,    8,    8,    8,   36,    8,
	   8,    8,   36,    8,    8,    8,   36,    8,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
extern uint8 g_terrain_grid[];	/* DGROUP 0x13a6 */
extern struct rect_rec g_hero_slots[];	/* DGROUP 0x23a0 */

/* DGROUP 0x2b48 */

struct party_rec g_party_table[PARTY_COUNT] = {
	{  84,  0,  11,   1,   0,   0,   0,    0,    0,     0,   0,   0 },
	{  86,  2,  11,   1,   0,   0,   0,  800,   48,  2000,   0,   0 },
	{   0,  0,   0,   0,   0,   0,   0,    0,    0,     0,   0,   0 },
	{   0,  0,   0,   0,   0,   0,   0,    0,    0,     0,   0,   0 },
	{   0,  0,   0,   0,   0,   0,   0,    0,    0,     0,   0,   0 },
	{   0,  0,   0,   0,   0,   0,   0,    0,    0,     0,   0,   0 },
	{   0,  0,   0,   0,   0,   0,   0,    0,    0,     0,   0,   0 },
	{   0,  0,   0,   0,   0,   0,   0,    0,    0,     0,   0,   0 },
	{   0,  0,   0,   0,   0,   0,   0,    0,    0,     0,   0,   0 },
	{   0,  0,   0,   0,   0,   0,   0,    0,    0,     0,   0,   0 },
	{  85,  2,  11,   0,  11,   1, 2880,    0,    0,     0,   0,   0 },
	{  87, 64,  11,   0,  11,   1, 2882,    0,    0,     0,   0,   0 },
	{  87, 64,  11,   0,  11,   1, 2882,    0,    0,     0,   0,   0 },
	{  87, 64,  11,   0,  11,   1, 2882,    0,    0,     0,   0,   0 },
	{  87, 64,  11,   0,  11,   1, 2882,    0,    0,     0,   0,   0 },
};
struct trail_point { uint16 x, y; };
struct trail_point g_trail[64];		/* DGROUP 0x2298 */
uint16 g_trail_head;			/* DGROUP 0x2398 */

uint16 g_chosen_party;
uint16 g_seek_suspended;
extern struct site_rec g_site_table[];
#define g_pending_tile_reward (g_site_table[SITE_SLOT_TILE_REWARD].id)
uint16 g_spawn_cooldown;
uint16 g_refugee_seek_hold;

extern int16  hero_slot_set_occupied(int16 code, int16 occupied);
extern int16 abs_w(int16 v);
extern int16  trail_find_cell(int16 gx, int16 gy);
extern int16  path_find_distance(int16 gx, int16 gy, int16 tx, int16 ty, int16 *dist);
extern void draw_map_view(void);
extern int16  event_raise(int16 v);
extern void sprite_draw_anchored(int16 px, int16 py, int16 kind);

/* DGROUP 0x20c4 */
uint8 g_waypoint_route_a[80] = {
	 10,   1,   9,   1,   8,   1,   7,   1,   6,   1,   5,   1,   4,   1,
	  3,   1,   2,   2,   2,   3,   1,   4,   1,   5,   1,   6,   1,   7,
	  0,   8,   1,   9,   1,  10,   1,  11,   1,  12,   1,  13,   1,  14,
	  1,  15,   1,  16,   1,  17,   1,  18,   2,  19,   1,  20,   2,  21,
	  2,  22,   2,  23,   3,  24,   4,  25,   4,  26,   5,  27,   5,  28,
	  6,  28,   7,  28,   8,  28,   9,  29,   0,   0,
};

/* DGROUP 0x2114 */
uint8 g_waypoint_route_b[70] = {
	 10,   1,   9,   2,   9,   3,   8,   4,   9,   5,   9,   6,   9,   7,
	  8,   8,   8,   9,   7,   9,   6,   9,   5,  10,   5,  11,   4,  12,
	  5,  13,   4,  14,   4,  15,   4,  16,   5,  17,   5,  18,   5,  19,
	  4,  20,   4,  21,   4,  22,   5,  23,   6,  23,   7,  23,   8,  23,
	  8,  24,   9,  24,  10,  25,   9,  26,   9,  27,   9,  28,   0,   0,
};

/* DGROUP 0x215a */
uint8 g_waypoint_route_c[58] = {
	 11,   1,  11,   2,  12,   2,  13,   2,  14,   3,  14,   4,  15,   5,
	 14,   6,  15,   7,  14,   8,  14,   9,  14,  10,  15,  11,  15,  12,
	 16,  13,  16,  14,  16,  15,  15,  16,  14,  16,  14,  17,  13,  18,
	 13,  19,  12,  20,  12,  21,  11,  22,  11,  23,  10,  23,   9,  24,
	  0,   0,
};

/* DGROUP 0x2194 */
uint8 g_waypoint_route_d[58] = {
	 12,   1,  13,   1,  14,   1,  15,   1,  16,   1,  17,   1,  17,   2,
	 18,   3,  18,   4,  18,   5,  18,   6,  18,   7,  18,   8,  18,   9,
	 18,  10,  18,  11,  17,  12,  18,  13,  18,  14,  18,  15,  18,  16,
	 18,  17,  18,  18,  18,  19,  18,  20,  18,  21,  18,  22,  18,  23,
	 17,  23,
};

typedef char party_rec_size_check[(sizeof(struct party_rec) == REC) ? 1 : -1];

extern int16  party_step_target(struct party_rec *rec, int16 idx);
extern int16  party_choose_target(struct party_rec *rec);

static uint8 *waypoint_table(int16 idx)
{
	if (idx == PARTY_FOLLOWER_FIRST)
		return g_waypoint_route_a;
	if (idx == PARTY_FOLLOWER_FIRST + 1)
		return g_waypoint_route_b;
	if (idx == PARTY_FOLLOWER_FIRST + 2)
		return g_waypoint_route_c;
	return g_waypoint_route_d;
}

/* 0x7807 */
int16 party_update_all(int16 advance, int16 draw)
{
	struct party_rec *rec;
	uint8 *wp;
	int16 idx, events, waypoint_hit, tile, ybase, nxt, dist;
	uint16 woff;
	int16 px, py;

	events = 0;
	ybase = (int16)PARTY_REC(0)->grid_y - PARTY_VIEW_ROWS_ABOVE;
	if (ybase < 0)
		ybase = 0;
	if (ybase > PARTY_VIEW_LAST_ROW)
		ybase = PARTY_VIEW_LAST_ROW;

	for (idx = PARTY_LAST, rec = PARTY(PARTY_LAST); idx >= PARTY_MAIN; --idx, --rec) {
		waypoint_hit = 0;
		if (rec->kind == 0)
			continue;
		if (idx < PARTY_FOLLOWER_LEADER)
			waypoint_hit = wander_step(idx, (uint8 *)rec);
		if (idx == PARTY_MAIN && waypoint_hit != 0)
			events |= (waypoint_hit == 1) ? PARTY_EVENT_SPAWN_SCENE : PARTY_EVENT_SPAWN_EIGHT;

		if (!advance)
			goto draw;

		if (rec->cooldown == 0)
			goto cold;
		rec->cooldown = (uint16)((int16)rec->cooldown - 1);
		if (rec->cooldown != 0)
			goto draw;

		events |= PARTY_EVENT_COOLDOWN;
		party_step_target(rec, idx);
		if (idx == PARTY_MAIN || idx >= PARTY_FOLLOWER_LEADER)
			goto trail_step;
		rec->cooldown = (uint16)((int16)rec->cooldown << 1);

		tile = g_terrain_grid[CELL(rec)] & TERRAIN_MASK;

		if (tile == MAP_FOOD_GROVE && g_pending_tile_reward == SITE_KIND_TRIGGER) {
			g_pending_tile_reward = 0;
			rec->score = (uint16)((int16)rec->score + FOOD_GROVE_SCORE);
			event_raise(EVENT_TILE_REWARD);
		} else if (tile == MAP_HIDDEN_FOOD_STORE && g_pending_tile_reward == SITE_KIND_GREEN_DRAGON) {
			g_pending_tile_reward = 0;
			rec->score = (uint16)((int16)rec->score + HIDDEN_FOOD_STORE_SCORE);
			event_raise(EVENT_TILE_REWARD);
		} else if ((g_map_tile[CELL(rec)] & TERRAIN_MASK) == 3) {

			int16 v = (int16)rec->refugees;
			v -= (v * 5) / 100 + ROLL_D20();
			rec->refugees = v < 0 ? 0 : v;
		}
		goto trail_step;

	trail_step:
		if (idx != PARTY_MAIN)
			goto draw;
		events |= PARTY_EVENT_MOVED;
		draw_map_view();
		g_trail_head = (g_trail_head + 1) & TRAIL_MASK;
		g_trail[g_trail_head].x = rec->grid_x;
		g_trail[g_trail_head].y = rec->grid_y;
		if ((int16)(int8)g_terrain_grid[CELL(rec)] > 10)
			events |= PARTY_EVENT_TILE_ENTRY;
		goto draw;

	cold:

		if (idx == PARTY_MAIN)
			goto draw;
		if (idx <= PARTY_COMPANION_LAST) {
			if (idx == PARTY_REFUGEES && g_refugee_seek_hold != 0)
				goto draw;
			if (g_seek_suspended != 0)
				goto draw;
			nxt = trail_find_cell(rec->grid_x, rec->grid_y);
			if (nxt == (int16)g_trail_head)
				goto draw;
			if (nxt == TRAIL_NO_CELL) {
				rec->target_x = rec->waypoint;
				rec->target_y = rec->target2;
			} else {
				if (rec->attr == 0)
					--nxt;
				else if (rec->attr & PARTY_ATTR_ALTERNATE)
					++nxt;
				nxt &= TRAIL_MASK;
				rec->target_x = g_trail[nxt].x;
				rec->target_y = g_trail[nxt].y;
			}
			rec->attr &= PARTY_ATTR_CLEAR_ALT;
			path_find_distance(rec->grid_x, rec->grid_y,
				 rec->target_x, rec->target_y, &dist);
			if (dist > PATH_DISTANCE_MAX)
				dist = 0;
			rec->cooldown = (uint16)(dist * COOLDOWN_PER_DISTANCE);
			goto draw;
		}
		if (idx == PARTY_FOLLOWER_LEADER) {
			if (g_seek_suspended != 0)
				goto draw;
			if (party_choose_target(rec) != 0)
				goto draw;
			rec->target_x = PARTY(g_chosen_party)->grid_x;
			rec->target_y = PARTY(g_chosen_party)->grid_y;
			rec->cooldown = FOLLOWER_COOLDOWN;
			if (rec->grid_x == rec->target_x && rec->grid_y == rec->target_y)
				g_chosen_party = rnd(FOLLOWER_COUNT) + PARTY_FOLLOWER_FIRST;
			goto draw;
		}
		wp = waypoint_table(idx);
		if (g_seek_suspended != 0)
			goto draw;
		if (party_choose_target(rec) != 0)
			rec->attr &= PARTY_ATTR_CLEAR_PATH;
		if (!(rec->attr & PARTY_ATTR_PATH))
			goto draw;
		woff = 2 * (uint16)rec->waypoint;
		rec->waypoint = (uint16)((int16)rec->waypoint + 1);
		rec->target_x = wp[woff];
		rec->target_y = wp[woff + 1];
		path_find_distance(rec->grid_x, rec->grid_y,
			 rec->target_x, rec->target_y, &dist);
		if (rec->target_x | rec->target_y)
			rec->cooldown = g_spawn_cooldown;
		else
			rec->attr &= PARTY_ATTR_CLEAR_PATH;
		goto draw;

	draw:
		px = (int16)(rec->grid_x << 4) + ((rec->grid_y & 1) ? 0 : HEX_HALF_STEP);
		py = (int16)(int16)(rec->grid_y - (uint16)ybase) * HEX_ROW_HEIGHT + PARTY_Y_BIAS;
		if (py < SCREEN_TOP_MARGIN)
			continue;
		if (draw && (g_map_tile[CELL(rec)] & TILE_VISIBLE)) {
			if (waypoint_hit != 0)
				sprite_draw_anchored(px, py + HIT_MARKER_LIFT, SPRITE_HIT_MARKER);
			sprite_draw_anchored(px, py, rec->kind);
		}
	}
	return events;
}

/* 0x74B6 */

int16 trail_find_cell(int16 gx, int16 gy)
{
	int16 i = 0;
	int16 idx = (int16)g_trail_head;

	for (; i != 0x40; ++i, --idx) {
		idx &= TRAIL_MASK;
		if (g_trail[idx].x == (uint16)gx && g_trail[idx].y == (uint16)gy)
			break;
	}
	return idx;
}

/* 0x738E */
int16 party_step_target(struct party_rec *rec, int16 idx)
{
	int16 dx = abs_w((int16)rec->grid_x - (int16)rec->target_x);
	int16 dy = abs_w((int16)rec->grid_y - (int16)rec->target_y);
	int16 dist = 0;

	if (!(dx | dy))
		return idx;

	if (dx != 0) {
		if ((int16)rec->target_x > (int16)rec->grid_x) {
			if (!(rec->grid_y & 1) || dy == 0)
				rec->grid_x = (uint16)((int16)rec->grid_x + 1);
		} else {
			if ((rec->grid_y & 1) || dy == 0)
				rec->grid_x = (uint16)((int16)rec->grid_x - 1);
		}
	}
	if (dy != 0) {
		if ((int16)rec->target_y > (int16)rec->grid_y)
			rec->grid_y = (uint16)((int16)rec->grid_y + 1);
		else
			rec->grid_y = (uint16)((int16)rec->grid_y - 1);
	}

	path_find_distance((int16)rec->grid_x, (int16)rec->grid_y,
		 (int16)rec->target_x, (int16)rec->target_y, &dist);
	if ((dx | dy) == 1)
		dist = 0;
	rec->cooldown = dist;

	if (idx >= PARTY_COMPANION_FIRST && idx <= PARTY_COMPANION_LAST) {
		struct party_rec *dest = PARTY(PARTY_REFUGEES);
		if ((int16)rec->grid_x == (int16)dest->grid_x &&
		    (int16)rec->grid_y == (int16)dest->grid_y) {
			dest->refugees = dest->refugees + rec->refugees;
			dest->food = (uint16)((int16)dest->food + (int16)rec->food);
			dest->score = (uint16)((int16)dest->score + (int16)rec->score);
			rec->kind = 0;
			return hero_slot_set_occupied(idx + 9, 0);
		}
	}
	return idx;
}

/* 0x7670 */
int16 party_choose_target(struct party_rec *rec)
{
	struct party_rec *e = PARTY(PARTY_MAIN);
	int16 i;

	for (i = 0; i != 7; i++, ++e) {
		int16 terr, dist = 0;

		if (e->kind == 0)
			continue;
		terr = g_terrain_grid[CELL(e)] & TERRAIN_MASK;

		if (terr == MAP_FOOD_GROVE || terr == MAP_DUSK_EYE || terr == MAP_DRAGON_ATTACK)
			continue;
		if (path_find_distance((int16)rec->grid_x, (int16)rec->grid_y,
			     (int16)e->target_x, (int16)e->target_y, &dist) == 0)
			continue;
		rec->target_x = e->grid_x;
		rec->target_y = e->grid_y;
		rec->cooldown = g_spawn_cooldown;
		return 1;
	}
	return 0;
}

/* 0x733D */
int16 hero_slot_set_occupied(int16 code, int16 occupied)
{
	struct rect_rec *slot;
	int16 i;

	for (i = 4, slot = &g_hero_slots[4]; i != HERO_SLOT_COUNT; i++, slot++) {
		if ((int16)(uint8)slot->state == code)
			break;
	}
	if (i == HERO_SLOT_COUNT)
		return 0;

	if (occupied != 0)
		slot->state |= HERO_SLOT_OCCUPIED;
	else
		slot->state &= (uint16)~HERO_SLOT_OCCUPIED;
	return code;
}

/* 0x7C8A */

/* 0x7C8A */
void party_spawn(int16 slot, int16 parts, int16 flags)
{
	struct party_rec *dst;
	struct party_rec *src = PARTY(PARTY_REFUGEES);
	int16 q;

	slot += 2;
	dst = PARTY(slot);							/* 7C95-7C9E */
	if (dst->kind != 0)
		return;

	dst->kind = src->kind;					/* 7CAB-7CAE */
	q = (int16)((int16)src->score / parts);
	dst->score = q;						/* 7CBC-7CBF */
	src->score = (uint16)((int16)src->score - q);	/* 7CC2-7CC5 */
	q = (int16)((int16)src->food / parts);			/* 7CC9-7CD2 */
	dst->food = q;						/* 7CD5-7CD8 */
	src->food = (uint16)((int16)src->food - q);	/* 7CDB-7CDE */
	q = (int16)((int16)src->refugees / parts);		/* 7CE2-7CEB */
	dst->refugees = q;					/* 7CEE-7CF1 */
	src->refugees = (uint16)((int16)src->refugees - q);	/* 7CF4-7CF7 */

	dst->attr = (uint8)(flags & 7);			/* 7CFB-7D03 */
	dst->grid_x = src->grid_x;				/* 7D06-7D11 */
	dst->target_x = src->grid_x;
	dst->grid_y = src->grid_y;				/* 7D14-7D1F */
	dst->target_y = src->grid_y;
	dst->cooldown = rnd(120);				/* 7D22-7D2F */
	(void)hero_slot_set_occupied(slot + 9, 1);			/* 7D32-7D40 */
}
