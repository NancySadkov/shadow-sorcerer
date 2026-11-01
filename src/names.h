/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#ifndef NAMES_H
#define NAMES_H

#include "types.h"

#define g_chosen_party                     DAT_0x1ba      /* 0x01BA */
#define g_panel_left                    (g_panel_boxes[0].x)     /* 0x0C04 */
#define g_panel_top                     (g_panel_boxes[0].y)     /* 0x0C06 */
#define g_panel_left2                   (g_panel_boxes[1].x)     /* 0x0C0E */
#define g_panel_top2                    (g_panel_boxes[1].y)     /* 0x0C10 */
#define g_panel_cursor_x                (g_panel_boxes[2].x)     /* 0x0C18 */
#define g_panel_cursor_y                (g_panel_boxes[2].y)     /* 0x0C1A */
#define g_panel_right                   (g_panel_boxes[3].x)     /* 0x0C22 */
#define g_panel_bottom                  (g_panel_boxes[3].y)     /* 0x0C24 */
#define g_text_msg_offset                 DAT_0d56       /* 0x0D56 */
#define g_text_msg_offset_default  DAT_0d58       /* 0x0D58 */
#define g_overworld_phase                 DAT_0x2260     /* 0x2260 */
#define g_screen_hold                         DAT_0x2264     /* 0x2264 */
#define g_state_countdown                 DAT_0x2266     /* 0x2266 */
#define g_timer_debt_a                       DAT_0x2268     /* 0x2268 */
#define g_timer_debt_b                       DAT_0x226a     /* 0x226A */
#define g_timer_debt_c                       DAT_0x226c     /* 0x226C */
#define g_timer_debt_d                       DAT_0x226e     /* 0x226E */
#define g_frame_debt                           DAT_0x2270     /* 0x2270 */
#define g_move_speed_shift               DAT_0x2272     /* 0x2272 */
#define g_map_N_event_done               DAT_0x2278     /* 0x2278 */
#define g_map_mode_char                     DAT_0x227a     /* 0x227A */
#define g_seek_suspended                   DAT_0x2280     /* 0x2280 */
#define g_hour_is_20                           DAT_0x2282     /* 0x2282 */
#define g_day_counter                         DAT_0x2284     /* 0x2284 */
#define g_clock_hour                           DAT_0x2286     /* 0x2286 */
#define g_hour_ticks                           DAT_0x2288     /* 0x2288 */
#define g_elapsed_accum                     DAT_0x228a     /* 0x228A */
#define g_elapsed_step                       DAT_0x228c     /* 0x228C */
#define g_overworld_hold                   DAT_0x943a     /* 0x943A */
#define g_save_slot_highlight         DAT_0x95e8     /* 0x95E8 */
#define g_action_freeze                     DAT_0x9660     /* 0x9660 */
#define g_menu_toggle                         DAT_0x98ca     /* 0x98CA */
#define g_bless_duration                     DAT_0x9e06     /* 0x9E06 */
#define g_stale_clear_done               DAT_0x9e08     /* 0x9E08 */
#define g_detect_invisible_ticks     val_09e0a      /* 0x9E0A */
#define g_sweep_counter                     DAT_0x9e0e     /* 0x9E0E */
#define g_spawn_cooldown                   DAT_0x9e10     /* 0x9E10 */
#define g_action_countdown               DAT_0x9e12     /* 0x9E12 */
#define g_terrain_change_done         DAT_0x9f16     /* 0x9F16 */
#define g_action_anchor_x                 DAT_0x9f20     /* 0x9F20 */
#define g_search_weight                     DAT_0x9f22     /* 0x9F22 */
#define g_action_anchor_y                 DAT_0x9f24     /* 0x9F24 */
#define g_hold_length                         DAT_0x9f26     /* 0x9F26 */
#define g_spell_purge_pending             DAT_0x9f28     /* 0x9F28 */
#define g_sound_request                     sound_request  /* 0x9F6A */
#define g_save_stream                         val_0a16e      /* 0xA16E */
#define g_game_end_flag                     DAT_0xa170     /* 0xA170 */
#define g_node_selector                     DAT_0xa174     /* 0xA174 */
#define g_pending_actor_event       DAT_0xa176     /* 0xA176 */
#define g_panel_kind_hi                   DAT_0xa17d     /* 0xA17D */
#define g_panel_kind_lo                   DAT_0xa17e     /* 0xA17E */
#define g_video_mode                           val_0a2ac      /* 0xA2AC */
#define g_video_mode_saved               val_0a2ae      /* 0xA2AE */
#define g_detect_magic_ticks           DAT_0xa2ca     /* 0xA2CA */
#define g_text_stream                         val_0a2ce      /* 0xA2CE */
#define g_sprite_record_off             val_0a2d0      /* 0xA2D0 */
#define g_sprite_stride_index         DAT_0xa61b     /* 0xA61B */
#define g_panel_stride_mode             DAT_0xa61c     /* 0xA61C */
#define g_longest_string                   DAT_0xa662     /* 0xA662 */
#define g_selected_hero_slot             DAT_0xa92e     /* 0xA92E */
#define g_mouse_last_x                       DAT_0xa98a     /* 0xA98A */
#define g_mouse_last_y                       DAT_0xa98c     /* 0xA98C */
#define g_cursor_mode                         DAT_0xa98e     /* 0xA98E */
#define g_sprite_last_frame             DAT_0xa992     /* 0xA992 */
#define g_string_cursor                     DAT_0xa998     /* 0xA998 */
#define g_protection_duration                     DAT_0xa99a     /* 0xA99A */
#define g_search_center_x                 DAT_0xa99c     /* 0xA99C */
#define g_search_center_y                 DAT_0xa99e     /* 0xA99E */
#define g_reserved_a                        DAT_0xa9a0     /* 0xA9A0 */
#define g_overworld_ready                 DAT_0xa9a4     /* 0xA9A4 */
#define g_reserved_b                        DAT_0xa9a6     /* 0xA9A6 */
#define g_sprite_draw_x                     DAT_0xa9aa     /* 0xA9AA */
#define g_map_load_gate                           word_0a9ac     /* 0xA9AC */
#define g_sprite_draw_y                     DAT_0xa9ae     /* 0xA9AE */
#define g_golmoon_menu_choice             DAT_0xa9b0     /* 0xA9B0 */
#define g_panel_busy                           DAT_0xa9b2     /* 0xA9B2 */
#define g_map_grid_ptr                   val_0a9b4      /* 0xA9B4 */
#define g_elistan_menu_choice           DAT_0xa9b6     /* 0xA9B6 */
#define g_find_traps_span                     DAT_0xa9b8     /* 0xA9B8 */
#define g_find_traps_gate                       DAT_0xa9ba     /* 0xA9BA */
#define g_raistlin_menu_choice             DAT_0xa9c0     /* 0xA9C0 */
#define g_paralysis_duration                     DAT_0xa9ca     /* 0xA9CA */
#define g_cursor_tile_x                     val_0a9cc      /* 0xA9CC */
#define g_cursor_tile_y                     val_0a9ce      /* 0xA9CE */
#define g_window_stamp                       DAT_0xa9d0     /* 0xA9D0 */
#define g_map_cell_kind                 byte_0a9d2     /* 0xA9D2 */
#define g_map_file                       DAT_0xa9fe     /* 0xA9FE */
#define g_row_height                           DAT_0xab2c     /* 0xAB2C */
#define g_companion_react_skip       DAT_0xad2e     /* 0xAD2E */
#define g_pending_terrain                 DAT_0xad30     /* 0xAD30 */
#define g_sprite_step_x               DAT_0xad34     /* 0xAD34 */
#define g_sprite_step_y               DAT_0xad3a     /* 0xAD3A */
#define g_pane_base_x                 DAT_0xad3c     /* 0xAD3C */
#define g_pane_base_y                 DAT_0xad3e     /* 0xAD3E */
#define g_sprite_frame_count                       DAT_0xad72     /* 0xAD72 */
#define g_sprite_frame_index                       DAT_0xad73     /* 0xAD73 */
#define g_refugee_seek_hold             DAT_0xad74     /* 0xAD74 */

