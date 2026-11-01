/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#include "types.h"
#include "ids.h"

struct word_bytes {
	uint8 lo;
	uint8 hi;
};
typedef char word_bytes_size_check[(sizeof(struct word_bytes) == 2) ? 1 : -1];

struct rect_state_bytes {
	uint8 state_lo;
	uint8 state_hi;
};
typedef char rect_state_bytes_size_check[(sizeof(struct rect_state_bytes) == 2) ? 1 : -1];

#define RECT_STATE_BYTES(p)	((struct rect_state_bytes *)&(p)->state)
#define RECT_STATE_LO(p)	(RECT_STATE_BYTES(p)->state_lo)
#define RECT_STATE_HI(p)	(RECT_STATE_BYTES(p)->state_hi)

struct effect_id_bytes {
	uint8 kind;
	uint8 id_hi;
};
typedef char effect_id_bytes_size_check[(sizeof(struct effect_id_bytes) == 2) ? 1 : -1];

struct actor_rec {
	uint8 state;
	uint8 mode;
	uint8 direction;
	uint8 status;
	uint8 health;
	uint8 impulse;
	uint8 action_timer;
	uint8 action_phase;
	uint8 pad_08;
	uint8 timer;
	int16 vel_x;
	int16 x;
	int16 vel_y;
	int16 y;
	uint16 pad_12;
	uint16 mode_value;
	uint8  motion_state;
	uint8  anchor_index;
	uint16 pixel_x;
	uint16 pixel_y;
	uint16 countdown;
};

typedef char actor_rec_size_check[(sizeof(struct actor_rec) == 30) ? 1 : -1];

#define ACTOR(p) ((struct actor_rec *)(p))
#define ACTOR_AT(i) (&g_actor_table[(i)])
#define ACTOR_COUNTDOWN_BYTES(p) ((struct word_bytes *)&(p)->countdown)
#define ACTOR_COUNTDOWN_HI(p)	(ACTOR_COUNTDOWN_BYTES(p)->hi)

struct node_rec {
	uint16 key;
	uint16 type;
	uint16 x;
	uint16 y;
	uint16 data;
	uint16 sibling;
	uint16 child;
};

#define NODE(p) ((struct node_rec *)(p))

struct effect_rec {
	uint16 id;
	uint8  map_lo;
	uint8  countdown;
	int16 x;
	int16 y;
	int16 z;
	int16 dx;
	int16 dy;
	int16 dz;
};
typedef char effect_rec_size_check[(sizeof(struct effect_rec) == 16) ? 1 : -1];

#define EFFECT(p) ((struct effect_rec *)(p))
#define EFFECT_ID_BYTES(p) ((struct effect_id_bytes *)&(p)->id)
#define EFFECT_KIND(p)	(EFFECT_ID_BYTES(p)->kind)
#define EFFECT_ID_HI(p)	(EFFECT_ID_BYTES(p)->id_hi)

/* 0x00C0 */

struct video_mode_config {
	int16  disk_class;
	int16  unknown_02;
	uint16 text_page;
	uint16 page_rect;
	int16  window_index;
	uint16 panel_text_attr;
	uint16 gauge_fill_short;
	uint16 gauge_fill_long;
};
typedef char video_mode_config_size_check[(sizeof(struct video_mode_config) == 16) ? 1 : -1];

struct hotspot_rec {
	uint16 x;
	uint16 y;
	uint8 pad_04[6];
};

#define HOTSPOT(p) ((struct hotspot_rec *)(p))

struct sprite_node_rec {
	uint8 id;
	uint8 stride_index;
	uint8 selector;
	uint8 count;
	int16 x;
	int16 y;
	int16 y_offset;
	uint16 stage_delay;
	uint16 stage_next;
	uint16 stage_flags;
	int16 iso_y;
	uint8 pad_12[0x0A];
	uint16 jitter;
};
typedef char sprite_node_rec_size_check[(sizeof(struct sprite_node_rec) == 0x1E) ? 1 : -1];

#define SPRITE_FRAME_WORD(p)	(*(uint16 *)(void *)&(p)->stride_index)

#define SPRITE_REC(p) ((struct sprite_node_rec *)(p))

struct char_stats {
	uint8 attributes[6];
	int8  armor_class;
};

struct weapon_slot {
	uint8 icon;
	uint8 flags;
};

struct character_rec {
	uint16 name_offset;
	uint8  unknown_02;
	uint8  orders;
	struct weapon_slot hand_weapon;
	struct weapon_slot ranged_weapon;
	uint16 hp;
	uint16 hp_max;
	uint32 experience;
	uint8  armor_type;
	uint8  level;
	uint8  unknown_12;
	uint8  unknown_13;
	struct char_stats stats;
	struct char_stats base_stats;
	uint8  action_code;
	int8   action_timer;
};

typedef char character_rec_size_check[(sizeof(struct character_rec) == 36) ? 1 : -1];
typedef char char_stats_size_check[(sizeof(struct char_stats) == 7) ? 1 : -1];
typedef char weapon_slot_size_check[(sizeof(struct weapon_slot) == 2) ? 1 : -1];

