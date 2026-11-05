/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Title, Options and Debug menus.  These run after the game's assets and text
  stream are ready but before the new-game map setup, so they can use the
  original text/framebuffer services and the load dialog.
*/

#include "names.h"
#include "ids.h"
#include "shadow.h"
#include "game.h"
#include "structs.h"

#ifdef SS_SDL

#include "ss_platform.h"
#include "ss_music.h"
#include "ss_title_data.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TITLE_NORMAL  0xFF0F
#define TITLE_HILITE  0xFF0E
#define TITLE_DIM     0xFF07

enum {
	TITLE_NEW = 0,
	TITLE_LOAD,
	TITLE_OPTIONS,
	TITLE_DEBUG,
	TITLE_EXIT
};

static uint8 ss_title_pixels[SS_SCREEN_W * SS_SCREEN_H];

static void title_decode_image(void)
{
	unsigned i = 0, r = 0, n = (unsigned)sizeof(ss_title_rle);

	while (r + 1 < n && i < sizeof(ss_title_pixels)) {
		unsigned count = ss_title_rle[r++];
		unsigned value = ss_title_rle[r++];

		while (count-- != 0 && i < sizeof(ss_title_pixels))
			ss_title_pixels[i++] = (uint8)value;
	}
}

static void title_set_palette(void)
{
	int i;

	for (i = 0; i < 16; i++) {
		ss_set_dac(i,
		           (int)ss_title_palette[i * 3 + 0] >> 2,
		           (int)ss_title_palette[i * 3 + 1] >> 2,
		           (int)ss_title_palette[i * 3 + 2] >> 2);
	}
}

static void title_draw_image(void)
{
	title_decode_image();
	ss_blit_indexed_screen(ss_title_pixels);
}

static void title_clear(void)
{
	(void)shadow_region_clear(0, 0, 0, SS_SCREEN_W / 8, SS_SCREEN_H);
}

static void title_text(int16 col, int16 row, const char *s, int16 attr)
{
	(void)shadow_text_draw(attr, col, row, s);
}

static void title_present(void)
{
	/* The title repaints the whole page each frame, which wipes the game's
	   deferred pointer; stamp it back on top before showing the frame. */
	ss_cursor_restamp();
	ss_present_current_page();
}

static void title_line(int16 x, int16 y, int16 w)
{
	int16 i;
	for (i = 0; i < w; i++)
		title_text((int16)(x + i), y, "-", TITLE_DIM);
}

/* Row hit-test.  Rows are `step` pixels apart and their boxes never overlap. */
static int16 title_mouse_row(int16 base, int16 count, int16 step)
{
	int16 i;

	if ((int16)g_mouse_x >= 160)
		return -1;
	for (i = 0; i < count; i++) {
		int16 y = (int16)(base + i * step);
		if ((int16)g_mouse_y >= y - 1 && (int16)g_mouse_y < y + step - 1)
			return i;
	}
	return -1;
}

static int16 title_poll_key(void)
{
	int16 word = shadow_poll();
	int16 scan = (int16)((word >> 8) & 0xff);
	int16 key = (int16)(word & 0xff);

	if (scan == 0x48 || scan == 0x50 || scan == 0x4b || scan == 0x4d ||
	    scan == 0x49 || scan == 0x51 || scan == 0x47 || scan == 0x4f)
		return scan;
	return (key == KEY_ENTER || key == KEY_ESC || key >= 0x20) ? key : 0;
}

