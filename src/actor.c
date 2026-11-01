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

#define SITE_AT(i)	(&g_site_table[i])

#define SPAWN_ATTACK_VALUE(kind) \
	((int16)g_spawn_table.rec.attack[(kind)])
#define VOTE_EXPERIENCE_VALUE(kind) \
	((int16)g_vote_data.reward.experience[(kind)])
#define PANEL_TEMPLATE_CHAR_ADDR(i) \
	(g_panel_template.words[(i) - CHARACTER_ADDR_COUNT])

uint16 g_free_swept;               /* 0xa2c8 */
uint8 g_free_head[434];    /* 0xa2d2 */

/* 0x8EFA */

int16 free_list_clear_nodes(void)
{
	struct node_rec *p;

	if (g_free_swept == 0)
		return 0;
	g_free_swept = 0;
	p = NODE(g_free_head);
	do {
		p->child = 0;
		p = NODE((uint16)p->sibling);
		g_free_swept++;
	} while (p != 0);
	return 0;
}

int16  actor_sweep(void);

int16  effect_tick_all(void);
uint16 g_detect_magic_ticks;                    /* 0xa2ca */
uint16 g_sweep_counter;                    /* 0x9e0e */

/* 0x8F33 */

/* 0x8DF2 */

#define DRAGON_ATTACK_SPRITE_X	0x108
#define DRAGON_ATTACK_SPRITE_Y	0x50

/* 0x8DF2 */
int16 draw_terrain_sprites(void)
{
	int16 id;
	struct node_rec *outer;
	struct node_rec *inner;

	if (g_free_swept == 0)
		return 0;						/* 8DF8-8E01 */

	if (g_detect_magic_ticks != 0) {
		id = (g_sweep_counter & 3) + 0x47;

		if (g_map_mode_char == MAP_SETTINGS_SCENE)
			sprite_blit_center(0x58, 0x40, id);
		else if (g_map_mode_char == MAP_DRAGON_ATTACK)
			sprite_blit_center(DRAGON_ATTACK_SPRITE_X, DRAGON_ATTACK_SPRITE_Y, id);
		else if (g_map_mode_char == MAP_TERRAIN_SPRITE_A)
			sprite_blit_center(0x28, 0x60, id);
		else if (g_map_mode_char == MAP_TERRAIN_SPRITE_B)
			sprite_blit_center(0x58, 0x48, id);
		else if (g_map_mode_char == MAP_TERRAIN_SPRITE_C)
			sprite_blit_center(0x78, 0x30, id);
	}

	outer = NODE(g_free_head);
	do {
		(void)draw_free_node(outer);

		if (outer->child != 0) {
			inner = NODE((uint16)outer->child);
			do {
				(void)draw_free_node(inner);
				inner = NODE((uint16)inner->child);
			} while (inner != 0);
		}

		outer = NODE((uint16)outer->sibling);
	} while (outer != 0);

	g_sweep_counter++;
	return 0;
}

/* 0x6455 */

void check_refugee_total(void)
{
	int16 sum = 0;
	int16 i;

	for (i = PARTY_REFUGEES; i != PARTY_FOLLOWER_LEADER; i++) {
		if (PARTY_REC(i)->kind != 0)
			sum += (int16)PARTY_REC(i)->refugees;
	}
	if (sum < 100) {
		(void)event_raise(EVENT_REFUGEES_LOST);
		g_game_end_flag = 1;

	}
}

/* DGROUP 0x2c92 */

