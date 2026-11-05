/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Platform layer used by the game sources when SS_SDL is defined.
*/

#ifndef SS_PLATFORM_H
#define SS_PLATFORM_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SS_SCREEN_W 320
#define SS_SCREEN_H 200

#ifndef far
#define far
#endif
#ifndef near
#define near
#endif
#ifndef interrupt
#define interrupt
#endif

/* Flat arena standing in for DOS conventional memory.  A "segment" is a
   paragraph offset into it, exactly like a real-mode segment, so the asset
   loader and the sprite banks keep working unchanged. */
extern uint8 *ss_arena;
/* Covers every real-mode seg:off address (0xFFFF:0xFFFF = 0x10FFEF), so a
   wild pointer left over from the 16-bit build reads garbage instead of
   walking off the buffer. */
#define SS_SEG_BYTES (0x110000u)

#define SS_SEG_ADDR(seg) ((uint32)(seg) << 4)
#define SS_PTR(t, seg, off) \
	((t *)(ss_arena + ((uint32)(seg) << 4) + (uint32)(off)))

/* First paragraph handed out by startup_checks().  Leaves the low part of the
   arena unused so a zero segment stays obviously invalid. */
#define SS_ARENA_START_PARA 0x0800

/* ---- lifetime ---- */
void ss_init(int argc, char **argv);
void ss_shutdown(void);

/* Advance event handling and the 60 Hz SHADOW counters. */
void ss_pump(void);
/* Block until the next 1/60 s frame boundary. */
void ss_wait_frame(void);
/* Wait until the shadow tick counter has advanced by `ticks`. */
void ss_wait_ticks(int32 ticks);

/* ---- video ---- */
extern uint8 ss_screen[SS_SCREEN_W * SS_SCREEN_H];   /* 4-bit colour indices */
void ss_present(void);
/* Compose the game's current display page and present it (used by the
   screenshot hook when the game is not flipping pages). */
void ss_force_present(void);
/* Copy a 320x200 8-bit palette-indexed image into the current display page. */
void ss_blit_indexed_screen(const unsigned char *pixels);
/* Present the current display page (not the back page). */
void ss_present_current_page(void);
/* Re-draw the mouse pointer after a full-page repaint. */
void ss_cursor_restamp(void);
/* Restore the startup display-page state after the title screen. */
void ss_reset_pages(void);
void ss_set_dac(int index, int r, int g, int b);
int  ss_quit_requested(void);

/* Port settings, changed from the title Options screen. */
#define SS_DIFF_EASY 0
#define SS_DIFF_NORMAL 1
#define SS_DIFF_HARD 2
extern int ss_crt_filter;      /* 0/1 scanline filter */
extern int ss_music_volume;    /* 0..100 */
extern int ss_sfx_volume;      /* 0..100 */
extern int ss_difficulty;      /* SS_DIFF_* */
int ss_scale_monster_damage(int amount);
int ss_score_percent(void);
int ss_sound_is_music(void);

/* Requests from the title menu. */
extern int ss_pending_load;
extern int ss_pending_debug;
extern int ss_debug_map;
extern int ss_debug_world_x;
extern int ss_debug_world_y;
extern int ss_debug_monster;   /* actor kind 5..32, or -1 */
extern int ss_debug_item;      /* item id, or -1 */
int ss_show_title(void);

/* ---- files ---- */
/* Load `name` into `dest_para`; returns the byte count, 0 when missing. */
int32 ss_load_asset(const char *name, uint16 dest_para);
/* Resolve a DOS-style name (case-insensitive, 8.3) to a path in the game dir. */
int  ss_path_for(const char *dos_name, char *out, int out_size);

/* ---- input (used by the SHADOW services) ---- */
int16 ss_key_poll(void);      /* 0, or (scancode << 8) | ascii */
int16 ss_key_wait(void);      /* blocking ASCII code */
/* Current pointer position, without consuming the click latch.  Used by the
   real-time cursor redraw between game ticks. */
void  ss_mouse_peek(uint16 *x, uint16 *y);
/* Redraw/present the deferred pointer if it moved; called from the frame
   wait so the cursor stays real-time even when the game logic is at 24 Hz. */
void  ss_cursor_tick(void);
/* Held arrow/numpad direction for the port's keyboard walking: 0 = up,
   clockwise to 7 = up-left, or -1 when nothing is held. */
int16 ss_direction_step(void);
void  ss_mouse_read(uint16 *x, uint16 *y, uint16 *buttons);
int16 ss_take_double_click(int16 button);
void  ss_set_mouse(int16 x, int16 y);

/* ---- sound ---- */
void ss_sound_init(void);
void ss_sound_shutdown(void);
void ss_internal_outport(uint16 port, uint16 value);

/* Command line handling: the port appends a mode letter when none is given. */
char **ss_build_argv(int argc, char **argv, int *out_argc);

#endif