static int16 title_option_menu(void)
{
	int16 sel = 0;
	int16 key, row;
	int16 count = 5;

	for (;;) {
		title_clear();
		title_text(2, 20, "Options", TITLE_HILITE);
		title_line(2, 30, 18);
		{
			char buf[48];
			sprintf(buf, "CRT Filter:       %s", ss_crt_filter ? "On" : "Off");
			title_text(3, 52, buf, sel == 0 ? TITLE_HILITE : TITLE_NORMAL);
			sprintf(buf, "Music Volume:     %d%%", ss_music_volume);
			title_text(3, 62, buf, sel == 1 ? TITLE_HILITE : TITLE_NORMAL);
			sprintf(buf, "SFX Volume:       %d%%", ss_sfx_volume);
			title_text(3, 72, buf, sel == 2 ? TITLE_HILITE : TITLE_NORMAL);
			sprintf(buf, "Difficulty:       %s",
			        ss_difficulty == SS_DIFF_EASY ? "Easy" :
			        ss_difficulty == SS_DIFF_HARD ? "Hard" : "Normal");
			title_text(3, 82, buf, sel == 3 ? TITLE_HILITE : TITLE_NORMAL);
		}
		title_text(3, 102, "Back", sel == 4 ? TITLE_HILITE : TITLE_NORMAL);
		title_text(2, 180, "Left/Right changes. Esc returns.", TITLE_DIM);
		title_present();

		if (mouse_button_read() != 0) {
			row = title_mouse_row(52, 4, 10);
			if (row < 0 && (int16)g_mouse_y >= 101 && (int16)g_mouse_y < 110)
				row = 4;
			if (row >= 0) {
				sel = row;
				while (mouse_button_read() != 0)
					;
				if (row == 4)
					return 0;
			}
			continue;
		}
		key = title_poll_key();
		if (key == 0) {
			(void)mouse_poll_moved(1);
			continue;
		}
		if (key == KEY_ESC)
			return 0;
		if (key == 0x48) {
			if (--sel < 0) sel = count - 1;
		} else if (key == 0x50) {
			if (++sel >= count) sel = 0;
		} else if (key == 0x4b) {
			if (sel == 0) ss_crt_filter = !ss_crt_filter;
			else if (sel == 1) { if ((ss_music_volume -= 5) < 0) ss_music_volume = 0; }
			else if (sel == 2) { if ((ss_sfx_volume -= 5) < 0) ss_sfx_volume = 0; }
			else if (sel == 3) { if (--ss_difficulty < 0) ss_difficulty = SS_DIFF_HARD; }
		} else if (key == 0x4d) {
			if (sel == 0) ss_crt_filter = !ss_crt_filter;
			else if (sel == 1) { if ((ss_music_volume += 5) > 100) ss_music_volume = 100; }
			else if (sel == 2) { if ((ss_sfx_volume += 5) > 100) ss_sfx_volume = 100; }
			else if (sel == 3) { if (++ss_difficulty > SS_DIFF_HARD) ss_difficulty = SS_DIFF_EASY; }
		} else if (key == KEY_ENTER) {
			if (sel == 0) ss_crt_filter = !ss_crt_filter;
			else if (sel == 3) {
				if (++ss_difficulty > SS_DIFF_HARD)
					ss_difficulty = SS_DIFF_EASY;
			} else if (sel == 4) {
				return 0;
			}
		}
	}
}

static const char *title_map_name(int16 rec)
{
	switch (rec) {
	case MAP_SETTINGS_SCENE: return "Settings scene";
	case MAP_START_WEST: return "Start (west)";
	case MAP_NEIDAR_VILLAGE: return "Neidar village";
	case MAP_FOOD_GROVE: return "Food grove";
	case MAP_START_EAST: return "Start (east)";
	case MAP_STONE_FORT: return "Stone Fort";
	case MAP_AGHAR_THRONE: return "Aghar throne";
	case MAP_DUSK_EYE: return "Dusk Eye";
	case MAP_DRAGON_ATTACK: return "Dragon attack";
	case MAP_NEIDAR_AUDIENCE: return "Neidar audience";
	case MAP_WINDOW_BOOST: return "Window boost";
	case MAP_OVERWORLD_GATE: return "Overworld gate";
	case MAP_HIDDEN_FOOD_STORE: return "Hidden food store";
	case MAP_DRAGON_RECORD: return "Dragon record";
	case MAP_SCENE_DRAGON_AFTERMATH: return "Dragon aftermath";
	case MAP_TERRAIN_SPRITE_A: return "Terrain sprite A";
	case MAP_BLEAK_HILLS: return "Bleak Hills";
	case MAP_DRY_PLAINS: return "Dry Plains";
	case MAP_TERRAIN_SPRITE_B: return "Terrain sprite B";
	case MAP_KHAROLIS_FOOTHILLS: return "Kharolis foothills";
	case MAP_KHAROLIS_MOUNTAINS: return "Kharolis mountains";
	case MAP_MOUNTAIN_LAKE: return "Mountain lake / swamp gate";
	case MAP_FRUIT_FOREST: return "Fruit forest";
	case MAP_TERRAIN_SPRITE_C: return "Terrain sprite C";
	case MAP_N_EVENT_RECORD: return "N event";
	case MAP_WIZARD_EVENT: return "Wizard event";
	default: return 0;
	}
}