uint8 g_creature_grid[TERRAIN_CELLS] = {
	  8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,   8,
	  8,   8,   8,   0,   8,  47,   5,   5,   5,   5,   5,   5,  24,   8,   8,  14,
	  8,   1,   1,   1,   1,   1,   1,   8,   8,   1,   1,   4,   1,   1,   1,  44,
	 11,   8,  17,   8,   8,   8,   1,   7,   1,  29,   8,   0,   8,   4,   1,   4,
	 48,   1,   4,   5,   7,  11,   8,  10,  15,   1,   1,   7,   7,   7,  29,   1,
	  9,   1,   1,   1,   6,   1,   1,   5,   7,  11,   8,   8,  14,   1,   7,  48,
	  4,   8,  30,   0,   8,  29,   7,   1,   1,   4,   1,   5,   5,   5,  10,  28,
	 18,   1,   1,   1,   1,   8,  31,   8,   8,  29,   7,   1,   1,   1,   1,   1,
	  1,   5,   5,   7,  14,   1,   1,  43,   1,   8,  30,   0,   8,   1,   6,   1,
	  8,   1,   7,   7,   1,   5,   4,   7,  20,  28,  44,   1,   1,   8,  31,   8,
	  7,   8,   5,   8,  43,  48,   4,   7,   5,   4,   4,   7,   1,   1,  12,   5,
	  1,   7,  29,   0,   8,  35,   5,   5,   8,   8,   4,   4,   4,   4,   4,   7,
	  1,   1,   1,   5,   5,   1,  46,   6,   8,  35,   7,   5,   7,   4,   4,   5,
	  5,   4,   4,   7,   1,   5,   5,   5,   5,   5,   1,   0,   8,  35,   7,   5,
	  5,   4,   8,   4,   7,   7,   7,   7,   5,   5,   1,   1,   1,   1,   1,   6,
	  8,  35,   7,   4,   4,  48,   5,   7,   5,   5,   5,   5,   1,   1,   1,   1,
	  5,   5,   6,   0,   8,  35,   7,   4,   6,   4,   4,   5,   7,   5,   1,   1,
	  1,   1,   1,   1,   1,   5,   1,   6,   8,  35,   7,   4,   6,  48,   4,   7,
	  5,   1,   1,   1,   5,   5,   5,   5,   5,   1,  31,   0,   8,  35,  35,  35,
	 35,  38,   5,   5,   5,   1,   1,   1,   1,  43,   5,   1,   1,   1,   1,  30,
	  8,   7,   7,   7,  30,   7,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,  33,   0,   8,   1,   1,   1,  33,   1,   1,   1,   1,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,  42,   1,   8,   1,   1,  33,   7,   1,   1,   1,
	  1,   1,   1,   1,   1,   1,   1,   1,   1,   5,   5,   0,   8,   1,   1,  33,
	  7,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   5,   5,
	  8,   1,  33,   5,   1,   1,   1,   1,   3,  43,   3,   1,   1,   1,   1,   1,
	  1,   1,   1,   0,   8,   1,  31,  47,   1,   1,   1,   1,   1,   3,   1,   3,
	 43,   3,   1,   1,   1,   1,   5,   5,   8,   1,  32,  22,  22,  41,   3,   3,
	  3,   1,   3,   1,   3,  43,   1,   1,   1,   1,   1,   0,   8,   1,   1,   1,
	  1,   1,   1,   1,   1,   3,   1,   1,   3,   1,   3,   5,   5,   1,   1,   1,
	  8,   5,   5,   5,   3,   3,   3,   3,   1,   1,   3,   1,   1,   3,   5,   5,
	  5,   1,   1,   0,   8,   7,   7,   7,   7,   3,   3,   3,   3,   3,   1,   3,
	  3,   3,   7,   5,   5,   5,   5,   6,   8,   8,   7,   7,   7,   3,   3,   3,
	  3,  45,   3,   3,   3,   3,   7,   5,   5,   5,   6,   0,   8,   8,   7,   7,
	  8,   7,   3,   3,   3,  17,   3,   3,   3,   3,   7,  39,   5,   5,   5,   6,
	  8,   7,   8,   8,   7,   5,   5,   1,   1,  25,  28,  15,   5,   7,   5,   7,
	  7,   7,   5,   0,   8,   7,   8,   8,  27,  28,  28,  28,  28,  24,   6,   6,
	 21,  15,   7,   7,   8,   8,   8,   8,   8,   7,   7,  23,   7,   5,  40,   5,
	 23,  11,   5,   5,  11,  10,  28,  28,  28,  15,   5,   0,   8,   8,   7,  23,
	  7,   5,   7,  27,  26,   5,  11,   7,   5,  10,  15,   8,   5,   5,  11,   8,
	  8,   8,   1,   8,   8,   8,   1,   8,   8,   8,   1,   8,   8,   8,   1,   8,
	  8,   8,   1,   0,   8,   8,  36,   8,   8,   8,  36,   8,   8,   8,  36,   8,
	  8,   8,  36,   8,   8,   8,  36,   8,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

/* 0x7D48 */

/* 0x7D48 */
void creature_behavior_round(void)
{
	int16 i, r, idx, sel;
	int16 amt, amt2;
	struct party_rec *e;

	for (i = PARTY_REFUGEES, e = PARTY_REC(PARTY_REFUGEES); i != PARTY_FOLLOWER_LEADER; i++, e++) {	 	/* 7D4F-7D66 */
		e->attr &= 0xf7;
		amt = -1;							/* 7D72-7D75 */
		amt2 = -1;
		r = (int16)ROLL_D100();					/* 7D7B-7D85 */

		idx = (int16)(int16)e->grid_x + TERRAIN_WIDTH * (int16)e->grid_y;	/* 7D88-7D9D */
		sel = (int16)(g_creature_grid[idx] & CREATURE_BEHAVIOUR_MASK);				/* 7D9D-7DA0 */

		if (sel == CREATURE_ATTACK_LIGHT) {
			if (r < 10) amt = (int16)rnd(3);
		} else if (sel == CREATURE_ATTACK_MEDIUM) {
			if (r < 20) amt = (int16)rnd(6);
		} else if (sel == CREATURE_ATTACK_DOUBLE) {
			if (r < 25) {
				amt = (int16)rnd(12);
				amt2 = 1;
			} else if (r < 75) {
				amt = (int16)rnd(4);
				amt2 = (int16)rnd(1);
			}
		} else if (sel == CREATURE_ATTACK_HEAVY) {
			if (r < 30) amt = (int16)rnd(4);
		} else if (sel == CREATURE_AMBUSH) {
			if (r < 0x28) amt = (int16)rnd(8) - 2;
			amt2 = (int16)rnd(1);
		} else if (sel == CREATURE_ATTACK_LIGHT2) {
			if (r < 10) amt = (int16)rnd(3);
		}

		if (amt >= 0) {
			e->attr |= 0x08;
			amt++;
			e->refugees -= amt;				/* 7E91-7E97 */
			if ((int16)e->refugees <= 0 && i != 1)		/* 7E9A-7EA4 */
				e->kind = 0;
		}

		if (amt2 > 0) {						/* 7EAC-7EB0 */
			e->attr |= 0x08;						/* 7EB2-7EB5 */
			e->food -= amt2;				/* 7EB9-7EBF */
		}

		amt = (int16)(int16)(0x28 * (int16)(int16)e->food)
		    + (int16)(int16)(((uint16)e->refugees) << 1);	/* 7EC2-7ED2 */
		if ((int16)(int16)e->score < amt)			/* 7ED5-7ED8 */
			e->score = amt;
	}
}

/* 0x8C7F */

uint8 *free_list_alloc(void)
{
	struct node_rec *p = NODE(g_free_head + 7u * g_free_swept);

	g_free_swept++;

	p->sibling = 0;
	p->child = 0;
	return (uint8 *)p;
}

/* 0x8CB6 */

uint8 *free_list_find_prev(int16 key, int16 flag)
{
	struct node_rec *p = NODE(g_free_head);
	struct node_rec *best = p;

	if (flag != 0) {
		while ((int16)p->key < key) {
			best = p;
			if (p->sibling == 0)
				break;
			p = NODE((uint16)p->sibling);
		}
	} else {
		while ((int16)p->key <= key) {
			best = p;
			if (p->child == 0)
				break;
			p = NODE((uint16)p->child);
		}
	}
	return (uint8 *)best;
}

/* 0x8D14 */

void free_list_insert_child(uint8 *q, uint8 *start)
{
	struct node_rec *nq = NODE(q);
	struct node_rec *p = NODE(start);
	struct node_rec *best = p;

	while ((int16)p->key <= (int16)nq->key) {
		best = p;
		if (p->child == 0)
			break;
		p = NODE((uint16)p->child);
	}

	nq->child = best->child;				/* 8D48-8D4B */
	best->child = (uint16)(uint16)q;		/* 8D4E-8D54 */
}

/* 0x8D5C - add */

int16 free_list_insert(int16 mode, int16 key_lo, int16 key_hi, int16 type, int16 x, int16 data, int16 y)
{
	struct node_rec *node;
	uint8 *prev;
	int16 key = key_lo + key_hi;				/* 8D63-8D69 */

	if (key < 0)
		return 0;								/* 8D6E-8D72 */
	if (g_free_swept == FREE_SWEEP_MAX)
		return 0;								/* 8D74-8D7D */

	node = NODE(free_list_alloc());

	node->type = (uint16)type;
	node->key = (uint16)key;
	node->data = (uint16)data;
	node->x = (uint16)x;
	node->y = (uint16)y;

	if (g_free_swept == 0)
		return 0;

	prev = free_list_find_prev(key, mode);						/* 8DB7-8DC3 */

	if (mode != 0) {
		free_list_insert_child((uint8 *)node, prev);		/* 8DCC-8DD0 */
	} else {
		struct node_rec *np = NODE(prev);
		node->sibling = np->sibling;				/* 8DDE-8DE1 */
		np->sibling = (uint16)(uint16)node;		/* 8DE4-8DEA */
	}
	return 0;
}

uint8 g_kind_gate[5];   /* 0xa9bb */
uint16      g_kind_segment[5];    /* 0xa282 */

/* 0x8F33 */

int16 draw_free_node(struct node_rec *node)
{
	struct sprite_node_rec *p;
	uint16 type;
	uint16 id;
	uint16 al;

	if ((int16)(int16)(int16)node->key < 0)
		return 0;

	type = node->type;

	switch ((int16)(type - 1)) {
	case NODE_TYPE_SINGLE_SPRITE - 1:
		p = SPRITE_REC((uint16)node->data);
		id = (uint16)p->id;
		if (id == SPRITE_ID_RARE) {
			if (ROLL_D100() > 10)				/* 8F64-8F71 */
				return 0;
		}
		(void)sprite_draw_segment((int16)node->x,
		               (int16)node->y,
		               (int16)id, (uint16)g_mappic_at);			/* 8F76-8F7A */
		return 0;

	case NODE_TYPE_ANIMATED_SPRITE - 1:
		p = SPRITE_REC((uint16)node->data);
		g_sprite_draw_x = (uint16)p->x + (uint16)p->y;	/* 8F9A-8FA0 */
		g_sprite_draw_y = (int16)((int16)(int16)(uint16)p->y >> 1)
		           - (int16)((int16)(int16)(uint16)p->x >> 1)
		           + 0x7a;						/* 8FA3-8FB2 */
		id = p->id;
		if (id != 0)
			(void)sprite_blit_center((int16)g_sprite_draw_x, (int16)g_sprite_draw_y,
			               (int16)(id & 0xff));				/* 8FBB-8FC8 */
		if (g_detect_magic_ticks != 0 && (p->stage_flags & STAGE_FLAG_ANIMATED) != 0)	/* 8FCE-8FDD */
			(void)sprite_blit_center((int16)g_sprite_draw_x, (int16)g_sprite_draw_y,
			               (int16)((g_sweep_counter & 3) + 0x47));		/* 8FDF-8FF1 */
		return 0;

	case NODE_TYPE_ANIMATION_A - 1:
	case NODE_TYPE_ANIMATION_B - 1:
		p = SPRITE_REC((uint16)node->data);
		g_sprite_draw_x = (uint16)p->stage_next + (uint16)p->iso_y;	/* 9005-900B */
		g_sprite_draw_y = (int16)((int16)(int16)(uint16)p->iso_y >> 1)
		           - (int16)((int16)(int16)(uint16)p->stage_next >> 1)
		           + 0x7c;						/* 900E-901D */
		g_node_selector = p->selector;						/* 9020-9023 */
		g_sprite_frame_index = p->count;						/* 9026-9029 */
		g_sprite_stride_index = p->stride_index;						/* 902C-902F */
		al = (uint16)p->id;

		if (al < 5)
			(void)sprite_draw_state_frame(g_kind_segment[g_kind_gate[al]]);	/* 903B-904E */
		else
			(void)sprite_draw_state_frame(g_kind_segment[al]);				/* 9056-9064 */

		if (p->stride_index == 1 &&
		    (p->stage_flags | p->stage_delay) == 0)
			goto settled;					/* 9076-9079 */
		p->count++;
settled:
		if (p->count < g_sprite_frame_count)
			goto tail;
		p->count = (uint8)(p->count - g_sprite_frame_count);
		if (p->stage_delay != 0) {
			p->stage_delay--;
			goto tail;
		}
		if (p->stage_flags == 0) {
			p->stride_index = 0;
			goto tail;
		}
		p->stride_index = (uint8)p->stage_next;		p->stage_next = 0;					/* 90A5-90AE */
		p->stage_delay = p->stage_flags;		p->stage_flags = 0;					/* 90B7-90C0 */
		return 0;

	case NODE_TYPE_MISSILE - 1:
		p = SPRITE_REC((uint16)node->data);
		g_sprite_draw_x = (uint16)p->x + (uint16)p->y;	/* 90F8-90FE */
		g_sprite_draw_y = (int16)((int16)(int16)(uint16)p->y >> 1)
		           - (int16)((int16)(int16)(uint16)p->x >> 1)
		           + 0x7c;						/* 9101-9110 */
		(void)sprite_draw_field((int16)g_sprite_draw_x,
		               (int16)(g_sprite_draw_y - (uint16)p->y_offset),	/* 9117-911D */
		               p, 0);
		return 0;

	default:
		return 0;
	}

tail:
	if (p->jitter & JITTER_HIGH_BIT)
		(void)sprite_blit_center((int16)g_sprite_draw_x, (int16)g_sprite_draw_y, 0);		/* 90DA-90E5 */
	return 0;
}

/* 0x8ADD */

int16 find_party_player_tile(int16 idx)
{
	struct party_rec *e;
	int16 i;

	if (idx == 0) {
		e = PARTY_REC(PARTY_REFUGEES);
		for (i = 1; i < 10; i++) {
			if (e->grid_x == PARTY_REC(0)->grid_x &&
			    e->grid_y == PARTY_REC(0)->grid_y)
				return i;			/* 8B15-8B18 */
			e++;					/* 8AF5-8AF8 */
		}
		(void)event_raise(EVENT_NO_COMPANION_ON_TILE);			/* 8B1F-8B23 */
		return 0;
	}
	e = PARTY_REC((uint16)idx);
	if (e->grid_x == PARTY_REC(0)->grid_x &&
	    e->grid_y == PARTY_REC(0)->grid_y)
		return 1;
	return 0;
}

/* 0x8C4C */

void free_list_reset(void)
{
	struct node_rec *p = NODE(g_free_head);

	p->key = LIST_KEY_NONE;
	p->type = 0;
	p->sibling = 0;
	p->child = 0;
	g_free_swept = 1;
}

/* 0x8120 */

/* 0x8120 */
int16 party_may_use_code(int16 code, int16 ent)
{
	struct rect_rec *r = g_hero_slots;
	int16 i;
	int16 here;

	for (i = 0; i != HERO_SLOT_COUNT; i++, r++) {		/* 812B-813D */
		if ((int16)(uint8)r->state == code)	/* 813F-8149 */
			break;
	}
	if (i == HERO_SLOT_COUNT)
		return 0;					/* 814F-8157 */
	if ((r->state & RECT_STATE_GONE) != 0)
		return 0;					/* 815A-8165 */

	here = (PARTY_REC(ent)->grid_x == PARTY_REC(0)->grid_x	/* 8168-8191 */
		&& PARTY_REC(ent)->grid_y == PARTY_REC(0)->grid_y) ? 1 : 0;

	if (i < 4)
		return here;					/* 819A-81A7 */

	if (ent == 0)
		return 1;					/* 81CE-81AE */
	if (ent == 1)
		return ((r->state & RECT_STATE_RETIRING) != 0) ? 0 : 1;	/* 81B0-81BD */
	if (ent < 2)
		return 1;
	if (ent <= 6)
		return ((r->state & RECT_STATE_RETIRING) != 0) ? 1 : 0;	/* 81BF-81CC */
	return 1;						/* 81E1-81E3 */
}

/* 0x29B0 */

/* 0x29B0 */
void award_claim(int16 kind, int16 val)
{
	struct character_rec *rec = CHAR_REC(character_at(g_record_addr[kind]));	/* 29B6-29BF */
	struct site_rec *w;
	int16 i;

	if ((int16)rec->ranged_weapon.icon == val)
		return;							/* 29C4-29CE */

	for (i = 0, w = SITE(g_site_table); i != SITE_COUNT; i++, ++w) {	/* 29D0-29E7 */
		if ((int16)w->id != val)				/* 29E9-29F1 */
			continue;
		if ((SITE_FLAG_LO(w) & 2) == 0)			/* 29F3-29F7 */
			continue;
		if ((int16)w->map != kind)			/* 29F9-29FF */
			continue;
		w->id = 0;
		rec->ranged_weapon.icon = (uint8)val;			/* 2A05-2A0B */
		return;
	}
}

/* 0x0BA9 */

/* 0x0BA9 */
void record_counter_advance(struct character_rec *s, int16 hi, int16 lo)
{

	s->hp = (uint16)((int16)s->hp
		+ rnd(hi - lo) + lo);				/* 0BAD-0BC0 */
	if ((int16)s->hp > (int16)s->hp_max)
		s->hp = s->hp_max;			/* 0BC8-0BD0 */
}

/* 0x3AFC */

int16 record_near_refpoint(struct actor_rec *w)
{
	int16 dx = abs_w((int16)w->x - (int16)ACTOR_AT(0)->x);	/* 3B02-3B13 */
	int16 dy = abs_w((int16)w->y - (int16)ACTOR_AT(0)->y);	/* 3B16-3B27 */

	return (dx < 0x10 && dy < 0x10);			/* 3B2A-3B3C */
}

/* 0x0EDE */

const uint8 g_dir_code[16] = {
	0x00, 0x00, 0x04, 0x00, 0x06, 0x07, 0x05, 0x00,
	0x02, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* 0x3C7E */

int16 record_direction(struct actor_rec *w)
{
	struct actor_rec *anchor = ACTOR_AT((int16)(int8)w->anchor_index);	/* 3C85-3C90 */
	int16 dx = (int16)w->x - (int16)anchor->x;					/* 3C93-3C9C */
	int16 dy = (int16)w->y - (int16)anchor->y;					/* 3C9F-3CAB */
	int16 code = 0;

	if (dx < 0)
		code |= 8;								/* 3CB3-3CB9 */
	if (dx > 0)
		code |= 4;								/* 3CBD-3CC3 */
	if (dy < 0)
		code |= 2;								/* 3CC7-3CCD */
	if (dy > 0)
		code |= 1;								/* 3CD1-3CD7 */

	w->direction = g_dir_code[code];				/* 3CDB-3CE5 */
	return g_dir_code[code];
}

/* 0x2084 */

void apply_rect_action(int16 kind, int16 *act)
{
	struct character_rec *rec = CHAR_REC(character_at(g_record_addr[kind]));	/* 208B-2094 */
	int16 op = *act;						/* 2097-209C */
	int16 i;

	switch ((uint8)op - ACTION_OP_BASE) {				/* 209F-2489 */
	case ACTION_CASE(ACTION_SET_WEAPON_MODE_9):
		if (rec->ranged_weapon.icon == 0 && kind != CHAR_STURM && kind != CHAR_FLINT &&
		    kind != CHAR_CARAMON && kind != CHAR_GOLMOON && kind != CHAR_ELISTAN) {
			rec->ranged_weapon.icon = RANGED_WEAPON_SPEAR;
			*act = 0;
		}
		break;
	case ACTION_CASE(ACTION_SET_SPRITE_04): case ACTION_CASE(ACTION_SET_SPRITE_05): case ACTION_CASE(ACTION_SET_SPRITE_08): case ACTION_CASE(ACTION_SET_SPRITE_0A): case ACTION_CASE(ACTION_SET_SPRITE_0C): case ACTION_CASE(ACTION_SET_SPRITE_13):
		if (kind != CHAR_GOLMOON && kind != CHAR_FLINT && kind != CHAR_TASSLEHOFF && kind != CHAR_RAISTLIN) {
			*act = (int16)rec->hand_weapon.icon;			/* 2134-213D */
			rec->hand_weapon.icon = (uint8)op;				/* 213F-2145 */
		}
		break;
	case ACTION_CASE(ACTION_SET_WEAPON_MODE_3):
		if (rec->ranged_weapon.icon == 0 && kind != CHAR_STURM && kind != CHAR_GOLMOON && kind != CHAR_ELISTAN) {
			rec->ranged_weapon.icon = RANGED_WEAPON_BOW;
			*act = 0;
		}
		break;
	case ACTION_CASE(ACTION_DROP_IF_FLINT_0B): case ACTION_CASE(ACTION_DROP_IF_FLINT_12):
		if (kind == CHAR_FLINT) {
			*act = (int16)rec->hand_weapon.icon;			/* 2154-215D */
			rec->hand_weapon.icon = (uint8)op;				/* 215F-2165 */
		}
		break;
	case ACTION_CASE(ACTION_PAIR_IF_MODE3_0E): case ACTION_CASE(ACTION_PAIR_IF_MODE3_0F):
		if ((WEAPON_WORD(rec->ranged_weapon) & LOW15_MASK) == RANGED_WEAPON_BOW)	/* 216E-2177 */
			goto pair_done;
		break;
	case ACTION_CASE(ACTION_PAIR_IF_MODE6_10):
		if ((WEAPON_WORD(rec->ranged_weapon) & LOW15_MASK) != RANGED_WEAPON_HOOPAK)	/* 2181-218A */
			break;
pair_done:
		*act = (int16)(((uint16)rec->action_timer << 8) +
			    (int16)(int8)rec->action_code); /* 2192-21A0 */
		rec->action_code = (uint8)op;	/* 21A2-21A8 */
		rec->action_timer = (uint8)(op >> 8); /* 21AB-21B1 */
		WEAPON_WORD(rec->ranged_weapon) &= 0x7fff;			/* 21B4-21B7 */
		break;
	case ACTION_CASE(ACTION_GOLMOON_FIREBALL):
		if (kind != CHAR_GOLMOON) {
			(void)message_box(MSG_BOX_NOTICE, MSG_GOLMOON_ONLY);
			break;
		}
		*act = 0;						/* 21D5-21D8 */
		show_text_panel(TEXT_FIREBALL_READY);						/* 21DC-21E3 */
		g_spell_purge_pending = 2;
		break;
	case ACTION_CASE(ACTION_GOLMOON_SELECT):
		if (kind != CHAR_GOLMOON) {
			(void)message_box(MSG_BOX_NOTICE, MSG_GOLMOON_ONLY);
			break;
		}
		*act = 0;						/* 2206-2209 */
		/* 0x220D */

		hero_slot_select(HERO_SLOT_SELECT_MISSING_ARG);
		break;
	case ACTION_CASE(ACTION_GOLMOON_CANCEL):
		if (kind != CHAR_GOLMOON) {
			(void)message_box(MSG_BOX_NOTICE, MSG_GOLMOON_ONLY);
			break;
		}
		*act = 0;						/* 222A-222D */
		break;
	case ACTION_CASE(ACTION_RAISTLIN_SPELL_7):
		if (kind != CHAR_RAISTLIN) {
			(void)message_box(MSG_BOX_NOTICE, MSG_RAISTLIN_ONLY);
			break;
		}
		*act = 0;						/* 224E-2251 */
		g_raistlin_menu_choice = SPELL_FIREBALL;
		show_text_panel(TEXT_FIREBALL_READY);						/* 225B-2262 */
		break;
	case ACTION_CASE(ACTION_RAISTLIN_SPELL_3):
		if (kind != CHAR_RAISTLIN) {
			show_text_panel(TEXT_ONLY_MAGIC_USER);
			break;
		}
		*act = 0;						/* 227B-227E */
		g_raistlin_menu_choice = SPELL_CHARM;
		show_text_panel(TEXT_FIREBALL_READY);						/* 2288-228F */
		break;
	case ACTION_CASE(ACTION_RAISTLIN_SPELL_2):
		if (kind != CHAR_RAISTLIN) {
			show_text_panel(TEXT_ONLY_MAGIC_USER);
			break;
		}
		*act = 0;						/* 22AB-22AE */
		g_raistlin_menu_choice = SPELL_DETECT_MAGIC;
		show_text_panel(TEXT_FIREBALL_READY);						/* 22B8-22BF */
		break;
	case ACTION_CASE(ACTION_RAISTLIN_SPELL_11):
		if (kind != CHAR_RAISTLIN) {
			(void)message_box(MSG_BOX_NOTICE, MSG_RAISTLIN_ONLY);
			break;
		}
		g_raistlin_menu_choice = RAISTLIN_MENU_CHOICE_11;
		op -= SPELL_OPCODE_BASE;
		if (op < 0)
			op = 0;
		*act = op;						/* 22EE-22F4 */
		break;
	case ACTION_CASE(ACTION_ADD_RANDOM_8):
		rec->hp = (uint16)((int16)rec->hp + rnd(8) + 1);				/* 23E7-23F1 */
		goto clamp_done;
	case ACTION_CASE(ACTION_DRAGON_TRANSFORM):
		if ((uint8)g_map_mode_char != MAP_SCENE_DRAGON_AFTERMATH) {
			(void)event_raise(EVENT_TRANSFORM_WRONG_MAP);				/* 23D6-23DD */
			break;
		}
		show_text_panel(TEXT_DRAGON_FLIES);						/* 2303-230A */
		(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);					/* 230D-2314 */
		MAP_GRID_VALUE(MAP_RECORD_AT(MAP_DRAGON_RECORD), MAP_DRAGON_MOVE_CELL) =
			MAP_RECORD_AT(MAP_DRAGON_RECORD)->free_list;		/* 2317-231A */
		MAP_GRID_HI(MAP_RECORD_AT(MAP_DRAGON_FLAG_RECORD), MAP_DRAGON_FLAG_CELL)
			|= DRAGON_TRANSFORM_FLAG_BITS;
		map_record_show(MAP_DRAGON_ATTACK, 1);				/* 2322-232D */
		g_map_mode_char = MAP_DRAGON_ATTACK;
		ACTOR_AT(ACTOR_DRAGON)->direction = DIRECTION_WEST;
		ACTOR_AT(ACTOR_DRAGON)->mode = ACTOR_MODE_CHASE;
		ACTOR_AT(ACTOR_DRAGON)->impulse = DRAGON_FLIGHT_TICKS;
		ACTOR_AT(ACTOR_DRAGON)->vel_x = -DRAGON_FLIGHT_SPEED;
		ACTOR_AT(ACTOR_DRAGON)->vel_y = 0;
		for (i = 0; i != DRAGON_FLIGHT_FRAMES; i++) {			/* 2350-2380 */
			(void)draw_map_record(MAP_DRAGON_ATTACK);
			(void)shadow_screen_restore(7);
			(void)free_list_clear_nodes();
			(void)actor_sweep();
			(void)draw_terrain_sprites();
			(void)shadow_page_flip();
		}
		(void)shadow_video_flags(SHADOW_BLIT_RESET);				/* 2382-2389 */
		g_map_mode_char = MAP_SCENE_DRAGON_AFTERMATH;
		g_pane_base_y = 4;
		g_pane_base_x = 4;
		terrain_change();
		(void)shadow_screen_restore(7);					/* 239D-23A4 */
		(void)actor_sweep();
		(void)draw_terrain_sprites();
		(void)shadow_page_flip();
		(void)shadow_screen_restore(7);					/* 23B0-23B7 */
		PARTY_REC(PARTY_FOLLOWER_LEADER)->kind = 0;
		MAP_GRID_HI(MAP_RECORD_AT(MAP_DRAGON_FLAG_RECORD), MAP_DRAGON_FLAG_CELL)
			= 0;
		(void)shadow_video_page(SHADOW_PAGE_DEFAULT);					/* 23C4-23CA */
		*act = 0;						/* 23CD-23D0 */
		break;
	case ACTION_CASE(ACTION_ADD_RANDOM_21):
		rec->hp = (uint16)((int16)rec->hp + rnd(21) + 6);			/* 23F9-2406 */
clamp_done:
		if ((int16)rec->hp_max < (int16)rec->hp)		/* 240C-2416 */
			rec->hp = rec->hp_max;
		*act = 0;						/* 2419-241C */
		(void)event_raise(EVENT_HP_CHANGED);					/* 2420-2427 */
		(void)draw_hero_gauges();
		break;
	case ACTION_CASE(ACTION_LEVEL_DOWN_1):
		rec->stats.armor_class = (int8)(rec->stats.armor_class - 1);
		goto edge_tail;
	case ACTION_CASE(ACTION_LEVEL_DOWN_2):
		rec->stats.armor_class = (int8)(rec->stats.armor_class - 2);
edge_tail:
		(void)event_raise(EVENT_LEVEL_DRAINED);					/* 243F-2446 */
		*act = 0;						/* 2449-244C */
		break;
	case ACTION_CASE(ACTION_RESET_LEVEL):
		if (((int16)(int8)rec->base_stats.armor_class - 8) !=
		    (int16)(int8)rec->stats.armor_class) {	/* 2456-2465 */
			rec->stats.armor_class = (int8)(rec->base_stats.armor_class - 8);
			*act = 0;					/* 2471-2474 */
		}
		break;
	case ACTION_CASE(ACTION_IGNORED_06):
	case ACTION_CASE(ACTION_IGNORED_07):
	case ACTION_CASE(ACTION_IGNORED_0D):
	case ACTION_CASE(ACTION_IGNORED_11):
	case ACTION_CASE(ACTION_IGNORED_17):
	case ACTION_CASE(ACTION_IGNORED_18):
	case ACTION_CASE(ACTION_IGNORED_1B):
	case ACTION_CASE(ACTION_IGNORED_1E):
	case ACTION_CASE(ACTION_IGNORED_1F):
	case ACTION_CASE(ACTION_IGNORED_20):
	case ACTION_CASE(ACTION_IGNORED_21):
	case ACTION_CASE(ACTION_IGNORED_22):
	case ACTION_CASE(ACTION_IGNORED_23):
	case ACTION_CASE(ACTION_IGNORED_24):
	case ACTION_CASE(ACTION_IGNORED_25):
	case ACTION_CASE(ACTION_IGNORED_26):
	case ACTION_CASE(ACTION_IGNORED_29):
	case ACTION_CASE(ACTION_IGNORED_2A):
	case ACTION_CASE(ACTION_IGNORED_2B):
	case ACTION_CASE(ACTION_IGNORED_2C):
	case ACTION_CASE(ACTION_IGNORED_2D):
	case ACTION_CASE(ACTION_IGNORED_2E):
	case ACTION_CASE(ACTION_IGNORED_2F):
	case ACTION_CASE(ACTION_IGNORED_30):
	case ACTION_CASE(ACTION_IGNORED_31):
	case ACTION_CASE(ACTION_IGNORED_32):
	default:
		break;
	}
}

/* 0x03DC */
void clear_stale_sites(void)
{
	uint8 *q;
	int16 idx;
	int16 i, j;
	int16 v;
	int16 off = SITE_TABLE_ADDR;
	int16 done = 0;

	if (g_stale_clear_done != 0)				/* 03E8-03EF */
		return;
	g_stale_clear_done = 1;

	for (i = 0; i != 4; i++) {			/* 03FD-040B */
		idx = (int16)(uint8)g_hero_slots[i].state;
		if (idx >= 0x14)
			continue;

		for (j = 0; j != SITE_COUNT; j++, off += 0x10) {	/* 0429-043B */
			q = walk_table(off);
			if (q == 0)
				continue;
			if ((SITE_FLAG_LO(q) & 2) == 0)		/* 0440-0444 */
				continue;
			if ((int16)SITE_MAP_BYTE(q) != idx)		/* 0446-044E */
				continue;

			v = SITE(q)->id;
			if (v >= 0x14 && v <= 0x21)	/* 0452-045A */
				SITE(q)->id = 0;

			if (done == 0) {		/* 0460-0464 */
				done = 1;
				(void)shadow_video_flags(SHADOW_BLIT_FLUSH);	/* 0466-046D */
				(void)shadow_video_page(SHADOW_PAGE_DEFAULT);	/* 0470-0476 */
				show_text_panel(TEXT_RUNE_ITEMS);		/* 0479-0480 */
				(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);	/* 0483-048A */
			}

			v = SITE(q)->id;
			if (v == 0x27 || v == 0x33)	/* 0497-049F */
				SITE(q)->id = 0;
		}
	}

	if (region_loaded(0) != 0)				/* 04AA-04B5 */
		mem_zero(g_game_state.golmoon_spell_count, 10);	/* 0x223C */
	if (region_loaded(0xb) != 0)			/* 04C5-04D1 */
		mem_zero(g_game_state.elistan_spell_count, 10);	/* 0x2248 */
	if (region_loaded(3) != 0)				/* 04E1-04ED */
		mem_zero(g_game_state.raistlin_spell_count, 8);	/* 0x2254 */
	if (region_loaded(8) != 0)				/* 04FD-0509 */
		mem_zero(g_game_state.gilthanas_spell_count, 4);	/* 0x225C */
}

/* 0x051E */

void calm_heroes_clear_items(void)
{
	struct character_rec *rec;
	struct site_rec *it;
	int16 kind;
	int16 i, j;
	int16 v;
	int16 slot = 0;

	if (g_terrain_change_done != 0)					/* 0525-052C */
		return;
	g_terrain_change_done = 1;
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);					/* 0535-053C */
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);					/* 053F-0545 */
	show_text_panel(TEXT_RUNE_WEAPONS);						/* 0548-054F */
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);					/* 0552-0559 */

	for (i = 0; i != 4; i++) {				/* 0561-0571 */
		kind = (int16)(uint8)g_hero_slots[i].state;
		rec = CHAR_REC(character_at(g_record_addr[kind]));	/* 0588-0590 */

		if (ROLL_D100() < 90)				/* 0593-05A0 */
			rec->ranged_weapon.icon = 0;				/* 05A2-05A5 */
		if (ROLL_D100() < 90)				/* 05AA-05B7 */
			rec->hand_weapon.icon = 0;			/* 05B9-05BC */

		if (kind >= 0x14)
			continue;

		for (j = 0; j != SITE_COUNT; j++, slot++) {		/* 05C7-05D9 */
			if (slot >= 80)
				continue;
			it = &g_site_table[slot];
			if ((SITE_FLAG_LO(it) & 2) == 0)		/* 05DE-05E2 */
				continue;
			if ((int16)SITE_MAP_BYTE(it) != kind)		/* 05E4-05EC */
				continue;
			v = it->id;
			if (v < 2 || v > 0x13)			/* 05F0-05F8 */
				continue;
			if (ROLL_D100() < 90)			/* 05FA-0607 */
				it->id = 0;			/* 0609-060C */
		}
	}
}