#define CHAR_REC(p) ((struct character_rec *)(p))

#define WEAPON_WORD(w)		(*(uint16 *)&(w))

#define ATTR_STR	0
#define ATTR_INT	1
#define ATTR_WIS	2
#define ATTR_CON	3
#define ATTR_DEX	4
#define ATTR_CHA	5

struct party_rec {
	uint8  kind;
	uint8  attr;
	uint16 grid_x;
	uint16 grid_y;
	uint16 target_x;
	uint16 target_y;
	uint16 cooldown;
	uint16 refugees;
	uint16 food;
	uint16 score;
	uint16 waypoint;
	uint16 target2;
};

struct site_rec {
	uint16 id;
	uint16 map;
	int16          x;
	int16          y;
	uint16 mode_value;
	uint16 weight;
	uint16 child;
	uint16 flags;
};

typedef char site_rec_size_check[(sizeof(struct site_rec) == 16) ? 1 : -1];

#define SITE(p) ((struct site_rec *)(p))

struct roll_entry {
	int8 dice;
	int8 bonus;
	int8 charge_dice;
	int8 charge_bonus;
	uint8 unknown_04;
	uint8 flags;
	int8 attack_row;
};

typedef char roll_entry_size_check[(sizeof(struct roll_entry) == 7) ? 1 : -1];

struct damage_entry {
	int8 defense[9];
	int8 bonus;
	int8 dice;
	int8 bonus_alt;
	int8 dice_alt;
};

typedef char damage_entry_size_check[(sizeof(struct damage_entry) == 13) ? 1 : -1];

struct roll_table {
	struct roll_entry rolls[28];		/* 0x1E50..0x1F13 */
	int8 base_to_hit[10];		/* 0x1F14 */
	int8 attack_threshold[24];	/* 0x1F1E */
	int8 armor_bonus_b[14];		/* 0x1F36 */
	int8 armor_bonus_d[14];		/* 0x1F44 */
	int8 armor_bonus_c[14];		/* 0x1F52 */
	int8 armor_bonus_e[18];		/* 0x1F60 */
	int8 defense_modifier[26];	/* 0x1F72 */
	int8 damage_modifier[26];	/* 0x1F8C */
	struct damage_entry damage[22];		/* 0x1FA6..0x20C3 */
};

typedef char roll_table_size_check[(sizeof(struct roll_table) == 628) ? 1 : -1];

struct site_bytes {
	uint8 kind;
	uint8 id_hi;
	uint8 map_lo;
	uint8 map_hi;
	uint8 pad_04[0x0A];
	uint8 flag_lo;
	uint8 flag_hi;
};
typedef char site_bytes_size_check[(sizeof(struct site_bytes) == 16) ? 1 : -1];

#define SITE_BYTES(p)	((struct site_bytes *)(p))
#define SITE_KIND(p)	(SITE_BYTES(p)->kind)
#define SITE_ID_HI(p)	(SITE_BYTES(p)->id_hi)
#define SITE_MAP_BYTE(p)	(SITE_BYTES(p)->map_lo)
#define SITE_MAP_HI(p)	(SITE_BYTES(p)->map_hi)
#define SITE_FLAG_LO(p)	(SITE_BYTES(p)->flag_lo)
#define SITE_FLAG_HI(p)	(SITE_BYTES(p)->flag_hi)
#define SITE_FLAG_HI_BIT0	0x0100

struct map_cell {
	union {
		uint16 value;
		struct {
			uint8 icon;
			uint8 kind;
		} part;
	} cell;
};
typedef char map_cell_size_check[(sizeof(struct map_cell) == 2) ? 1 : -1];

struct map_record {
	uint16 header_word;
	uint16 free_list;
	uint8  cell_kind;
	uint8  pad_05[0x1b];
	uint8  icons[20];
	uint8  exit_top_guard;
	uint8  exit_top_map;
	uint8  exit_right_guard;
	uint8  exit_right_map;
	uint8  exit_bottom_guard;
	uint8  exit_bottom_map;
	uint8  exit_left_guard;
	uint8  exit_left_map;
	uint8  pad_3c[0x0c];
	struct map_cell grid[100];
};

#define MAP_RECORD_AT(i) (&g_map_records[(i)])
#define MAP_GRID_VALUE(p, i)	((p)->grid[i].cell.value)
#define MAP_GRID_HI(p, i)	((p)->grid[i].cell.part.kind)
typedef char map_record_size_check[(sizeof(struct map_record) == 0x110) ? 1 : -1];

#define MAP_ICON_SLOT_HIDDEN_PASSAGE	5
#define MAP_ICON_SLOT_EXIT	16
#define MAP_ICON_HIDDEN_PASSAGE	0x40
#define MAP_ICON_EXIT	0x39

/* 0x1DFA */