/* Short label for a map record: the curated name when one is known, else the
   first clause of the game's own terrain description for that record.  The
   text resources are cached once so the list does not hit the file per row. */
static char g_title_map_labels[MAP_RECORD_COUNT + 1][30];
static int g_title_map_labels_ready;

static void title_build_map_labels(void)
{
	int16 rec;

	if (g_title_map_labels_ready)
		return;
	g_title_map_labels_ready = 1;
	for (rec = 1; rec <= MAP_RECORD_COUNT; rec++) {
		const char *name = title_map_name(rec);
		char *out = g_title_map_labels[rec];
		int16 n = 0;

		if (name != 0) {
			while (name[n] != 0 && n < 28) {
				out[n] = name[n];
				++n;
			}
			out[n] = 0;
			continue;
		}
		text_resource_read(rec);
		{
			const char *p = (const char *)&g_credits;

			while (*p == ' ')
				++p;
			while (p[n] != 0 && p[n] != '\r' && p[n] != '\n' && n < 28) {
				unsigned char c = (unsigned char)p[n];

				out[n] = (c >= 0x20 && c < 0x7f) ? (char)c : ' ';
				++n;
			}
			while (n > 0 && out[n - 1] == ' ')
				--n;
			if (n > 3 && n == 28) {
				while (n > 3 && out[n - 1] != ' ')
					--n;
				while (n > 0 && out[n - 1] == ' ')
					--n;
			}
			out[n] = 0;
		}
		if (out[0] == 0)
			sprintf(out, "Map %02d", (int)rec);
	}
}

static int16 title_choose_map(void)
{
	int16 sel = 0, top = 0, key, row;
	int16 count = MAP_RECORD_COUNT;
	const int16 visible = 16;

	title_build_map_labels();
	for (;;) {
		int16 i;
		char buf[48];

		title_clear();
		title_text(2, 12, "Warp to map", TITLE_HILITE);
		title_line(2, 22, 28);
		if (sel < top)
			top = sel;
		if (sel >= top + visible)
			top = (int16)(sel - visible + 1);
		for (i = 0; i < visible; i++) {
			int16 rec = (int16)(top + i + 1);
			if (rec > count)
				break;
			sprintf(buf, "%c %02d  %s", rec == sel + 1 ? '>' : ' ', rec,
			        g_title_map_labels[rec]);
			title_text(2, (int16)(34 + i * 9), buf,
			           rec == sel + 1 ? TITLE_HILITE : TITLE_NORMAL);
		}
		title_text(2, 184, "Up/Down. Enter starts. Esc back.", TITLE_DIM);
		title_present();

		if (mouse_button_read() != 0) {
			row = title_mouse_row(34, visible, 9);
			if (row >= 0) {
				sel = (int16)(top + row);
				while (mouse_button_read() != 0)
					;
			}
			continue;
		}
		key = title_poll_key();
		if (key == 0) {
			(void)mouse_poll_moved(1);
			continue;
		}
		if (key == KEY_ESC)
			return 0;
		if (key == 0x48) { if (--sel < 0) sel = count - 1; }
		else if (key == 0x50) { if (++sel >= count) sel = 0; }
		else if (key == 0x49) { sel = (int16)(sel - visible); if (sel < 0) sel = 0; }
		else if (key == 0x51) { sel = (int16)(sel + visible); if (sel >= count) sel = count - 1; }
		else if (key == 0x47) sel = 0;
		else if (key == 0x4f) sel = (int16)(count - 1);
		else if (key == KEY_ENTER) {
			ss_debug_map = (int16)(sel + 1);
			ss_pending_debug = 1;
			return 1;
		}
	}
}

static int16 title_read_number(const char *prompt, int16 min, int16 max, int16 *out)
{
	char buf[8];
	int16 n = 0;
	int16 key;

	buf[0] = 0;

	for (;;) {
		title_clear();
		title_text(2, 40, prompt, TITLE_HILITE);
		title_text(2, 60, "Enter a number, then Enter. Esc cancels.", TITLE_NORMAL);
		title_text(2, 80, buf, TITLE_HILITE);
		title_present();
		key = title_poll_key();
		if (key == 0) {
			(void)mouse_poll_moved(1);
			continue;
		}
		if (key == KEY_ESC)
			return 0;
		if (key == KEY_ENTER) {
			if (n >= min && n <= max) {
				*out = n;
				return 1;
			}
			continue;
		}
		if (key >= '0' && key <= '9' && n < 100)
			n = (int16)(n * 10 + (key - '0'));
		else if (key == 8) {
			n = (int16)(n / 10);
		}
		if (n > 999)
			n = 999;
		if (n == 0)
			buf[0] = 0;
		else
			sprintf(buf, "%d", (int)n);
	}
}

