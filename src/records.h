/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#ifndef RECORDS_H
#define RECORDS_H

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
typedef char party_rec_size_check[(sizeof(struct party_rec) == 22) ? 1 : -1];

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

struct char_stats {
	uint8 attributes[6];
	int8  armor_class;
};
typedef char char_stats_size_check[(sizeof(struct char_stats) == 7) ? 1 : -1];

struct weapon_slot {
	uint8 icon;
	uint8 flags;
};
typedef char weapon_slot_size_check[(sizeof(struct weapon_slot) == 2) ? 1 : -1];

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

struct rect_rec {
	uint16 state;
	uint16 x;
	uint16 y;
	uint16 w;
	uint16 h;
};
typedef char rect_rec_size_check[(sizeof(struct rect_rec) == 10) ? 1 : -1];

#endif
