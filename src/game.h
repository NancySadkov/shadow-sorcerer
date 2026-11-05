/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"
#include "structs.h"
#include <stdio.h>

#ifndef GAME_H
#define GAME_H

union dword {
	uint32 l;
	uint16 w[2];
	uint8  b[4];
};

#define MAP_RECORD_SIZE	0x110
#define g_map_master_record		(&g_map_records[29])	/* 0x4F30 */
#define g_map_record_11	(&g_map_records[10])	/* 0x3B00 */

#define RECT_PROBE()	do {							\
		rx = (int16)(rec)->x + (int16)(rec)->vel_x;			\
		ry = (int16)(rec)->y + (int16)(rec)->vel_y;			\
		up_x = (int16)(oth)->x + 0xf;					\
		t = rx + 0xf;							\
		if (t < up_x) up_x = t;						\
		up_y = (int16)(oth)->y + 0xf;					\
		t = ry + 0xf;							\
		if (t < up_y) up_y = t;						\
		dn_x = (int16)(oth)->x;						\
		if (rx > dn_x) dn_x = rx;					\
		dn_y = (int16)(oth)->y;						\
		if (ry > dn_y) dn_y = ry;					\
	} while (0)

#define ABS_SHR3(a)  ((a) < 0 ? -((-(a)) >> 3) : ((a) >> 3))

#define SLOT_RECT_REC(i) \
	((i) < 4 ? &g_hero_slot_boxes[i] : \
	 (i) < 8 ? &g_panel_boxes[(i) - 4] : \
	 (struct rect_rec *)&g_hot_spots[((i) - 8) * 5])

#define SLOT_RECT_WORD(i)  (SLOT_RECT_REC(i)->state)

#define SLOT_RECT_FLAG(i)  RECT_STATE_HI(SLOT_RECT_REC(i))

#define SLOT_KIND_COUNT(k) \
	(((struct rect_rec *)((uint8 *)&g_game_state + 0x244))[(k)].state)

#define P_STR(off)	(g_name_offset[(off) - 1])

#define P_NAME(i)	(g_name_offset[85 + (i)])

/* 0x0E7A */

#define g_label_offset		(g_name_offsets)
#define g_party_name_offset	(g_name_offsets + 9)