/* 0x24FB */

void spend_character_work(int16 delta, int16 mode)
{
	int16 i;

	for (i = 0; i != 4; i++) {					/* 2508-2516 */
		int16 si = (int16)(int16)g_hero_slots[i].state;

		if (si < 0 || si >= 0x16)				/* 252A-2531 */
			continue;

		{
			struct character_rec *rec =
			    CHAR_REC(character_at(g_record_addr[si]));	/* 2533-253B */

			if (mode != 0) {					/* 253E-2542 */
				rec->hp = 1;				/* 2544-2546 */
			} else {
				rec->hp = (uint16)((int16)rec->hp + delta);	/* 254D-2553 */
				if ((int16)rec->hp > (int16)rec->hp_max)	/* 2556-2561 */
					rec->hp = rec->hp_max;
			}
		}
	}

	g_frame_debt += TICKS_PER_SECOND;
}

extern int16 shadow_video_flags(int16 v);
extern int16 mouse_button_read(void);

/* 0x0EEE */

const uint16 g_move_delta[9] = {
	0xfe00, 0xfe02, 0x0002, 0x0202, 0x0200, 0x02fe, 0x00fe, 0xfefe, 0x0000,
};

/* 0x0FCA */

const uint16 g_spawn_pairs[8] = {
	0x0005, 0x0102, 0x0106, 0x0401, 0x0601, 0x0702, 0x0804, 0x0806,
};

extern void terrain_change(void);					/* 0x5F32 */
extern void actor_try_step(struct actor_rec *);				/* 0x3292 */
extern int16 path_find(int16, int16, int16, int16);			/* 0xA7AF */
extern void driver_resume_and_call(int16 entry, int16 arg);	/* 0xD2C7 */
extern char g_play_mode;						/* 0x9E0C */

/* 0x33F6 */

#include "structs.h"