static int16 title_world_picker(void)
{
	int16 x = 0, y = 0;

	if (!title_read_number("World map X (0-19)", 0, 19, &x))
		return 0;
	if (!title_read_number("World map Y (0-34)", 0, 34, &y))
		return 0;
	ss_debug_world_x = x;
	ss_debug_world_y = y;
	ss_pending_debug = 1;
	return 1;
}

static const char *g_title_monsters[28] = {
	"Baaz", "Bozak", "Kapak", "Aurak", "Sivak", "Spider",
	"Hobgoblin", "Skeleton", "Troll", "Death Knight", "Zombie", "Wight",
	"Ghoul", "Ghost", "Giant Wasp", "Ogre", "Aghar", "Dwarf",
	"Golem", "Hydra", "Lizard", "Slig", "Spectral", "Dwarves",
	"Lich", "Red Dragon", "Black Dragon", "Green Dragon"
};

static int16 title_choose_monster(void)
{
	int16 sel = 0, top = 0, key, row;
	const int16 count = 28;
	const int16 visible = 16;

	for (;;) {
		int16 i;
		char buf[40];

		title_clear();
		title_text(2, 12, "Spawn monster", TITLE_HILITE);
		title_line(2, 22, 26);
		if (sel < top) top = sel;
		if (sel >= top + visible) top = (int16)(sel - visible + 1);
		for (i = 0; i < visible; i++) {
			int16 k = (int16)(top + i);
			if (k >= count)
				break;
			sprintf(buf, "%c %-16s", k == sel ? '>' : ' ', g_title_monsters[k]);
			title_text(2, (int16)(34 + i * 9), buf,
			           k == sel ? TITLE_HILITE : TITLE_NORMAL);
		}
		title_text(2, 184, "Up/Down. Enter queues. Esc back.", TITLE_DIM);
		title_present();
		if (mouse_button_read() != 0) {
			row = title_mouse_row(34, visible, 9);
			if (row >= 0) {
				sel = (int16)(top + row);
				while (mouse_button_read() != 0)
					;
			}
			continue;
		}
		key = title_poll_key();
		if (key == 0) { (void)mouse_poll_moved(1); continue; }
		if (key == KEY_ESC) return 0;
		if (key == 0x48) { if (--sel < 0) sel = count - 1; }
		else if (key == 0x50) { if (++sel >= count) sel = 0; }
		else if (key == 0x49) { sel = (int16)(sel - visible); if (sel < 0) sel = 0; }
		else if (key == 0x51) { sel = (int16)(sel + visible); if (sel >= count) sel = count - 1; }
		else if (key == KEY_ENTER) {
			ss_debug_monster = (int16)(5 + sel);
			ss_pending_debug = 1;
			return 1;
		}
	}
}

static const struct { uint16 id; const char *name; } g_title_items[] = {
	{ 0x02, "Staff of Magi" }, { 0x03, "Spear" }, { 0x04, "Longsword" },
	{ 0x05, "Club +2" }, { 0x06, "Hoopak" }, { 0x07, "Hoopak +2" },
	{ 0x08, "Two-handed sword +3" }, { 0x09, "Longbow" },
	{ 0x0a, "Longsword +1" }, { 0x0b, "Hand axe +1" },
	{ 0x0c, "Longsword +2" }, { 0x0d, "Mace +2" }, { 0x0e, "Arrows" },
	{ 0x0f, "Arrows +2" }, { 0x10, "Sling stones +1" },
	{ 0x11, "Throwing axe" }, { 0x12, "Throwing axe +3" },
	{ 0x13, "Wyrmslayer +3" }, { 0x14, "Fireball item" },
	{ 0x22, "Potion" }, { 0x23, "Chest" }, { 0x25, "Web" },
	{ 0x27, "Healing herbs" }, { 0x28, "Wand" }, { 0x29, "Chain mail" },
	{ 0x2b, "Sack" }, { 0x2c, "Ring" }, { 0x2d, "Amulet" },
	{ 0x2e, "Torch" }, { 0x2f, "Key" }, { 0x30, "Staff" },
	{ 0x31, "Ring" }, { 0x33, "Food" }, { 0x36, "Chest" },
	{ 0x46, "Bread" }, { 0x48, "Gem" }, { 0x49, "Necklace" },
	{ 0x4a, "Shield" }, { 0x4b, "Statue" }, { 0x4c, "Harp" }
};

