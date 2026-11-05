/*  Shadow Sorcerer  License: Public Domain (CC0)  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)*/
#include "names.h"
#include "ids.h"
#include <stdio.h>
#include <signal.h>
#ifdef SS_SDL
#include "ss_platform.h"
#include "ss_music.h"
#include "ss_sfx.h"
#endif
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#endif
#include "shadow.h"
#include "game.h"
#include "mlocals.h"
char g_play_mode;                        /* 0x9E0C */
int16 g_hw_config;                         /* 0x9E14 */
/* 0xA9C8 */
uint16 g_mem_next;
extern uint16 g_overlay_segment;           /* 0x9f2a */
uint16 g_monster_bank_segments[30];	/* 0xA28C */
uint16 g_missile_at;               /* 0xA172 */
uint16 g_objpic_at;             /* 0xA9A8 */
uint16 g_mappic_at;				              /* 0xA9A2 */
#include "structs.h"
extern int16  startup_checks(void);
extern void install_tick_hook(void);
extern void load_driver_assets(void);
extern uint16 g_driver_seg;                  /* 0x9442 */
extern void driver_start(int16);
extern int16  mouse_button_read(void);
extern void terminate_callback(void);
extern uint16 g_text_msg_offset;
extern struct credits_text credits_08d77;
extern uint16 g_video_mode, g_video_mode_saved;
extern uint16 g_alloc_mark;
extern FILE *g_text_stream;
extern uint16 g_cursor_mode;
extern void  set_dac_palette(void);            /* 0x9B81 */
#ifdef SS_SDL
extern int16 title_menu(void);
extern void  title_apply_debug(void);
#endif
extern int16   draw_text_window(int16 x, int16 y, int16 w, int16 h);   /* 0x4693 */
extern void  load_map_records(void);            /* 0x5C40 */
extern void  load_pane_blocks(void);            /* 0x6144 */
extern uint16 g_text_index[];		/* 0x9c10 */
extern int16   message_box(int16, int16);        /* 0x4918 */
extern int16   mouse_poll_moved(int16);             /* 0x49BC */
extern int16   shadow_video_flags(int16);             /* 0x9CFA */
extern int16   shadow_video_copy(uint16);
extern int16   event_raise(int16);             /* 0xB4BA */
extern void  draw_map_view(void);            /* 0xAEFC */
extern void  effect_clear_flags(void);            /* 0xBDCF */
extern void  tile_set_position(void);            /* 0x60DE */
/* 0xB88 */
extern void  hide_one_dollar(void);            /* 0xB072 */
extern void  creature_behavior_round(void);            /* 0x7D48 */
extern void  party_upkeep_charge(void);            /* 0x7EE5 */
extern int16   companion_react_tile(int16, int16, int16);   /* 0x8314 */
extern uint16 g_state_countdown, g_overworld_phase, g_hour_is_20;
extern uint8 g_map_mode_char;
extern uint16 g_pane_base_y, g_pane_base_x, g_companion_react_skip;
extern uint16 g_elistan_menu_choice, g_golmoon_menu_choice, g_raistlin_menu_choice, g_panel_busy, g_selected_hero_slot;
extern uint16 g_timer_debt_d, g_timer_debt_c, g_timer_debt_b, g_timer_debt_a;
extern uint16 g_sound_request, g_game_end_flag, g_reserved_b;
extern uint16 g_clock_hour, g_overworld_hold, g_elapsed_accum, g_elapsed_step;
extern volatile uint16 g_shadow_counters[4];   /* 0xA8A4..0xA8AA */
#define g_shadow_event_count g_shadow_counters[0]
extern uint16 g_mouse_y;
extern int16   clock_advance(int16);             /* 0x5E1A */
extern int16   party_update_all(int16 advance, int16 draw);        /* 0x7807 */
extern int16   mouse_poll_moved(int16);             /* 0x49BC */
extern int16   mouse_button_read(void);            /* 0x4A36 */
extern void  tactical_command_panel(int16, int16);        /* 0xB5DF */
extern int16   shadow_screen_restore(uint16);/* 0x703C */
extern void  sound_start_force(int16 sel);
extern void  driver_resume_and_call(int16 entry, int16 arg);   /* 0xD2C7 */
extern int16   shadow_page_flip(void);
extern int16   driver_play_sequence(int16);             /* 0xD1DC */
extern void  wait_ticks(int16);            /* 0x4866 */
extern void  terrain_change(void);            /* 0x5F32 */
extern void  spawn_monsters(int16);             /* 0x8B53 */
extern int16   tile_first_entry(void);            /* 0x628C */
extern void  world_map_click_view(void);            /* 0x8659 */
extern void  clock_draw_time(void);            /* 0x5EC9 */
#ifdef SS_SDL
extern int16   shadow_set_palette(const uint8 *);
#else
extern int16   shadow_set_palette(uint16);
#endif
extern uint16 g_hold_length;
extern int16   rect_scan(int16 count, uint8 *table);  /* 0xCDC */
extern struct rect_rec g_hero_slots[20]; /* DGROUP 0x23a0 */
extern struct rect_rec g_rect_panel_boxes[4];       /* DGROUP 0xbb2 */
extern struct rect_rec g_hero_slot_boxes[4];       /* DGROUP 0xbda */
extern uint8 g_terrain_grid[];        /* DGROUP 0x13a6 */
extern uint16 g_mouse_x;
extern uint16 g_reserved_a, g_overworld_ready, g_pending_actor_event, g_pending_terrain;
extern uint16 g_map_N_event_done;
extern int16   draw_map_record(int16);             /* 0x9753 */
extern int16   hero_cursor_rotate(int16);              /* 0x27A */
extern int16   free_list_clear_nodes(void);            /* 0x8EFA */
extern int16   actor_sweep(void);            /* 0x4115 */
extern void  map_clear_cell_kinds(int16);           /* 0x981F */
extern int16   effect_tick_all(void);            /* 0xBEA2 */
extern int16   draw_terrain_sprites(void);            /* 0x8DF2 */
extern int16   character_detail_panel(int16, int16, int16);   /* 0x130E */
extern int16   character_command_panel(int16, int16, int16);   /* 0x4A4B */
extern int16   pick_up_ground_item(int16 slot);
extern int16   show_ground_item_description(int16 slot);
extern void    group_move_order(void);
extern int16   hit_test_buttons(void);            /* 0xB4E7 */
extern void  draw_hero_overlay(void);             /* 0x32E */
extern void  camera_publish_cells(int16);             /* 0x5B50 */
extern void  clock_tick_overworld(void);            /* 0x633C */
extern void  draw_presentation(int16);             /* 0x98AB */
extern void  reveal_cells(const uint8 *);            /* 0x706E */
extern int16   shadow_poll(void);           /* 0xD374 */
extern int16   rect_scan2(int16 count, uint8 *t);
extern int16   menubar_click(int16, int16);          /* 0x59B2 */
extern int16   joystick_calibrate(void);              /* 0x45F6 */
extern int16   move_view(void);              /* 0x63DB */
extern int16   save_load_dialog(void);              /* 0x2A18 */
extern void  check_refugee_total(void);              /* 0x6455 */
extern void  experience_points_panel(void);              /* 0x2C66 */
extern void  show_text_panel(int16);               /* 0x47B2 */
extern void  draw_hero_gauges(void);               /* 0x1D2 */
extern int16   shadow_video_page(int16);               /* 0xD443 */
extern struct game_state g_game_state;    /* DGROUP 0x2152 */
extern char g_name_vmon[10];             /* DGROUP 0xafe */
#define g_monster_bank_suffix (*(uint8 *)&g_name_vmon[4])
extern uint8 g_rect_menubar[50];  /* DGROUP 0xb4e */
extern uint16 g_menu_toggle;               /* 0x98CA */
extern uint8 g_action_freeze;          /* 0x9660 */
extern void text_copy(char *dest, const char *src);	/* 0x0EE0 */
/* 0x64EC */
#ifdef SS_SDL
int game_main(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
	int16 i;
	int16 input_event;
	int16 cursor_rotate_phase;
	int16 tail_countdown;
	int16 previous_hour;
	int16 panel_command;
	int16 draw_events;
	uint16 hold_before;
	int16 advance_parties;
	int16 party_events;
	int32 n;
	char c;
	char saved_prefix;
	int16 started_from_load = 0;
#ifndef SS_SDL
	{
		union {
 uint16 far *p;
 struct {
 uint16 off, seg;
 }
 r;
 }
 ivt;
		ivt.r.off = (uint16)(SHADOW_VECTOR * 4);
		ivt.r.seg = 0;
		if (ivt.p[0] == 0 && ivt.p[1] == 0)			for (;;)				;
	}
#endif
	do {
		n = shadow_poll();
	} while (n == 0);
	credits_save();
	g_hw_config = shadow_config();
	if (startup_checks() != 0)		return 0;
	g_shadow_prefix = 'e';
	for (i = 1; i < argc; i++) {
		c = argv[i][0];
		g_play_mode = 0;
		if (c > '@' && c < '[')			c |= 0x20;
		if (c == 'a') {
			g_play_mode = 2;
		} else if (c == 'r') {
			g_play_mode = 3;
		} else if (c == 't') {
			g_play_mode = 1;
		} else if (c == 'v') {
			g_shadow_prefix = 'v';
			g_text_page = TEXT_PAGE_VGA;
			g_page_rect = PAGE_RECT_VGA;
			g_window_index = TEXT_WINDOW_VGA;
			g_panel_text_attr = PANEL_TEXT_ATTR_VGA;
			g_gauge_fill_short = GAUGE_FILL_SHORT_VGA;
			g_gauge_fill_long = GAUGE_FILL_LONG_VGA;
		}
}
	if (g_play_mode < 2) {
		install_tick_hook();
	} else {
		load_driver_assets();
		saved_prefix = g_shadow_prefix;
		g_shadow_prefix = 's';
		g_driver_seg = (uint16)g_mem_next;
		n = shadow_load("sound.drv", (uint16)g_mem_next);
		g_mem_next += (((uint16)n >> 4) + 1);
		driver_start(g_play_mode);
		g_shadow_prefix = saved_prefix;
	}
#ifdef SS_SDL
	/* The Adlib/MT-32 driver is 16-bit DOS code; the port uses the PC	   speaker engine, which is the same fallback the release ships. */
	if (g_play_mode >= 2)		g_play_mode = 0;
#endif
	g_hold_length = 9;
#ifdef __EMSCRIPTEN__
	signal(SIGINT, (void (*)(int))terminate_callback);
#else
	signal(SIGINT, terminate_callback);
#endif
	/* 0x6617-0x661A */
	g_overlay_segment = (uint16)g_mem_next;
	while ((n = shadow_load("vclips.dat", (uint16)g_mem_next)) == 0) {
		shadow_video_page(SHADOW_PAGE_DEFAULT);
#ifdef SS_SDL
		shadow_text(SHADOW_TEXT_PAGE_3, DISK_PROMPT_COL, DISK_PROMPT_ROW,		            "Please insert the game disk");
#else
		shadow_text(SHADOW_TEXT_PAGE_3, DISK_PROMPT_COL, DISK_PROMPT_ROW,		            g_text_msg_offset);
#endif
		do {
			if (shadow_poll() != 0)				break;
		} while (mouse_button_read() == 0);
		while (mouse_button_read() != 0)			;
		shadow_video_page(SHADOW_PAGE_PREVIOUS);
	}
	g_mem_next += (((uint16)n >> 4) + 1);
	for (i = 0; i < VMON_BANK_STARTUP_COUNT; i++) {
		if (i == VMON_BANK_FIRST_DYNAMIC)			continue;
		g_monster_bank_segments[i] = (uint16)g_mem_next;
		g_monster_bank_suffix = (uint8)(i + VMON_BANK_FIRST_LETTER);
		n = shadow_load(g_name_vmon, (uint16)g_mem_next);
		if (n != 0)			g_mem_next += (((uint16)n >> 4) + 1);
	}
	/* The startup loop skips the dynamic slot.  The release leaves that	   slot pointing at the previous bank (VMONQ, which holds the Death	   Knight/dwarven guardian); in the DOS build the skipped entry kept	   the same paragraph.  Set it explicitly so the actor is both drawn	   and clickable instead of reading bank 0 / segment zero. */
	g_monster_bank_segments[VMON_BANK_FIRST_DYNAMIC] =		g_monster_bank_segments[VMON_BANK_FIRST_DYNAMIC - 1];
	g_missile_at = (uint16)g_mem_next;
	n = shadow_load("vmissle.dat", (uint16)g_mem_next);
	g_mem_next += (((uint16)n >> 4) + 1);
	g_objpic_at = (uint16)g_mem_next;
	n = shadow_load("vobjpic.dat", (uint16)g_mem_next);
	g_mem_next += (((uint16)n >> 4) + 1);
	g_mappic_at = (uint16)g_mem_next;
	n = shadow_load("vmappic.dat", (uint16)g_mem_next);
	g_mem_next += (((uint16)n >> 4) + 1);	/* 0x671E..0x6733 */
	for (; i != EFFECT_COUNT; i++)		g_monster_bank_segments[i] = g_monster_bank_segments[0];	/* 0x6735 */
	g_video_mode_saved = g_video_mode;
	if (g_hw_config != HW_CONFIG_20)
#ifdef SS_SDL
		shadow_set_palette(g_palette_registers);
#else
		shadow_set_palette((uint16)g_palette_registers);
#endif
	/* 0x674C */
	shadow_video_copy(SCREEN_HEIGHT);	/* 0x6756 */
	if (g_shadow_prefix == 'v')		set_dac_palette();	/* 0x6760 */
	/* 0x678C */
	shadow_video_copy(SCREEN_HEIGHT);
	shadow_page_flip();
	load_map_records();
	load_pane_blocks();
	g_alloc_mark = (uint16)g_mem_next;	/* 0x67C1 */
	g_text_stream = fopen("text.dat", "r+b");
	fread(g_text_index, 2, 0xfa, g_text_stream);
	g_cursor_mode = CURSOR_MODE_DEFAULT;
#ifdef SS_SDL
	if (ss_show_title()) {
		int16 tr = title_menu();
		if (tr < 0)			return 0;
		if (tr == 1)			started_from_load = 1;
	}
#endif
	/* The title screen sets up its own display page; return to the	   startup page layout before the game draws again. */
#ifdef SS_SDL
	ss_reset_pages();
#endif
	shadow_video_copy(SCREEN_HEIGHT);
	shadow_video_page(SHADOW_PAGE_DEFAULT);	/* 0x67FE */
	shadow_video_page(SHADOW_PAGE_PREVIOUS);
	shadow_video_flags(SHADOW_BLIT_RESET);
	shadow_video_copy(SCREEN_HEIGHT);
	mouse_poll_moved(0);
#ifdef SS_SDL
	if (!started_from_load) {
#endif
	event_raise(EVENT_GAME_START);
	g_state_countdown = 3;
	draw_map_view();
	g_overworld_phase = 1;
	effect_clear_flags();
	tile_set_position();
g_map_mode_char = 1;	/* 0x6857 */
	{
		int16 r = rnd(12);
		r += rnd(12);
		r = r * TICKS_PER_SECOND + PARTY_COOLDOWN_BASE;
		PARTY_REC(PARTY_FOLLOWER_LEADER)->cooldown = r;
		PARTY_REC(PARTY_FOLLOWER_FIRST)->cooldown = r;
	}
	hide_one_dollar();
	g_pane_base_y = 4;
	g_pane_base_x = 4;
	g_companion_react_skip = 0;
	g_elistan_menu_choice = 0;
	g_golmoon_menu_choice = 0;
	g_raistlin_menu_choice = 0;
	g_panel_busy = 0;
	g_timer_debt_d = 0;
	g_timer_debt_c = 0;
	g_timer_debt_b = 0;
	g_timer_debt_a = 0;
	g_selected_hero_slot = 0;
	cursor_rotate_phase = 0;
#ifdef SS_SDL
		if (ss_pending_debug)			title_apply_debug();
	}
#endif	/* 0x68AC */
#ifdef SS_SDL
	{
	int last_music_phase = -1;
	int last_music_map = -1;
#endif
	for (;;) {
	g_sound_request = 0;
	g_game_end_flag = 0;
	g_reserved_b = 0;
#ifdef SS_SDL
	{
		int mp = (int)g_overworld_phase;
		int mm = (int)(uint8)g_map_mode_char;
		if (mp == OVERWORLD_PHASE_MAP && mm == 1 &&		    (last_music_phase != OVERWORLD_PHASE_MAP || last_music_map != 1))			ss_music_play(SS_MUSIC_OVERWORLD);
		last_music_phase = mp;
		last_music_map = mm;
	}
#endif
#ifdef SS_SDL
	if (g_overworld_phase == OVERWORLD_PHASE_MAP &&	    (uint8)g_map_mode_char == 1) {
		if (ss_music_current() == SS_MUSIC_DUNGEON)			ss_music_stop();
	} else if (g_overworld_phase == 2 &&	           (uint8)g_map_mode_char != 1) {
		if (ss_music_current() == SS_MUSIC_NONE)			ss_music_play(SS_MUSIC_DUNGEON);
	}
#endif
	if (g_hour_is_20 != 0) {
		creature_behavior_round();
		party_upkeep_charge();
		if (g_companion_react_skip == 0)			/* 0x68CA */			companion_react_tile(1, 1, 0);
		g_hour_is_20 = 0;
		g_companion_react_skip = 0;
	}
	/* 0x6C98 */
	switch (g_overworld_phase) {
	case OVERWORLD_PHASE_MAP:
		/* 0x68E6 */
		if (g_hw_config == HW_CONFIG_20) {
			while (g_shadow_event_count < 2)				mouse_poll_moved(1);
			g_shadow_event_count = 0;
		}
		/* 0x6906 */
		if (g_clock_hour == HOUR_20 && previous_hour != HOUR_20)			draw_map_view();
		previous_hour = g_clock_hour;
		advance_parties = 0;
		g_overworld_hold = 0;
		if (PARTY_REC(0)->cooldown != 0) {
			/* 0x692A */
			do {
				clock_advance(1);
				mouse_poll_moved(1);
				party_events = party_update_all(1, 1);
			} while (party_events == 0);
			shadow_screen_restore(7);
			draw_events = party_update_all(0, 1);      /* 0x695A */
			if ((draw_events & PARTY_EVENT_SPAWN_MASK) != 0)				party_events = draw_events;
		} else {
			/* 0x6975 */
			shadow_screen_restore(7);
			g_elapsed_accum += g_elapsed_step;
			if (g_elapsed_accum >= TICKS_PER_SECOND) {
      /* 0x6986 */
				advance_parties = 1;
				g_elapsed_accum -= TICKS_PER_SECOND;
				clock_advance(1);
			}
			/* 0x69A1 */
			party_events = party_update_all(advance_parties, 1);
		}
		/* 0x69B1 */
		if (PARTY_REC(10)->kind == 0)			party_events &= (int16)~PARTY_EVENT_SPAWN_SCENE;           /* 0x69B8 */
		if (party_events != 0) {
#ifdef SS_SDL
			if ((party_events & PARTY_EVENT_MOVED) != 0)				ss_sfx_step();
#endif
			if ((party_events & PARTY_EVENT_PRIMARY_MASK) == PARTY_EVENT_MOVED) {
				/* 0x69CE */
				if (ROLL_D100() < 10) {
					shadow_video_flags(SHADOW_BLIT_FLUSH);
					driver_play_sequence(SOUND_SEQUENCE_RANDOM_ENCOUNTER);
					wait_ticks(TEXT_CHANCE_ENCOUNTER);
					g_pane_base_y = 2;
					g_pane_base_x = 2;
					terrain_change();
					spawn_monsters(0);
				}
} else if ((party_events & PARTY_EVENT_TILE_ENTRY) != 0) {
				/* 0x6A12 */
				shadow_video_flags(SHADOW_BLIT_FLUSH);
				if (tile_first_entry() != 0)					terrain_change();
			} else if ((party_events & PARTY_EVENT_SPAWN_MASK) != 0) {
				/* 0x6A34 */
				if ((party_events & PARTY_EVENT_SPAWN_EIGHT) != 0) {
					driver_play_sequence(SOUND_SEQUENCE_EIGHT_MONSTERS);
					wait_ticks(TEXT_DRACONIAN_AMBUSH);
				} else {
					driver_play_sequence(SOUND_SEQUENCE_SCENE_MONSTER);
					wait_ticks(TEXT_DRAGON_BREATH);
				}
				g_pane_base_y = 2;
				g_pane_base_x = 2;
				terrain_change();
				spawn_monsters(PARTY_EVENT_SPAWN_SOURCE(party_events));   /* 0x6A70 */
			}
}
		/* 0x6A81 */
		/* Arrow keys and the numeric keypad walk the party one tile. */
#ifdef SS_SDL
		if (mouse_button_read() == 0) {
			int16 walk_dir = ss_direction_step();
			if (walk_dir >= 0)				(void)port_walk_step(walk_dir);
		}
#endif
		if (mouse_button_read() != 0) {
			shadow_video_flags(SHADOW_BLIT_FLUSH);
			world_map_click_view();
		}
		clock_draw_time();
		shadow_page_flip();
		goto common_tail;                /* 0x6A9B */
	case OVERWORLD_PHASE_WAIT:
		/* 0x6A9E */
		while ((uint16)g_state_countdown > (uint16)g_shadow_event_count)   /* 0x6A9E */			mouse_poll_moved(1);
		g_shadow_event_count = 0;
		g_reserved_a = 0;
		draw_map_record((int16)(int8)g_map_mode_char);		/* 0x6ACA */
		cursor_rotate_phase %= 3;
		hero_cursor_rotate(cursor_rotate_phase);
		++cursor_rotate_phase;
		mouse_poll_moved(1);
		shadow_screen_restore(7);                                  /* 0x6AEA */
		free_list_clear_nodes();
		mouse_poll_moved(1);
		hold_before = g_overworld_hold;                                /* 0x6B01 */
		actor_sweep();
		if (g_pending_actor_event != 0 && hold_before != 0 && g_overworld_hold == 0)			map_clear_cell_kinds((int16)(int8)g_map_mode_char);           /* 0x6B1E */
		mouse_poll_moved(1);
		effect_tick_all();
		mouse_poll_moved(1);
		draw_terrain_sprites();
		if (g_game_end_flag == 0) {
			/* 0x6B4D */
#ifdef SS_SDL
			/* Arrow keys / numeric keypad move the selection cursor. */
			if (mouse_button_read() == 0) {
				int16 site_dir = ss_direction_step();
				if (site_dir >= 0)					(void)port_site_step(site_dir);
			}
#endif
			if ((mouse_button_read() & 1) != 0) {
				/* Left button: select the hero under the pointer. */
				int16 pick = port_pick_actor((int16)g_mouse_x,				                             (int16)g_mouse_y);
				if (pick < 0)					pick = rect_scan(4, (uint8 *)g_hero_slots);
				if (pick >= 0 &&				    (int16)g_hero_slots[pick].state <= HERO_SLOT_STATE_LIMIT) {
					g_selected_hero_slot = (uint16)pick;
					draw_hero_gauges();
				} else {
					pick = rect_scan(4, (uint8 *)g_rect_panel_boxes);
					if (pick >= 0) {
						(void)pick_up_ground_item(pick);
					} else {
						int16 mon = port_pick_monster((int16)g_mouse_x,						                              (int16)g_mouse_y);
						if (mon >= 0) {
							show_actor_description(mon);
							while (mouse_button_read() != 0)								;
						} else {
							pick = rect_scan(4, (uint8 *)g_hero_slot_boxes);
							if (pick >= 0) {
								/* 0x6BBE */
								RECT_STATE_HI(&g_hero_slot_boxes[pick]) += 0x80;
								draw_hero_overlay();
								while (mouse_button_read() != 0)   /* 0x6BC6 */									;
							}
}
}
}
}
			if ((mouse_button_read() & 2) != 0) {
				/* Right button: portrait opens the Character View, a				   hero sprite opens the command menu; otherwise order				   a move or engage a monster.  A double right click on				   the floor is the group-move order. */
				int16 slot = rect_scan(4, (uint8 *)g_hero_slots);
#ifdef SS_SDL
				int16 dbl = ss_take_double_click(2);
#else
				int16 dbl = 0;
#endif
				if (slot >= 0 &&				    (int16)g_hero_slots[slot].state <= HERO_SLOT_STATE_LIMIT) {
					(void)character_detail_panel(slot, 0, 0);
				} else {
					int16 pick = port_pick_actor((int16)g_mouse_x,					                             (int16)g_mouse_y);
					if (pick >= 0 &&					    (int16)g_hero_slots[pick].state <= HERO_SLOT_STATE_LIMIT) {
						tactical_command_panel(pick, QPANEL_CMD_NONE);
					} else {
						pick = rect_scan(4, (uint8 *)g_rect_panel_boxes);
						if (pick >= 0) {
							(void)show_ground_item_description(pick);
						} else if (rect_scan(4, (uint8 *)g_hero_slot_boxes) < 0) {
							/* A click on a monster orders the selected							   hero to the monster's cell (combat itself							   follows the heroes' default orders). */
							int16 mon = port_pick_monster((int16)g_mouse_x,							                              (int16)g_mouse_y);
							if (mon >= 0) {
								int16 mcol = (int16)(((int16)g_actor_table[mon].x - 8) >> 4);
								int16 mrow = (int16)(((int16)g_actor_table[mon].y - 8) >> 4);
								actor_provoke(mon);
								g_mouse_x = (uint16)(((mcol + mrow) << 4) + 16);
								g_mouse_y = (uint16)(((mrow - mcol) << 3) + 0x77);
							}
							camera_publish_cells(1);
							if (dbl != 0)								group_move_order();
						}
}
}
}
}
		shadow_page_flip();                                   /* 0x6BF3 */
		if (g_game_end_flag == 0) {
			if (g_pending_actor_event == 0
#ifdef SS_SDL
			           || g_site_exit_pending != 0
#endif
			           ) {
				if (g_pending_terrain == 0
#ifdef SS_SDL
				    && g_site_exit_pending == 0
#endif
				    ) {
					/* 0x6C17 */
					if (g_terrain_grid[PARTY_REC(0)->grid_x + TERRAIN_WIDTH * PARTY_REC(0)->grid_y]					    == (uint8)g_map_mode_char)						clock_tick_overworld();
				} else if ((int16)g_hero_slots->state < HERO_SLOT_STATE_LIMIT					|| (int16)g_hero_slots[1].state < HERO_SLOT_STATE_LIMIT					|| (int16)g_hero_slots[2].state < HERO_SLOT_STATE_LIMIT					|| (int16)g_hero_slots[3].state < HERO_SLOT_STATE_LIMIT) {
					/* 0x6C32 */
#ifdef SS_SDL
					if (g_pending_terrain == 0) {
						/* Exit map 0 means the overland map. */
						g_site_exit_pending = 0;
						clock_tick_overworld();
					} else {
						g_site_exit_pending = 0;
						terrain_change();
					}
#else
					terrain_change();
#endif
				}
}
}
		/* 0x6C51 */
		if (g_overworld_ready == 0 && g_map_N_event_done == 0 && g_map_mode_char == MAP_N_EVENT_RECORD) {
			g_map_N_event_done = 1;
			draw_presentation(PRESENTATION_DWARF_RESCUE);
			reveal_cells(g_reveal_cells_n_event);
			g_pending_terrain = MAP_N_EVENT_TARGET;
			g_pane_base_x = 3;
			g_pane_base_y = 0;
			terrain_change();
		}
		goto common_tail;                                    /* 0x6C94 */
	default:
		goto common_tail;
	}
common_tail:
	/* 0x6CAA */
	if (tail_countdown != 0)		--tail_countdown;
	if ((mouse_button_read() & 2) != 0 && g_mouse_y <= 8	    && g_mouse_x >= 200) {
		/* The time bar occupies the top-right of the screen. */
		describe_time();
		while (mouse_button_read() != 0)			;
	}
	if (mouse_button_read() != 0 && g_mouse_y <= 8) {
		/* 0x6CC1 */
		input_event = rect_scan(5, g_rect_menubar);
		if (input_event >= 0)			menubar_click(input_event, 0);
	}
	/* 0x6CE2 */
	g_action_freeze = 0;
	input_event = shadow_poll();
	if (input_event == 0)		goto late_block;                       /* 0x6CF1 */
	if (((uint8)input_event) != 0) {
		/* 0x6CFD */
		input_event &= 0xff;
		if (input_event >= 0x41 && input_event <= 0x5a)			input_event |= 0x20;
		panel_command = QPANEL_CMD_NONE;
		if (input_event == ' ') {
			/* 0x6DCF */
			shadow_video_flags(SHADOW_BLIT_FLUSH);
			shadow_video_page(SHADOW_PAGE_DEFAULT);
			message_box(MSG_BOX_NOTICE, MSG_PAUSED);
			if (g_overworld_phase == 1)				shadow_screen_restore(7);
			shadow_video_page(SHADOW_PAGE_PREVIOUS);
		} else if (input_event == '0') {
			clock_tick_overworld();                      /* 0x6D1C */
		} else if (input_event >= '1' && input_event <= '4') {
			if (g_overworld_phase == 2)             /* 0x6D22 */				character_detail_panel(input_event - '1', 0, 1);
		} else if (input_event == 'a') {
			panel_command = QPANEL_CMD_CONTROL;
		} else if (input_event == 'c') {
			panel_command = QPANEL_CMD_MAP_EVENT;
		} else if (input_event == 'g') {
			if (g_overworld_phase == 2)             /* 0x6D50 */				character_command_panel(g_selected_hero_slot, 0, 1);
			panel_command = QPANEL_CMD_HOLD;
		} else if (input_event == 'h') {
			panel_command = QPANEL_CMD_HOLD;                     /* 0x6D68 */
		} else if (input_event == 'i') {
			panel_command = QPANEL_CMD_PICK;                     /* 0x6D70 */
		} else if (input_event == 'j') {
			joystick_calibrate();                      /* 0x6D78 */
		} else if (input_event == 'm') {
			if (g_overworld_phase == 2)             /* 0x6D7E */				character_detail_panel(g_selected_hero_slot, 1, 0);
		} else if (input_event == 'p') {
			panel_command = QPANEL_CMD_GROUP_MOVE;
		} else if (input_event == 'r') {
			panel_command = QPANEL_CMD_QUERY;
		} else if (input_event == 's') {
			panel_command = QPANEL_CMD_MENU;
		} else if (input_event == 't') {
			panel_command = QPANEL_CMD_USE;
		} else if (input_event == 'v') {
			/* 0x6DB9 */
			g_menu_toggle = ~g_menu_toggle & 1;
		} else if (input_event == 'w') {
			panel_command = QPANEL_CMD_SELECT;
		}
		goto end_of_iteration;
	}
	/* 0x6EA0 */
	input_event = (int16)input_event >> 8;
	if (input_event == SCANCODE_ALT_X) {
		terminate_callback();                              /* 0x6ECE */
		goto f_key_select;
	}
	if (input_event == SCANCODE_ALT_R) {
 input_event = MENUBAR_REFUGEES;
 goto arrow;
 }
   /* 0x6EAB */
	if (input_event == SCANCODE_ALT_O) {
 input_event = MENUBAR_OPTIONS;
 goto arrow;
 }
   /* 0x6EB2 */
	if (input_event == SCANCODE_ALT_P) {
 input_event = MENUBAR_PARTY;
 goto arrow;
 }
   /* 0x6EB9 */
	if (input_event >= SCANCODE_F1 && input_event <= SCANCODE_F10) {
		switch (input_event) {
		case SCANCODE_F1:
 case SCANCODE_F2:
 case SCANCODE_F3:
 case SCANCODE_F4:
			goto f_key_select;
		case SCANCODE_F5:
		{
			int16 k;
			for (k = 0; k < 4; ++k)				if ((int16)g_hero_slots[k].state < HERO_SLOT_NONE)					RECT_STATE_HI(&g_hero_slot_boxes[k]) |= 0x80;
			break;
		}
		case SCANCODE_F9:
			if (g_state_countdown != 0)				--g_state_countdown;
			break;
		case SCANCODE_F10:
			if (g_state_countdown != 5)				++g_state_countdown;
			break;
		default:
			break;
		}
		goto late_block;
	}
	goto late_block;
arrow:
	/* 0x6EBE */
	menubar_click(input_event, 1);
	goto late_block;
f_key_select:
	/* 0x6ED1 */
	if ((int16)((struct rect_rec *)((uint8 *)&g_game_state + input_event * 10))->state <= HERO_SLOT_STATE_LIMIT) {
		g_selected_hero_slot = input_event - SCANCODE_F1;
		if (g_overworld_phase == 2)			draw_hero_gauges();
	}
	goto late_block;
end_of_iteration:
	/* 0x6E83 */
	if (panel_command != QPANEL_CMD_NONE && g_overworld_phase == 2)		tactical_command_panel(g_selected_hero_slot, panel_command);
	goto late_block;                              /* 0x6E9D */
late_block:
	/* 0x6F88 */
	if ((int16)g_hero_slots->state > HERO_SLOT_STATE_LIMIT	    && (int16)g_hero_slots[1].state > HERO_SLOT_STATE_LIMIT	    && (int16)g_hero_slots[2].state > HERO_SLOT_STATE_LIMIT	    && (int16)g_hero_slots[3].state > HERO_SLOT_STATE_LIMIT	    && move_view() == 0) {
		shadow_video_page(SHADOW_PAGE_DEFAULT);
		message_box(MSG_BOX_ERROR, MSG_DEFEAT);
		if (save_load_dialog() == 0) {
			shadow_video_page(SHADOW_PAGE_PREVIOUS);
			terminate_callback();                              /* 0x6FD9 */
		}
}
	/* 0x6FDC */
	check_refugee_total();
	if (g_game_end_flag != 0) {
		shadow_video_page(SHADOW_PAGE_DEFAULT);
		experience_points_panel();
		while (shadow_poll() == 0 && mouse_button_read() == 0)	/* 0x6FF2 */			;
		shadow_video_copy(SCREEN_HEIGHT);
		show_text_panel(TEXT_GATES_CLOSE);
		if (save_load_dialog() == 0)			terminate_callback();
		shadow_video_page(SHADOW_PAGE_PREVIOUS);
	}
	/* 0x702A */
	if (g_sound_request != 0) {
		if (g_play_mode < 2) {
			sound_start_force(g_sound_request);
		} else {
			/* 0x7044 */
			g_sound_request += (g_play_mode == 3)			    ? DRIVER_SEQ_BANK_MT32 : DRIVER_SEQ_BANK_ADLIB;
			driver_resume_and_call(DRIVER_ENTRY_QUEUE, g_sound_request);
		}
}
}
#ifdef SS_SDL
	}
#endif
	return 0;
}