struct spawn_kind_record {
	uint8 attack_index;
	uint8 flags;
	uint8 pad_02[5];
};
typedef char spawn_kind_record_size_check[(sizeof(struct spawn_kind_record) == 7) ? 1 : -1];

struct spawn_table_rec {
	uint16 position[16];
	int16  attack[11];
	uint8  pad_36;
	struct spawn_kind_record kind[9];
};
typedef char spawn_table_rec_size_check[(sizeof(struct spawn_table_rec) == 118) ? 1 : -1];

union spawn_table {
	uint8 raw[118];
	struct spawn_table_rec rec;
};
typedef char spawn_table_size_check[(sizeof(union spawn_table) == 118) ? 1 : -1];

/* 0x1CD4 */

struct vote_reward_view {
	uint8  pad_00[50];
	int16  experience[55];
	uint8  pad_a0[33];
	uint8  web_hold[32];
	uint8  sleep_hold[32];
	uint8  charm_hold[37];
};
typedef char vote_reward_view_size_check[(sizeof(struct vote_reward_view) == 294) ? 1 : -1];

union vote_data {
	uint8 raw[294];
	struct vote_reward_view reward;
};
typedef char vote_data_size_check[(sizeof(union vote_data) == 294) ? 1 : -1];

/* 0x9444 */

struct high_score_panel {
	char   title[86];
	uint32 score[5];
};
typedef char high_score_panel_size_check[(sizeof(struct high_score_panel) == 106) ? 1 : -1];

union panel_template {
	struct high_score_panel panel;
	uint8  raw[106];
	uint16 words[53];
};
typedef char panel_template_size_check[(sizeof(union panel_template) == 106) ? 1 : -1];

struct game_state {
	uint8 waypoint_image[0x90];	/* 0x0000 */
	uint16 kill_count[45];		/* 0x0090 */
	uint8 golmoon_spell_count[11];	/* 0x00EA */
	uint8 pad_00f5;			/* 0x00F5 */
	uint8 elistan_spell_count[11];	/* 0x00F6 */
	uint8 pad_0101;			/* 0x0101 */
	uint8 raistlin_spell_count[8];	/* 0x0102 */
	uint8 gilthanas_spell_count[4];	/* 0x010A */
	uint16 overworld_phase_image;	/* 0x010E */
	int16 river_valley_done;			/* 0x0110 */
	uint16 screen_hold_image;	/* 0x0112 */
	uint16 state_countdown_image;	/* 0x0114 */
	uint16 timer_debt_image[4];	/* 0x0116 */
	uint16 frame_debt_image;	/* 0x011E */
	uint16 move_speed_shift_image;	/* 0x0120 */
	int16 map_event_done;			/* 0x0122 */
	int16 fruit_forest_scene_done;		/* 0x0124 */
	uint16 map_N_event_done_image;	/* 0x0126 */
	uint8 map_mode_char_image;	/* 0x0128 */
	uint8 search_active;		/* 0x0129 */
	uint16 search_x;		/* 0x012A */
	uint16 search_y;		/* 0x012C */
	uint16 seek_suspended_image;	/* 0x012E */
	uint16 hour_is_20_image;	/* 0x0130 */
	uint16 day_counter_image;	/* 0x0132 */
	uint16 clock_hour_image;	/* 0x0134 */
	uint16 hour_ticks_image;	/* 0x0136 */
	uint16 elapsed_accum_image;	/* 0x0138 */
	uint16 elapsed_step_image;	/* 0x013A */
	int16 combat_experience;			/* 0x013C */
	uint16 spell_pick_pool;		/* 0x013E */
	int16 kharolis_done;			/* 0x0140 */
	int16 mountain_lake_done;			/* 0x0142 */
	int16 fruit_forest_done;			/* 0x0144 */
	uint8 trail_image[0x100];	/* 0x0146 */
	uint16 trail_head_image;	/* 0x0246 */
	uint8 hero_slot_gap_image[6];	/* 0x0248 */
	uint8 hero_slot_image[0x92];	/* 0x024E */
};

typedef char game_state_size_check[(sizeof(struct game_state) == 0x2E0) ? 1 : -1];

struct rect_rec {
	uint16 state;
	uint16 x;
	uint16 y;
	uint16 w;
	uint16 h;
};

#define RECT(p) ((struct rect_rec *)(p))

#define RECT_STRIDE	((int16)sizeof(struct rect_rec))
#define SITE_STRIDE	((int16)sizeof(struct site_rec))
#define PARTY_STRIDE	((int16)sizeof(struct party_rec))
#define ACTOR_STRIDE	((int16)sizeof(struct actor_rec))

#define RECT_STATE_GONE	0x4000
#define RECT_STATE_RETIRING	0x2000
#define RECT_STATE_RETIRING_MASK	(RECT_STATE_GONE | RECT_STATE_RETIRING)
#define RECT_STATE_CONTROL	0x8000

#define HERO_SLOT_OCCUPIED	RECT_STATE_RETIRING

#define HERO_SLOT_NONE	0x17
#define HERO_SLOT_STATE_LIMIT	0x16

#endif