static int16 title_choose_item(void)
{
	int16 sel = 0, top = 0, key, row;
	int16 count = (int16)(sizeof(g_title_items) / sizeof(g_title_items[0]));
	int16 visible = 16;

	for (;;) {
		int16 i;
		char buf[44];
		title_clear();
		title_text(2, 12, "Spawn item", TITLE_HILITE);
		title_line(2, 22, 26);
		if (sel < top) top = sel;
		if (sel >= top + visible) top = (int16)(sel - visible + 1);
		for (i = 0; i < visible; i++) {
			int16 k = (int16)(top + i);
			if (k >= count)
				break;
			sprintf(buf, "%c %02X  %s", k == sel ? '>' : ' ',
			        g_title_items[k].id, g_title_items[k].name);
			title_text(2, (int16)(34 + i * 9), buf,
			           k == sel ? TITLE_HILITE : TITLE_NORMAL);
		}
		title_text(2, 184, "Up/Down. Enter queues. Esc back.", TITLE_DIM);
		title_present();
		if (mouse_button_read() != 0) {
			row = title_mouse_row(34, visible, 9);
			if (row >= 0) {
				sel = (int16)(top + row);
				while (mouse_button_read() != 0)
					;
			}
			continue;
		}
		key = title_poll_key();
		if (key == 0) { (void)mouse_poll_moved(1); continue; }
		if (key == KEY_ESC) return 0;
		if (key == 0x48) { if (--sel < 0) sel = count - 1; }
		else if (key == 0x50) { if (++sel >= count) sel = 0; }
		else if (key == 0x49) { sel = (int16)(sel - visible); if (sel < 0) sel = 0; }
		else if (key == 0x51) { sel = (int16)(sel + visible); if (sel >= count) sel = count - 1; }
		else if (key == KEY_ENTER) {
			ss_debug_item = (int16)g_title_items[sel].id;
			ss_pending_debug = 1;
			return 1;
		}
	}
}

static int16 title_debug_menu(void)
{
	int16 sel = 0;
	int16 key, row;
	int16 count = 5;

	for (;;) {
		char buf[64];
		title_clear();
		title_text(2, 12, "Debug", TITLE_HILITE);
		title_line(2, 22, 24);
		title_text(3, 44, "Warp to map...", sel == 0 ? TITLE_HILITE : TITLE_NORMAL);
		title_text(3, 54, "Spawn heroes at world map...", sel == 1 ? TITLE_HILITE : TITLE_NORMAL);
		title_text(3, 64, "Spawn monster...", sel == 2 ? TITLE_HILITE : TITLE_NORMAL);
		title_text(3, 74, "Spawn item...", sel == 3 ? TITLE_HILITE : TITLE_NORMAL);
		title_text(3, 94, "Back", sel == 4 ? TITLE_HILITE : TITLE_NORMAL);
		sprintf(buf, "Map %d  World %d,%d",
		        ss_debug_map, ss_debug_world_x, ss_debug_world_y);
		title_text(2, 166, buf, TITLE_DIM);
		sprintf(buf, "Monster %d  Item %d", ss_debug_monster, ss_debug_item);
		title_text(2, 176, buf, TITLE_DIM);
		title_text(2, 186, "Apply on NG; warp to see spawns.", TITLE_DIM);
		title_present();
		{
			int16 activate = 0;

			if (mouse_button_read() != 0) {
				row = title_mouse_row(44, 4, 10);
				if (row < 0 && (int16)g_mouse_y >= 93 && (int16)g_mouse_y < 102)
					row = 4;
				if (row < 0)
					continue;
				sel = row;
				while (mouse_button_read() != 0)
					;
				activate = 1;
			} else {
				key = title_poll_key();
				if (key == 0) {
					(void)mouse_poll_moved(1);
					continue;
				}
				if (key == KEY_ESC)
					return 0;
				if (key == 0x48) {
					if (--sel < 0)
						sel = count - 1;
					continue;
				}
				if (key == 0x50) {
					if (++sel >= count)
						sel = 0;
					continue;
				}
				if (key != KEY_ENTER)
					continue;
				activate = 1;
			}

			if (!activate)
				continue;
			switch (sel) {
			case 0: (void)title_choose_map(); break;
			case 1: (void)title_world_picker(); break;
			case 2: (void)title_choose_monster(); break;
			case 3: (void)title_choose_item(); break;
			case 4: return 0;
			}
		}
	}
}

