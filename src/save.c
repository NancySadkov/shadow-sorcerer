/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"

#include <dos.h>

#include "shadow.h"
#include "game.h"
#include "structs.h"

/* 0x9C04 */

struct save_block {
	uint16 offset;
	uint8 *data;
	uint16 size;
};

static void save_image_copy(uint8 far *block, int16 to_block)
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
		uint8 far *b = block + parts[i].offset;
		uint8 *p = parts[i].data;

		if (to_block)
			for (j = 0; j < parts[i].size; j++)
				b[j] = p[j];
		else
			for (j = 0; j < parts[i].size; j++)
				p[j] = b[j];
	}
}

static void save_image_transfer(int16 to_block)
{
	union {
		uint8 far *p;
		struct { uint16 off, seg; } r;
	} block;
	uint16 i;

	block.r.off = 0;
	block.r.seg = (uint16)g_mem_next;

	if (to_block)
		for (i = 0; i < SAVE_IMAGE_SIZE; i++)
			block.p[i] = 0;

	save_image_copy(block.p, to_block);
}

/* 0x9C6A */

int16 save_restore(char *pattern)
{
	union REGS r;

	r.x.dx = FP_OFF(pattern);
	r.x.bx = (uint16)g_mem_next;
	r.x.ax = SHADOW_SEL_LOAD;
	int86(SHADOW_VECTOR, &r, &r);				/* 9C79-9C7C */
	if (r.x.ax == 0)					/* 9C7E-9C80 */
		return 0;
	save_image_transfer(0);				/* 9C82-9C9E */
	return -1;
}

/* 0x9C04 */

int16 save_pack(char *pattern)
{
	union REGS r;

	r.x.dx = FP_OFF(pattern);
	save_image_transfer(1);				/* 9C0D-9C27 */
	r.x.bx = (uint16)g_mem_next;
	r.x.cx = SAVE_IMAGE_SIZE;
	r.x.ax = SHADOW_SEL_SAVE;
	int86(SHADOW_VECTOR, &r, &r);
	return r.x.ax;
}