extern uint16 g_text_msg_offset;
extern uint16 g_text_msg_offset_default;
extern struct video_mode_config g_video_mode_config;
#define g_disk_class		(g_video_mode_config.disk_class)	/* 0x00C0 */
#define g_disk_class_ui		(g_video_mode_config.disk_class)	/* 0x00C0 */
#define g_window_index		(g_video_mode_config.window_index)	/* 0x00C8 */
#define g_gauge_fill_short	(g_video_mode_config.gauge_fill_short)	/* 0x00CC */
#define g_gauge_fill_long	(g_video_mode_config.gauge_fill_long)	/* 0x00CE */
#define g_text_page		(g_video_mode_config.text_page)		/* 0x00C4 */
#define g_page_rect		(g_video_mode_config.page_rect)		/* 0x00C6 */
#define g_panel_text_attr	(g_video_mode_config.panel_text_attr)	/* 0x00CA */
#define g_vote_text_attr	(g_video_mode_config.panel_text_attr)	/* 0x00CA */
extern uint16 g_special_monster_bank_loaded;
extern int16 g_save_dialog_x;
extern int16 g_save_dialog_y;
extern uint16 g_overworld_phase;
extern uint16 g_screen_hold;
extern uint16 g_state_countdown;
extern uint16 g_timer_debt_a;
extern uint16 g_timer_debt_b;
extern uint16 g_timer_debt_c;
extern uint16 g_timer_debt_d;
extern uint16 g_frame_debt;
extern uint16 g_move_speed_shift;
extern uint16 g_map_N_event_done;
extern uint8 g_map_mode_char;
extern uint16 g_clock_hour;
extern uint16 g_day_counter;
extern uint16 g_hour_ticks;
extern uint16 g_elapsed_accum;
extern uint16 g_elapsed_step;
extern uint16 g_overworld_hold;
extern int16 g_save_slot_highlight;
extern uint8 g_action_freeze;
extern uint16 g_menu_toggle;
extern uint16 g_bless_duration;
extern uint16 g_stale_clear_done;
extern uint16 g_sweep_counter;
extern uint16 g_action_countdown;
extern uint16 g_terrain_change_done;
extern uint16 g_action_anchor_x;
extern uint16 g_search_weight;
extern uint8 g_matching_site_list[SITE_COUNT];
extern uint16 g_action_anchor_y;
extern uint16 g_hold_length;
extern uint16 g_spell_purge_pending;
extern uint16 g_sound_request;
extern uint16 g_game_end_flag;
extern uint8 g_node_selector;
extern uint16 g_pending_actor_event;
#ifdef SS_SDL
extern uint16 g_site_exit_pending;
#endif
extern uint8 g_panel_kind_hi;
extern uint8 g_panel_kind_lo;
extern uint16 g_sticky_click_flag;
extern uint16 g_detect_magic_ticks;
extern uint8 g_sprite_stride_index;
extern uint16 g_panel_stride_mode;
extern uint16 g_longest_string;
extern volatile uint16 g_shadow_counters[4];	/* 0xA8A4..0xA8AA */
#define g_shadow_event_count	g_shadow_counters[0]  /* 0xA8A4 */
#define g_shadow_tick_count	g_shadow_counters[3]  /* 0xA8AA */
extern uint16 g_selected_hero_slot;
extern uint16 g_mouse_last_x, g_mouse_last_y, g_cursor_mode;
extern uint8 g_sprite_last_frame;
extern uint16 g_string_cursor;
extern uint16 g_protection_duration;
extern uint16 g_search_center_x;
extern uint16 g_search_center_y;
extern uint16 g_reserved_a;
extern uint16 g_overworld_ready;
extern uint16 g_reserved_b;
extern uint16 g_sprite_draw_x;
extern uint16 g_sprite_draw_y;
extern uint16 g_golmoon_menu_choice;
extern uint16 g_panel_busy;
extern uint16 g_elistan_menu_choice;
extern uint16 g_find_traps_span;
extern uint16 g_find_traps_gate;
extern uint16 g_raistlin_menu_choice;
extern uint16 g_paralysis_duration;
extern uint16 g_window_stamp;
extern FILE *g_map_file;
extern uint16 g_row_height;
extern uint16 g_companion_react_skip;
extern uint16 g_pending_terrain;
extern uint8 g_sprite_step_x;
extern uint8 g_sprite_step_y;
extern uint16 g_pane_base_x;
extern uint16 g_pane_base_y;
extern uint8 g_sprite_frame_count;
extern uint8 g_sprite_frame_index;
extern uint16 g_refugee_seek_hold;
extern uint16 g_acc_hi;
extern uint16 g_acc_lo;
extern uint16 g_alloc_mark;
extern uint8 g_terrain_grid[TERRAIN_CELLS];
extern char g_bar_frame[];
extern uint8 g_btn_table[130];
extern uint8 g_btn_pairs[20];
extern uint8 g_btn_rows[50];
extern struct map_record g_map_records[MAP_RECORD_COUNT];
extern uint16 g_text_index[250];
extern uint8 g_map_cell_kind;
extern uint8 g_cell_grid[TERRAIN_CELLS];
struct credits_text {
	char block_a[228];	/* 0x8D77-0x8E5B */
	char block_b[228];	/* 0x8E5B-0x8F3F */
	char block_c[228];	/* 0x8F3F-0x9023 */
	char block_d[39];	/* 0x9023-0x904A */
};