int16 title_menu(void)
{
	static const char *items[5] = {
		"New Game", "Load", "Options", "Debug", "Exit"
	};
	int16 sel = 0;
	int16 key, row;

	ss_music_play(SS_MUSIC_TITLE);
	title_set_palette();
	/* Title screens draw directly; keep the deferred-present page identical
	   to the draw page so cursors and flush presents show the same content. */
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	for (;;) {
		int16 i;
		title_draw_image();
		for (i = 0; i < 5; i++) {
			char buf[24];
			sprintf(buf, "%c %s", i == sel ? '>' : ' ', items[i]);
			title_text(2, (int16)(84 + i * 12), buf,
			           i == sel ? TITLE_HILITE : TITLE_NORMAL);
		}
		title_text(2, 178, "Arrow keys / mouse. Enter selects.", TITLE_DIM);
		title_present();

		if (mouse_button_read() != 0) {
			row = title_mouse_row(84, 5, 12);
			if (row >= 0) {
				sel = row;
				while (mouse_button_read() != 0)
					;
			} else {
				continue;
			}
		} else {
			key = title_poll_key();
			if (key == 0) { (void)mouse_poll_moved(1); continue; }
			if (key == KEY_ESC) {
				terminate_callback();
				return -1;
			}
			if (key == 0x48) { if (--sel < 0) sel = 4; continue; }
			if (key == 0x50) { if (++sel >= 5) sel = 0; continue; }
			if (key != KEY_ENTER) continue;
		}

		switch (sel) {
		case TITLE_NEW:
			set_dac_palette();
			return 0;
		case TITLE_LOAD:
			set_dac_palette();
			if (save_load_dialog() != 0)
				return 1;
			title_set_palette();
			break;
		case TITLE_OPTIONS:
			(void)title_option_menu();
			title_set_palette();
			break;
		case TITLE_DEBUG:
			(void)title_debug_menu();
			title_set_palette();
			break;
		case TITLE_EXIT:
			terminate_callback();
			return -1;
		}
	}
}

void title_apply_debug(void)
{
	struct actor_rec *hero = &g_actor_table[0];
	int16 i, cell;

	if (ss_debug_map > 0) {
		g_pending_terrain = (int16)ss_debug_map;
		g_overworld_phase = 1;
		terrain_change();
	} else if (ss_debug_world_x >= 0 && ss_debug_world_x < TERRAIN_WIDTH &&
	           ss_debug_world_y >= 0 && ss_debug_world_y < TERRAIN_HEIGHT) {
		PARTY_REC(0)->grid_x = (uint16)ss_debug_world_x;
		PARTY_REC(0)->grid_y = (uint16)ss_debug_world_y;
		cell = (int16)(ss_debug_world_y * TERRAIN_WIDTH + ss_debug_world_x);
		g_map_mode_char = g_terrain_grid[cell];
		g_pending_terrain = 0;
		g_overworld_phase = 1;
		tile_set_position();
		draw_map_view();
		g_overworld_phase = 1;
	}

	if (ss_debug_monster >= 5 && ss_debug_monster <= 32) {
		(void)spawn_descriptor((int16)(ss_debug_monster - MONSTER_STATE_OFFSET),
		                       (int16)(hero->x + 16), (int16)hero->y);
	}
	if (ss_debug_item >= 0) {
		for (i = 0; i < SITE_COUNT; i++) {
			struct site_rec *s = &g_site_table[i];
			if (s->id != 0)
				continue;
			s->id = (uint16)ss_debug_item;
			s->map = g_map_mode_char;
			s->x = (int16)(hero->x + 48);
			s->y = (int16)hero->y;
			s->mode_value = 0;
			s->weight = 8;
			s->child = 0;
			s->flags = 0x81;
			break;
		}
	}
	ss_pending_debug = 0;
}

#endif /* SS_SDL */
