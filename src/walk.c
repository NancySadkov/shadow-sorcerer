/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"

#include "shadow.h"

/* 0x0B88 */

uint16 g_rng_seed;		/* DGROUP 0xa9c6 */

#include "structs.h"

/* 0x0B88 */
int16 rnd(int16 n)
{
	g_rng_seed = (uint16)(((int32)(int16)g_rng_seed * 0x805L
				    + 0x3619L) & 0x7fffl);

	return (int16)((int16)g_rng_seed % n);
}

/* 0x7720 */

extern struct party_rec g_party_table[];	/* DGROUP 0x2b48 */
#define PARTY_FOLLOWERS	(&g_party_table[10])	/* DGROUP 0x2c24 */
#define ENT_PTR(p)	((struct party_rec *)(p))

/* 0x7720 */
int16 wander_step(int16 kind, uint8 *p)
{
	struct party_rec *r = PARTY_FOLLOWERS;
	int16 i;

	for (i = 0; i < 5; ++i, ++r) {
		if (r->grid_x != ENT_PTR(p)->grid_x)
			continue;
		if (r->grid_y != ENT_PTR(p)->grid_y)
			continue;
		break;
	}
	if (i == 5)
		return 0;

	if (r->cooldown == 0) {
		uint16 v = ENT_PTR(p)->refugees;
		int16 q = (int16)(uint16)((v << 4) - v) / 100;
		int16 n = (int16)v - q - rnd(15);

		if (n < 0) {
			n = 0;
			if (kind > 2)
				ENT_PTR(p)->kind = 0;
		}
		ENT_PTR(p)->refugees = (uint16)n;
	}
	r->cooldown = (uint16)(rnd(180) + 120);
	r->target_x = r->grid_x;
	r->target_y = r->grid_y;
	r->grid_y += (rnd(2) == 0) ? 1 : -1;
	return i + 1;
}