extern struct credits_text credits_08d77;	/* 0x8D77 */
void credits_save(void);
extern const uint8 g_dir_code[16];
extern const uint16 g_name_offsets[24];
extern const uint16 g_move_delta[9];
extern const uint16 g_spawn_pairs[8];
extern const uint8 g_order_masks[4];
extern const uint8 g_spell_levels[SPELLCASTER_COUNT][12];
extern const uint8 g_spell_caps[SPELLCASTER_COUNT][6];
extern const uint8 g_reveal_cells_n_event[26];
extern const uint8 g_reveal_cells_key_ring[8];
extern volatile uint16 g_disp_clear;  /* 0xCE66 */
extern volatile uint16 g_disp_control;  /* 0xCE6E */
extern volatile uint16 g_disp_flag;  /* 0xCE7C */
extern volatile uint16 g_disp_index;  /* 0xCE80 */
extern volatile uint16 g_disp_last;  /* 0xCE7E */
#ifdef SS_SDL
extern volatile const uint8 *g_disp_ptr;  /* 0xCE78 */
extern volatile const uint8 *g_disp_ptr2;  /* 0xCE7A */
#else
extern volatile uint16 g_disp_ptr;  /* 0xCE78 */
extern volatile uint16 g_disp_ptr2;  /* 0xCE7A */
#endif
extern volatile uint16 g_disp_state;  /* 0xCE76 */
extern uint16 g_drv_handle;
extern struct party_rec g_party_table[];
extern struct game_state g_game_state;
extern uint8 g_free_head[434];
extern uint16 g_free_swept;
extern uint8 g_creature_grid[TERRAIN_CELLS];
extern uint16 g_hot_spots[45];
extern int16 g_hw_config;
extern const uint16 g_kind_ceiling[23];
extern char * g_label_buttons[10];
extern char g_line_buffer[50];
extern uint16 g_monster_bank_segments[];  /* 0xA28C */
extern struct map_record g_map_working;
extern const uint16 g_map_ring[80];
extern uint8 g_map_tile[];
extern uint16 g_mappic_at;  /* 0xA9A2 */
extern uint8 g_mask[48];
extern uint16 g_mem_next;		/* 0xa9c8 */
extern uint16 g_overlay_segment;		/* 0x9f2a */
extern uint8 g_menu_boxes[2000];
extern char * g_menubar_labels[24];
extern char * g_messages[33];
extern union vote_data g_vote_data;
extern uint16 g_missile_at;  /* 0xA172 */
extern uint16 g_mouse_button;
extern uint16 g_mouse_x;
extern uint16 g_mouse_y;
extern uint16 g_name_offset[123];
extern char g_name_pool[10][25];
extern struct class_name_pool name_pool_717;	/* 0x0717 */
extern struct party_name_pool name_pool_a1a;	/* 0x0A1A */
extern char g_name_vcin[10];
extern char g_name_vmpl[10];
extern char g_name_vmon[10];
/* 0xB02 */

#define g_monster_bank_suffix	(*(uint8 *)&g_name_vmon[4])
extern const int16 g_neighbour_table[8][3];
extern int8 g_noise_table[250];
extern struct effect_rec g_effect_table[EFFECT_COUNT];
extern uint16 g_objpic_at;  /* 0xA9A8 */
extern const int8 g_ofs_even_ring[38];
extern const int8 g_ofs_odd_ring[38];
extern uint16 g_pal_rgb[16];
extern const uint8 g_palette_registers[17];
extern uint8 g_path_grid[100];
extern uint8 g_path_qx[64];
extern uint8 g_path_qy[64];
extern uint16 g_path_read;
extern uint16 g_path_write;
extern uint8 g_pixel_order[256];
extern char g_play_mode;
extern const int16 g_probe_table[42];
extern struct actor_rec g_actor_table[ACTOR_COUNT];
extern uint16 g_record_addr[23];
extern struct rect_rec g_panel_boxes[4];
extern struct rect_rec g_rect_panel_boxes[4];
extern struct rect_rec g_hero_slot_boxes[4];
extern uint8 g_rect_menubar[50];
extern struct rect_rec g_hero_slots[20];
extern struct roll_table g_roll_table;
extern struct character_rec g_character_table[];
extern const uint8 g_presentation_scripts[479];
extern char * g_slot_name[10];
extern int16 g_slot_rows[12][5];
extern const uint8 g_song_bank[1494];
extern union spawn_table g_spawn_table;
extern struct site_rec g_site_table[];
struct trail_point { uint16 x, y; };
extern struct trail_point g_trail[64];
extern uint16 g_trail_head;
extern const int8 g_tick_by_kind[134];
extern volatile uint16 g_tick_enabled;  /* 0x98CA */
extern uint8 g_tile_flags[56];
extern union panel_template g_panel_template;
extern const char g_tmpl_horiz[41];
extern const char g_tmpl_vert[48];
extern uint16 g_detect_invisible_ticks;
extern FILE *g_save_stream;
extern uint16 g_video_mode;
extern uint16 g_video_mode_saved;
extern FILE *g_text_stream;
extern uint16 g_sprite_record_off;
extern uint16 g_map_grid_ptr;
extern uint16 g_cursor_tile_x;
extern uint16 g_cursor_tile_y;
extern uint16 g_vmpl_para[4];
extern uint8 g_vmpl_slot[4];
extern char * g_vote_options[7];
extern const uint8 g_vote_tbl[60];
extern uint8 g_walk_grid[TERRAIN_CELLS];
extern const uint8 g_walk_delta[20];
extern uint16 g_map_load_gate;