#define g_rng_seed                               rng_seed_a9c6  /* 0xA9C6 */

#define g_terrain_grid                       arr_13a6       /* 0x13A6 */
#define g_creature_grid                     grid_2c92      /* 0x2C92 */
#define g_overlay_segment                 seg_9f2a       /* 0x9F2A */
#define g_special_monster_bank_loaded           DAT_0x01bc     /* 0x01BC */
#define g_save_dialog_x                     DAT_0x0ccc     /* 0x0CCC */
#define g_save_dialog_y                     DAT_0x0cce     /* 0x0CCE */
#define g_sticky_click_flag             DAT_0xa288     /* 0xA288 */

#define g_acc_hi                 acc_hi_a620    /* 0xA620 */
#define g_acc_lo                 acc_lo_a61e    /* 0xA61E */
#define g_alloc_mark             alloc_mark_0a8ac /* 0xA8AC */
#define g_bar_frame              bar_frame_00db /* 0x00DB */
#define g_btn_table              btn_b80        /* 0x0B80 */
#define g_btn_pairs              btn_pair_c84   /* 0x0C84 */
#define g_btn_rows               btn_rows_c98
#define g_map_records            buf_0x3060     /* 0x3060 */
#define g_mode_names             mode_names     /* 0x98EE */
#define g_text_index             buf_0x9c10     /* 0x9C10 */
#define g_cell_grid              cell_191e      /* 0x191E */
#define g_cp_alpha               cp_alpha
#define g_cp_off                 cp_off
#define g_cp_which               cp_which
#define g_credits                credits_08d77
#define g_dir_code               dir_code_ede   /* 0x0EDE */
#define g_disp_clear             disp_clear     /* 0xCE66 */
#define g_disp_control           disp_control   /* 0xCE6E */
#define g_disp_flag              disp_flag      /* 0xCE7C */
#define g_disp_handler           disp_handler   /* 0xCE82 */
#define g_disp_index             disp_index     /* 0xCE80 */
#define g_disp_last              disp_last      /* 0xCE7E */
#define g_disp_ptr               disp_ptr       /* 0xCE78 */
#define g_disp_ptr2              disp_ptr2      /* 0xCE7A */
#define g_disp_state             disp_state     /* 0xCE76 */
#define g_driver_stopped         driver_stopped /* 0x9E04 */
#define g_driver_seg             driver_seg     /* 0x9442 */
#define g_drv_handle             drv_handle_0a984
#define g_party_table               entities       /* 0x2B48 */
#define g_entry_off              entry_off      /* 0x9440 */
#define g_fat_dest               fat_dest       /* 0xA8AE */
#define g_fixed_string               fixed_str      /* 0x9C35 */
#define g_game_state              fkey_gate      /* 0x2152 */
#define g_free_head              free_head_a2d2 /* 0xA2D2 */
#define g_free_swept             free_swept_a2c8 /* 0xA2C8 */
#define g_hot_spots              hot_spot_c2a   /* 0x0C2A */
#define g_hw_config              hw_config      /* 0x9E14 */
#define g_kind_ceiling           kind_ceiling_106e /* 0x106E */
#define g_kind_gate              kind_gate_a9bb /* 0xA9BB */
#define g_kind_segment           kind_seg_a282  /* 0xA282 */
#define g_label_buttons          lbl_b44        /* 0x0B44 */
#define g_line_buffer            line_buf_ad40  /* 0xAD40 */
#define g_monster_bank_segments                load_at        /* 0xA28C */
#define g_map_working                map_rec_a178   /* 0xA178 */
#define g_map_ring               map_ring_01be  /* 0x01BE */
#define g_map_tile               map_tile       /* 0x2C92 */
#define g_mappic_at              mappic_at      /* 0xA9A2 */
#define g_mask                   mask_0f1c
#define g_mem_next               mem_next       /* 0xA9C8 */
#define g_menu_boxes             menu_box_aa00  /* 0xAA00 */
#define g_menubar_labels         menubar_labels
#define g_messages               messages
#define g_vote_data              misc_1cd4      /* 0x1CD4 */
#define g_missile_at             missile_at     /* 0xA172 */
#define g_mode_dest              mode_dest      /* 0xA986 */
#define g_mouse_button           mouse_btn_a990 /* 0xA990 */
#define g_mouse_x                mouse_x_a9c2   /* 0xA9C2 */
#define g_mouse_y                mouse_y_a9c4   /* 0xA9C4 */
#define g_move_cost_a            move_cost_a    /* 0x11BE */
#define g_move_cost_b            move_cost_b    /* 0x1238 */
#define g_name_offset            name_off_d84   /* 0x0D84 */
#define g_name_pool              name_pool      /* 0x94B0 */
#define g_class_name_pool        name_pool_717  /* 0x0717 */
#define g_party_name_pool       name_pool_a1a  /* 0x0A1A */
#define g_name_vcin              name_vcin      /* 0x0262 */
#define g_name_vmpl              name_vmpl      /* 0x0AF4 */
#define g_neighbour_table        nbr_9662       /* 0x9662 */
#define g_noise_table            noise_1bda     /* 0x1BDA */
#define g_noise_mask             noise_mask     /* 0xCE70 */
#define g_noise_or               noise_or       /* 0xCE72 */
#define g_noise_period           noise_period   /* 0xCE74 */
#define g_noise_shift            noise_shift    /* 0xCE68 */
#define g_effect_table                obj_rec        /* 0x9696 */
#define g_objpic_at              objpic_at      /* 0xA9A8 */
#define g_ofs_even_ring          ofs_even_ring  /* 0x1024 */
#define g_ofs_odd_ring           ofs_odd_ring   /* 0x0FDA */
#define g_pal_rgb                pal_rgb        /* 0x1122 */
#define g_path_grid              path_grid_0a8b2 /* 0xA8B2 */
#define g_path_qx                path_qx_0a622  /* 0xA622 */
#define g_path_qy                path_qy_0a864  /* 0xA864 */
#define g_path_read              path_rd_09f18  /* 0x9F18 */
#define g_path_write             path_wr_0a16c  /* 0xA16C */
#define g_pit_divisor            pit_divisor    /* 0xCE6C */
#define g_pixel_order            pixel_order    /* 0x9E16 */
#define g_play_mode              play_mode      /* 0x9E0C */
#define g_probe_table            probe_9876     /* 0x0BD8 */
#define g_actor_table           actor_tbl_2968   /* 0x2968 */
#define g_record_addr            rec_addr_9416  /* 0x9416 */
#define g_rect_panel_boxes               rect_bb2       /* 0x0BB2 */
#define g_hero_slot_boxes               rect_bda       /* 0x0BDA */
#define g_rect_menubar           rect_menubar   /* 0x0B4E */
#define g_hero_slots          party_slot_tab  /* 0x23A0 */
#define g_reg_slots              reg_slots
#define g_roll_table             roll_tab_1e50  /* 0x1E50 */
#define g_character_table            char_table_904a  /* 0x904A */
#define g_presentation_scripts   pres_scripts_8ab4 /* 0x8AB4 */
#define g_service_count          service_count
#define g_service_flag           service_flag
#define g_service_id             service_id
#define g_service_index          service_index
#define g_service_state          service_state
#define g_shadow_counters        shadow_counters /* 0xA8A4 */
#define g_shadow_prefix          shadow_prefix  /* 0xA61A */
#define g_slot_name              slot_name      /* 0x95B8 */
#define g_slot_rows              slot_rows      /* 0x0CCA */
#define g_song_bank              song_bank      /* 0xC890 */
#define g_spawn_table            spawn_tab_1dfa /* 0x1DFA */
#define g_ssi_dest               ssi_dest       /* 0x9F1C */
#define g_svc_pos                svc_pos_9f1a   /* 0x9F1A */
#define g_site_table           table_2468     /* 0x2468 */
#define g_terr_bonus_a           terr_bonus_a   /* 0x1156 */
#define g_terr_bonus_b           terr_bonus_b   /* 0x118A */
#define g_tick_accum             tick_accum     /* 0xCE6A */
#define g_tick_by_kind           tick_by_kind_109c /* 0x109C */
#define g_tick_enabled           tick_enabled   /* 0x98CA */
#define g_tick_in_handler        tick_in_handler /* 0xCF53 */
#define g_tick_prev_off          tick_prev_off  /* 0xCE84 */
#define g_tick_prev_seg          tick_prev_seg  /* 0xCE86 */
#define g_tile_flags             tile_flag_10ea /* 0x10EA */
#define g_timers                 timers_2970
#define g_panel_template         tmpl_9444      /* 0x9444 */
#define g_tmpl_horiz             tmpl_horiz_0061
#define g_tmpl_vert              tmpl_vert_008a
#define g_trail                  trail          /* 0x2298 */
#define g_trail_head             trail_head     /* 0x2398 */
#define g_vmpl_para              vmpl_para      /* 0x9F2E */
#define g_vmpl_slot              vmpl_slot      /* 0xA9BC */
#define g_vote_options           vote_opt_d74   /* 0x0D74 */
#define g_vote_tbl               vote_tbl       /* 0x1CD4 */
#define g_wait_cost_a            wait_cost_a    /* 0x12B2 */
#define g_wait_cost_b            wait_cost_b    /* 0x132C */
#define g_walk_grid              walk_1662      /* 0x1662 */
#define g_walk_delta             walk_delta_f94 /* 0x0F94 */
#define g_waypoint_route_a       wp_20c4        /* 0x20C4 */
#define g_waypoint_route_b       wp_2114        /* 0x2114 */
#define g_waypoint_route_c       wp_215a        /* 0x215A */
#define g_waypoint_route_d       wp_2194        /* 0x2194 */

#endif
