/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"
/* 0x7501 */

#include "shadow.h"

#define TERR_MASK	0x3f

extern int16 abs_w(int16);			/* 0x9BC4 */
extern uint8 g_map_tile[];	/* DGROUP 0x2c92 */
extern uint16 g_seek_suspended;
extern uint16 g_spawn_cooldown;

uint16 g_move_speed_shift;

/* DGROUP 0x1156 */

uint8 g_terr_bonus_a[64] = {
	156, 251, 231,   0, 246,   5,   0,  15,   0,   0, 251, 251, 251, 251,
	251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
	251, 251, 251, 251, 251, 251, 251, 251,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 156, 251, 241, 251,
	241,   0, 251, 241,   0,   0, 251, 251,
};

/* DGROUP 0x118a */

uint8 g_terr_bonus_b[64] = {
	156, 251, 241, 251, 241,   0, 251, 241,   0,   0, 251, 251, 251, 251,
	251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251, 251,
	251, 251, 251, 251, 251, 251, 251, 251,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,  30,   0,  30,   0,   0,   0,
};

/* DGROUP 0x11be */

uint16 g_move_cost_a[64] = {
	  0,   0,   0,  30,  30,   0,   0,  60,  60,  60,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  30,   0,
	 60,  60,   0,   0,   0,  30,  30,   0,   0,   0,   0,  30,  60,   0,
	  0,   0,   0,   0,   0,   0,  30,  45,
};

/* DGROUP 0x1238 */

uint16 g_move_cost_b[64] = {
	  0,  30,  45,  45,  60,  60, 999, 120, 999, 120,  15,  15,  15,  15,
	 15,  15,  15,  15,  15,  15,  15,  15,  30,  15,  15,  15,  15,  15,
	 15,  30,  30,  30,  30,  30,  30,  30,  30,  15,  15,  30,  60,  30,
	120,  30,  30,  30,  60,  60,  30,  30,  30,  30,  45, 120,  60,   0,
	  0,   0,   0,   0,   0,   0,  15,  15,
};

/* DGROUP 0x12b2 */

uint16 g_wait_cost_a[64] = {
	  0,  15,  15,  45,  15,   0,   0,  45,   0,  45,  15,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,  15,  15,   0,  15,  15,  15,
	 45,  45,  15,  15,  15,  15,  15,  15,  15,  15,  15,  45,  45,   0,
	  0,   0,   0,   0,   0,   0,  45,  60,
};

/* DGROUP 0x132c */

uint16 g_wait_cost_b[64] = {
	  0,  45,  60,  60,  30,  30, 999,  90, 999,  90,  23,  23,  23,  23,
	 23,  23,  23,  23,  23,  23,  23,  23,  45,  23,  23,  23,  23,  23,
	 23,  45,  45,  45,  45,  45,  45,  23,  45,  23,  45,  45,  30,  45,
	 90,  90,  45,  23,  23,  30,  30,  45,  45,  45,  45,  60,  90,  30,
	  1,   1,   1,   1,   1, 1542, 1542, 1542,
};

/* 0xA232 */

static uint16 isqrt_w(uint16 n)
{
	uint16 bx = (uint16)(((n >> 8) | (n & 0xff)) & 0xff);

	if (bx == 0)
		bx = 1;
	for (;;) {
		uint16 d = (uint16)(n / bx - bx);
		if (d <= 1u || d >= 0xffffu)
			break;
		bx = (uint16)(bx + (uint16)((int16)d >> 1));
	}
	return bx;
}

/* 0x7501 */
int16 path_find_distance(int16 gx, int16 gy, int16 tx, int16 ty, int16 *dist)
{
	int16 from = g_map_tile[gx + TERRAIN_WIDTH * gy] & TERR_MASK;
	int16 to = g_map_tile[tx + TERRAIN_WIDTH * ty] & TERR_MASK;
	int16 dx = abs_w(gx - tx);
	int16 dy = abs_w(gy - ty);
	int16 chance = g_seek_suspended ? 0 : 0x37;
	int16 nx = tx, ny = ty;
	int16 cost, d2, root;

	chance += (int16)g_terr_bonus_a[from] + (int16)g_terr_bonus_b[to];

	if ((dx | dy) > 1) {
		if (dx != 0) {
			if (tx > gx) {
				if (!(gy & 1) || dy == 0)
					gx = gx + 1;
			} else if ((gy & 1) || dy == 0) {
				gx = gx - 1;
			}
			nx = gx;
		}
		if (dy != 0)
			ny = (ty > gy) ? gy + 1 : gy - 1;
		to = g_map_tile[nx + TERRAIN_WIDTH * ny] & TERR_MASK;
	}

	cost = (int16)g_move_cost_a[from] + (int16)g_move_cost_b[to];
	if (cost > PATH_DISTANCE_MAX)
		cost = 0;
	*dist = cost;
	g_spawn_cooldown = (uint16)(g_wait_cost_a[from] + g_wait_cost_b[to]);

	d2 = (int16)(uint16)((uint16)(dy * dy)
	    + (uint16)(dx * dx));
	root = (int16)isqrt_w((uint16)d2);

	chance -= (root * 10) >> (int16)(uint8)g_move_speed_shift;
	if (chance <= 0)
		return 0;
	return (ROLL_D100() < chance) ? 1 : 0;
}