/* 0x33F6 */
void record_move_dispatch(struct actor_rec *actor)
{
	uint8 *p;
	int16 snap = 0;
	int16 b;
		int16 sel;
	int16 v;

	if ((int16)(int8)actor->state > 4)			/* 3403-3409 */
		snap = actor->vel_x | actor->vel_y;
	actor_try_step(actor);
	actor->x = (int16)((int16)actor->x + (int16)actor->vel_x);
	actor->y = (int16)((int16)actor->y + (int16)actor->vel_y);

	if ((int16)(int8)actor->state > 4 && snap != 0 &&
	    (actor->vel_x | actor->vel_y) == 0) {	/* 3431-3448 */
		actor->impulse = ACTOR_REPATH_TICKS;
		actor->mode = ACTOR_MODE_CHASE;
		actor->direction = (char)rnd(8);				/* 3455-3462 */
		actor->direction = (char)path_find(
			(int16)ACTOR_AT((int16)(int8)actor->anchor_index)->x >> 4,	/* 0x2974 */
			(int16)ACTOR_AT((int16)(int8)actor->anchor_index)->y >> 4,	/* 0x2978 */
			(int16)actor->x >> 4,
			(int16)actor->y >> 4);	/* 3470-3493 */
		actor->vel_x = (int16)(int8)(g_move_delta[(int16)(int8)actor->direction] & 0xff);
		actor->vel_y = (int16)(int8)(g_move_delta[(int16)(int8)actor->direction] >> 8);
	}

	if (g_map_cell_kind != 0) {
		if (actor->x <= 0) {
			b = (int16)(int8)g_map_working.exit_left_guard;	/* 0xa1b2 */
			if ((b & MAP_EXIT_GUARD_DISABLED) == 0) {
				if (b == 0 || ((uint16)actor->y & 8) != 8 ||
				    ((int16)actor->y >> 4) != b) {
					actor->x -= actor->vel_x;
					if (actor->vel_y == 0)
						actor->mode = ACTOR_MODE_IDLE;
					else
						actor->direction = (uint8)(actor->direction & 0xfc);
				} else {
					g_pending_terrain = (int16)(int8)g_map_working.exit_left_map;
					g_window_stamp = (int16)(int8)actor->direction;
					g_pane_base_x = 8;
					g_pane_base_y = (int16)actor->y >> 4;
				}
			}
		} else if (actor->x > 0x9f) {
			b = (int16)(int8)g_map_working.exit_right_guard;	/* 0xa1ae */
			if ((b & MAP_EXIT_GUARD_DISABLED) == 0) {
				if (b == 0 || ((uint16)actor->y & 8) != 8 ||
				    ((int16)actor->y >> 4) != b) {
					actor->x -= actor->vel_x;
					if (actor->vel_y == 0)
						actor->mode = ACTOR_MODE_IDLE;
					else
						actor->direction = (uint8)(actor->direction & 0xfc);
				} else {
					g_pending_terrain = (int16)(int8)g_map_working.exit_right_map;
					g_window_stamp = (int16)(int8)actor->direction;
					g_pane_base_x = 0;
					g_pane_base_y = (int16)actor->y >> 4;
				}
			}
		}
		if (actor->y <= 0) {
			b = (int16)(int8)g_map_working.exit_top_guard;	/* 0xa1ac */
			if ((b & MAP_EXIT_GUARD_DISABLED) == 0) {
				if (b == 0 || (actor->x & 8) != 8 ||
				    ((int16)actor->x >> 4) != b) {
					actor->y -= actor->vel_y;
					if (actor->vel_x == 0)
						actor->mode = ACTOR_MODE_IDLE;
					else {
						actor->direction = (uint8)(actor->direction & 0xfe);
						actor->direction = (uint8)(actor->direction | 2);
					}
				} else {
					g_pending_terrain = (int16)(int8)g_map_working.exit_top_map;
					g_window_stamp = (int16)(int8)actor->direction;
					g_pane_base_x = (int16)actor->x >> 4;
					g_pane_base_y = 8;
				}
			}
		} else if (actor->y > 0x9f) {
			b = (int16)(int8)g_map_working.exit_bottom_guard;	/* 0xa1b0 */
			if ((b & MAP_EXIT_GUARD_DISABLED) == 0) {
				if (b == 0 || (actor->x & 8) != 8 ||
				    ((int16)actor->x >> 4) != b) {
					actor->y -= actor->vel_y;
					if (actor->vel_x == 0)
						actor->mode = ACTOR_MODE_IDLE;
					else {
						actor->direction = (uint8)(actor->direction & 0xfe);
						actor->direction = (uint8)(actor->direction | 2);
					}
				} else {
					g_pending_terrain = (int16)(int8)g_map_working.exit_bottom_map;
					g_window_stamp = (int16)(int8)actor->direction;
					g_pane_base_x = (int16)actor->x >> 4;
					g_pane_base_y = 0;
				}
			}
		}
	}

	sel = (int16)g_pending_terrain;
	if (sel == TERRAIN_FRUIT_FOREST) {
		if (g_game_state.fruit_forest_done == 0) {		/* 0x2296 */
			if ((g_map_tile[MAP_TILE_DRAGON_LAIR] & TILE_DRAGON_EVENT) == 0) {	/* 0x2d47 */
				event_raise(EVENT_TERRAIN_40);
				actor->x -= actor->vel_x;
				actor->pixel_x = actor->x & (int16)PIXEL_TILE_MASK;
				actor->status = 0;
				g_pending_terrain = 0;
			} else {
				if (g_game_state.fruit_forest_scene_done == 0) {	/* 0x2276 */
					draw_presentation(PRESENTATION_FRUIT_FOREST);
					g_game_state.fruit_forest_scene_done = 1;
					((uint8 *)g_map_records)[MAP_FRUIT_FOREST_MARK] = MAP_FRUIT_FOREST_MARK_VALUE;
					map_record_show(TERRAIN_MOUNTAIN_LAKE, 0);
				}
				g_game_state.fruit_forest_done = 1;
				if ((SITE_AT(SITE_SLOT_FRUIT_FOREST)->flags | 2) == 0 ||
				    region_loaded(SITE_AT(SITE_SLOT_FRUIT_FOREST)->map) == 0)
					clear_stale_sites();
			}
		}
	} else if (sel == TERRAIN_RIVER_VALLEY) {
		if (g_game_state.river_valley_done == 0) {		/* 0x2262 */
			g_game_state.river_valley_done = 1;
			event_raise((g_map_tile[MAP_TILE_DRAGON_LAIR] & TILE_DRAGON_EVENT) == 0 ? EVENT_TERRAIN_33_CLEAR : EVENT_TERRAIN_33_BLOCKED);
		}
	} else if (sel == TERRAIN_KHAROLIS_MOUNTAINS) {
		if (g_game_state.kharolis_done == 0) {		/* 0x2292 */
			g_game_state.kharolis_done = 1;
			if ((SITE_AT(SITE_SLOT_KHAROLIS_A)->flags | 2) == 0 ||
			    (SITE_AT(SITE_SLOT_KHAROLIS_B)->flags | 2) == 0 ||
			    region_loaded(SITE_AT(SITE_SLOT_KHAROLIS_A)->map) == 0 ||
			    region_loaded(SITE_AT(SITE_SLOT_KHAROLIS_B)->map) == 0)
				calm_heroes_clear_items();
		}
	} else if (sel == TERRAIN_MOUNTAIN_LAKE) {
		if (g_game_state.mountain_lake_done == 0) {		/* 0x2294 */
			g_game_state.mountain_lake_done = 1;
			if ((SITE_AT(SITE_SLOT_MOUNTAIN_LAKE_A)->flags | 2) == 0 ||
			    (SITE_AT(SITE_SLOT_MOUNTAIN_LAKE_B)->flags | 2) == 0 ||
			    region_loaded(SITE_AT(SITE_SLOT_MOUNTAIN_LAKE_A)->map) == 0 ||
			    region_loaded(SITE_AT(SITE_SLOT_MOUNTAIN_LAKE_B)->map) == 0) {
				event_raise(EVENT_TERRAIN_3F);
				spend_character_work(0, 1);
			}
		}
	} else if (sel == TERRAIN_SWAMP) {
		if (g_play_mode > 1)
			driver_resume_and_call(DRIVER_ENTRY_QUEUE, DRIVER_SEQ_ENDGAME);
		draw_presentation(PRESENTATION_ENDGAME);
		g_game_end_flag = 1;
		g_pending_terrain = 0;
	} else if (sel == TERRAIN_DWARF_STOCKADE) {
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
		(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
		show_text_panel(TEXT_KEEP_OUT);
		(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
		g_pane_base_y = 4;
		g_pane_base_x = 4;
		terrain_change();
	} else if (sel == TERRAIN_RUNE_STONES) {
		do {
			screen_refresh();
			v = question_five_choices((int16)g_map_mode_char == MAP_NEIDAR_VILLAGE ? TEXT_NEIDAR_KING : TEXT_AGHAR_KING);
			if (v == 0) {
				g_companion_react_skip = 1;
				clock_bank_timers();
				g_frame_debt = 0;
				while (g_clock_hour != 4) {
					(void)clock_advance(1);
					(void)party_update_all(1, 0);
					(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
					clock_draw_time();
					(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
				}
			} else if (v == 1) {
				draw_presentation(PRESENTATION_KING_FOOD);
				spend_character_work(8, 0);
			} else if (v == 2) {
				event_raise(EVENT_MAP_FEATURE_USED);
				(void)show_screen_wait(TEXT_KING_MAP);
				map_record_show((int16)(int8)g_map_mode_char, 0);
			} else if (v == 3) {
				(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
				if ((int16)g_map_mode_char == MAP_NEIDAR_VILLAGE) {
					p = (uint8 *)SITE_AT(SITE_SLOT_BEES);	/* 0x2778 */
					if (SITE(p)->id != 0) {
						if ((SITE_FLAG_LO(p) & 1) == 0) {	/* 0x2786 */
							if (region_loaded(SITE(p)->map) != 0) {
								SITE(p)->id = 0;
								p = (uint8 *)SITE_AT(SITE_SLOT_NECTAR);
								SITE(p)->id = 0;
								show_text_panel(TEXT_NECTAR);
								PARTY_REC(PARTY_REFUGEES)->score += NECTAR_SCORE_REWARD;
							}
						} else {
							show_text_panel(TEXT_BEES);
						}
					}
				} else {
					show_text_panel(TEXT_GOOD_DRAGON);
				}
				(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
				map_record_show((int16)(int8)g_map_mode_char, 0);
			} else if (v == 4) {
				(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
				show_text_panel((int16)g_map_mode_char == MAP_NEIDAR_VILLAGE ? TEXT_NEIDAR_JOURNAL9 : TEXT_AGHAR_JOURNAL10);
				(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
			}
		} while (v != 4);
		g_pane_base_y = 4;
		g_pane_base_x = 4;
		g_pending_terrain = 0;
		terrain_change();
	} else if (sel == TERRAIN_NARROW_CANYON) {
		event_raise(EVENT_COMPANION_QUESTION);
		if (question_two_buttons(TEXT_WAIT_UNTIL_DUSK) != 0) {
			g_companion_react_skip = 1;
			clock_bank_timers();
			g_frame_debt = 0;
			while (g_clock_hour != HOUR_20) {
				(void)clock_advance(1);
				(void)party_update_all(1, 1);
				(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
				clock_draw_time();
				(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
			}
			(void)show_screen_wait(SCREEN_WAIT_NO_PANEL);
		}
		g_pane_base_y = 4;
		g_pane_base_x = 4;
		g_pending_terrain = 0;
		terrain_change();
	} else if (sel == TERRAIN_DIFFICULT_CLIMB) {
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
		(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
		show_text_panel(TEXT_VIEW_BREATHLESS);
		(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
		g_move_speed_shift = 1;
		mark_cells_redraw(PARTY_REC(0)->grid_x, PARTY_REC(0)->grid_y, 0x25,
			(uint8 *)g_ofs_even_ring);
		tile_set_position();
	}
}

/* 0x3B42 */

void record_head_for(struct actor_rec *actor, int16 goal_x, int16 goal_y)
{
	int16 dx;
	int16 dy;
	int16 code;

	actor->mode = ACTOR_MODE_CHASE;					/* 3B49-3B4C */
	dx = (int16)actor->x - goal_x;				/* 3B50-3B59 */
	dy = (int16)actor->y - goal_y;				/* 3B5C-3B62 */

	code = 0;
	if (dx < 0)
		code |= 8;					/* 3B6A-3B70 */
	if (dx > 0)
		code |= 4;					/* 3B74-3B7A */
	if (dy < 0)
		code |= 2;					/* 3B7E-3B84 */
	if (dy > 0)
		code |= 1;					/* 3B88-3B8E */

	actor->direction = g_dir_code[code];			/* 3B92-3B9C */
	actor->vel_x = (int16)(int8)(g_move_delta[(int16)(int8)actor->direction] & 0xff);	/* 3BAE-3BB9 */
	actor->vel_y = (int16)(int8)(g_move_delta[(int16)(int8)actor->direction] >> 8);	/* 3BBC-3BC5 */

	record_move_dispatch(actor);				/* 3BC8-3BCE */

	if ((int16)actor->x == (int16)actor->pixel_x &&
	    (int16)actor->y == (int16)actor->pixel_y)			/* 3BD1-3BE4 */
		actor->motion_state = ACTOR_MOTION_STOP;
}

/* 0x3BEF - dec */

void record_stray_check(struct actor_rec *actor)
{
	struct actor_rec *anchor = ACTOR_AT((int16)(int8)actor->anchor_index);	/* 3BF5-3C00 */
	int16 dx = abs_w((int16)actor->x - (int16)anchor->x) >> 1;		/* 3C03-3C15 */
	int16 dy = abs_w((int16)actor->y - (int16)anchor->y) >> 1;		/* 3C18-3C2D */
	int16 d2 = dx * dx + dy * dy;					/* 3C30-3C3E */

	if (d2 > 128)							/* 3C41-3C45 */
		record_head_for(actor, (int16)anchor->x, (int16)anchor->y);	/* 3C47-3C56 */
	else if (ROLL_D100() <= 50)					/* 3C5B-3C68 */
		actor->mode = ACTOR_MODE_TRACK;
	else
		actor->mode = ACTOR_MODE_WAIT;
}

/* 0x401D */

void record_tick_state(struct actor_rec *actor)
{
	int16 st;

	if ((int16)(int8)actor->mode > 1)
		return;
	actor->vel_y = 0;						/* 4030-4034 */
	actor->vel_x = 0;

	st = (int16)(int8)actor->motion_state;
	if (st == 0) {
		actor->mode = ACTOR_MODE_IDLE;
	} else if (st == 1) {
		if (actor->impulse != 0)
			actor_try_step(actor);
		else
			record_head_for(actor, (int16)actor->pixel_x, (int16)actor->pixel_y);
	} else if (st == 2) {
		if (actor->anchor_index != 0) {
			(void)record_direction(actor);
			record_stray_check(actor);		/* 4083-4089 */
		}
	} else if (st == 4) {
		actor->mode = ACTOR_MODE_CHASE;
		if (actor->anchor_index != 0)
			(void)record_direction(actor);
		actor->direction = (uint8)((actor->direction + 4) & 7);
		actor->vel_x = (int16)(int8)(g_move_delta[(int16)(int8)actor->direction] & 0xff);
		actor->vel_y = (int16)(int8)(g_move_delta[(int16)(int8)actor->direction] >> 8);
		record_move_dispatch(actor);
	}
}

uint16 g_search_weight;					/* 0x9f22 */
uint16 g_search_center_x, g_search_center_y;			/* 0xa99c */
uint8 g_matching_site_list[SITE_COUNT];			/* 0xa9d4 */

/* 0x061A */

void collect_matching_sites(int16 *outp, int16 want, uint16 mask_req, uint16 mask_forbid, int16 depth)
{
	struct site_rec *p;
	int16 i;
	int16 kind;
	int16 xlo = (int16)g_search_center_x - 0x20;			/* 0625-062B */
	int16 xhi = (int16)g_search_center_x + 0x20;			/* 062E-0634 */
	int16 ylo = (int16)g_search_center_y - 0x20;			/* 0637-063D */
	int16 yhi = (int16)g_search_center_y + 0x20;			/* 0640-0646 */

	for (i = 0, p = SITE(g_site_table); i != SITE_COUNT; i++, ++p) {
		kind = (int16)SITE_KIND(p);				/* 065C-0663 */
		if (kind == SITE_KIND_NONE)
			continue;
		if (kind >= SITE_KIND_LIMIT)
			continue;
		if (kind == SITE_KIND_TRIGGER)
			continue;
		if ((p->flags & mask_req) != mask_req)		/* 067A-0683 */
			continue;
		if ((p->flags & mask_forbid) != 0)		/* 0685-068B */
			continue;
		if ((int16)SITE_MAP_BYTE(p) != want)			/* 068D-0695 */
			continue;
		if ((mask_req & 1) != 0) {				/* 0697-06BB */
			if ((int16)p->x < xlo || (int16)p->x > xhi)
				continue;
			if ((int16)p->y < ylo || (int16)p->y > yhi)
				continue;
		}
		g_matching_site_list[*outp] = (uint8)i;	/* 06BD-06C5 */
		(*outp)++;						/* 06C9-06CC */
		g_search_weight += p->weight;				/* 06CE-06D4 */
		if ((SITE_FLAG_LO(p) & 0x10) != 0)			/* 06D8-06DC */
			collect_matching_sites(outp, i, 4, mask_forbid, depth + 1);	/* 06DE-06F3 */
	}
}

extern int16 rect_scan(int16 count, uint8 *table);	/* 0x0CDC */
extern void draw_hero_gauge(int16 i);					/* 0x00C0 */
extern void hero_member_retire(int16 idx);					/* 0x0873 */
extern int16 effect_publish(int16, int16, int16, int16, int16, int16, int16, int16, int16, int16);	/* 0xBdfa */
extern uint16 g_objpic_at;					/* 0xa9a8 */
extern uint16 g_mouse_x;					/* 0xa9c2 */
extern uint16 g_panel_stride_mode, g_row_height;

/* 0x1AB6 */

void player_pick_nearby(int16 slot)
{
	struct site_rec *rec;
	struct character_rec *srec;
	int16 q_kind;
	int16 r_kind;
	int16 count = 0;
	int16 cursor = 0;
	int16 code;
	int16 ox, oy;
	int16 x, y;
	int16 w;
	int16 i;

	while (mouse_button_read() != 0) ;					/* 1AC2-1AC9 */

	q_kind = (int16)(int16)g_hero_slots[slot].state;		/* 1AD0-1ADE */
	collect_matching_sites(&count, (int16)(int8)g_map_mode_char, 0x41, 0, 0);	/* 1AE4-1AF8 */

	if (count == 0) {
		screen_refresh();
		panel_kind_draw();
		return;
	}

	panel_layout(&ox, &oy, slot, count, 0);				/* 1B04-1B18 */

	do {
		rec = &g_site_table[(int16)(int8)			/* 1B20-1B2F */
			(g_matching_site_list[cursor])];
		r_kind = (int16)SITE_KIND(rec);					/* 1B32-1B38 */
		far_pair_load(r_kind, g_objpic_at);					/* 1B3B-1B43 */

		x = ((int16)g_panel_stride_mode == 1 ? 0x10 : 10) + ox;			/* 1B46-1B5B */
		y = (((int16)0x20 - (int16)g_row_height) >> 1) + oy;			/* 1B5E-1B67 */
		sprite_draw_segment(x, y, r_kind, g_objpic_at);				/* 1B6D-1B7B */

		while (mouse_button_read() == 0)						/* 1B7E-1B8F */
			(void)mouse_poll_moved(1);
		code = rect_scan(3, (uint8 *)g_panel_boxes);			/* 1B91-1B9F */
	} while (code < 0);

	x = (int16)(int16)g_panel_boxes[code].x >> 3;				/* 1BA9-1BB9 */
	y = (int16)g_panel_top2;
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	if (code != 0) {
		(void)sprite_draw_current(code + SPRITE_SCROLL_ARROW_BASE, SPRITE_DRAW_SOLID, x, y);				/* 1BD4-1BE8 */
		(void)mouse_poll_moved(0);
		while (mouse_button_read() != 0) { }
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	}

	if (code == 0) {
		(void)shadow_video_flags(SHADOW_BLIT_RESET);
		(void)sprite_draw_current(SPRITE_PANEL_BACKGROUND, SPRITE_DRAW_SOLID, ((int16)g_panel_left >> 3) - 1,			/* 1C17-1C32 */
			(int16)g_panel_top - 8);
		g_mouse_x -= (int16)g_panel_stride_mode << 3;				/* 1C35-1C3C */
		(void)mouse_poll_moved(0);
		while (mouse_button_read() != 0)						/* 1C49-1C5A */
			(void)mouse_poll_moved(1);
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);

		SITE_FLAG_LO(rec) = (uint8)(SITE_FLAG_LO(rec) | 0x10);	/* 1C66-1C69 */
		record_flag_rebuild(rec);			/* 1C6D-1C73 */
		if ((SITE_FLAG_LO(rec) & 0x20) != 0) {
			(void)effect_publish(EFFECT_KIND_MARKER, 0, 4, rec->x, rec->y,
				0x10, 0, 0, 0, 0);				/* 1C82-1CA0 */
			SITE_FLAG_LO(rec) = (uint8)(SITE_FLAG_LO(rec) & 0xdf);	/* 1CA3-1CA6 */
			if ((int16)(int16)g_hero_slots[q_kind].state != CHAR_TASSLEHOFF || ROLL_D100() >= 45) {
				show_text_panel(TEXT_TRAP_SET_OFF);
				srec = character_at(g_record_addr[q_kind]);		/* 1CE3-1CEC */
				srec->hp =
				    (uint16)((int16)srec->hp - (rnd(10) + 2));		/* 1CEF-1CFF */
				if ((int16)(int16)srec->hp > 0) {		/* 1D02-1D09 */
					draw_hero_gauge(slot);
				} else {
					srec->hp = 0;
					hero_member_retire(slot);
				}
			} else {
				show_text_panel(TEXT_TASSLEHOFF_TRAP);
			}
		}

		if ((int16)g_map_mode_char == MAP_AGHAR_THRONE) {				/* 1D24-1D29 */
			for (i = 0; i != 8; i++) {
				w = g_spawn_pairs[i];	/* 0xfca */
				x = (((int16)(int8)(w & 0xff)) << 4) + 8;
				y = (((int16)(int8)(w >> 8)) << 4) + 8;
				spawn_descriptor(MONSTER_REQUEST_SKELETON, x, y);				/* 1D5C-1D67 */
				(void)effect_publish(EFFECT_KIND_CREATURE, 0, 4, x, y, 0x10, 0, 0, 0, 0);
			}
		}
		return;
	}

	if (code == 1) {
		if (cursor != 0) {
			(void)sprite_draw_current(SPRITE_PANEL_BACKGROUND, SPRITE_DRAW_SOLID, ((int16)g_panel_left >> 3) - 1,	/* 1D96-1DB1 */
				(int16)g_panel_top - 8);
			(void)shadow_video_flags(SHADOW_BLIT_RESET);
			cursor--;
		}
	} else if (code == 2) {
		if (count - 1 != cursor) {
			(void)sprite_draw_current(SPRITE_PANEL_BACKGROUND, SPRITE_DRAW_SOLID, ((int16)g_panel_left >> 3) - 1,	/* 1DCC-1DE7 */
				(int16)g_panel_top - 8);
			(void)shadow_video_flags(SHADOW_BLIT_RESET);
			cursor++;
		}
	}

}

extern struct effect_rec g_effect_table[EFFECT_COUNT];			/* 0x9696 */

/* 0x109C */

const int8 g_tick_by_kind[134] = {
	0x00, 0x0e, 0x0e, 0x0e, 0x06, 0x06, 0x06, 0x06, 0x06, 0x0d,
	0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x00, 0x00, 0x00, 0x00,
	0x0d, 0x06, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x0a, 0x0a,
	0x0a, 0x08, 0x08, 0x08, 0x08, 0x08, 0x07, 0x0a, 0x0a, 0x0a,
	0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,
	0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x00, 0x01, 0x01,
};

/* 0xBDFA */

int16 effect_publish(int16 kind, int16 map_lo, int16 countdown, int16 x, int16 y, int16 z,
             int16 dx, int16 dy, int16 dz, int16 id_hi)
{
	struct effect_rec *p;
	int16 i;

	if (kind == EFFECT_KIND_FRAME_SPECIAL)

		countdown = 5;

	sound_request_max((int16)g_tick_by_kind[kind]);

	for (i = 0, p = g_effect_table; i != EFFECT_COUNT; i++, p++) {	/* BE1F-BE31 */
		if (p->id != 0)
			continue;

		p->id = (uint8)kind;			/* BE3B-BE3E */
		EFFECT_ID_HI(p) = (uint8)id_hi & 7;
		p->map_lo = (uint8)map_lo;		/* BE4B-BE51 */
		p->countdown = (uint8)countdown;	/* BE54-BE5A */
		p->x = (int16)x;				/* BE5D-BE63 */
		p->y = (int16)y;				/* BE66-BE6C */
		p->z = (int16)z;				/* BE6F-BE75 */
		p->dz = (int16)dz;

		p->dx = (int16)dx;				/* BE81-BE87 */
		p->dy = (int16)dy;				/* BE8A-BE90 */
		return 1;
	}

	return 0;
}

/* 0x3039 */
void rect_separate(struct actor_rec *rec, struct actor_rec *oth)
{
	int16 rx, ry, t;
	int16 up_x, up_y, dn_x, dn_y;

	RECT_PROBE();							/* 303F-309A */

	if (up_x <= dn_x || up_y <= dn_y)				/* 309D-30B0 */
		return;

	if (ROLL_D100() < 50) {						/* 30B3-30C2 */
		if (up_x > dn_x) {
			t = up_x - dn_x;					/* 30D0-30D6 */
			if (abs_w((int16)rec->vel_x) >= t)		/* 30D9-30E8 */
				rec->vel_x = 0;

			RECT_PROBE();					/* 30F2-314D */
			if (up_x > dn_x && up_y > dn_y) {		/* 3150-315E */
				t = up_y - dn_y;				/* 3160-3166 */
				if (abs_w((int16)rec->vel_y) >= t)
					rec->vel_y = 0;
			}
		}
		return;
	}

	if (up_y > dn_y) {
		t = up_y - dn_y;						/* 3190-3196 */
		if (abs_w((int16)rec->vel_y) >= t)			/* 319C-31A8 */
			rec->vel_y = 0;

		RECT_PROBE();						/* 31B2-320D */
		if (up_x > dn_x && up_y > dn_y) {			/* 3210-321E */
			t = up_x - dn_x;					/* 3220-3226 */
			if (abs_w((int16)rec->vel_x) >= t)
				rec->vel_x = 0;
		}
	}
}

/* DGROUP 0x1CD4 */

union vote_data g_vote_data = { {
	0,1,2,2,2,2,0,0,1,2,2,2,
	0,0,1,1,2,2,0,1,2,2,2,2,
	0,0,1,1,2,2,0,0,0,0,0,8,
	0,8,0,0,0,0,8,0,0,0,0,0,
	8,8,8,0,0,8,8,0,8,0,0,0,
	38,0,123,0,67,0,212,3,18,1,35,0,
	11,0,8,0,19,1,148,2,41,0,38,0,
	40,0,140,5,9,0,120,0,10,0,10,0,
	143,2,44,1,67,0,65,0,14,1,160,15,
	232,3,217,7,217,7,217,7,20,0,20,0,
	20,0,20,0,20,0,20,0,20,0,20,0,
	20,0,20,0,20,0,20,0,20,0,20,0,
	20,0,20,0,20,0,20,0,20,0,20,0,
	20,0,20,0,0,30,35,40,45,50,55,60,
	65,70,75,80,85,90,92,94,96,98,100,0,
	2,3,4,5,7,8,9,10,11,13,2,16,
	19,2,4,8,9,0,30,30,30,10,30,0,
	30,127,5,127,127,127,127,0,127,127,60,60,
	127,0,60,30,1,0,1,1,1,0,0,0,
	0,0,127,30,30,10,15,0,127,30,10,0,
	15,20,10,0,0,0,127,127,5,0,127,10,
	0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,127,0,0,0,0,0,0,0,
	0,0,127,127,0,0,0,127,0,0,0,0,
	0,0,0,0,0,0
} };

/* 0xC5EC */

void character_attack_actor(struct actor_rec *target, int16 character)
{
	struct character_rec *attacker;
	struct damage_entry *pr;
	int16 kind, amt, n, sw, flags;

	--character;
	kind = (int16)(int16)g_hero_slots[(uint8)character].state;	/* C5F6-C609 */
	attacker = character_at(g_record_addr[kind]);			/* C60B-C60F */

	amt = (int16)attacker->level;				/* C612-C618 */
	kind = (int16)(int8)target->state;			/* C61B-C621 */

	if (kind == ACTOR_KIND_DEATH_KNIGHT && g_game_state.map_event_done == 0) {	/* C624-C630 */
		g_game_state.map_event_done = 1;
		(void)shadow_video_page(SHADOW_PAGE_DEFAULT);					/* C636-C63C */
		show_text_panel(TEXT_DWARVES_HOSTILE);						/* C63F-C646 */
		(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);					/* C649-C650 */
	}

	sw = (int16)attacker->armor_type;					/* C653-C65A */
	switch (sw) {
	case ARMOR_TABLE_A:
	case ARMOR_TABLE_B:	amt = (int16)g_roll_table.armor_bonus_b[amt] + 9;	break;
	case ARMOR_TABLE_C:	amt = (int16)g_roll_table.armor_bonus_c[amt] + 8;	break;
	case ARMOR_TABLE_D:	amt = (int16)g_roll_table.armor_bonus_d[amt] + 8;	break;
	case ARMOR_TABLE_E:	amt = (int16)g_roll_table.armor_bonus_e[amt] + 8;	break;
	default:break;
	}

	n = (int16)(int8)g_spawn_table.rec.kind[kind].attack_index;		/* C6B5-C6C9 */
	amt = (int16)g_roll_table.base_to_hit[amt + n];		/* C6CC-C6D4 */

	n -= 2;								/* C6D7-C6EC */
	if (n < 0) n = 0;
	if (n > 7) n = 7;

	pr = &g_roll_table.damage[(int16)WEAPON_WORD(attacker->hand_weapon)];		/* C6ED-C705 */
	amt -= (int16)pr->defense[n];					/* C708-C710 */
	amt -= (int16)g_roll_table.defense_modifier[(int16)(int8)attacker->stats.attributes[ATTR_STR]];		/* C713-C721 */

	if (ROLL_D20() + 1 < amt					/* C724-C73D */
	 && target->countdown == 0)
		return;

	flags = g_spawn_table.rec.kind[kind].flags;				/* C740-C751 */
	if ((flags & 2) != 0) {						/* C754-C758 */
		amt = rnd((int16)pr->dice_alt)
		    + (int16)pr->bonus_alt;				/* C75A-C773 */
	} else {
		amt = rnd((int16)pr->dice)
		    + (int16)pr->bonus;				/* C778-C791 */
	}

	sound_request_max(SOUND_REQUEST_DAMAGE);							/* C794-C79B */
	amt += (int16)g_roll_table.damage_modifier[(int16)(int8)attacker->stats.attributes[ATTR_STR]];		/* C79E-C7AC */

	if ((int16)WEAPON_WORD(attacker->hand_weapon) == WEAPON_WYRMSLAYER	/* C7AF-C7C4 */
	 && (kind <= ACTOR_KIND_SIVAK || kind >= ACTOR_KIND_RDRAGON))
		amt = (int16)((uint16)amt << 1);

	if ((int16)WEAPON_WORD(attacker->hand_weapon) == WEAPON_LONGSWORD		/* C7C7-C7DC */
	 && (kind == ACTOR_KIND_WIGHT || kind == ACTOR_KIND_GHOUL))
		amt = 0;

	target->health = (uint8)(target->health - (uint8)amt);

	if ((int16)(int8)target->health >= 0)				/* C7EA-C7F1 */
		return;

	if (kind == ACTOR_KIND_TROLL) {						/* C7F3-C7F7 */
		target->health = 1;
		return;
	}

	target->mode = ACTOR_MODE_RETIRED;							/* C7F9-C803 */
	target->status = 0;
	if (kind == ACTOR_KIND_DWARVES)						/* C804-C80A */
		load_current_map();

	g_game_state.kill_count[kind] += 1;			/* C80D-C812 */
	g_game_state.combat_experience = g_game_state.combat_experience	/* C816-C81A */
		+ (int16)VOTE_EXPERIENCE_VALUE(kind);
}

/* 0x0BD8 */

const int16 g_probe_table[42] = {
	-16,-32,0,-32,16,-32,-32,
	-16,-16,-16,0,-16,16,-16,
	32,-16,-32,0,-16,0,0,
	0,16,0,32,0,-32,16,
	-16,16,0,16,16,16,32,
	16,-16,32,0,32,16,32
};

uint16 g_action_countdown;					/* 0x9e12 */
uint16 g_action_anchor_x;					/* 0x9f20 */
uint16 g_action_anchor_y;					/* 0x9f24 */

/* 0xC053 */

int16 effect_step_kind(struct effect_rec *rec)
{
	struct effect_rec *o = rec;
	int16 kind = (int16)(int8)EFFECT_KIND(o);			/* C05D-C060 */
	int16 state, b3, i, x, y, v, sel, n;
	struct map_record *map;
	const int16 *pb;

	if (kind == EFFECT_KIND_INERT)
		return 0;							/* C063-C06A */

	state = 1;
	b3 = 4;

	if (kind == EFFECT_KIND_RECOVER_WEAPON_17 || kind == EFFECT_KIND_RECOVER_WEAPON_9) {				/* C077-C081 */
		state = EFFECT_KIND_INERT;
		b3 = 1;
		x = (int16)o->x - (int16)o->dx;	/* C08D-C096 */
		y = (int16)o->y - (int16)o->dy;	/* C099-C09F */
		n = (kind == EFFECT_KIND_RECOVER_WEAPON_17) ? WEAPON_RECORD_17 : WEAPON_RECORD_9;				/* C0A2-C0B0 */
		probe_record_kind(n, x, y, 0, PROBE_MASK_VISIBLE);				/* C0B3-C0C6 */
	}

	if (kind == EFFECT_KIND_FIREBALL || kind == EFFECT_KIND_LARGE_EFFECT) {			/* C0C9-C0D3 */
		g_action_anchor_x = (int16)o->x;			/* C0D8-C0DE */
		g_action_anchor_y = (int16)o->y;			/* C0E1-C0E4 */
		n = (kind == EFFECT_KIND_FIREBALL) ? AREA_RADIUS_FIREBALL : AREA_RADIUS_LARGE;			/* C0E7-C0F5 */
		g_action_countdown = rnd(n) + 6;				/* C0F8-C102 */
		EFFECT_KIND(o) = 2;							/* C105-C108 */
		o->countdown = 6;							/* C10B-C10E */

		n = (int16)(int8)g_map_mode_char;
		if (n < 10)
			map = &g_map_working;
		else
			map = MAP_RECORD_AT(n - 1);	/* C123-C12C */

		pb = g_probe_table;
		for (i = 0; i != 21; ++i) {				/* C132-C142 */
			x = (int16)o->x + *pb++;		/* C145-C154 */
			y = (int16)o->y + *pb++;		/* C157-C166 */
			if (x <= 0 || x >= WORLD_COORD_MAX)			/* C169-C179 */
				continue;
			if (y <= 0 || y >= WORLD_COORD_MAX)			/* C17C-C188 */
				continue;

			effect_publish(EFFECT_KIND_MARKER, 0, 4, x, y, EFFECT_PUBLISH_HEIGHT, 0, 0, 0, 0);	/* C18B-C1A9 */
			x >>= 4;
			y >>= 4;
			v = (int16)MAP_GRID_VALUE(map, 10 * y + x);	/* C1B4-C1D0 */

			sel = MAP_CELL_DEFAULT;
			if (v == MAP_CELL_WATER)
				sel = MAP_CELL_WATER;						/* C1D8-C1E2 */
			if (v >= MAP_CELL_HIGH_MIN) {
				if (v == MAP_CELL_DOLLAR)
					sel = MAP_CELL_SPECIAL_13;					/* C1E8-C1F3 */
				else if (v == MAP_CELL_22 || v == MAP_CELL_23 || v == MAP_CELL_25)
					sel = MAP_CELL_DEFAULT;					/* C1F5-C20C */
				else if (v >= MAP_CELL_27 && v <= MAP_CELL_29)
					sel = MAP_CELL_SPECIAL_26;					/* C20E-C21F */
				else
					sel = v;					/* C221-C224 */
			}
			MAP_GRID_VALUE(map, 10 * y + x) = (uint16)sel;	/* C227-C232 */
		}

		map_record_show((int16)(int8)g_map_mode_char, 0);	/* C237-C242 */
	} else if (kind == EFFECT_KIND_DEACTIVATE_6 || kind == EFFECT_KIND_DEACTIVATE_7 || kind == EFFECT_KIND_DEACTIVATE_8 || kind == EFFECT_KIND_DEACTIVATE_15) {
		state = EFFECT_KIND_INERT;						/* C247-C25D */
		b3 = 1;								/* C25F-C264 */
	}

	EFFECT_KIND(o) = (uint8)state;				/* C269-C26F */
	o->countdown = (uint8)b3;					/* C271-C277 */

	sound_request_max((int16)g_tick_by_kind[(int16)(int8)EFFECT_KIND(o)]);	/* C27A-C28B */

	EFFECT_ID_HI(o) = 0;
	o->dx = 0;
	o->dy = 0;
	o->dz = 0;

	return 0;

}

/* 0x3246 */

void offer_record_to_actors(struct actor_rec *rec)
{
	struct actor_rec *row;
	int16 i;

	for (i = 0, row = g_actor_table; i != ACTOR_COUNT; ++i, row++) {	/* 3252-3264 */
		if (row->state == 0)								/* 3266-326C */
			continue;
		if (row->x == rec->x &&							/* 326E-327F */
		    row->y == rec->y)
			continue;
		rect_separate(rec, row);							/* 3281-3288 */
	}
}

/* 0xC58A */

void character_react_move(struct actor_rec *w)
{
	struct actor_rec *target = ACTOR_AT((int16)(int8)w->anchor_index);	/* C590-C59B */

	if (target->state == 0)									/* C5A0-C5A3 */
		return;

	if (ROLL_D100() > 50)								/* C5A5-C5B2 */
		character_attack_actor(target, (int16)(int8)w->state);	/* C5B4-C5C1 */

	effect_publish(EFFECT_KIND_INERT, 0, 1, (int16)target->x, (int16)target->y,
	    0x10, 0, 0, 0, 0);								/* C5C4-C5E5 */
}

/* 0x3292 */

void actor_try_step(struct actor_rec *actor)
{
	int16 x = (int16)actor->x + (int16)actor->vel_x;				/* 3298-32A1 */
	int16 y = (int16)actor->y + (int16)actor->vel_y;				/* 32A4-32AA */

	if ((x | y) & 0xf) {							/* 32AD-32B5 */
		switch ((uint8)actor->direction) {					/* 32BA-33D4 */
		case DIRECTION_NORTH:
			probe_north(&x, &y, actor);			/* 32C1-32CF */
			break;
		case DIRECTION_NORTH_EAST:
			probe_north(&x, &y, actor);			/* 32D5-32E3 */
			y = (int16)actor->y + (int16)actor->vel_y;	/* 32E6-32EF */
			probe_east(&x, &y, actor);			/* 32F2-32FE */
			break;
		case DIRECTION_EAST:
			probe_east(&x, &y, actor);			/* 3304-3312 */
			break;
		case DIRECTION_SOUTH_EAST:
			probe_east(&x, &y, actor);			/* 3318-3326 */
			x = (int16)actor->x + (int16)actor->vel_x;	/* 3329-3332 */
			probe_south(&x, &y, actor);			/* 3335-3341 */
			break;
		case DIRECTION_SOUTH:
			probe_south(&x, &y, actor);			/* 3347-3355 */
			break;
		case DIRECTION_SOUTH_WEST:
			probe_south(&x, &y, actor);			/* 335B-3369 */
			y = (int16)actor->y + (int16)actor->vel_y;	/* 336C-3375 */
			probe_west(&x, &y, actor);			/* 3378-3384 */
			break;
		case DIRECTION_WEST:
			probe_west(&x, &y, actor);			/* 3389-3397 */
			break;
		case DIRECTION_NORTH_WEST:
			probe_west(&x, &y, actor);			/* 339C-33AA */
			x = (int16)actor->x + (int16)actor->vel_x;	/* 33AD-33B6 */
			probe_north(&x, &y, actor);			/* 33B9-33C5 */
			break;
		default:
			break;
		}
	}
	offer_record_to_actors(actor);								/* 33E9-33EF */
}

/* 0x3E02 - dec */

void actor_take_action(int16 idx, struct actor_rec *rec)
{
	struct character_rec *tgt;
	struct actor_rec *target;
	uint16 i;
	int16 w, qi, dx, dy, d2, j;

	if ((int16)(int8)rec->mode > 1)				/* 3E08-3E11 */
		return;
	if (rec->impulse != 0)								/* 3E14-3E1A */
		return;

	w = (int16)(int16)g_hero_slots[idx].state;				/* 3E1D-3E33 */
	i = (uint16)(w & 0xff);							/* 3E36-3E3B */
	if (i < 23)
		tgt = character_at(g_record_addr[i]);
	else
		tgt = character_at((uint16)PANEL_TEMPLATE_CHAR_ADDR(i));

	if (tgt->orders & 4) {									/* 3E44-3E4A */
		if (((int16)tgt->hp_max >> 2) > (int16)tgt->hp) {	/* 3E4C-3E56 */
			target->motion_state = ACTOR_MOTION_WANDER;							/* 3E58-3E5F */
			return;
		}
	}

	if (tgt->orders & 2) {									/* 3E62-3E69 */
		qi = choose_target(rec, idx, 4, 0x10);					/* 3E6E-3E82 */
		if (qi != -1) {								/* 3E85-3E88 */
			target = ACTOR_AT(qi);					/* 3E8B-3E94 */
			dx = ((int16)rec->x - (int16)target->x) >> 1;	/* 3E97-3EA5 */
			dy = ((int16)rec->y - (int16)target->y) >> 1;	/* 3EA8-3EB6 */
			d2 = dx * dx + dy * dy;						/* 3EB9-3EC7 */
			if (d2 > SPELL_RANGE_SQ) {							/* 3ECA-3ECE */
				j = state_query(idx);					/* 3ED0-3ED9 */
				if (j != 0) {
					g_cursor_tile_x = (int16)target->x >> 4;	/* 3EE0-3EEA */
					g_cursor_tile_y = (int16)target->y >> 4;	/* 3EEB-3EF2 */
					hero_window_point_draw(idx, j);				/* 3EF5-3EFE */
					target->action_timer = 0;
					target->action_phase = (uint8)(rnd(10) + 0xf);	/* 3F08-3F17 */
					return;
				}
			} else if ((tgt->orders & 1) == 0) {					/* 3F1F-3F26 */
				target->motion_state = ACTOR_MOTION_WANDER;
				return;
			}
		}
	}

	if (tgt->orders & 8) {									/* 3F32-3F39 */
		qi = choose_target(rec, idx, 4, 0x10);					/* 3F3E-3F52 */
		if (qi != -1) {								/* 3F55-3F58 */
			target = ACTOR_AT(qi);					/* 3F5B-3F64 */
			if ((int16)target->countdown != 0)					/* 3F67-3F6F */
				return;
			dx = ((int16)rec->x - (int16)target->x) >> 1;	/* 3F72-3F80 */
			dy = ((int16)rec->y - (int16)target->y) >> 1;	/* 3F83-3F91 */
			d2 = dx * dx + dy * dy;						/* 3F94-3FA2 */
			g_cursor_tile_x = (int16)target->x >> 4;				/* 3FA5-3FAC */
			g_cursor_tile_y = (int16)target->y >> 4;				/* 3FAF-3FB4 */
			if (d2 > SPELL_RANGE_SQ) {							/* 3FB7-3FBC */
				j = spell_spend_charge(idx);					/* 3FBE-3FC7 */
				if (j != 0)
					hero_window_point_draw(idx, j);				/* 3FCE-3FD5 */
				target->action_timer = 0;
				target->action_phase = (uint8)(ROLL_D20() + 30);	/* 3FDF-3FEE */
				return;
			}
			if ((tgt->orders & 1) == 0) {						/* 3FF5-3FFC */
				target->motion_state = ACTOR_MOTION_WANDER;
				return;
			}
		}
	}

	if (tgt->orders & 1)									/* 4007-400E */
		target->motion_state = ACTOR_MOTION_SEEK;
}

/* 0xC2B2 */

int16 record_disturb_quads(struct effect_rec *rec)
{
	struct effect_rec *o = rec;
	int16 kind = (int16)(int8)EFFECT_KIND(o);		/* C2B8-C2BB */
	int16 x0, x1, y0, y1, i, v;

	if (kind < 4)								/* C2BB-C2C2 */
		return 0;

	x0 = (int16)o->x - 8;						/* C2C5-C2CB */
	x1 = (int16)o->x + 8;						/* C2CE-C2D4 */
	y0 = (int16)o->y - 8;						/* C2D7-C2DD */
	y1 = (int16)o->y + 8;						/* C2E0-C2E6 */
	v = (int16)o->z;

	if (kind < 0x14) {							/* C2EF-C2F4 */
		for (i = ACTOR_HERO_COUNT; i != ACTOR_COUNT; ++i) {
			struct actor_rec *row = ACTOR_AT(i);
			if (row->state == 0)						/* C313-C319 */
				continue;
			if (row->mode == ACTOR_MODE_RETIRED)						/* C31E-C322 */
				continue;
			if ((int16)row->x <= x0 || (int16)row->x >= x1)	/* C327-C338 */
				continue;
			if ((int16)row->y <= y0 || (int16)row->y >= y1)	/* C33D-C34E */
				continue;

			v = SPAWN_ATTACK_VALUE(kind);		/* C353-C361 */

			if (kind == SPELL_EFFECT_WEB) {					/* C364-C367 */
				row->countdown =
				    (int16)(int8)g_vote_data.reward.web_hold[(int16)(int8)row->state];
				if ((int16)row->countdown != 0)
					ACTOR_COUNTDOWN_HI(row) =
					    (uint8)(ACTOR_COUNTDOWN_HI(row) | COUNTDOWN_HIGH_BYTE);
				row->mode = ACTOR_MODE_IDLE; row->status = 0;				/* C390-C39A */
				effect_step_kind(rec);					/* C39E-C3A4 */
				return 0;
			}
			if (kind == SPELL_EFFECT_CHARM) {					/* C3AA-C3B0 */
				row->countdown =
				    (int16)(int8)g_vote_data.reward.charm_hold[(int16)(int8)row->state]
				    * (int16)g_hold_length;				/* C3B2-C3C6 */
				row->mode = ACTOR_MODE_IDLE; row->status = 0;				/* C3C9-C3D3 */
				effect_step_kind(rec);					/* C3D7-C3DD */
				return 0;
			}
			if (kind == SPELL_EFFECT_SLEEP) {					/* C3E3-C3E9 */
				row->countdown =
				    (int16)(int8)g_vote_data.reward.sleep_hold[(int16)(int8)row->state]
				    * (int16)g_hold_length;				/* C3EB-C3FF */
				row->mode = ACTOR_MODE_IDLE; row->status = 0;				/* C402-C40C */
				if (row->countdown == 0 || ROLL_D100() >= 75)	/* C413-C426 */
					effect_step_kind(rec);				/* C428-C42E */
				return 0;
			}
			effect_step_kind(rec);							/* C434-C43A */
			if (kind == SPELL_EFFECT_FIREBALL)						/* C440-C445 */
				return 0;

			row->health = (uint8)((uint8)row->health
			    - (uint8)((uint8)rnd(v >> 8)
			    + (uint8)v));				/* C448-C45C */
			if ((int16)(int8)row->health >= 0)					/* C462-C466 */
				return 0;
			row->mode = ACTOR_MODE_RETIRED;

			if (row->state == ACTOR_KIND_BOZAK) {						/* C46F-C472 */
				EFFECT_KIND(o) = 2;					/* C474-C47D */
				o->countdown = 6;
			} else {
				EFFECT_KIND(o) = 3;					/* C483-C48C */
				o->countdown = 7;
			}
			row->status = 0;							/* C490-C493 */

			if (row->state == ACTOR_KIND_DWARVES) {						/* C497-C49D */
				load_current_map();
			} else if (row->state == ACTOR_KIND_RDRAGON) {					/* C4A4-C4AA */
				PARTY_REC(PARTY_FOLLOWER_LEADER)->kind = 0;
			}

			g_game_state.kill_count[(int16)(int8)row->state] += 1;	/* C4B1-C4BB */
			g_game_state.combat_experience +=
			    VOTE_EXPERIENCE_VALUE((int16)(int8)row->state);	/* C4BF-C4CD */
			return 0;
		}
	} else {
		for (i = 0; i != ACTOR_HERO_COUNT; ++i) {
			struct actor_rec *row = ACTOR_AT(i);	/* C4DC-C4EF */
			if (row->state == 0)						/* C4F8-C4FE */
				continue;
			if (row->mode == ACTOR_MODE_RETIRED)						/* C503-C507 */
				continue;
			if ((int16)row->x <= x0 || (int16)row->x >= x1)	/* C509-C517 */
				continue;
			if ((int16)row->y <= y0 || (int16)row->y >= y1)	/* C519-C527 */
				continue;

			v = SPAWN_ATTACK_VALUE(kind);		/* C529-C537 */
			v = (int16)(uint8)v + rnd(v >> 8);			/* C53A-C54C */
			if (kind == EFFECT_KIND_LARGE_EFFECT && row->timer != 0)		/* C54F-C55E */
				v = (30 * v) / 100;					/* C560-C56B */
			hero_member_damage(i, v);							/* C56E-C577 */
			effect_step_kind(rec);							/* C57A-C580 */
		}
	}
	return 0;
}

/* 0x0717 */

struct class_name_pool {
	char ability_bonus[89];	/* 0x0717-0x0770 */
	char mage_spells_a[66];	/* 0x0770-0x07B2 */
	char mage_spells_b[140];	/* 0x07B2-0x083E */
	char cleric_spells_a[105];	/* 0x083E-0x08A7 */
	char cleric_spells_b[119];	/* 0x08A7-0x091E */
	char stat_labels[60];	/* 0x091E-0x095A */
	char monster_names_a[103];	/* 0x095A-0x09C1 */
	char monster_names_b[89];	/* 0x09C1-0x0A1A */
};

struct class_name_pool name_pool_717 = {
	/* 0x0717-0x0770 */
	"\0\0\0\0+2\0\0+2\0+3\0\0+1\0+1\0+2\0+2\0\0+2\0+1\0\0+3\0+3W\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0+1\0+2\0AC2\0\0\0\0\0\0",
	/* 0x0770-0x07B2 */
	"   (1) Sleep\0   (1) Magic missile\0   (1) Detect magic\0   (2) Web\0\0",
	/* 0x07B2-0x083E */
	"   (1) Sleep\0   (1) Magic missile\0   (1) Detect magic\0   (1) Charm\0   (1) Burning hands\0   (2) Web\0   (2) Detect invisible\0   (3) Fire Ball\0",
	/* 0x083E-0x08A7 */
	"\0   (1) Cure light wounds\0   (1) Protection from evil\0   (1) Bless\0   (1) Detect magic\0   (2) Find traps\0",
	/* 0x08A7-0x091E */
	"   (2) Hold person\0   (2) Resist Fire\0   (3) Prayer\0   (3) Dispel Magic\0   (4) Cure Serious wounds\0       Turn Undead\0\0",
	/* 0x091E-0x095A */
	"    Min    Max\0    Min    Max\0Str\0Int\0Wis\0Con\0Dex\0Cha\0HP\0AC\0",
	/* 0x095A-0x09C1 */
	"Baaz\0Bozak\0Kapak\0Aurak\0Sivak\0Spider\0Hobgoblin\0Skeleton\0Troll\0Haunt\0Zombie\0Wight\0Ghoul\0Ghost\0Giant Wasp\0",
	/* 0x09C1-0x0A1A */
	"Ogre\0Aghar\0Dknight\0Golem\0Hydra\0Lizard\0Slig\0Spectral\0Dwarves\0Lich\0RDRAGON\0BDRAGON\0GDRAGON\0",
};

/* 0xD84 */

uint16 g_name_offset[123] = {
	1815,1816,1817,1818,1819,1822,1823,1826,1829,1830,1833,1836,
	1839,1842,1843,1846,1849,1850,1853,1857,1858,1859,1860,1861,
	1862,1863,1864,1865,1866,1867,1868,1869,1870,1871,1872,1873,
	1874,1875,1876,1877,1878,1879,1880,1881,1882,1883,1884,1885,
	1886,1887,1888,1889,1892,1895,1899,1900,1901,1902,1903,1904,
	1917,1938,1958,1969,1970,1983,2004,2024,2037,2058,2069,2093,
	2110,2111,2136,2164,2177,2197,2215,2234,2253,2267,2287,2314,
	2333,2334,2349,2364,2368,2372,2376,2380,2384,2388,2391,2394,
	2399,2405,2411,2417,2423,2430,2440,2449,2455,2461,2468,2474,
	2480,2486,2497,2502,2508,2516,2522,2528,2535,2540,2549,2557,
	2562,2570,2578
};

/* 0xBEA2 */

extern struct effect_rec g_effect_table[EFFECT_COUNT];            /* 0x9696 */
extern int16 rnd(int16 n);                            /* 0x0B88 */

/* 0xBEA2 */
int16 effect_tick_all(void)
{
	struct effect_rec *rec;
	int16 i, kind, bx, ax;
	int16 dx, dy;

	for (i = 0; i != 30; i++) {
		rec = &g_effect_table[i];
		kind = (int16)(int8)rec->id;
		if (kind == 0)
			continue;

		if (g_spell_purge_pending != 0 && g_tick_by_kind[kind + 52] == 0x14) {
			if (ROLL_D100() < 50) {
				rec->countdown = 0;
				rec->id = 0;
			}
		}

		if (rec->countdown-- != 0) {
			dx = rec->dx;
			dy = rec->dy;
			if ((dx | dy | rec->dz) != 0 && kind > 3) {
				bx = (int16)(int16)(uint16)rec->y >> 4;
				ax = (int16)(int16)(uint16)rec->x >> 4;
				bx = bx * 5 + ax;
				if (g_map_working.grid[bx].cell.value != 4 &&
				    tile_property(rec->x, rec->y) == 1)
					effect_step_kind(rec);
			}
		} else {
			if (rec->id == EFFECT_KIND_RECOVER_WEAPON_17
			 || rec->id == EFFECT_KIND_RECOVER_WEAPON_9
			 || rec->id == EFFECT_KIND_LARGE_EFFECT
			 || rec->id == SPELL_EFFECT_FIREBALL)
				effect_step_kind(rec);
			rec->id = 0;
			continue;
		}

		(void)free_list_insert(1, 0xa0 - (int16)rec->x,
		               (int16)rec->y, 5, (int16)rec, 0, 0);
		(void)record_disturb_quads(rec);

		if (kind != EFFECT_KIND_RECOVER_WEAPON_9
		 && kind != EFFECT_KIND_DEACTIVATE_6
		 && kind != EFFECT_KIND_DEACTIVATE_7
		 && kind != EFFECT_KIND_HIT_DEFAULT)
			EFFECT_ID_HI(rec)++;

		dx = rec->dx;
		dy = rec->dy;
		rec->x = (int16)(rec->x + dx);
		rec->y = (int16)(rec->y + dy);
		rec->z = (int16)(rec->z +
		                              (int16)rec->dz);

		if ((int16)rec->x < 0 || (int16)rec->x > 0xa0 ||
		    (int16)rec->y < 0 || (int16)rec->y > 0xa0) {
			rec->x = (int16)(rec->x - dx);
			rec->y = (int16)(rec->y - dy);
			if (rec->id == EFFECT_KIND_RECOVER_WEAPON_17
			 || rec->id == EFFECT_KIND_RECOVER_WEAPON_9)
				effect_step_kind(rec);
			rec->countdown = 0;
		}
	}

	g_spell_purge_pending = 0;
	return 0;
}

/* 0xA9CC */

extern struct game_state g_game_state;                 /* 0x2152 */
extern struct site_rec g_site_table[];                /* 0x2468 */
extern uint8 g_action_freeze;                  /* 0x9660 */
extern uint16 g_cursor_tile_x, g_cursor_tile_y;
extern int16 rnd(int16 n);                            /* 0x0B88 */
extern int16 region_loaded(int16 a);                       /* 0x0010 */
extern void sound_request_max(int16 v);                       /* 0x07D6 */
extern int16 hero_window_point_draw(int16 slot, int16 kind);             /* 0x1136 */
extern void record_move_dispatch(struct actor_rec *rec);         /* 0x33F6 */
extern void record_head_for(struct actor_rec *rec, int16 gx, int16 gy);   /* 0x3B42 */
extern void record_stray_check(struct actor_rec *rec);         /* 0x3BEF */
extern int16 choose_target(struct actor_rec *rec, int16 tag, int16 start, int16 end);  /* 0x3CED */
extern int16 abs_w(int16 v);                       /* 0x9BC4 */
extern int16 actor_attack_character(struct actor_rec *actor);            /* 0xAC5F */

/* 0xA9CC */
int16 actor_proximity_pass(struct actor_rec *rec, int16 idx)
{
	struct actor_rec *hs;
	uint16 w;
	int16 dx, dy, d2, v, kind;

	if (rec->mode == ACTOR_MODE_RETIRED)
		return 0;
	if (rec->impulse != 0) {
		rec->impulse--;
		record_move_dispatch(rec);
		return 0;
	}
	choose_target(rec, idx, 0, 4);
	if (rec->anchor_index == PARTY_NO_WAYPOINT) {
		rec->mode = ACTOR_MODE_IDLE;
		rec->status = ACTOR_STATUS_DISABLED;
		return 0;
	}

	hs = ACTOR_AT((int16)(int8)rec->anchor_index);
	dx = abs_w((int16)rec->x
	            - (int16)hs->x) >> 1;
	dy = abs_w((int16)rec->y
	            - (int16)hs->y) >> 1;
	d2 = (int16)(int16)((uint16)(dx * dx)
	                + (uint16)(dy * dy));

	if (rec->status == ACTOR_STATUS_ACTIVE) {
		if (rec->mode == ACTOR_MODE_WAIT || rec->mode == ACTOR_MODE_TRACK) {
			actor_attack_character(rec);
		} else if (rec->mode == ACTOR_MODE_ATTACK) {
			v = (rec->state == ACTOR_KIND_BOZAK) ? EFFECT_KIND_HIT_BOZAK : EFFECT_KIND_HIT_DEFAULT;
			if ((int16)(int8)rec->state >= ACTOR_KIND_RDRAGON)
				v = EFFECT_KIND_LARGE_EFFECT;
			if (rec->state == ACTOR_KIND_DWARVES)
				v = EFFECT_KIND_LARGE_EFFECT;
			g_cursor_tile_x = (uint16)((int16)hs->x >> 4);
			g_cursor_tile_y = (uint16)((int16)hs->y >> 4);
			hero_window_point_draw(idx, v);
		}
	}

	w = rec->countdown;
	if (w != 0) {
		v = (int16)(int16)(w & COUNTDOWN_MASK) - 1;
		if (v != 0) {
			rec->countdown &= COUNTDOWN_HIGH_BIT;
			rec->countdown |= (uint16)v;
		} else {
			rec->countdown = 0;
		}
		goto tail;
	}

	if ((int16)(int8)rec->mode > 1)
		goto tail;
	if (rec->impulse != 0)
		goto tail;
	if (d2 <= PROXIMITY_CLOSE) {
		if (rec->mode != ACTOR_MODE_WAIT) {
			if (rec->state == ACTOR_KIND_DEATH_KNIGHT
			    && g_game_state.map_event_done == 0)
				rec->mode = ACTOR_MODE_IDLE;
			else
				rec->mode = ACTOR_MODE_WAIT;
			rec->status = 0;
		}
		goto tail;
	}
	if (d2 < PROXIMITY_STRAY) {
		record_stray_check(rec);
		goto tail;
	}
	if (d2 >= PROXIMITY_FAR) {
		if (rec->mode != ACTOR_MODE_IDLE) {
			rec->mode = ACTOR_MODE_IDLE;
			rec->status = 0;
		}
		goto tail;
	}

	if (rec->anchor_index != PARTY_NO_WAYPOINT)
		record_head_for(rec, (int16)hs->x, (int16)hs->y);

	kind = rec->state;
	if (kind == ACTOR_KIND_BOZAK || (int16)(int8)kind >= ACTOR_KIND_RDRAGON || kind == ACTOR_KIND_DWARVES) {
		if (rec->timer != 0) {
			rec->mode = ACTOR_MODE_ATTACK;
			rec->status = 0;
			rec->timer--;
			rec->action_timer = 0;
			rec->countdown = (uint16)
			    (rnd(10) + (kind == ACTOR_KIND_BOZAK ? COUNTDOWN_BOZAK : COUNTDOWN_DEFAULT));
		}
	}

tail:
	if (d2 < PROXIMITY_REACT && (int16)(int8)rec->mode == 1 && rec->status == ACTOR_STATUS_ACTIVE
	    && rec->state != ACTOR_KIND_GHOST)
		sound_request_max(SOUND_REQUEST_ATTACK);
	if (rec->state == ACTOR_KIND_GDRAGON && (SITE_FLAG_LO(SITE_AT(SITE_SLOT_GREEN_DRAGON)) & 2) != 0) {
		if (region_loaded((int16)(int16)SITE_AT(SITE_SLOT_GREEN_DRAGON)->map) != 0)
			rec->mode = ACTOR_MODE_CHASE;
	}
	if (g_action_freeze != 0)
		rec->countdown = COUNTDOWN_FREEZE;
	return 0;
}

/* 0x4115 */

extern uint16 g_overworld_hold, g_game_end_flag, g_pending_actor_event, g_overworld_ready;
extern uint16 g_screen_hold, g_detect_magic_ticks, g_action_countdown, g_action_anchor_x;
extern uint16 g_action_anchor_y, g_hold_length;
extern struct party_rec g_party_table[];		/* 0x2b48 */
extern struct rect_rec g_hero_slot_boxes[];		/* 0xbda */
extern struct game_state g_game_state;		/* 0x2152 */
extern int16 isqrt_word(uint16 lo, uint16 hi);
extern void actor_take_action(int16 idx, struct actor_rec *rec);
extern int16 choose_target(struct actor_rec *rec, int16 tag, int16 start, int16 end);
extern int16 hero_member_damage(int16 idx, int16 amount);
extern void record_tick_state(struct actor_rec *rec);
extern void character_react_move(struct actor_rec *rec);
extern int16 effect_publish(int16 kind, int16 a2, int16 a3, int16 a4, int16 a5, int16 a6,
		    int16 a7, int16 a8, int16 a9, int16 a10);
extern int16 free_list_insert(int16 mode, int16 key_lo, int16 key_hi, int16 type, int16 x, int16 data, int16 y);
extern int16 actor_proximity_pass(struct actor_rec *rec, int16 idx);
extern void draw_map_items(void);
extern int16 abs_w(int16 v);
extern int16 rnd(int16 n);
extern int16 shadow_rect_fill(uint16 a, uint16 b, uint16 c, uint16 d);

/* 0x4115 */
int16 actor_sweep(void)
{
	struct actor_rec *hs;
	struct character_rec *p;
	int16 i, j, x, y, dx, dy, vx, vy, d2, v, n, chunk, row;
	int16 kind, cnt, mask, carry;
	int16 xbase, rows;
	uint16 lo, hi;
	uint16 mag, quo, sum;

	g_overworld_hold = 0;
	g_pending_actor_event = 0;
	g_overworld_ready = 0;
	if (g_detect_magic_ticks != 0)
		g_detect_magic_ticks--;
	if (g_screen_hold != 0)
		g_screen_hold--;

	xbase = 0x40;
	rows = 0;
	draw_map_items();

	for (i = 0; i != 16; i++) {
		hs = ACTOR_AT(i);
		if (i == 8)
			xbase = 0xde;

		if (hs->state == 0)
			goto next;
		x = (int16)hs->x;
		y = (int16)hs->y;
		if (!(x >= 0 && y >= 0 && x < 0xa0 && y < 0xa0)) {
			hs->state = 0;
			goto next;
		}

		if (g_action_countdown != 0) {
			dx = abs_w(x - (int16)g_action_anchor_x);
			dy = abs_w(y - (int16)g_action_anchor_y);
			if (dx <= 0x28 && dy <= 0x28) {
				d2 = (int16)(int16)((uint16)(dx * dx)
						  + (uint16)(dy * dy));
				v = isqrt_word((uint16)d2, 0u);
				if (v <= 0x26) {
					if ((int16)(int8)hs->state > 4) {
						hs->health -= (uint8)g_action_countdown;
						if ((int16)(int8)hs->health < 0) {
							if (hs->state == ACTOR_KIND_RDRAGON)
								PARTY_REC(PARTY_FOLLOWER_LEADER)->kind = 0;
							hs->mode = ACTOR_MODE_RETIRED;
						}
						(void)effect_publish(EFFECT_KIND_CREATURE, 0, 4,
							(int16)hs->x,
							(int16)hs->y,
							0x10, 0, 0, 0, 0);
					} else if (hs->timer != 0) {
						(void)hero_member_damage(i, muldiv100(
							(int16)g_action_countdown, 30));
					} else {
						(void)hero_member_damage(i, (int16)g_action_countdown);
					}
				}
			}
		}

		g_node_selector = hs->direction;
		g_sprite_frame_index = (uint8)hs->status;

		if ((int16)(int8)hs->state >= 1
		    && (int16)(int8)hs->state <= 4)
			goto tick;
		goto toward;

tick:
		rows++;
		g_pending_actor_event = 1;
		if (hs->health != 0)
			hs->health--;
		if ((SLOT_RECT_WORD(i) & RECT_STATE_CONTROL) != 0)
			actor_take_action(i, hs);
		if (choose_target(hs, i, 4, 0x10) + 1 == 0) {
			if (hs->motion_state == ACTOR_MOTION_SEEK)
				hs->motion_state = ACTOR_MOTION_STOP;
			SLOT_RECT_FLAG(i) &= 0x7f;
		}
		record_tick_state(hs);
		if (hs->status == 2 && (hs->mode == 2 || hs->mode == 3))
			character_react_move(hs);
		hs->anchor_index = 0xff;
		goto retire;

toward:
		g_overworld_ready = 1;
		if (g_game_state.search_active != 0) {
			dx = abs_w((int16)hs->x
				- (int16)g_game_state.search_x) >> 3;
			dy = abs_w((int16)hs->y
				- (int16)g_game_state.search_y) >> 3;
			d2 = (int16)(int16)((uint16)(dx * dx)
					  + (uint16)(dy * dy));
			v = isqrt_word((uint16)d2, 0u);
			if (v < 7) {
				kind = (int16)(int8)hs->state;
				if (kind == ACTOR_KIND_SKELETON
				    || (kind >= ACTOR_KIND_HAUNT && kind <= ACTOR_KIND_GHOUL)
				    || kind == ACTOR_KIND_GOLEM) {
					hs->countdown =
					    (uint16)(100 * (int16)g_hold_length);
					hs->mode = ACTOR_MODE_IDLE;
					hs->status = 0;
					if (hs->state == ACTOR_KIND_WIGHT && ROLL_D100() > 20)
						hs->countdown = 0;
					(void)effect_publish(EFFECT_KIND_CREATURE, 0, 4,
						(int16)hs->x,
						(int16)hs->y,
						0x10, 0, 0, 0, 0);
				}
			}
		}

		if (g_pending_actor_event != 0)
			g_overworld_hold = 1;
		n = (int16)(int8)hs->health;
		if (n < 0)
			n = 0;
		row = 0;
		if (xbase < HEALTH_BAR_X_LIMIT) {
			while (n != 0) {
				chunk = (n > 12) ? 12 : n;
				if (chunk != 0) {
					(void)shadow_rect_fill((uint16)(xbase + row),
						(uint16)(0xc6 - chunk),
						13u, (uint16)chunk);
					row++;
				}
				n -= chunk;
			}
			xbase += 10;
		}
		(void)actor_proximity_pass(hs, i);

retire:
		if (hs->mode == 6) {
			if (hs->state == ACTOR_KIND_RDRAGON)
				PARTY_REC(PARTY_FOLLOWER_LEADER)->kind = 0;
			hs->state = 0;
			goto next;
		}
		if (((uint16)hs->vel_x
		     | (uint16)hs->vel_y) != 0) {
			vx = (int16)hs->vel_x;
			vy = (int16)hs->vel_y;
			mask = 0;
			if (vx > 0) mask |= 8;
			if (vx < 0) mask |= 4;
			if (vy > 0) mask |= 2;
			if (vy < 0) mask |= 1;
			hs->direction = g_dir_code[mask];
		}
		if (hs->state == ACTOR_KIND_AURAK && hs->mode == 1 && g_detect_invisible_ticks == 0)
			goto next;
		(void)free_list_insert(1, 0xa0 - (int16)hs->x,
			(int16)hs->y, 3, (int16)hs, 0, 0);
next:
		if (g_game_end_flag != 0)
			break;
	}

	g_game_state.search_active = 0;
	if (rows != 0 && g_game_state.combat_experience != 0) {
		v = (int16)g_game_state.combat_experience;
		mag = (v < 0) ? (uint16)(0u - (uint16)v) : (uint16)v;
		quo = (uint16)(mag / (uint16)rows);
		if (v < 0)
			quo = (uint16)(0u - quo);
		g_game_state.combat_experience = (int16)quo;
		for (j = 0; j != 4; j++) {
			kind = (int16)(int8)ACTOR_AT(j)->state;
			if (kind == 0)
				continue;
			cnt = (int16)SLOT_KIND_COUNT(kind);
			if (cnt < 0 || cnt >= 0x16)
				continue;
			p = character_at(g_record_addr[cnt]);
			v = (int16)g_game_state.combat_experience;
			lo = (uint16)p->experience;
			hi = (uint16)(p->experience >> 16);
			sum = (uint16)(lo + (uint16)v);
			carry = (sum < lo) ? 1 : 0;
			hi = (uint16)(hi + (uint16)(v >> 15) + (uint16)carry);
			p->experience = ((uint32)hi << 16) | sum;	/* 45DD-45E0 */
		}
		g_game_state.combat_experience = 0;
	}
	g_action_countdown = 0;
	return 0;
}

/* 0x1974 */

void hero_member_spend_or_retire(struct character_rec *rec, int16 slot)
{
	int16 amount = rnd(10) + 1;
	uint8 *dead;

	if ((int16)rec->hp > amount) {
		rec->hp = (uint16)((int16)rec->hp - amount);
		draw_hero_gauge(slot);
	} else {
		dead = (uint8 *)(g_hero_slots + slot);
		(void)dead;
		hero_member_retire(slot);
	}
}