extern int16 abs_w(int16);
extern void terminate_callback(void);
uint16 div32_by_10(uint16 *plo, uint16 *phi);
void draw_label_row(int16 row, int16 x, int16 y, int16 attr);
void draw_row_scratch(int16 ix, int16 attr);
extern void driver_stop(void);
void driver_entry(int16 entry, int16 arg);
extern void driver_resume_and_call(int16 entry, int16 arg);
extern void service_registry_reset(int16 a, int16 b);
int16 region_loaded(int16 a);
void load_pane_maps(void);
void draw_glyph_corners(void);
void tile_event_clear(void);
void clear_stale_sites(void);
void calm_heroes_clear_items(void);
void collect_matching_sites(int16 *outp, int16 want, uint16 mask_req, uint16 mask_forbid, int16 depth);
void high_score_build(void);
void high_score_show(void);
void hero_member_retire(int16 idx);
void hero_member_spend_or_retire(struct character_rec *rec, int16 slot);
void copy_protection(void);
int16 monster_experience_list(void);
int16 scan_pair_window(uint8 *p1, uint8 *p2);
int16 spell_spend_charge(int16 idx);
int16 state_query(int16 slot);
int16 probe_record_kind(int16 id, int16 x, int16 y, int16 mode_value, int16 flags);
void text_measure(char *s, int16 *w, int16 *h);
int16 text_equal(uint8 *a, uint8 *b);
void text_copy(char *dest, const char *src);
int16 text_length(char *s);
void draw_status_bar(int16 fraction, int16 x, int16 y);
void draw_combat_menu(int16 count, int16 sel);
void screen_refresh(void);
void wait_key_drain(void);
int16 hero_window_point_draw(int16 slot, int16 kind);
int16 character_detail_panel(int16 a, int16 b, int16 c);
void record_flag_rebuild(struct site_rec *p);
void panel_kind_draw(void);
void player_pick_nearby(int16 slot);
void draw_hero_gauges(void);
void panel_layout(int16 *px, int16 *py, int16 flags, int16 mode, int16 cursor);
void hero_slot_click_arm(void);
void hero_slot_select(int16 idx);
void apply_character_action(int16 kind, int16 *act);
void spend_character_work(int16 delta, int16 mode);
void hero_slot_select_spread(int16 idx);
void clock_hold_span(void);
int16 setting_apply_choice(int16 sel);
void settle_hero_score(void);
int16 hero_cursor_rotate(int16 a);
void tile_event_apply(void);
int16 setting_apply_checked(int16 sel);
void award_claim(int16 kind, int16 val);
int16 save_load_dialog(void);
void experience_points_panel(void);
int16 tile_property(int16 x, int16 y);
void probe_north(int16 *a, int16 *b, struct actor_rec *c);
void probe_east(int16 *a, int16 *b, struct actor_rec *c);
void probe_south(int16 *a, int16 *b, struct actor_rec *c);
void probe_west(int16 *a, int16 *b, struct actor_rec *c);
void rect_separate(struct actor_rec *rec, struct actor_rec *oth);
void offer_record_to_actors(struct actor_rec *rec);
void actor_try_step(struct actor_rec *rec);
void draw_hero_overlay(void);
void record_move_dispatch(struct actor_rec *actor);
int16 record_near_refpoint(struct actor_rec *rec);
void record_head_for(struct actor_rec *rec, int16 goal_x, int16 goal_y);
void record_stray_check(struct actor_rec *rec);
int16 record_direction(struct actor_rec *rec);
int16 choose_target(struct actor_rec *rec, int16 tag, int16 start, int16 end);
void actor_take_action(int16 idx, struct actor_rec *rec);
int16 actor_is_neutral(struct actor_rec *a);
int16 actor_is_invisible(struct actor_rec *a);
void actor_provoke(int16 idx);
void actor_clear_provoke(int16 idx);
void show_actor_description(int16 idx);
void describe_world_hex(int16 col, int16 row, int16 travel);
void describe_time(void);
void record_tick_state(struct actor_rec *rec);
int16 actor_sweep(void);
int16 joystick_calibrate(void);
int16 draw_text_window(int16 x, int16 y, int16 w, int16 h);
void text_resource_read(int16 idx);
void show_text_panel(int16 a);
void wait_ticks(int16 a);
int16 message_box(int16 box_style, int16 id);
int16 mouse_poll_moved(int16 a);
extern int16 mouse_button_read(void);
int16 character_command_panel(int16 slot, int16 mode, int16 flag);
int16 menu_select(int16 panel, char **labeltab, int16 flag);
int16 setting_apply(int16 a);
int16 menubar_rearrange(int16 panel, int16 arg2);
int16 menubar_click(int16 hit, int16 mode);
void camera_publish_cells(int16 a);
void load_map_records(void);
void map_export_debug(void);
int16 read_3digit(void);
int16 read_2digit(void);
extern int16 clock_advance(int16);
extern void clock_draw_time(void);
void clock_advance_frames(int16 n);
void terrain_change(void);
void tile_set_position(void);
void load_pane_blocks(void);
void clock_bank_timers(void);
int16 tile_first_entry(void);
void clock_tick_overworld(void);
int16 move_view(void);
void check_refugee_total(void);
void reveal_cells(const uint8 *cells);
int16 show_screen_wait(int16 a);
void walk_steps(void);
void travelling_company_panel(void);
extern int16 path_find_distance(int16 gx, int16 gy, int16 tx, int16 ty, int16 *dist);
extern int16 party_update_all(int16 advance, int16 draw);
extern void party_spawn(int16 slot, int16 parts, int16 flags);
void creature_behavior_round(void);
void sound_request_max(int16 v);
void sound_request_or_sample(int16 v);
void sound_request_or_sample_id(int16 v, int sample);
void sound_request_or_effect(int16 kind, int16 v);
void sound_request_damage(void);
void sound_request_spellcast(void);
int16 roll_dice_amount(int16 i);
void party_upkeep_charge(void);
int16 wait_button_click(struct rect_rec *rec, int16 n);
int16 question_two_buttons(int16 res_id);
int16 question_five_choices(int16 res_id);
int16 question_chain(int16 flag);
int16 party_may_use_code(int16 code, int16 ent);
int16 random_cell(int16 col, int16 row);
int16 announce_vote(int16 opt, int16 ent);
int16 companion_react_tile(int16 ent, int16 ask, int16 kind);
void world_map_click_view(void);
void world_map_click(void);
#ifdef SS_SDL
int16 port_walk_step(int16 dir);
int16 port_site_step(int16 dir);
int16 port_pick_actor(int16 mx, int16 my);
int16 port_pick_monster(int16 mx, int16 my);
int16 port_aim_clicked_monster(void);
extern int16 g_aim_actor;
#endif
int16 find_party_player_tile(int16 idx);
void spawn_monsters(int16 which);
void mark_map_unseen(void);
void free_list_reset(void);
uint8 *free_list_alloc(void);
uint8 *free_list_find_prev(int16 key, int16 flag);
void free_list_insert_child(uint8 *q, uint8 *start);
#ifdef SS_SDL
int16 free_list_insert(int16 mode, int16 key_lo, int16 key_hi, int16 type, uintptr_t data, int16 x, int16 y);
#define FREE_DATA(p)	((uintptr_t)(p))
#else
int16 free_list_insert(int16 mode, int16 key_lo, int16 key_hi, int16 type, int16 data, int16 x, int16 y);
#define FREE_DATA(p)	((int16)(p))
#endif
int16 draw_terrain_sprites(void);
int16 free_list_clear_nodes(void);
int16 draw_free_node(struct node_rec *node);
void draw_frame_marks(void);
void map_record_show(int16 rec_no, int16 transition);
int16 draw_map_record(int16 rec_no);
void load_vcin_clip(int16 which);
void draw_presentation(int16 presentation);
void set_dac_palette(void);
int16 word_xor(int16 a, int16 b);
void publish_panel_template(int16 arg);
extern int16 save_pack(char *pattern);
int16 load_panel_template(char *name);
extern int16 save_restore(char *pattern);
extern int16 sprite_draw_current(int16 sprite, int16 mode, int16 column, int16 row);
#ifdef SS_SDL
extern int16 sprite_draw_block(int16, int16, int16, int16, const uint8 *);
#else
extern int16 sprite_draw_block(int16, int16, int16, int16, uint16);
#endif
extern int16 shadow_video_flags(int16 flags);
int16 sprite_draw_far(int16 id, int16 x, int16 y);
void sprite_draw_state_frame(uint16 seg);
#ifdef SS_SDL
void sprite_blit_pair(int16 pair, int16 kind, int16 x, int16 y,
                      const uint8 *src);
#else
void sprite_blit_pair(int16 pair, int16 kind, int16 x, int16 y, int16 data_off);
#endif
void sprite_blit_center(int16 x, int16 y, int16 id);
void sprite_blit_offset(int16 x, int16 y, int16 id);
#ifdef SS_SDL
void sprite_actor_rect(int16 hero_slot, int16 gx, int16 gy,
                       int16 *left, int16 *top, int16 *right, int16 *bottom);
void sprite_monster_rect(int16 kind, int16 gx, int16 gy,
                         int16 *left, int16 *top, int16 *right, int16 *bottom);
#endif
void sprite_draw_dispatch(int16 x, int16 y, int16 id);
void sprite_draw_forced(int16 x, int16 y, int16 id);
void sprite_draw_field(int16 x, int16 y, struct sprite_node_rec *rec, int16 sel);
void sprite_draw_index(int16 x, int16 y, int16 idx);
void sprite_draw_selector(int16 x, int16 y, int16 id);
void sprite_draw_segment(int16 x, int16 y, int16 id, uint16 seg);
void far_pair_load(int16 index, uint16 seg);
void sprite_draw_anchored(int16 px, int16 py, int16 kind);
int16 isqrt_word(uint16 lo, uint16 hi);
void ensure_text_open(void);
int16 save_load_panel(int16 mode);
int16 queue_expand(void);
int16 path_find(int16 start_x, int16 start_y, int16 goal_x, int16 goal_y);
void mem_zero(uint8 *p, int16 n);
int16 actor_proximity_pass(struct actor_rec *rec, int16 idx);
int16 actor_attack_character(struct actor_rec *actor);
void hero_window_suppress(int16 i);
int16 hero_member_damage(int16 idx, int16 amount);
void cursor_set(int16 idx);
void mark_cells_redraw(int16 xbase, int16 ybase, int16 count, uint8 *ofs);
void draw_map_view(void);
void hide_one_dollar(void);
void rebuild_map_tail(void);
int16 box_menu(uint16 *labels, uint8 *codes, int16 n, int16 quiet);
int16 spawn_descriptor(int16 request, int16 w, int16 h);
extern int16 event_raise(int16 v);
int16 hit_test_buttons(void);
void layout_nine_buttons(int16 origin_x, int16 origin_y);
void tactical_command_panel(int16 slot, int16 command);
void record_counter_advance(struct character_rec *rec, int16 hi, int16 lo);
void draw_map_items(void);
void effect_clear_flags(void);
void map_clear_cell_kinds(int16 rec_no);
int16 effect_publish(int16 kind, int16 map_lo, int16 countdown, int16 x, int16 y, int16 z, int16 dx, int16 dy, int16 dz, int16 id_hi);
int16 effect_tick_all(void);
void draw_hero_gauge(int16 i);
int16 effect_step_kind(struct effect_rec *rec);
int16 record_disturb_quads(struct effect_rec *rec);
void record_set_mode(struct site_rec *rec, int16 x, int16 y, int16 mode_value);
void character_react_move(struct actor_rec *rec);
void character_attack_actor(struct actor_rec *target, int16 character);
void draw_glyph_wait(const struct rect_rec *rec, int16 row);
void load_current_map(void);
void draw_button_caption(struct rect_rec *rec, int16 n);
void sound_start_force(int16 sel);
void sound_start(int16 sel);
int16 driver_play_sequence(int16 sequence);
extern int16 shadow_text_draw(int16 attr, int16 col, int16 row, const char *text);
void draw_menubar(void);
extern int16 shadow_key_read(void);
int16 shadow_asset_load(char *name, uint16 paragraph);
extern int16 shadow_video_page(int16 page);
void word_mul(uint16 *acc, uint16 yl, uint16 yh);
#ifdef SS_SDL
int16 table_longest_string(void *items, int16 image_addrs);
#else
int16 table_longest_string(uint16 *items, int16 image_addrs);
#endif
void long_mul(uint16 xl, uint16 xh, uint16 yl, uint16 yh, uint16 *plo, uint16 *phi);
void mem_copy_bytes(uint8 *dst, const uint8 *src, int16 n);
void format_uint(uint16 v, char *p);
void format_ulong(uint32 v, char *p);
void tick_handler_type2(void);
void tick_handler_type0(void);
void tick_handler_type1(void);
void menu_row(char **tab, int16 idx, int16 attr);
void mul16(uint16 a, uint16 b, uint16 *plo, uint16 *phi);
int16 muldiv100(int16 a, int16 b);
char *name_from_pool(uint16 off);
extern void pit_reload(int16 div);
extern void pit_stop(void);
struct character_rec *character_at(uint16 addr);
int16 rect_scan(int16 count, uint8 *t);
void redraw_rect_slot(void);
#ifdef SS_SDL
void sprite_expand_group4(int16 kind, int16 x, int16 y, const uint8 *src);
void sprite_expand_group2(int16 kind, int16 x, int16 y, const uint8 *src);
void remap_group(int16 kind, const uint8 *src, int16 grp);
#else
void sprite_expand_group4(int16 kind, int16 x, int16 y, int16 data_off);
void sprite_expand_group2(int16 kind, int16 x, int16 y, int16 data_off);
void remap_group(int16 kind, int16 data_off, int16 grp);
#endif
extern int16 rnd(int16 n);
extern int16 shadow_getkey_noecho(void);
extern int16 shadow_poll(void);
extern int16 shadow_save(void);
#ifdef SS_SDL
void shadow_sprite(int16 sel, int16 kind, int16 x, int16 y, const uint8 *src);
#else
void shadow_sprite(int16 sel, int16 kind, int16 x, int16 y, int16 data_off, uint16 seg);
#endif
extern int16 shadow_video_copy(uint16 lines);
extern int16 shadow_page_flip(void);
extern int16 shadow_rect_fill(uint16 a, uint16 b, uint16 c, uint16 d);
extern int16 shadow_screen_refresh(void);
void shadow_mouse_read(void);
extern void shadow_mouse_draw(uint16, uint16);
extern int16 shadow_screen_restore(uint16 v);
extern int16 shadow_region_clear(int16 and_mask, int16 column, int16 row, int16 width_bytes, int16 height_rows);
extern void (*volatile g_disp_handler)(void);  /* 0xCE82 */
uint8 *walk_table(int16 off);

#endif
