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

/* 0xD35 */

void draw_menubar(void)
{
	struct rect_rec *rec;
	int16 i;
	int16 accum = 0;
	int16 n;

	shadow_video_page(SHADOW_PAGE_MAP);				/* D40-D47 */
	shadow_region_clear(g_window_index, 0, 0, SCREEN_WIDTH_BYTES, SCREEN_TOP_MARGIN);	/* D4A-D5D */
	rec = RECT(g_rect_menubar);
	for (i = 0; i != 3; i++, rec++) {	/* D65-D77 */
		n = (int16)(int16)rec->state;		/* D82-D85 */
		rec->x = (uint16)accum;		/* D79-D7F */
		rec->w = (uint16)((text_length(g_menubar_labels[n]) << 3) + 0x10);
							/* D87-D9D */
		accum += rec->w;			/* DA0-DA6 */
		shadow_text(TEXT_COLOUR_HIGHLIGHT, (int16)(int16)rec->x >> 3, rec->y,
			  (uint16)g_menubar_labels[n]);	/* DA9-DCC */
	}
	sprite_draw_current(SPRITE_MENU_RULE, SPRITE_DRAW_SOLID, 0x1c, 1);			/* DC9-DDC */
	shadow_video_page(SHADOW_PAGE_PREVIOUS);				/* DDF-DE6 */
}

/* 0xCA3 */

void draw_button_caption(struct rect_rec *rec, int16 n)
{
	int16 i;

	for (i = 0; i != n; i++, rec++)		/* CB0-CBD */
		sprite_draw_dispatch((int16)rec->x - 5, (int16)rec->y,
			 SPRITE_BUTTON_MARK);
}

extern int16 draw_text_window(int16 x, int16 y, int16 w, int16 h);		/* 0x4693 */
extern int16 mouse_button_read(void);				/* 0x4A36 */

/* 0x4918 */

int16 message_box(int16 box_style, int16 id)
{
	char *m = g_messages[id];
	int16 w, h;
	int16 x;

	(void)box_style;
	shadow_video_flags(SHADOW_BLIT_FLUSH);					/* 491E-4925 */
	text_measure(m, &w, &h);					/* 4928-493C */
	x = ((SCREEN_WIDTH - (w << 3)) >> 1) / 8;			/* 493F-495A */
	draw_text_window(x, 60, w, h);					/* 495D-496A */
	shadow_text_draw(TEXT_COLOUR_NORMAL, x, 0x3c, m);			/* 4970-4984 */
	while (mouse_button_read() != 0) {				/* 498A-4991 */
	}
	while (mouse_button_read() == 0) {				/* 4993-4998 */
		if ((shadow_poll() & 0xff) == KEY_ESC)
			break;
		mouse_poll_moved(1);					/* 49A3-49AA */
	}
	while (mouse_button_read() != 0) {				/* 49AF-49B6 */
	}
	return 0;
}

/* DGROUP 0xb4e */

uint8 g_rect_menubar[50] = {
	0,0, 0,0, 0,0, 0,0, 0,8,
	1,0, 0,0, 0,0, 0,0, 0,8,
	2,0, 0,0, 0,0, 0,0, 0,8,
	3,0, 0,0, 0,0, 0,0, 0,8,
	4,0, 0,0, 0,0, 0,0, 0,8
};
/* DGROUP 0x2152 */

struct game_state g_game_state = {
	{
		9, 26, 9, 27, 9, 28, 0, 0, 11, 1, 11, 2,
		12, 2, 13, 2, 14, 3, 14, 4, 15, 5, 14, 6,
		15, 7, 14, 8, 14, 9, 14, 10, 15, 11, 15, 12,
		16, 13, 16, 14, 16, 15, 15, 16, 14, 16, 14, 17,
		13, 18, 13, 19, 12, 20, 12, 21, 11, 22, 11, 23,
		10, 23, 9, 24, 0, 0, 12, 1, 13, 1, 14, 1,
		15, 1, 16, 1, 17, 1, 17, 2, 18, 3, 18, 4,
		18, 5, 18, 6, 18, 7, 18, 8, 18, 9, 18, 10,
		18, 11, 17, 12, 18, 13, 18, 14, 18, 15, 18, 16,
		18, 17, 18, 18, 18, 19, 18, 20, 18, 21, 18, 22,
		18, 23, 17, 23, 16, 23, 15, 23, 14, 24, 13, 24,
		13, 25, 12, 26, 12, 27, 11, 28, 10, 28, 9, 28,
	},
	{
		0, 4876, 516, 127, 32512, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		4, 0, 1, 0, 1, 4, 0, 2, 0, 1, 1,
	},
	0,
	{
		4, 0, 1, 0, 1, 0, 0, 3, 2, 1, 1,
	},
	0,
	{
		1, 2, 1, 0, 0, 1, 1, 1,
	},
	{
		1, 2, 0, 2,
	},
	0,
	0,
	0,
	0,
	{
		0, 0, 0, 0,
	},
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	4,
	0,
	0,
	10,
	0,
	30,
	0,
	0,
	0,
	{
		11, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0,
	},
	0,
	{
		0, 0, 0, 0, 0, 0,
	},
	{
		3, 0, 0, 0, 8, 0, 32, 0, 32, 0, 1, 0,
		32, 1, 8, 0, 32, 0, 32, 0, 0, 0, 0, 0,
		168, 0, 32, 0, 32, 0, 4, 0, 32, 1, 168, 0,
		32, 0, 32, 0, 2, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0,
		0, 0,
	},
};	/* DGROUP 0x2152 */
uint16 g_menu_toggle = 1;    /* 0x98ca */              /* 0x98CA */
uint8 g_action_freeze;         /* 0x9660 */

extern int16 shadow_screen_refresh(void);			/* 0xD403 */
extern int16 shadow_getkey_noecho(void);			/* 0xD36C */

/* 0x45F6 */

/* 0x45F6 */
int16 joystick_calibrate(void)
{
	int16 w, h, cx;

	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);						/* 45FC-4602 */

	text_measure("IBM joystick Port 'A'\rTANDY Joystick LEFT socket\rCenter joystick then press\rENTER key\rPress ESC to abort", &w, &h);				/* 4605-4614 */
	w += 2;
	h += 2;

	cx = (SCREEN_WIDTH - (w * 8)) >> 1;
	cx = cx / 8;

	(void)draw_text_window(cx, 24, w, h);					/* 463D-464D */

	(void)shadow_text_draw(TEXT_COLOUR_HEADING, cx, 24, "Joystick Calibration:");		/* 4650-4662 */
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, cx, 0x28, "IBM joystick Port 'A'\rTANDY Joystick LEFT socket\rCenter joystick then press\rENTER key\rPress ESC to abort");		/* 4665-4677 */

	if (shadow_getkey_noecho() == KEY_ENTER)				/* 467A-4680 */
		(void)shadow_screen_refresh();

	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);						/* 4685-468C */
	return 0;
}

/* 0x2A18 */

int16 save_load_dialog(void)
{
	if (save_load_panel(1) == 0)
		return 0;
	effect_clear_flags();
	load_pane_maps();
	rebuild_map_tail();
	g_panel_busy = 1;
	g_overworld_hold = 0;
	g_game_end_flag = 0;
	if (g_overworld_phase == 1) {
		(void)tile_set_position();
		draw_map_view();
	} else {
		(void)map_record_show(g_map_mode_char, 0);
	}
	g_panel_busy = 0;
	return 1;
}

extern int16 save_pack(char *pattern);			/* 0x9C04 */
extern int16 save_restore(char *pattern);			/* 0x9C6A */

/* 0xA2A1 */

extern int16 mouse_button_read(void);				/* 0x4A36 */
extern int16 draw_text_window(int16 x, int16 y, int16 w, int16 h);			/* 0x4693 */
extern int16 mouse_poll_moved(int16);				/* 0x49BC */
extern int16 message_box(int16, int16);				/* 0x4918 */
extern int16 shadow_getkey_noecho(void);			/* 0xD36C */
extern void draw_button_caption(struct rect_rec *, int16);		/* 0xCA3 */
extern void ensure_text_open(void);				/* 0xA270 */

/* 0x0CCA */

int16 g_slot_rows[12][5] = {
	   {  0,   32,  58, 192, 8 },   {  1,   32,  68, 192, 8 },
	   {  2,   32,  78, 192, 8 },   {  3,   32,  88, 192, 8 },
	   {  4,   32,  98, 192, 8 },   {  5,   32, 108, 192, 8 },
	   {  6,   32, 118, 192, 8 },   {  7,   32, 128, 192, 8 },
	   {  8,   32, 138, 192, 8 },   {  9,   32, 148, 192, 8 },
	   { 10,  240,  88,  48, 8 },   { 11,  240, 118,  48, 8 }
};

/* 0x94B0 */

/* 0x94b0 */

char g_name_pool[10][25] = {
	"game a                  ",
	"  game b                ",
	"    game c              ",
	"      game d            ",
	"        game e          ",
	"          game f        ",
	"            game g      ",
	"              game h    ",
	"                game i  ",
	"                  game j"
};
char *g_slot_name[10] = {					/* 0x95b8 */
	g_name_pool[0], g_name_pool[1], g_name_pool[2], g_name_pool[3], g_name_pool[4],
	g_name_pool[5], g_name_pool[6], g_name_pool[7], g_name_pool[8],
	g_name_pool[9]
};

int16 g_save_dialog_x = 0x20;					/* 0x0ccc */
int16 g_save_dialog_y = 0x3a;					/* 0x0cce */
FILE *g_save_stream;					/* 0xa16e */
int16 g_save_slot_highlight = 0;					/* 0x95e8 */

/* 0xA2A1 */
int16 save_load_panel(int16 mode)
{
	int16 *r;
	char *namep;
	int16 xcells, ypos;
	int16 sel, key, colour;
	FILE *fp;
	char savegame_tpl[13];

	text_copy(savegame_tpl, "savegame.___");
	shadow_video_flags(SHADOW_BLIT_FLUSH);					/* A2A8-A2AF */
	if (g_disk_class_ui == 0) {
		fclose(g_text_stream);
		message_box(MSG_BOX_SAVE_PROMPT, MSG_SAVE_DISK);
	}
	shadow_video_flags(SHADOW_BLIT_FLUSH);
	g_save_stream = fp = fopen("savegame.dir", "rb");
	if (fp != 0) {
	fread((char *)g_name_pool, 0x19, 0xa, fp);
	fclose(fp);
	}

	xcells = g_save_dialog_x >> 3;				/* A30F-A316 */
	ypos = g_save_dialog_y;
	draw_text_window(xcells - 1, ypos - 26, 34, 17);

	for (sel = 0; sel != 10; sel++) {			/* A339-A34B */
	r = g_slot_rows[sel];
	shadow_region_clear(g_text_page >> 8, r[1] >> 3, r[2], r[3] >> 3, r[4]);
	colour = (sel == g_save_slot_highlight) ? TEXT_COLOUR_HIGHLIGHT : TEXT_COLOUR_NORMAL;		/* A372-A37F */
	shadow_text(colour, r[1] >> 3, r[2], (uint16)g_slot_name[sel]);
	}
	draw_button_caption(RECT(g_slot_rows[10]), 2);

	if (mode != 0) {
	shadow_text(TEXT_COLOUR_NORMAL, xcells, ypos - SAVE_DIALOG_LABEL_OFFSET, (uint16)"select a game to LOAD");
	shadow_text(TEXT_COLOUR_NORMAL, (g_slot_rows[SAVE_DIALOG_OK][1] >> 3) + 1, g_slot_rows[SAVE_DIALOG_OK][2],
		    (uint16)"LOAD");
	} else {
	shadow_text(TEXT_COLOUR_NORMAL, xcells, ypos - SAVE_DIALOG_LABEL_OFFSET, (uint16)"name your SAVE game");
	shadow_text(TEXT_COLOUR_NORMAL, (g_slot_rows[SAVE_DIALOG_OK][1] >> 3) + 1, g_slot_rows[SAVE_DIALOG_OK][2],
		    (uint16)"SAVE");
	}
	shadow_text(TEXT_COLOUR_NORMAL, (g_slot_rows[SAVE_DIALOG_CANCEL][1] >> 3) + 1, g_slot_rows[SAVE_DIALOG_CANCEL][2],
	     (uint16)"CANCEL");		/* A426-A444 */

	events:
	mouse_poll_moved(0);
	poll:
	if (mouse_button_read() != 0)
		goto mouse;				/* A450-A457 */
	mouse_poll_moved(1);
	key = shadow_poll() & LOW_BYTE_MASK;

	if (key == 0)
		goto idle;
	if (key == KEY_ESC)
		return 0;
	if (key == KEY_ENTER) {
		sel = 0xa;
		goto settled;
	}

	shadow_video_flags(SHADOW_BLIT_FLUSH);
	r = g_slot_rows[g_save_slot_highlight];
	namep = g_slot_name[g_save_slot_highlight];
	if (key < 0x20)
		key = 0x20;
	namep[sel] = (char)key;				/* A4C1-A4C9 */
	namep[sel + 1] = 0;
	shadow_region_clear(g_text_page >> 8, r[1] >> 3, r[2], r[3] >> 3, r[4]);
	sel = 1;
	edit:
	shadow_text(g_text_page, r[1] >> 3, r[2], (uint16)namep);
	key = shadow_getkey_noecho();
	if (key == KEY_ENTER || key == KEY_ESC) {
		mouse_poll_moved(0);
		goto write_dir;
	}
	if (key == 8) {
		if (sel != 0) {
			sel--;
			namep[sel] = ' ';
		}
	} else if (key >= 0x20 && sel < 24) {
		namep[sel] = (char)key;
		sel++;
		namep[sel] = 0;
	}
	goto edit;
	write_dir:
	fp = fopen("savegame.dir", "wb");
	if (fp != 0) {
		fwrite((char *)g_name_pool, 0x19, 0xa, fp);
		fclose(fp);
	}
	idle:
	goto poll;

	mouse:
	sel = rect_scan(0xc, (uint8 *)g_slot_rows);
	settled:
	while (mouse_button_read() != 0) {
	}
	shadow_video_flags(SHADOW_BLIT_FLUSH);
	if (sel == -1)
		goto events;
	if (sel < 10 && sel != g_save_slot_highlight) {		/* A5CE-A5DA */
		r = g_slot_rows[g_save_slot_highlight];
		shadow_text(TEXT_COLOUR_NORMAL, r[1] >> 3, r[2],
			    (uint16)g_slot_name[g_save_slot_highlight]);
		g_save_slot_highlight = sel;
		r = g_slot_rows[g_save_slot_highlight];
		shadow_text(TEXT_COLOUR_HIGHLIGHT, r[1] >> 3, r[2],
			    (uint16)g_slot_name[g_save_slot_highlight]);
	}

	if (sel == SAVE_DIALOG_CANCEL) {
		if (g_disk_class_ui == 0)
			ensure_text_open();
		return 0;
	}
	if (sel != SAVE_DIALOG_OK)
		goto events;
	savegame_tpl[10] = (char)(g_save_slot_highlight + 'A');
	if (mode != 0) {
	sel = save_restore(savegame_tpl);
	if (sel == 0) {
		message_box(MSG_BOX_ERROR, MSG_READ_ERR);
		sel = 0;
	}
	} else {
	sel = save_pack(savegame_tpl);
	if (sel != 0)
		message_box(MSG_BOX_ERROR, MSG_WRITE_ERR);
	}
	if (g_disk_class_ui == 0)
	ensure_text_open();
	return sel;
}

/* 0x47B2 */

extern int16 shadow_key_read(void);				/* 0xD374 */

/* 0x47B2 */
void show_text_panel(int16 a)
{
	int16 w, h;
	int16 x, y;

	text_resource_read(a);						/* 47B8-47BE */
	shadow_video_flags(SHADOW_BLIT_FLUSH);						/* 47C1-47C8 */
	text_measure((char *)&g_credits, &w, &h);				/* 47CB-47DA */

	x = ((SCREEN_WIDTH - (w << 3)) >> 1) / 8;
	y = (SCREEN_HEIGHT - (h << 3)) >> 1;

	draw_text_window(x, y, w, h);					/* 4810-481D */
	shadow_text_draw(TEXT_COLOUR_NORMAL, x, y, (char *)&g_credits);		/* 4820-4831 */

	while (mouse_button_read() != 0)					/* 4834-483B */
		;
	while (mouse_button_read() == 0) {				/* 483D-4842 */
		if ((shadow_key_read() & LOW_BYTE_MASK) == KEY_ESC)		/* 4844-484B */
			break;
		mouse_poll_moved(1);					/* 484D-4857 */
	}
	while (mouse_button_read() != 0)					/* 4859-4862 */
		;
}

#define g_hotspot_grid ((uint8 *)&g_hot_spots[1])

/* 0xB57D */

/* 0xB57D */
void layout_nine_buttons(int16 origin_x, int16 origin_y)
{
	int16 i, j;

	for (i = 0; i != 3; ++i) {				/* B584-B589 B58B-B592 */
		for (j = 0; j != 3; ++j) {		/* B594-B599 B59B-B5A2 */
			struct hotspot_rec *r = HOTSPOT(g_hotspot_grid + (i * 3 + j) * RECT_STRIDE);	/* B5A4-B5BA */

			r->x = (uint16)(origin_x + 18 * j);	/* B5BC-B5C5 */
			r->y = (uint16)(origin_y + 18 * i);	/* B5C9-B5D2 */
		}
	}
}

struct character_rec g_character_table[18] = {

	{ 0x92D0, 0, 0x04, { 13, 0 }, { 0, 0 },
	  29, 29, 0L, 0, 7, 0, 5,
	  { { 12, 12, 16, 12, 14, 17 }, 8 },
	  { { 12, 12, 16, 12, 14, 17 }, 8 }, 0, 0 },

	{ 0x92E2, 0, 0x07, { 19, 0 }, { 3, 0 },
	  55, 55, 0L, 2, 7, 0, 2,
	  { { 16, 12, 13, 12, 16, 15 }, 4 },
	  { { 16, 12, 13, 12, 16, 15 }, 4 }, 0, 0 },

	{ 0x92F2, 0, 0x04, { 7, 0 }, { 6, 0 },
	  24, 24, 0L, 4, 6, 0, 0,
	  { { 13, 9, 12, 14, 16, 11 }, 5 },
	  { { 13, 9, 12, 14, 16, 11 }, 5 }, 0, 0 },

	{ 0x9309, 0, 0x07, { 12, 0 }, { 3, 0 },
	  42, 42, 0L, 2, 7, 0, 5,
	  { { 19, 13, 14, 13, 16, 13 }, 5 },
	  { { 19, 13, 14, 13, 16, 13 }, 5 }, 0, 0 },

	{ 0x931B, 0, 0x01, { 4, 0 }, { 9, 0 },
	  52, 52, 0L, 2, 8, 0, 5,
	  { { 20, 12, 10, 17, 11, 15 }, 6 },
	  { { 20, 12, 10, 17, 11, 15 }, 6 }, 0, 1 },

	{ 0x932C, 0, 0x01, { 8, 0 }, { 0, 0 },
	  47, 47, 0L, 2, 8, 0, 5,
	  { { 17, 14, 11, 16, 12, 12 }, 5 },
	  { { 17, 14, 11, 16, 12, 12 }, 5 }, 0, 0 },

	{ 0x9342, 0, 0x0C, { 2, 0 }, { 2, 0 },
	  15, 15, 0L, 3, 5, 0, 0,
	  { { 10, 17, 14, 10, 16, 10 }, 5 },
	  { { 10, 17, 14, 10, 16, 10 }, 5 }, 0, 1 },

	{ 0x9353, 0, 0x01, { 11, 0 }, { 17, 0 },
	  60, 60, 0L, 2, 6, 0, 2,
	  { { 16, 7, 12, 18, 10, 13 }, 6 },
	  { { 16, 7, 12, 18, 10, 13 }, 6 }, 0, 1 },

	{ 0x9368, 0, 0x06, { 10, 0 }, { 0, 0 },
	  19, 19, 0L, 2, 5, 0, 8,
	  { { 12, 14, 10, 12, 16, 13 }, 2 },
	  { { 12, 14, 10, 12, 16, 13 }, 2 }, 0, 0 },

	{ 0x937B, 0, 0x01, { 10, 0 }, { 0, 0 },
	  24, 24, 0L, 2, 4, 0, 8,
	  { { 13, 15, 12, 14, 17, 16 }, 0 },
	  { { 13, 15, 12, 14, 17, 16 }, 0 }, 0, 0 },

	{ 0x93C2, 0, 0x05, { 4, 0 }, { 0, 0 },
	  28, 28, 0L, 2, 5, 0, 7,
	  { { 14, 15, 7, 8, 13, 15 }, 4 },
	  { { 14, 15, 7, 8, 13, 15 }, 4 }, 0, 0 },

	{ 0x9390, 0, 0x01, { 12, 0 }, { 0, 0 },
	  30, 30, 0L, 2, 5, 0, 2,
	  { { 14, 9, 12, 13, 16, 14 }, 8 },
	  { { 14, 9, 12, 13, 16, 14 }, 8 }, 0, 0 },

	{ 0x939F, 0, 0x04, { 13, 0 }, { 0, 0 },
	  40, 40, 0L, 2, 7, 0, 0,
	  { { 13, 14, 17, 12, 12, 16 }, 5 },
	  { { 13, 14, 17, 12, 12, 16 }, 5 }, 0, 0 },

	{ 0x93B0, 0, 0x00, { 10, 0 }, { 0, 0 },
	  27, 27, 0L, 2, 4, 0, 7,
	  { { 12, 10, 9, 15, 10, 6 }, 6 },
	  { { 12, 10, 9, 15, 10, 6 }, 6 }, 0, 0 },

	{ 0x93D8, 0, 0x01, { 4, 0 }, { 0, 0 },
	  28, 28, 0L, 2, 4, 0, 7,
	  { { 14, 13, 10, 15, 15, 11 }, 5 },
	  { { 14, 13, 10, 15, 15, 11 }, 5 }, 0, 0 },

	{ 0x93E8, 0, 0x04, { 4, 0 }, { 0, 0 },
	  21, 21, 0L, 2, 4, 0, 7,
	  { { 10, 11, 13, 14, 11, 18 }, 7 },
	  { { 10, 11, 13, 14, 11, 18 }, 7 }, 0, 0 },

	{ 0x93F8, 0, 0x00, { 10, 0 }, { 0, 0 },
	  19, 19, 0L, 2, 4, 0, 7,
	  { { 14, 9, 16, 10, 10, 14 }, 7 },
	  { { 14, 9, 16, 10, 10, 14 }, 7 }, 0, 0 },

	{ 0x9405, 0, 0x00, { 0, 0 }, { 0, 0 },
	  0, 1, 0L, 0, 0, 0, 0,
	  { { 0, 0, 0, 0, 0, 0 }, 0 },
	  { { 0, 0, 0, 0, 0, 0 }, 0 }, 32, 32 },
};

/* 0x92D0 */

struct character_name_pool {
	char goldmoon[18];	/* 0x92D0 */
	char tanis[16];		/* 0x92E2 */
	char tasslehoff[23];	/* 0x92F2 */
	char riverwind[18];	/* 0x9309 */
	char caramon[17];	/* 0x931B */
	char sturm[22];		/* 0x932C */
	char raistlin[17];	/* 0x9342 */
	char flint[21];		/* 0x9353 */
	char gilthanas[19];	/* 0x9368 */
	char laurana[21];	/* 0x937B */
	char tika[15];		/* 0x9390 */
	char elistan[17];	/* 0x939F */
	char brookland[18];	/* 0x93B0 */
	char eben[22];		/* 0x93C2 */
	char briar[16];		/* 0x93D8 */
	char locar[16];		/* 0x93E8 */
	char fizban[13];	/* 0x93F8 */
	char no_character[16];	/* 0x9405 */
	char tail[1];		/* 0x9415 */
};

struct character_name_pool g_character_name_pool = {	/* 0x92D0 */
	"      Goldmoon 7C",
	"       Tanis 7F",
	"Tasslehoff Burrfoot 6T",
	"     Riverwind 7R",
	"      Caramon 8F",
	" Sturm Brightblade 8F",
	"     Raistlin 7M",
	"  Flint Fireforge 6F",
	"   Gilthanas 5F/4M",
	" Princess Laurana 4F",
	"       Tika 5F",
	"      Elistan 7C",
	"     Brookland 5F",
	" Eben Shatterstone 5F",
	"       Briar 5F",
	"       Locar 5F",
	"      Fizban",
	"   No Character",
	"",
};

#define CHARACTER_NAME(p) \
	((char *)&g_character_name_pool + ((p)->name_offset - CHARACTER_NAME_POOL_ADDR))

/* 0x9416 */

uint16 g_record_addr[23] = {
	36938,37082,37118,37154,36974,37010,37046,37190,
	37226,37262,37334,37370,37406,37298,37442,37478,
	37514,37550,
	0,0,0,0,0,
};

/* 0x0A1A */

struct party_name_pool {
	char names[182];	/* 0x0A1A-0x0AD0 */
};

struct party_name_pool name_pool_a1a = {
	/* 0x0A1A-0x0AD0 */
	"Goldmoon 7C\0Caramon 8F\0Sturm 8F\0Raistlin 7M\0Tanis 7F\0Tasslehoff 6T\0Riverwind 7R\0Flint 6F\0Gilthanas 5F/4M\0Laurana 4F\0Tika 5F\0Elistan 7C\0Brookland 5F\0Eben 5F\0Briar 5F\0Locar 5F\0Fizban\0\0",
};

/* 0x0E7A */

const uint16 g_name_offsets[24] = {
	0x0a1a, 0x0a26, 0x0a31, 0x0a3a, 0x0a46, 0x0a4f, 0x0a5d, 0x0a6a,
	0x0a73, 0x0a83, 0x0a8e, 0x0a96, 0x0aa1, 0x0aae, 0x0ab6, 0x0abf,
	0x0ac8, 0x0acf, 0x0000, 0x0000, 0xffff, 0x0000, 0x0001, 0x0000,
};

/* 0x0F18 */

const uint8 g_order_masks[4] = { 1, 2, 4, 8 };

/* 0x0F4C..0x0F91 */

const uint8 g_spell_levels[SPELLCASTER_COUNT][12] = {	/* 0x0F4C */

	{ 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 5, 0 },

	{ 1, 1, 1, 1, 2, 2, 2, 3, 3, 4, 5, 0 },

	{ 1, 1, 1, 1, 1, 2, 2, 3, 0, 0, 0, 0 },

	{ 1, 1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0 },
};

const uint8 g_spell_caps[SPELLCASTER_COUNT][6] = {	/* 0x0F7C */

	{ 0, 5, 5, 2, 1, 1 },

	{ 0, 5, 5, 5, 1, 1 },

	{ 0, 4, 3, 2, 0, 0 },

	{ 0, 3, 2, 0, 0, 0 },
};

uint16 g_acc_lo;					/* 0xa61e */
uint16 g_acc_hi;					/* 0xa620 */

char *name_from_pool(uint16 off)
{
	return (char *)&g_party_name_pool + (off - PARTY_NAME_POOL_ADDR);
}

char *class_name_text(uint16 off)
{
	return (char *)&g_class_name_pool + (off - CLASS_NAME_POOL_ADDR);
}

char *image_text(uint16 off)
{
	if (off >= PARTY_NAME_POOL_ADDR && off < PARTY_NAME_POOL_END)
		return name_from_pool(off);
	return class_name_text(off);
}

struct character_rec *character_at(uint16 addr)
{
	return (struct character_rec *)((uint8 *)g_character_table + (addr - CHARACTER_TABLE_ADDR));
}

/* 0x2C66 */

void experience_points_panel(void)
{
	char field[14];
	int16 count = 0;
	int16 i, y, kind, subtotal;
	struct character_rec *rec;
	union dword sum_acc;
	uint16 carry, k;

	for (i = 0; i != 20; ++i) {					/* 2C7C-2CAA */
		kind = (uint8)g_hero_slots[i].state;
		if (kind >= 0 && kind <= 22)
			++count;
	}

	y = 84 - 4 * count;						/* 2CAC-2CB8 */
	draw_text_window(4, y, 32, count + 4);					/* 2CBB-2CD0 */
	(void)shadow_text_draw(TEXT_COLOUR_HEADING, 10, y, "Experience Points");		/* 2CD3-2CE2 */

	y += 4;
	g_acc_lo = 0;
	g_acc_hi = 0;						/* 2CF1-2CFD */

	for (i = 0; i != 20; ++i) {					/* 2D04-2D0D */
		kind = g_hero_slots[i].state;
		if (kind < 0 || kind > 22)
			continue;
		y += 8;
		(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 9, y, name_from_pool(g_label_offset[kind]));	/* 2D39-2D4F */
		rec = CHAR_REC(character_at(g_record_addr[kind]));		/* 2D52-2D5B */
		subtotal = (int16)(uint16)rec->experience + 1000;			/* 2D60-2D66 */
		carry = g_acc_lo;
		g_acc_lo += (uint16)subtotal;
		if (g_acc_lo < carry)
			++g_acc_hi;
		text_copy(field, "          ");				/* 2D76-2D81 */
		format_uint(subtotal, &field[7]);					/* 2D84-2D8E */
		(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 0x17, y, field);			/* 2D91-2DA3 */
	}

	y += 8;
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 9, y, " refugees");		/* 2DAD-2DBF */

	sum_acc.w[0] = 0;
	sum_acc.w[1] = 0;
	for (k = 1; k != 10; ++k) {
		if (g_party_table[k].kind != 0) {
			int16 v = (int16)(int16)PARTY_REC(k)->refugees;
			uint16 lo = (uint16)v, hi = (v < 0) ? 0xffffu : 0u;
			carry = sum_acc.w[0];
			sum_acc.w[0] += lo;
			if (sum_acc.w[0] < carry)
				sum_acc.w[1] += 1;
			sum_acc.w[1] += hi;
		}
	}

	word_mul(&sum_acc.w[0], 100u, 0u);
	carry = g_acc_lo;
	g_acc_lo += sum_acc.w[0];
	if (g_acc_lo < carry)
		sum_acc.w[1] += 1;
	g_acc_hi += sum_acc.w[1];

	text_copy(field, "          ");					/* 2E12-2E1D */
	{
		union dword t;
		t.w[0] = sum_acc.w[0];
		t.w[1] = sum_acc.w[1];
		format_ulong(t.l, &field[7]);					/* 2E20-2E2D */
	}
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 0x17, y, field);			/* 2E30-2E42 */

	y += 12;
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 9, y, "Total Points");	/* 2E49-2E58 */
	text_copy(field, "          ");					/* 2E5E-2E69 */
	{
		union dword t;
		t.w[0] = g_acc_lo;
		t.w[1] = g_acc_hi;
		format_ulong(t.l, &field[7]);					/* 2E6C-2E78 */
	}
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 20, y, field);			/* 2E7E-2E8D */
}

/* 0xCC */

/* 0x0C0 */

void draw_hero_gauge(int16 i)
{
	uint16 kind_word, kind;
	struct character_rec *rec;
	int16 delta, x, track_y, frac, colour, fill_y;
	int16 cur, max, num, an, ad, q;

	kind_word = g_hero_slots[i].state;
	kind = kind_word & 0xff;
	rec = CHAR_REC(character_at(g_record_addr[kind]));

	delta = (i & 1) ? -5 : 31;
	x = (int16)g_hero_slots[i].x + delta + 2;
	track_y = (int16)g_hero_slots[i].y + 2;	/* 118-12C */

	(void)shadow_rect_fill((uint16)x, (uint16)track_y, 4u, 28u);	/* 12F-140 */
	(void)shadow_rect_fill((uint16)(x + 1), (uint16)track_y, 4u, 28u);	/* 143-153 */

	if ((int16)kind_word > 22)							/* 159-15D */
		return;
	if (kind_word & RECT_STATE_GONE)							/* 161-166 */
		return;

	cur = (int16)(int16)rec->hp;				/* 168-16F */
	if (cur < 0) {
		cur = 0;
		rec->hp = 0;
	}

	num = 28 * cur;
	max = (int16)(int16)rec->hp_max;
	an = num < 0 ? -num : num;
	ad = max < 0 ? -max : max;
	q = an / ad;
	if ((num < 0) != (max < 0))
		q = -q;
	frac = q;									/* 183-185 */

	colour = (frac > 7) ? (int16)g_gauge_fill_long : (int16)g_gauge_fill_short;		/* 188-195 */
	fill_y = track_y + 28 - frac;						/* 198-1A1 */

	if (frac == 0)								/* 1A4-1A8 */
		return;

	(void)shadow_rect_fill((uint16)x, (uint16)fill_y, (uint16)colour, (uint16)frac);	/* 1AA-1B7 */
	(void)shadow_rect_fill((uint16)(x + 1), (uint16)fill_y, (uint16)colour, (uint16)frac);	/* 1BA-1CB */
}

/* 0x00CA */

/* 0xE8C */

/* 0x7194 */

void travelling_company_panel(void)
{
	char field[5];
	struct party_rec *rec;
	int16 i;

	draw_text_window(9, 24, 22, 5);					/* 719F-71B5 */
	sprite_draw_dispatch(0x50, 0x24, SPRITE_STATUS_REFUGEES);		/* 71B8-71C4 */
	sprite_draw_dispatch(0x88, 0x24, SPRITE_STATUS_FOOD);			/* 71C7-71D6 */
	sprite_draw_dispatch(0xba, 0x24, SPRITE_STATUS_SCORE);			/* 71D9-71E8 */

	for (i = PARTY_REFUGEES, rec = PARTY_REC(PARTY_REFUGEES); i != PARTY_FOLLOWER_LEADER; i++, rec++) {	/* 71EB-71F9 */
		if (rec->kind == 0)
			continue;						/* 7205-720A */
		if (rec->grid_x != PARTY_REC(0)->grid_x)
			continue;						/* 720D-7215 */
		if (rec->grid_y != PARTY_REC(0)->grid_y)
			continue;						/* 7218-7220 */
		shadow_region_clear(g_page_rect, 0xc, 24, 0x10, 8);	/* 7223-723A */
		if (i == 1)
			(void)shadow_text_draw(g_panel_text_attr, 0x10, 24,
				"Refugees");				/* 7243-7256 */
		else
			(void)shadow_text_draw(g_panel_text_attr, 0x10, 0x18,
				name_from_pool(g_party_name_offset[i]));	/* 725B-7273 */
		text_copy(field, "    ");			/* 7276-7284 */
		format_uint(rec->refugees, &field[3]);			/* 7287-7294 */
		(void)shadow_text_draw(g_panel_text_attr, 0xc, 0x28, field);	/* 7297-72AA */
		text_copy(field, "   ");			/* 72AD-72BB */
		format_uint(rec->food, &field[2]);			/* 72BE-72CB */
		(void)shadow_text_draw(g_panel_text_attr, 0x13, 0x28, field);	/* 72CE-72E1 */
		text_copy(field, "    ");			/* 72E4-72F2 */
		format_uint(rec->score, &field[3]);			/* 72F5-7302 */
		(void)shadow_text_draw(g_panel_text_attr, 0x1a, 0x28, field);	/* 7305-7318 */
		while (mouse_button_read() != 0) ;					/* 731B-7322 */
		while (mouse_button_read() == 0) ;					/* 7324-732B */
		while (mouse_button_read() != 0) ;					/* 732D-7334 */
	}
}

/* 0x275F */

/* 0x275F */
void settle_hero_score(void)
{
	int16 cnt = 0;
	int16 total = 0;
	int16 j;
	int16 val;
	struct character_rec *rec;

	shadow_video_flags(SHADOW_BLIT_FLUSH);						/* 2766-276D */
	if (g_overworld_phase == 1) {					/* 2770-2781 */
		show_text_panel(TEXT_HEAL_WILDERNESS);
		return;
	}
	if (g_overworld_hold != 0) {					/* 2784-2795 */
		show_text_panel(TEXT_HEAL_COMBAT);
		return;
	}

	for (j = 0; j != 4; j++) {					/* 27A3-27AC */
		val = (int16)g_hero_slots[j].state;		/* 27AE-27BF */
		if (g_actor_table[j].state == 0)		/* 27C2-27D1 */
			val = 0x17;
		if (val < 0x16) {
			if (val == CHAR_GOLMOON || val == CHAR_ELISTAN)
				cnt++;					/* 27DC-27E8 */
			rec = CHAR_REC(character_at(g_record_addr[val]));	/* 27EB-27F4 */
			total = (int16)(total + (int16)
				(rec->hp_max - rec->hp));
										/* 27F7-27FF */
		}
	}

	if (cnt == 0) {							/* 2804-2814 */
		show_text_panel(TEXT_NO_CLERICS);
		return;
	}
	show_text_panel(TEXT_WOUNDS_HEALED);							/* 2816-281D */
	if (total == 0)
		return;							/* 2820-2826 */

	total = total / 15;
	total = total / cnt;
	total = (int16)(60 * total);					/* 283D-2843 */
	total = (int16)(total + 15);
	g_frame_debt += (uint16)total;					/* 284A-2851 */

	for (j = 0; j != 4; j++) {					/* 2851-285F */
		val = (int16)g_hero_slots[j].state;		/* 2861-2872 */
		if (val < 0x16) {
			rec = CHAR_REC(character_at(g_record_addr[val]));	/* 287A-2882 */
			rec->hp = rec->hp_max;
										/* 2885-288C */
		}
	}
}

extern void shadow_mouse_draw(uint16, uint16);	/* 0xD42B */

/* 0x1E20 */

/* 0x1E20 */
void panel_layout(int16 *px, int16 *py, int16 flags, int16 mode, int16 cursor)
{
	*px = 10;							/* 1E23-1E26 */
	*py = 0x50;							/* 1E2A-1E2D */
	if ((flags & 1) != 0)
		*px = 0x18;						/* 1E31-1E3A */
	sprite_draw_current(SPRITE_PANEL_BACKGROUND, SPRITE_DRAW_SOLID, *px, *py);					/* 1E3E-1E53 */
	*px <<= 3;							/* 1E59-1E5B */

	g_panel_left = (uint16)(*px + 8);				/* 1E60-1E65 */
	g_panel_top = (uint16)(*py + 8);				/* 1E68-1E70 */
	g_panel_left2 = (uint16)(*px + 8);				/* 1E73-1E7B */
	g_panel_cursor_x = (uint16)(*px + 0x18);				/* 1E7E-1E83 */
	g_panel_right = (uint16)(*px + 0x28);				/* 1E86-1E8B */
	g_panel_bottom = (uint16)(*py + 0x1a);				/* 1E8E-1E96 */
	g_panel_cursor_y = (uint16)(*py + 0x1a);
	g_panel_top2 = (uint16)(*py + 0x1a);

	sprite_draw_current(SPRITE_ARROW_UP, SPRITE_DRAW_SOLID, (int16)g_panel_left2 >> 3, (int16)g_panel_bottom);	/* 1E9F-1EB1 */
	if (mode == 1)
		sprite_draw_current(SPRITE_ARROW_DOWN, SPRITE_DRAW_SOLID, (int16)g_panel_cursor_x >> 3,		/* 1EB4-1ED1 */
			(int16)g_panel_cursor_y);
	if (cursor != 0)
		shadow_mouse_draw(g_panel_cursor_x + 5, g_panel_cursor_y + 3);	/* 1ED4-1EEB */
}

/* 0xB80 */

uint8 g_btn_table[130] = {
	0x00, 0x00, 0x78, 0x00, 0x7e, 0x00, 0x40, 0x00, 0x08, 0x00,
	0x01, 0x00, 0xc8, 0x00, 0x7e, 0x00, 0x40, 0x00, 0x08, 0x00,
	0x02, 0x00, 0x78, 0x00, 0x8e, 0x00, 0x40, 0x00, 0x08, 0x00,
	0x03, 0x00, 0xc8, 0x00, 0x8e, 0x00, 0x40, 0x00, 0x08, 0x00,
	0x04, 0x00, 0xc8, 0x00, 0x6e, 0x00, 0x40, 0x00, 0x08, 0x00,
	0x1f, 0x00, 0x24, 0x00, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00,
	0x1f, 0x00, 0x0c, 0x01, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00,
	0x1f, 0x00, 0x24, 0x00, 0xb8, 0x00, 0x10, 0x00, 0x10, 0x00,
	0x1f, 0x00, 0x0c, 0x01, 0xb8, 0x00, 0x10, 0x00, 0x10, 0x00,
	0x19, 0x00, 0x00, 0x00, 0x27, 0x00, 0x10, 0x00, 0x10, 0x00,
	0x19, 0x00, 0x30, 0x01, 0x27, 0x00, 0x10, 0x00, 0x10, 0x00,
	0x19, 0x00, 0x00, 0x00, 0x99, 0x00, 0x10, 0x00, 0x10, 0x00,
	0x19, 0x00, 0x30, 0x01, 0x99, 0x00, 0x10, 0x00, 0x10, 0x00,
};

/* 0xB44 */

char *g_label_buttons[10] = {
	"Attack", "Ranged", "Flee", "Cast", "Memorize",
	"", "", "", "", "",
};

/* 0x101F */

/* 0x101F */
void draw_combat_menu(int16 count, int16 sel)
{
	struct rect_rec *rec;
	int16 attr;
	int16 i;

	if (sel == -1) {
		attr = TEXT_COLOUR_NORMAL;
		rec = RECT(g_btn_table);
		for (i = 0; i != count; i++, rec++) {		/* 1035-1049 */
			if (i == 0)
				attr = TEXT_COLOUR_DIM;			/* 104B-1051 */
			if (i == 4)
				attr = TEXT_COLOUR_NORMAL;			/* 1056-105C */
			(void)shadow_text_draw(attr,			/* 1061-1082 */
				(int16)((int16)rec->x >> 3) + 1,
				rec->y, g_label_buttons[i]);
		}
	} else {
		rec = RECT(g_btn_table + sel * RECT_STRIDE);		/* 1086-1099 */
		(void)shadow_text_draw(TEXT_COLOUR_HIGHLIGHT,				/* 109B-10B6 */
			(int16)((int16)rec->x >> 3) + 1,
			rec->y, g_label_buttons[sel]);
	}
}

/* 0x00DB */

char g_bar_frame[] = {
	'[', 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, 0x5c, ']', 0x00
};

/* 0x0F85 */

/* 0x0F85 */
void draw_status_bar(int16 fraction, int16 x, int16 y)
{
	char buf[13];
	int16 b;
	int16 n;

	text_copy(buf, "__________");				/* F8C-F97 */
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, x + 1, y, buf);		/* F9A-0FAD */

	b = (int16)(int8)(fraction & 0xff);		/* 0FB0-0FBD */
	if (b < 0)
		b = 0;
	n = b >> 1;
	if ((b & 1) != 0) {					/* 0FC3-0FCE */
		buf[n] = '^';
		++n;
	}
	buf[n] = '\0';						/* 0FD2-0FD5 */
	if (n != 0)						/* 0FD9-0FF5 */
		(void)shadow_text_draw((g_gauge_fill_long & LOW_BYTE_MASK) | HIGH_BYTE_MASK,
			x + 1, y, buf);

	text_copy(buf, g_bar_frame);				/* 0FF8-1003 */
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, x, y, buf);		/* 1006-1017 */
}

/* 0xA662 */

uint16 g_longest_string;
uint16 g_string_cursor;

/* 0x0DED */

/* 0x0DED */

int16 table_longest_string(uint16 *items, int16 image_addrs)
{
	g_longest_string = 0;
	g_string_cursor = 0;

	for (;;) {
		uint16 item = items[g_string_cursor];
		int16 v = text_length(image_addrs ? image_text(item) : (char *)item);	/* 0E06-0E17 */
		if (v == 0)
			return 0;				/* 0E1E-0E20 */
		if (v > (int16)g_longest_string)
			g_longest_string = (uint16)v;		/* 0E22-0E2D */
		++g_string_cursor;
	}
}

extern uint8 g_rect_menubar[50];		/* 0xB4E */
extern int16 draw_text_window(int16 x, int16 y, int16 w, int16 h);		/* 0x4693 */
extern int16 shadow_key_read(void);			/* 0xD374 */
extern void shadow_mouse_draw(uint16, uint16);	/* 0xD42B */

/* 0xAA00 */

uint8 g_menu_boxes[2000];

void menu_row(char **tab, int16 idx, int16 attr)
{
	struct rect_rec *r = RECT(g_menu_boxes + idx * RECT_STRIDE);
	int16 w = r->x;
	int16 sgn = (w < 0) ? -1 : 0;

	(void)shadow_text_draw(attr, ((w ^ sgn) - sgn) >> 3,
		r->y,
		tab[idx]);			/* 52A0-52AC */
}

/* 0x5120 */

/* 0x5120 */
int16 menu_select(int16 panel, char **labeltab, int16 flag)
{
	int16 width, x, y, tx, i, prev;
	uint8 *r;

	(void)table_longest_string((uint16 *)labeltab, 0);			/* 5127-512D */
	width = (int16)(int16)((uint16)g_longest_string << 3);	/* 5130-5137 */
	x = (int16)RECT(g_rect_menubar + panel * RECT_STRIDE)->x + 8;	/* 5149-5150 */
	y = (int16)RECT(g_rect_menubar + panel * RECT_STRIDE)->y + 8;	/* 5153-515A */

	tx = x + width;						/* 515D-5163 */
	if (tx > SCREEN_WIDTH) {
		tx -= SCREEN_WIDTH;
		x -= tx;						/* 516B-516E */
	}
	{
		int16 sgn = (x < 0) ? -1 : 0;
		tx = (((x ^ sgn) - sgn) >> 3);
		tx = (tx ^ sgn) - sgn;					/* 5171-5182 */
	}

	(void)draw_text_window(tx, y, (int16)g_longest_string, (int16)g_string_cursor);	/* 5185-5194 */

	r = g_menu_boxes;
	for (i = 0; i != (int16)g_string_cursor; i++, r += 10) {		/* 51AA-51B0 */
		RECT(r)->x = (uint16)x;
		RECT(r)->y = (uint16)y;
		RECT(r)->w = (uint16)width;
		RECT(r)->h = 8;
		y += 8;
		(void)shadow_text_draw(TEXT_COLOUR_NORMAL, tx, (int16)RECT(r)->y,
			labeltab[i]);		/* 51E1-51F3 */
	}

	if (flag != 0)
		shadow_mouse_draw((uint16)(x + (width >> 1)), 4);		/* 51FE-520E */

	(void)mouse_poll_moved(0);						/* 5211-5217 */
	i = -1;
	prev = -1;							/* 521D-5220 */

	while (mouse_button_read() == 0) {				/* 5223-5228 */
		if (flag != 0) {				/* 522D-5231 */
			i = shadow_key_read() & 0xff;		/* 5233-5238 */
			if (i == KEY_ESC)
				return -1;			/* 523B-5243 */
		}
		(void)mouse_poll_moved(1);				/* 5246-524D */
		i = rect_scan((int16)g_string_cursor, g_menu_boxes);	/* 5250-525E */
		if (i != prev) {				/* 5264-5267 */
			(void)shadow_video_flags(SHADOW_BLIT_FLUSH);		/* 526C-5273 */
			if (i != -1)
				menu_row(labeltab, i, TEXT_COLOUR_HIGHLIGHT);	/* 5276-52B6 */
			if (prev != -1)
				menu_row(labeltab, prev, TEXT_COLOUR_NORMAL);	/* 52B9-52F9 */
			(void)mouse_poll_moved(0);		/* 52FC-5302 */
		}
		prev = i;					/* 5305-5308 */
	}

	while (mouse_button_read() != 0)					/* 530E-5315 */
		;
	return i;
}

extern int16 mouse_button_read(void);				/* 0x4A36 */
extern int16 shadow_video_flags(int16);					/* 0x9CFA */

/* 0x7F73 */

int16 wait_button_click(struct rect_rec *rec, int16 n)
{
	int16 sel;

	(void)draw_button_caption(rec, n);

	do {
		while (mouse_button_read() != 0)
			;
		while (mouse_button_read() == 0)
			(void)mouse_poll_moved(1);

		sel = rect_scan(n, (uint8 *)rec);				/* 7FA1-7FAD */
	} while (sel == -1);

	while (mouse_button_read() != 0)
		;
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);					/* 7FBE-7FC5 */
	return sel;
}

/* 0xC84 */

uint8 g_btn_pairs[20] = {
	0x01, 0x00, 0x7c, 0x00, 0x6c, 0x00, 0x10, 0x00, 0x10, 0x00,
	0x00, 0x00, 0xb4, 0x00, 0x6c, 0x00, 0x10, 0x00, 0x10, 0x00
};
uint8 g_btn_rows[50] = {
	0x00, 0x00, 0x30, 0x00, 0x5f, 0x00, 0x20, 0x00, 0x08, 0x00,
	0x01, 0x00, 0x30, 0x00, 0x67, 0x00, 0x20, 0x00, 0x08, 0x00,
	0x02, 0x00, 0x30, 0x00, 0x6f, 0x00, 0x20, 0x00, 0x08, 0x00,
	0x03, 0x00, 0x30, 0x00, 0x77, 0x00, 0x20, 0x00, 0x08, 0x00,
	0x04, 0x00, 0x30, 0x00, 0x7f, 0x00, 0x20, 0x00, 0x08, 0x00
};

/* 0x7FD1 */

int16 question_two_buttons(int16 res_id)
{
	int16 sel;

	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	(void)draw_text_window(3, 32, 76, 7);

	(void)text_resource_read(res_id);
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 5, 0x54, (char *)&g_credits);

	(void)text_resource_read(TEXT_BUTTON_NO_YES);
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL,
		(int16)((int16)RECT(g_btn_pairs)->x) >> 3,
		(int16)RECT(g_btn_pairs)->y,
		(char *)&g_credits);

	sel = wait_button_click(RECT(g_btn_pairs), 2);
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
	return sel;
}

/* 0x806B */

int16 question_five_choices(int16 res_id)
{
	int16 sel;

	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	(void)draw_text_window(3, 40, 34, 14);

	(void)text_resource_read(res_id);
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 4, 0x30, (char *)&g_credits);

	sel = wait_button_click(RECT(g_btn_rows), 5);
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
	return sel;
}

/* 0x80E1 */

int16 question_chain(int16 flag)
{
	int16 first = 1;
	int16 second = 2;

	if (flag != 0) {
		first = question_two_buttons(TEXT_COUNCIL_MEETING);
	}
	if (first != 0) {
		second = question_five_choices(TEXT_COUNCIL_ADDRESS);
	}
	return second;
}

/* 0xCA */

/* 0xD74 */

char *g_vote_options[7] = {
	"go back", "stop here", "continue", "leave refugees",
	" ", " ", "  Game paused  "
};

/* 0x8237 */

int16 announce_vote(int16 opt, int16 ent)
{
	if (find_party_player_tile(ent) == 0)						/* 823D-8245 */
		return 0;

	if (ent == 1) {
		(void)driver_play_sequence(SOUND_SEQUENCE_REFUGEE_VOTE);
		(void)draw_presentation(PRESENTATION_REFUGEE_VOTE);
	}

	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	(void)draw_text_window(5, 16, 76, 12);

	if (ent == 1)
		(void)shadow_text_draw((int16)g_vote_text_attr, 0x10, 0x4c, "Refugees");
	else
		(void)shadow_text_draw((int16)g_vote_text_attr, 0x10, 0x4c,
			name_from_pool(g_party_name_offset[ent]));

	(void)shadow_text_draw((int16)g_vote_text_attr, 0xc, 0x5c, "Vote to:");
	(void)shadow_text_draw((int16)g_vote_text_attr, 0xe, 100,
		g_vote_options[opt]);

	while (mouse_button_read() != 0)						/* 82ED-82F4 */
		;
	while (mouse_button_read() == 0 && shadow_key_read() == 0)		/* 82F6-8304 */
		;
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
	return 0;
}

extern int16  path_find_distance(int16 gx, int16 gy, int16 tx, int16 ty, int16 *dist);	/* 0x7501 */
extern int16  party_update_all(int16 advance, int16 draw);			/* 0x7807 */
extern int16  shadow_screen_restore(uint16 v);						/* 0xD450 */

/* 0x88DA */

void world_map_click(void)
{
	int16 mx, row, row0, cell, kind, dist;
	int16 off, i;
	struct party_rec *rec;

	if (g_overworld_phase == 2) {
		(void)event_raise(EVENT_WORLD_MAP_DEFAULT);
		return;
	}

	(void)event_raise(EVENT_WORLD_MAP_CLICK);
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	(void)shadow_screen_restore(7);
	(void)party_update_all(0, 1);

	while (mouse_button_read() == 0)
		(void)mouse_poll_moved(1);
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);

	if ((int16)g_mouse_y <= 8) {
		(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
		return;
	}

	mx = (int16)g_mouse_x;
	row = (int16)PARTY_REC(0)->grid_y - 6;							/* 8956-895C */
	if (row < 0)
		row = 0;
	if (row > 12)
		row = 12;
	row0 = row;
	row += ((int16)g_mouse_y - 8) / 12;

	if ((row & 1) == 0)
		mx -= 8;
	mx = mx >> 4;
	if (mx < 0) {
		(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
		return;
	}

	cell = (int16)(int8)g_map_tile[row * TERRAIN_WIDTH + mx];	/* 89AA-89BA */
	if ((cell & 0x80) == 0)
		goto drained;

	dist = 0;
	(void)path_find_distance((int16)PARTY_REC(0)->grid_x, (int16)PARTY_REC(0)->grid_y, mx, row, &dist);
	(void)party_update_all(0, 1);

	off = ((row & 1) ? 0 : 8) + (mx << 4);				/* 89E9-8A00 */
	(void)sprite_draw_anchored(off, 0x39 - row0 * 12, 0x39);

	rec = PARTY_REC(PARTY_COMPANION_LAST);
	cell &= 0x7f;

	kind = (cell == MAP_FOOD_GROVE || cell == MAP_DUSK_EYE || cell == MAP_DRAGON_ATTACK) ? 1 : 0;	/* 8A3F-8A58 */

	for (i = PARTY_COMPANION_LAST; i != PARTY_MAIN; i--, rec--) {
		if (rec->grid_x != PARTY_REC(0)->grid_x)
			continue;
		if (rec->grid_y != PARTY_REC(0)->grid_y)
			continue;
		rec->attr |= 0x10;
		rec->target_x = (uint16)mx;
		rec->target_y = (uint16)row;
		rec->cooldown = (uint16)(dist * 2);	/* 8AA0-8AA2 */
		if (i == PARTY_REFUGEES && PARTY_REC(PARTY_REFUGEES)->attr == 2)
			continue;
		(void)companion_react_tile(i, 0, kind);
	}

drained:
	while (mouse_button_read() != 0)
		;
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
}

extern uint16 g_refugee_seek_hold;					/* 0xAD74 */
extern void party_spawn(int16 slot, int16 parts, int16 flags);	/* 0x7C8A */

/* 0x1CD4 */

const uint8 g_vote_tbl[60] = {
	0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x01, 0x02, 0x02, 0x02,
	0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02,
	0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
	0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x08, 0x08, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00
};

/* 0x8314 */

int16 companion_react_tile(int16 ent, int16 ask, int16 kind)
{
	int16 A[5], B[5], C[5], D[5];
	int16 delta = -1;
	int16 answer = 0;
	int16 k8, k4, n, i, j, scratch, winner;

	(void)C;
	g_refugee_seek_hold = 0;
	for (i = 0; i != 5; i++) {				/* 8327-834C */
		A[i] = 0;
		B[i] = 0;
		C[i] = 0;
		D[i] = 0;
	}

	if (find_party_player_tile(ent) != 0)					/* 834E-8359 */
		answer = question_chain(ask);				/* 835B-8364 */

	k8 = (int16)(int8)g_party_table[ent].attr;	/* 8367-8374 */
	k4 = k8;
	k4 &= 4;
	k8 &= 8;

	for (i = PARTY_FOLLOWER_FIRST; i != PARTY_LAST + 1; i++)
		B[i - 11] = random_cell(i, answer);

	if (party_may_use_code(0xb, ent) != 0) {			/* 83A8-83BD */
		B[0] += 10;
		B[4] -= 10;
	}

	if (party_may_use_code(0, ent) != 0)				/* 83C1-83D5 */
		B[3] += 15;
	else if (party_may_use_code(6, ent) != 0)			/* 83D7-83E8 */
		B[3] += 5;

	if (party_may_use_code(8, ent) != 0 || party_may_use_code(9, ent) != 0)	/* 83EC-840E */
		B[1] += 5;

	if (k4 != 0)
		delta -= 10;						/* 8412-8418 */
	if (k8 != 0)
		delta -= 3;							/* 841C-8422 */

	if (ent == PARTY_REFUGEES) {

		if (kind != 0) {
			(void)announce_vote(2, 1);
			PARTY_REC(PARTY_REFUGEES)->attr &= 0xfc;
			PARTY_REC(PARTY_REFUGEES)->attr |= 0x02;
			return 0;
		}

		n = 1;
		scratch = 0;
		for (i = 0; i != 5; i++) {			/* 8452-8460 */
			A[i] = 0;
			if (g_party_table[i + 2].kind != 0)
				continue;
			++n;
			B[i] += delta;
			scratch = B[i];
			if (scratch < 0)
				scratch = 0;
			if (scratch > 20)
				scratch = 20;
			scratch += 4;
			scratch = scratch / 5;
			B[i] = scratch;
			if (i == 2)
				continue;
			A[i] = (int16)(int8)g_vote_tbl[30 + i * 6 + scratch];
			scratch = (int16)(int8)g_vote_tbl[i * 6 + scratch];
			D[scratch]++;
			A[i] |= scratch;
		}

		winner = 2;
		if (D[0] > D[2])
			winner = 0;
		if (D[1] > D[2])
			winner = 1;

		if (winner == 2 && D[0] == D[2] && D[1] == D[2]) {	/* 8514-8525 */
			D[(int16)(int8)g_vote_tbl[12 + scratch]]++;	/* 852A-8533 */
			if (D[0] > D[2])
				winner = 0;
			if (D[1] > D[2])
				winner = 1;
		}

		(void)announce_vote(winner, 1);
		PARTY_REC(PARTY_REFUGEES)->attr &= 0xfc;
		PARTY_REC(PARTY_REFUGEES)->attr |= (uint8)winner;	/* 8562-8565 */

		for (i = 0; i != 5; i++) {			/* 8569-85B5 */
			if ((A[i] & 8) == 0)
				continue;
			if ((A[i] & 7) == winner)
				continue;
			(void)party_spawn(i, n, A[i] & 7);
			(void)announce_vote(winner + 1, i + 2);
		}
		return 0;
	}

	if (ent >= 2 && ent <= 6) {
		j = ent - 2;
		B[j] += delta;
		scratch = B[j];
		if (scratch < 0)
			scratch = 0;
		if (scratch > 20)
			scratch = 20;
		scratch += 4;
		scratch = scratch / 5;				/* 85F1-85F7 */
		B[j] = scratch;
		scratch = (int16)(int8)g_vote_tbl[j * 6 + scratch];
		g_party_table[ent].attr = (uint8)
			(g_party_table[ent].attr & 0xfc) | (uint8)scratch;	/* 8624-862A */
		(void)announce_vote(scratch + 1, ent);
	}

	return 0;
}

extern uint8 g_map_tile[];			/* DGROUP 0x2c92 */
extern int16 shadow_screen_restore(uint16 v);		/* 0xD450 */

/* 0x70AC */

int16 show_screen_wait(int16 a)
{
	int16 saved = (int16)PARTY_REC(0)->grid_y;			/* 70B2-70B5 */
	int16 i;

	PARTY_REC(0)->grid_y = 0x20;

	for (i = 620; i != 680; i++)
		g_map_tile[i] |= 0x80;

	(void)draw_map_view();
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	(void)shadow_screen_restore(7);

	if (a != 0)
		show_text_panel(a);					/* 70F4-7100 */

	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);

	while (mouse_button_read() != 0)
		;
	while (mouse_button_read() == 0)
		(void)mouse_poll_moved(1);
	while (mouse_button_read() != 0)
		;

	PARTY_REC(0)->grid_y = (uint16)saved;			/* 7132-7135 */
	return saved;
}

void draw_label_row(int16 row, int16 x, int16 y, int16 attr)
{
	(void)shadow_text_draw(attr, x / 8, y, image_text(g_label_offset[row]));
}

void draw_row_scratch(int16 ix, int16 attr)
{
	struct rect_rec *rp = RECT(g_menu_boxes + ix * RECT_STRIDE);

	draw_label_row((int16)g_hero_slots[ix].state,
	                (int16)rp->x, (int16)rp->y, attr);
}

/* 0x5403 */

int16 menubar_rearrange(int16 panel, int16 arg2)
{
	int16 w, xbase, ybase, x;
	int16 count, i, flag;
	int16 sel, prev, pickA, pickB, attr;
	int16 t, m;
	uint8 *rp, *rq;
	uint8 swapbuf[30];

	(void)table_longest_string((uint16 *)g_label_offset, 1);		/* 5410-5417 */

	w = (int16)((uint16)g_longest_string << 3);				/* 541A-5421 */
	xbase = (int16)RECT(g_rect_menubar + panel * RECT_STRIDE)->x;	/* 5424-5437 */
	ybase = (int16)RECT(g_rect_menubar + panel * RECT_STRIDE)->y + 8;	/* 543A-5441 */

	x = xbase + w;							/* 5444-544A */
	if (x > SCREEN_WIDTH)						/* 544D-5455 */
		xbase -= (x - SCREEN_WIDTH);
	x = xbase / 8;

	g_string_cursor = 4;
	if (arg2 != 0)
		(void)shadow_mouse_draw((uint16)(xbase + (w >> 1)), 4);	/* 547D-548A */

	(void)setting_apply(4);							/* 5490-5494 */

	flag = 0;
	if ((int16)g_overworld_phase == 1					/* 549A-54B1 */
	 && (int16)PARTY_REC(0)->grid_x == (int16)PARTY_REC(PARTY_REFUGEES)->grid_x
	 && (int16)PARTY_REC(0)->grid_y == (int16)PARTY_REC(PARTY_REFUGEES)->grid_y) {
		flag = setting_apply(0);					/* 54B3-54BC */
		for (i = 4; i != HERO_SLOT_COUNT; i++) {				/* 54BF-54E9 */
			rp = (uint8 *)(g_hero_slots + i);
			if ((RECT(rp)->state & RECT_STATE_RETIRING_MASK) != 0)
				break;
			if (RECT(rp)->state == HERO_SLOT_NONE)
				break;
			++g_string_cursor;
		}
	}

	count = (int16)g_string_cursor;
	(void)draw_text_window(x, ybase, (int16)g_longest_string, count);		/* 54EB-54FC */

	for (i = 0; i != count; i++) {				/* 54FF-5592 */
		rp = g_menu_boxes + i * RECT_STRIDE;
		RECT(rp)->x = (uint16)xbase;
		RECT(rp)->y = (uint16)ybase;
		RECT(rp)->w = (uint16)w;
		RECT(rp)->h = 8;
		ybase += 8;
		attr = ((g_hero_slots[i].state & RECT_STATE_GONE) != 0)
		     ? TEXT_COLOUR_DIM : TEXT_COLOUR_NORMAL;				/* 5557-5561 */
		draw_label_row((int16)(uint8)g_hero_slots[i].state,
		                xbase, (int16)RECT(rp)->y, attr);	/* 5566-558F */
	}

	sel = -1;							/* 5595-559B */
	prev = -1;
	while (mouse_button_read() == 0) {
		if ((shadow_key_read() & 0xff) == KEY_ESC)			/* 55A8-55AD */
			return -1;
		(void)mouse_poll_moved(1);
		sel = rect_scan(count, g_menu_boxes);			/* 55BF-55CD */
		if (sel != prev) {
			(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
			if (sel != -1)
				draw_row_scratch(sel, TEXT_COLOUR_HIGHLIGHT);			/* 55EB-5628 */
			if (prev != -1)
				draw_row_scratch(prev, TEXT_COLOUR_NORMAL);			/* 562B-566E */
			(void)mouse_poll_moved(0);
		}
		prev = sel;
	}

	if (sel < 0) {
		if (flag != 0)
			(void)load_pane_maps();
		return -1;
	}
	pickA = sel;
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	draw_row_scratch(pickA, TEXT_COLOUR_CHOSEN);					/* 56AA-56E7 */
	(void)mouse_poll_moved(0);
	while (mouse_button_read() != 0)
		;

	sel = -1;							/* 56FC-5702 */
	prev = -1;
	while (mouse_button_read() == 0) {
		if ((shadow_key_read() & 0xff) == KEY_ESC)			/* 570F-5714 */
			return -1;
		(void)mouse_poll_moved(1);
		sel = rect_scan(count, g_menu_boxes);			/* 5726-5734 */
		if (sel != prev) {
			(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
			if (sel != -1)
				draw_row_scratch(sel, TEXT_COLOUR_HIGHLIGHT);			/* 574C-578F */
			if (prev != -1) {

				attr = (prev == pickA) ? TEXT_COLOUR_CHOSEN : TEXT_COLOUR_NORMAL;	/* 57B2-57BC */
				draw_row_scratch(prev, attr);			/* 57BF-57E7 */
			}
			(void)mouse_poll_moved(0);
		}
		prev = sel;
	}

	if (sel < 0) {
		if (flag != 0)
			(void)load_pane_maps();
		return -1;
	}
	pickB = sel;

	if (pickA == 0 && pickB == 0) {				/* 581E-583E */
		if (flag != 0)
			(void)load_pane_maps();
		return -1;
	}

	rp = (uint8 *)(g_hero_slots + pickA);				/* 5841-5861 */
	if ((RECT(rp)->state & RECT_STATE_GONE) != 0)
		goto done;
	if (RECT(rp)->state == HERO_SLOT_NONE)
		goto done;
	rq = (uint8 *)(g_hero_slots + pickB);				/* 5864-5884 */
	if ((RECT(rq)->state & RECT_STATE_GONE) != 0)
		goto done;
	if (RECT(rq)->state == HERO_SLOT_NONE)
		goto done;

	t = RECT(rp)->state;						/* 5887-58A3 */
	RECT(rp)->state = RECT(rq)->state;
	RECT(rq)->state = (uint16)t;

	if (flag != 0) {
		(void)load_pane_maps();
		goto done;
	}

	if (pickA >= 4 && pickB >= 4)					/* 58B1-58BD */
		goto done;

	if (pickA < 4 && pickB < 4) {
		struct actor_rec *qa = &g_actor_table[pickA];		/* 58D2-591F */
		struct actor_rec *qb = &g_actor_table[pickB];

		mem_copy_bytes(swapbuf, (uint8 *)qa, sizeof *qa);
		mem_copy_bytes((uint8 *)qa, (uint8 *)qb, sizeof *qa);
		mem_copy_bytes((uint8 *)qb, swapbuf, sizeof *qa);

		t = qa->state;

		qa->state = qb->state;
		qb->state = (uint8)t;

		t = (int16)g_vmpl_slot[pickA];				/* 5941-5963 */
		g_vmpl_slot[pickA] = g_vmpl_slot[pickB];
		g_vmpl_slot[pickB] = (uint8)t;
	} else {
		m = (pickA <= pickB) ? pickA : pickB;			/* 5966-5971 */
		g_name_vmpl[4] = (char)((uint8)g_hero_slots[m].state + 0x41);	/* 597E-5985 */
		(void)shadow_asset_load(g_name_vmpl,				/* 5990-599F */
		                 g_vmpl_para[(int16)(int8)g_vmpl_slot[m]]);
	}

done:
	if ((int16)g_overworld_phase == 2)					/* 59A2-59A9 */
		draw_hero_gauges();
	return 0;
}

extern void terminate_callback(void);				/* 0x64A3 */

/* 0x59B2 */

int16 menubar_click(int16 hit, int16 mode)
{
	int16 id, r;

	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);					/* 59B9-59C0 */
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);						/* 59C3-59C6 */

	id = (int16)RECT(g_rect_menubar + hit * RECT_STRIDE)->state;		/* 59CC-59E3 */
	(void)shadow_text_draw(TEXT_COLOUR_HIGHLIGHT,

		       (int16)RECT(g_rect_menubar + hit * RECT_STRIDE)->x >> 3,
		       (int16)RECT(g_rect_menubar + hit * RECT_STRIDE)->y,
		       g_menubar_labels[id]);

	while (mouse_button_read() != 0)						/* 5A00-5A07 */
		;

	switch (hit) {
	case MENUBAR_OPTIONS:
		r = menu_select(hit, g_menubar_labels + 5, mode);	/* 5A0F-5A1F */
		if (r < 0)
			break;
		switch (r) {						/* 5A96-5AA1 */
		case OPTION_ABOUT:
			(void)message_box(MSG_BOX_NOTICE, MSG_VERSION);				/* 5A2E-5A35 */
			break;
		case OPTION_REFRESH_MAP:
			g_map_mode_char =					/* 5A3D-5A4E */
			    g_terrain_grid[PARTY_REC(0)->grid_x + TERRAIN_WIDTH * PARTY_REC(0)->grid_y];
			break;
		case OPTION_SETTLE_SCORES:
			settle_hero_score();
			break;
		case OPTION_TERRAIN_CHANGE:
			if (g_overworld_hold == 0) {				/* 5A58-5A72 */
				(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
				terrain_change();
				(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
			}
			break;
		case OPTION_SEPARATOR_1:
			break;
		case OPTION_LOAD:
			(void)save_load_dialog();					/* 5A79-5A7F */
			break;
		case OPTION_SAVE:
			(void)save_load_panel(0);				/* 5A84-5A87 */
			break;
		case OPTION_SEPARATOR_2:
			break;
		case OPTION_QUIT:
			terminate_callback();
			break;
		default:
			break;
		}
		break;

	case MENUBAR_PARTY:
		(void)menubar_rearrange(hit, mode);				/* 5AB7-5ABD */
		break;

	case MENUBAR_REFUGEES:
		r = menu_select(hit, g_menubar_labels + 15, mode);	/* 5AC5-5AD5 */
		if (r < 0)
			break;
		if (r == 0) {						/* 5B07-5B0E */
			if (find_party_player_tile(0) != 0)				/* 5AE3-5AF0 */
				world_map_click();
		} else if (r == 1) {
			if (find_party_player_tile(0) != 0)				/* 5AF5-5B02 */
				travelling_company_panel();
		}
		break;

	default:

		break;
	}

	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);						/* 5B2F-5B33 */
	(void)mouse_poll_moved(0);						/* 5B39-5B3C */
	while (mouse_button_read() != 0)						/* 5B42-5B49 */
		;
	return 0;
}

extern int16 shadow_page_flip(void);			/* 0xD3D0 */

/* 0x0701 */

void high_score_build(void)
{
	char fld[10];
	int16 i;

	(void)draw_text_window(6, 24, 27, 10);			/* 0707-071A */
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 0xf, 30, "HIGH SCORES");	/* 071D-0730 */
	(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 7, 0x28, g_panel_template.panel.title);		/* 0733-0746 */

	for (i = 0; i != 5; i++) {				/* 0749-07A0 */
		text_copy(fld, "         ");
		format_ulong(g_panel_template.panel.score[i],	/* 076A-0780 */
		        &fld[8]);
		(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 0x17, (i << 3) + 0x28, fld);	/* 0783-079D */
	}

	(void)shadow_page_flip();
}

extern int16 shadow_video_copy(uint16 n);		/* 0xD360 */

/* 0x07A9 */

void high_score_show(void)
{
	(void)shadow_video_copy(SCREEN_HEIGHT);				/* 07AC-07B3 */
	high_score_build();

	while (shadow_key_read() == 0 && mouse_button_read() == 0)		/* 07B9-07C7 */
		;
	while (mouse_button_read() != 0)					/* 07C9-07CC */
		;
}

extern int16 table_longest_string(uint16 *items, int16 image_addrs);
extern void text_copy(char *dest, const char *src);
extern void format_uint(uint16 v, char *p);
extern int16 draw_text_window(int16 x, int16 y, int16 w, int16 h);
extern int16 mouse_poll_moved(int16 a);
extern int16 mouse_button_read(void);
extern int16 sprite_draw_current(int16 sprite, int16 mode, int16 column, int16 row);
extern int16 shadow_video_flags(int16 v);
extern void sprite_blit_offset(int16 x, int16 y, int16 id);
extern int16 shadow_text_draw(int16 attr, int16 col, int16 row, const char *text);
extern int16 shadow_key_read(void);
extern int16 rect_scan(int16 count, uint8 *table);
extern uint16 g_hour_ticks;					/* 0x2288 */

/* 0xB101 */

int16 box_menu(uint16 *labels, uint8 *codes, int16 n, int16 quiet)
{
	int16 w, h, y0, x, i, y, cur, prev;
	struct rect_rec *rec;
	char buf[4];

	shadow_video_flags(SHADOW_BLIT_FLUSH);						/* B108-B10F */
	table_longest_string(labels, 1);						/* B112-B118 */

	w = (0x28 - (int16)g_longest_string) << 2;				/* B11B-B126 */
	h = (SCREEN_HEIGHT - ((int16)g_string_cursor << 3)) >> 1;			/* B129-B137 */
	y0 = ((int16)g_longest_string << 3) + 24;				/* B13A-B142 */
	x = ABS_SHR3(w);						/* B145-B156 */

	if (quiet == 0) {							/* B162-B166 */
		draw_text_window(x, h, (int16)g_longest_string, (int16)g_string_cursor);		/* B168-B179 */
		if (n != -1)
			sprite_draw_current(SPRITE_MENU_RULE, SPRITE_DRAW_MASKED_0E, 0, h - 7);			/* B17C-B193 */
	}

	if (n != -1) {							/* B196-B19A */
		n = n / 15;
		y = (int16)g_clock_hour * 4 + (int16)g_hour_ticks / 15;		/* B1A8-B1B8 */
		sprite_blit_offset(y + 0x70, h - 7 + 3, 0x53);			/* B1BB-B1D0 */
		for (i = 0; i != n; ++i) {					/* B1D3-B1E3 */
			sprite_blit_offset(y + 0x70, h - 7 + 3, 0x53);		/* B1E5-B1FA */
			if (++y == CLOCK_RING_SIZE)
				y = 0;						/* B1FD-B206 */
		}
	}

	rec = RECT(g_menu_boxes);
	for (i = 0; i != (int16)g_string_cursor; ++i, rec++) {		/* B20D-B226 */
		rec->x = (uint16)w;				/* B22B-B231 */
		rec->y = (uint16)h;				/* B234-B23A */
		rec->w = (uint16)y0;				/* B23D-B243 */
		rec->h = 8;						/* B246-B249 */
		h += 8;

		shadow_text_draw(TEXT_COLOUR_NORMAL, x, rec->y,
		                 class_name_text(labels[i]));	/* B252-B26C */

		text_copy(buf, "  ");
		format_uint((uint16)(int8)codes[i], buf + 1);	/* B27D-B28E */
		shadow_text_draw((int16)g_vote_text_attr, x, rec->y, buf);	/* B291-B2A5 */
	}

	cur = -1;								/* B2AB-B2B1 */
	prev = -1;
	mouse_poll_moved(0);								/* B2B4-B2BA */

	while (mouse_button_read() == 0) {						/* B2BD-B2C4 */
		mouse_poll_moved(1);							/* B2C7-B2CE */
		if ((uint8)shadow_key_read() == KEY_ESC)				/* B2D1-B2DB */
			return -1;
		cur = rect_scan((int16)g_string_cursor, g_menu_boxes);		/* B2DE-B2EC */

		if (cur != prev) {						/* B2F2-B2F7 */
			shadow_video_flags(SHADOW_BLIT_FLUSH);					/* B2FA-B301 */
			if (cur != -1) {					/* B304-B308 */
				rec = RECT(g_menu_boxes + cur * RECT_STRIDE);		/* B30A-B31A */
				shadow_text_draw(TEXT_COLOUR_HIGHLIGHT,
				    ABS_SHR3((int16)(int16)rec->x),
				    rec->y, class_name_text(labels[cur]));	/* B31D-B344 */
			}
			if (prev != -1) {					/* B347-B34B */
				rec = RECT(g_menu_boxes + prev * RECT_STRIDE);		/* B34D-B35D */
				shadow_text_draw(TEXT_COLOUR_NORMAL,
				    ABS_SHR3((int16)(int16)rec->x),
				    rec->y, class_name_text(labels[prev]));	/* B360-B387 */
			}
			mouse_poll_moved(0);						/* B38A-B390 */
		}
		prev = cur;								/* B393-B396 */
	}

	if (mouse_button_read() & 2)							/* B39C-B3A3 */
		cur = -1;
	while (mouse_button_read() != 0)							/* B3A8-B3AF */
		;
	return cur;
}

/* 0x130E */

extern struct rect_rec g_hero_slots[];			/* 0x23a0 */
extern struct rect_rec g_hero_slot_boxes[];			/* 0xbda */
extern struct game_state g_game_state;			/* 0x2152 */
extern uint16 g_timer_debt_a, g_timer_debt_b, g_timer_debt_c, g_timer_debt_d;
extern uint16 g_overworld_hold, g_cursor_mode;
extern int16 shadow_video_flags(int16 v);
extern int16 shadow_video_page(int16 v);
extern int16 draw_text_window(int16 x, int16 y, int16 w, int16 h);
extern int16 shadow_text_draw(int16 attr, int16 col, int16 row, const char *text);
extern void draw_status_bar(int16 fraction, int16 x, int16 y);
extern void text_copy(char *dest, const char *src);
extern void format_uint(uint16 v, char *p);
extern void sprite_blit_offset(int16 x, int16 y, int16 id);
extern void draw_combat_menu(int16 count, int16 sel);
extern void draw_button_caption(struct rect_rec *rec, int16 n);
extern void draw_glyph_wait(const struct rect_rec *rec, int16 selected);
extern int16 rect_scan(int16 count, uint8 *table);
extern int16 mouse_button_read(void);
extern int16 shadow_key_read(void);
extern int16 mouse_poll_moved(int16 a);
extern int16 message_box(int16 box_style, int16 id);
extern int16 word_xor(int16 a, int16 b);
extern int16 box_menu(uint16 *labels, uint8 *codes, int16 n, int16 quiet);
extern void shadow_mouse_draw(uint16 a, uint16 b);

/* 0x130E */
int16 character_detail_panel(int16 a, int16 b, int16 c)
{
	struct character_rec *rec;
	uint8 *p8;
	const uint8 *plbl, *p2;
	uint16 *labels, *pw;
	uint8 scr[16];
	int16 kind, i, k, v, n, sel, res, rows, count, changed, flag;

	kind = (int16)(int16)g_hero_slots[a].state;
	labels = 0;
	p8 = 0;
	plbl = 0;
	p2 = 0;
	pw = 0;
	if (kind & RECT_STATE_GONE)
		return 0;
	if (kind == HERO_SLOT_NONE)
		return 0;
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	rec = character_at(g_record_addr[kind & 0xff]);

	if (b == 0) {
		(void)draw_text_window(4, 48, 32, 14);
		(void)shadow_text_draw(TEXT_COLOUR_HEADING, 0xa, 0x30,
		               CHARACTER_NAME(rec));
		for (i = 0; i != 10; i++) {
			(void)shadow_text_draw(TEXT_COLOUR_NORMAL, ((i & 1) ? 0x10 : 0) + 5,
			               ((i >> 1) * 8) + 0x3e,
			               class_name_text(P_NAME(i)));
		}
		p8 = rec->stats.attributes;
		for (i = 0; i != 6; i++)
			(void)draw_status_bar((int16)(int8)*p8++,
			               (i & 1) ? 24 : 8,
			               ((i >> 1) * 8) + 0x46);
		text_copy((char *)scr, "     ");
		v = (int16)rec->stats.armor_class;
		if (v < 0) {
			v = -v;
			scr[1] = '-';
		}
		format_uint((uint16)v, (char *)(scr + 3));
		(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 24, 0x5e, (char *)scr);
		(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 8, 0x5e,
		               "          ");
		text_copy((char *)scr, "     /    ");
		if (kind != HERO_SLOT_NONE) {
			format_uint((uint16)rec->hp,
			        (char *)(scr + 3));
			format_uint((uint16)rec->hp_max,
			        (char *)(scr + 9));
		}
		(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 8, 0x5e, (char *)scr);
		n = rec->hand_weapon.icon;
		if (n != 0) {
			sprite_blit_offset(0x48, 0x72, n);
			(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 0xb, 0x6e,
			               class_name_text(P_STR(n)));
		}
		n = rec->ranged_weapon.icon;
		if (n == 6)
			n++;
		if (n != 0) {
			sprite_blit_offset(0x78, 0x72, n);
			(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 0x11, 0x6e,
			               class_name_text(P_STR(n)));
			if (n == 3 && rec->action_timer != 0) {
				text_copy((char *)scr, "     ");
				format_uint((uint16)(int16)rec->action_timer,
				        (char *)(scr + 3));
				(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 0xf, 0x6b, (char *)scr);
				if (rec->action_code == ACTION_PAIR_IF_MODE3_0F)
					(void)shadow_text_draw(TEXT_COLOUR_NORMAL, 0x11, 0x73,
					               "+2");
			}
		}
		(void)shadow_text_draw(TEXT_COLOUR_HEADING, 5, 0x86, "Defaults");
	}

	switch (kind) {
	case CHAR_GOLMOON:
		labels = (uint16 *)((uint8 *)g_name_offset + 146);
		p8 = g_game_state.golmoon_spell_count;
		plbl = g_spell_levels[SPELLCASTER_GOLMOON];
		p2 = g_spell_caps[SPELLCASTER_GOLMOON];	pw = &g_timer_debt_c;
		break;
	case CHAR_RAISTLIN:
		labels = (uint16 *)((uint8 *)g_name_offset + 128);
		p8 = g_game_state.raistlin_spell_count;
		plbl = g_spell_levels[SPELLCASTER_RAISTLIN];
		p2 = g_spell_caps[SPELLCASTER_RAISTLIN];	pw = &g_timer_debt_a;
		break;
	case CHAR_GILTHANAS:
		labels = (uint16 *)((uint8 *)g_name_offset + 118);
		p8 = g_game_state.gilthanas_spell_count;
		plbl = g_spell_levels[SPELLCASTER_GILTHANAS];
		p2 = g_spell_caps[SPELLCASTER_GILTHANAS];	pw = &g_timer_debt_d;
		break;
	case CHAR_ELISTAN:
		labels = (uint16 *)((uint8 *)g_name_offset + 146);
		p8 = g_game_state.elistan_spell_count;
		plbl = g_spell_levels[SPELLCASTER_ELISTAN];
		p2 = g_spell_caps[SPELLCASTER_ELISTAN];	pw = &g_timer_debt_b;
		break;
	default:
		break;
	}

	if (b == 0) {
		count = (labels != 0) ? 5 : 3;
		rows = (labels != 0) ? 4 : 3;
		draw_combat_menu(count, MENU_SELECTION_NONE);
		draw_button_caption(RECT(g_btn_table), count);
	}
	while (mouse_button_read() != 0)
		;
	if (c != 0)
		(void)shadow_mouse_draw(
		    (uint16)((int16)(int16)RECT(g_btn_table)->x + 3),
		    (uint16)((int16)(int16)RECT(g_btn_table)->y + 3));

more:
	if (b != 0) {
		sel = 4;
	} else {
		for (sel = 0; sel != rows; sel++) {

			v = (int16)(int8)g_order_masks[sel]
			    & (int16)(int8)rec->orders;
			draw_glyph_wait(RECT(g_btn_table + (uint16)(sel * RECT_STRIDE)), v != 0);
		}
		while (mouse_button_read() == 0) {
			(void)mouse_poll_moved(1);
			if ((shadow_key_read() & 0xff) == KEY_ESC) {
				sel = -1;
				goto finish;
			}
		}
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
		sel = rect_scan(count, g_btn_table);
	}
	flag = 1;
	if (b != 0 && labels == 0)
		sel = -1;
	if (sel >= 0) {
		changed = 0;
		if (b == 0)
			draw_glyph_wait(RECT(g_btn_table + (uint16)(sel * RECT_STRIDE)), 1);
		RECT_STATE_HI(&g_hero_slot_boxes[a]) &= 0x7f;
		while (mouse_button_read() != 0)
			;
		if (sel <= 3) {
			v = (int16)(int8)g_order_masks[sel];
			rec->orders = (uint8)
			    word_xor((int16)(int8)rec->orders, v);	/* 1920-1929 */
			flag = 0;
			goto again;
		}
		if (sel == 4) {
			if (g_overworld_hold != 0) {
				(void)message_box(MSG_BOX_NOTICE, MSG_NO_MEMORIZE);
				goto drain;
			}
			if (kind == CHAR_ELISTAN || kind == CHAR_GOLMOON) {
				g_game_state.golmoon_spell_count[10] = 1;
				g_game_state.elistan_spell_count[10] = 1;
			} else if (kind == CHAR_RAISTLIN) {
				if (g_game_state.raistlin_spell_count[2] == 0)
					g_game_state.raistlin_spell_count[2]++;
				if (g_game_state.raistlin_spell_count[7] == 0
				    && g_game_state.spell_pick_pool != 0) {
					g_game_state.raistlin_spell_count[7]++;
					g_game_state.spell_pick_pool -= 1;
				}
				if (g_game_state.raistlin_spell_count[5] == 0
				    && g_game_state.spell_pick_pool != 0) {
					g_game_state.raistlin_spell_count[5]++;
					g_game_state.spell_pick_pool -= 1;
				}
			}
			for (;;) {
				res = box_menu(labels, p8, (int16)*pw, changed);
				changed = 1;
				if (res == -1)
					break;
				for (i = 0; i != 7; i++)
					scr[i] = 0;
				for (i = 10; i >= 0; i--) {
					k = (int16)(int8)plbl[i];
					scr[k] = (uint8)(scr[k] + p8[i]);
				}
				for (i = 0; i != 7; i++) {

					k = ((i == 6 ? 0 : p2[i]) != scr[i]) ? 1 : 0;
					scr[i] = (uint8)k;
				}
				if (scr[(int16)(int8)plbl[res]] != 0) {
					p8[res]++;
					*pw += (uint16)
					    ((int16)(int8)plbl[res] * 15);
				}
			}
		}
	}
drain:
	while (mouse_button_read() != 0)
		;
again:
	if (flag == 0)
		goto more;
finish:
	g_cursor_mode = CURSOR_MODE_DEFAULT;
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
	draw_hero_overlay();
	while (mouse_button_read() != 0)
		;
	return 0;
}

/* 0x106E */

const uint16 g_kind_ceiling[23] = {
	40, 100, 50, 30, 50, 35, 105, 60,
	25, 25, 45, 15, 35, 35, 35, 35,
	35, 35, 35, 35, 35, 35, 35,
};

extern int16 rect_scan(int16 count, uint8 *table);	/* 0x0CDC */

void redraw_rect_slot(void)
{
	(void)sprite_draw_current(SPRITE_PANEL_BACKGROUND, SPRITE_DRAW_SOLID, ((int16)g_panel_left >> 3) - 1, (int16)g_panel_top - 8);
}

/* 0x4A4B */

int16 character_command_panel(int16 slot, int16 mode, int16 flag)
{
	struct site_rec *rec;
	int16 q_kind;
	int16 r_kind;
	int16 code;
	int16 idx;
	int16 ox, oy;
	int16 count;
	int16 cursor;
	int16 x, y;
	struct actor_rec *win;

	while (mouse_button_read() != 0) { }			/* 4A57-4A5C */
	if (mode == 0) {				/* 4A60-4A76 */
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
		(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	}
	q_kind = (int16)(int16)g_hero_slots[slot].state;	/* 4A7E-4A8F */

	if (mode == 0) {
		if ((q_kind & RECT_STATE_GONE) != 0 || q_kind == HERO_SLOT_NONE) {	/* 4A98-4AA2 */
			(void)event_raise(EVENT_HERO_SLOT_EMPTY);
			(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
			return 0;
		}
		collect_matching_sites(&count, q_kind, 2, 0, 0);	/* 4ABD-4AD2 */
	} else {
		collect_matching_sites(&count, (int16)(int8)g_map_mode_char, 1, 0, 0);
	}

	if (count == 0) {
		(void)message_box(MSG_BOX_NOTICE, MSG_NO_ITEMS);
		if (mode == 0)
			(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
		return 0;
	}

	panel_layout(&ox, &oy, slot, count, flag);	/* 4B14-4B28 */
	cursor = 0;

	for (;;) {
		rec = &g_site_table[(int16)(int8)	/* 4B30-4B3F */
			(g_matching_site_list[cursor])];
		r_kind = (int16)SITE_KIND(rec);		/* 4B42-4B48 */
		far_pair_load(r_kind, g_objpic_at);
		x = ((int16)g_panel_stride_mode == 1 ? 0x10 : 10) + ox;	/* 4B56-4B6B */
		y = (((int16)0x20 - (int16)g_row_height) >> 1) + oy;	/* 4B6E-4B7A */
		sprite_draw_segment(x, y, r_kind, g_objpic_at);	/* 4B7D-4B8B */

		(void)mouse_poll_moved(0);
		while (mouse_button_read() == 0) {
			(void)mouse_poll_moved(1);
			if ((shadow_key_read() & 0xff) == KEY_ESC) {
				(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
				return 0;
			}
		}

		code = rect_scan(3, (uint8 *)g_panel_boxes);	/* 4BC0-4BCB */
		if (code < 0) {
			if (mode == 0)
				(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
			return 0;
		}
		x = (int16)g_panel_boxes[code].x >> 3;	/* 4BD8-4BE8 */
		y = (int16)g_panel_top2;
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
		if (code != 0) {			/* 4BFD-4C33 */
			(void)sprite_draw_current(code + SPRITE_SCROLL_ARROW_BASE, SPRITE_DRAW_SOLID, x, y);
			(void)mouse_poll_moved(0);
			while (mouse_button_read() != 0) { }
			(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
		}

		switch (code) {				/* 50EC-5100 */
		case USE_PANEL_COMMIT:
			(void)shadow_video_flags(SHADOW_BLIT_RESET);
			redraw_rect_slot();		/* 4C46-4C61 */
			g_mouse_x -= ((int16)g_panel_stride_mode << 3);	/* 4C64-4C6B */
			g_cursor_mode = (uint16)r_kind;
			(void)mouse_poll_moved(0);
			if (flag != 0) {
				while (mouse_button_read() == 0) { }
				do {
					(void)mouse_poll_moved(1);
				} while (mouse_button_read() == 0);
			} else {
				while (mouse_button_read() != 0)
					(void)mouse_poll_moved(1);
			}
			g_cursor_mode = CURSOR_MODE_DEFAULT;
			(void)mouse_poll_moved(0);
			(void)shadow_video_flags(SHADOW_BLIT_FLUSH);

			idx = rect_scan(4, (uint8 *)g_hero_slots);
			if (idx >= 0) {
				idx = (int16)(int16)g_hero_slots[idx].state;
				if ((idx & RECT_STATE_GONE) != 0 || idx == HERO_SLOT_NONE) {
					(void)event_raise(EVENT_HERO_SLOT_GONE);
					screen_refresh();
					redraw_rect_slot();
					continue;
				}
				apply_rect_action(idx, (int16 *)rec);	/* 4D32-4D38 */
				screen_refresh();
				redraw_rect_slot();
				(void)shadow_video_flags(SHADOW_BLIT_RESET);
				if (flag != 0)
					shadow_mouse_draw(g_panel_left + 0x10,
					                g_panel_top + 8);
				count = 0;
				cursor = 0;
				if (mode == 0)
					collect_matching_sites(&count, q_kind, 2, 0, 0);
				else
					collect_matching_sites(&count,
					    (int16)(int8)g_map_mode_char, 1, 0, 0);
				if (count == 0) {
					(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
					return 0;
				}
				continue;
			}

			idx = rect_scan(4, (uint8 *)g_rect_panel_boxes);
			if (idx >= 0) {
				if (g_actor_table[idx].state == 0) {
					(void)event_raise(EVENT_HERO_SLOT_EMPTY);
					screen_refresh();
					redraw_rect_slot();
					continue;
				}
				idx = (int16)(int16)g_hero_slots[idx].state;
				if ((idx & RECT_STATE_GONE) != 0 || idx == HERO_SLOT_NONE) {
					(void)event_raise(EVENT_HERO_SLOT_SLOT_EMPTY);
					screen_refresh();
					redraw_rect_slot();
					continue;
				}
				if (mode == 0 && idx == q_kind)
					continue;
				g_search_weight = 0;
				count = 0;
				if (mode == 0)
					collect_matching_sites(&count, idx, 2, 0, 0);
				else
					collect_matching_sites(&count,
					    (int16)(int8)g_map_mode_char, 1, 0, 0);
				g_search_weight += rec->weight;
				if ((int16)g_kind_ceiling[idx] >= (int16)g_search_weight) {
					(void)message_box(MSG_BOX_NOTICE, MSG_NO_CARRY);
					screen_refresh();
					redraw_rect_slot();
					continue;
				}
				if (rec->id == SITE_ID_KEY_RING) {
					show_text_panel(TEXT_KEY_RING);
					reveal_cells(g_reveal_cells_key_ring);
				} else if (rec->id == SITE_ID_KEY_RING_EVENT) {
					(void)event_raise(EVENT_KEY_RING_SITE);
				}
				rec->map = (uint16)idx;
				SITE_FLAG_LO(rec) &= 0xf8;
				SITE_FLAG_LO(rec) |= 2;
				(void)award_claim(7, WEAPON_RECORD_17);
				(void)award_claim(1, WEAPON_RECORD_9);
				count = 0;
				if (mode == 0)
					collect_matching_sites(&count, q_kind, 2, 0, 0);
				else
					collect_matching_sites(&count,
					    (int16)(int8)g_map_mode_char, 1, 0, 0);
				if (count == 0) {
					(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
					return 0;
				}
				if (cursor != 0)
					--cursor;
				continue;
			}

			code = rect_scan(4, (uint8 *)g_panel_boxes);
			if (code != 0) {
				win = &g_actor_table[slot];			/* 4FF3-4FF9 */
				record_set_mode(SITE(rec), (int16)win->x,
				        (int16)win->y,
				        (int16)win->mode_value);	/* 5001-500D */
				count = 0;
				if (mode == 0)
					collect_matching_sites(&count, q_kind, 2, 0, 0);
				else
					collect_matching_sites(&count,
					    (int16)(int8)g_map_mode_char, 1, 0, 0);
				if (count == 0) {
					(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
					return 0;
				}
				if (cursor != 0)
					--cursor;
			}
			continue;

		case USE_PANEL_UP:
			if (cursor != 0) {
				redraw_rect_slot();
				(void)shadow_video_flags(SHADOW_BLIT_RESET);
				(void)mouse_poll_moved(0);
				--cursor;
			}
			break;

		case USE_PANEL_DOWN:
			if (count - 1 != cursor) {
				redraw_rect_slot();
				(void)shadow_video_flags(SHADOW_BLIT_RESET);
				(void)mouse_poll_moved(0);
				++cursor;
			}
			break;

		default:
			break;
		}
	}
}

/* 0xC2A */

uint16 g_hot_spots[45] = {
	1, 0, 0, 16, 16,
	0, 0, 0, 16, 16,
	2, 0, 0, 16, 16,
	3, 0, 0, 16, 16,
	4, 0, 0, 16, 16,
	5, 0, 0, 16, 16,
	6, 0, 0, 16, 16,
	7, 0, 0, 16, 16,
	8, 0, 0, 16, 16,
};

extern int16 state_query(int16 slot);			/* 0x0AC7 */
extern void wait_key_drain(void);				/* 0x10FE */
extern int16 hero_window_point_draw(int16 slot, int16 kind);			/* 0x1136 */
extern int16 setting_apply_choice(int16 sel);				/* 0x265E */
extern int16 setting_apply_checked(int16 sel);				/* 0x28EF */
extern int16 box_menu(uint16 *labels, uint8 *codes, int16 n, int16 quiet);
extern void layout_nine_buttons(int16 origin_x, int16 origin_y);	/* 0xB57D */
extern void sprite_draw_dispatch(int16 a, int16 b, int16 c);		/* 0xA097 */
extern int16 effect_publish(int16, int16, int16, int16, int16, int16, int16, int16, int16, int16);
extern int16 sprite_draw_current(int16 sprite, int16 mode, int16 column, int16 row);
extern int16 shadow_video_flags(int16);
extern int16 mouse_poll_moved(int16);
extern int16 mouse_button_read(void);
extern int16 shadow_key_read(void);
extern int16 shadow_video_page(int16);
extern int16 rect_scan(int16 count, uint8 *table);	/* 0x0CDC */
extern uint16 g_hold_length, g_overworld_hold, g_screen_hold;
extern uint16 g_selected_hero_slot, g_golmoon_menu_choice, g_elistan_menu_choice, g_raistlin_menu_choice, g_cursor_mode;
extern uint16 g_search_center_x, g_search_center_y, g_cursor_tile_x, g_cursor_tile_y;
extern uint8 g_map_mode_char;
extern struct game_state g_game_state;			/* 0x2152 */
extern struct rect_rec g_hero_slot_boxes[];			/* 0xbda */
extern struct map_record g_map_working;	/* 0xa178 */
extern uint16 g_name_offset[];				/* 0x0d84 */
struct character_rec *character_at(uint16 addr);

/* 0xB5DF */

#include "structs.h"

/* 0xB5DF */
void tactical_command_panel(int16 slot, int16 command)
{
	struct actor_rec *win;
	struct character_rec *rec;
	uint16 *labels;
	uint8 *codes;
	int16 kind;
	int16 x, y;
	int16 sel;
	int16 picked;
	int16 taken;
	int16 w;

	labels = 0;
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);

	w = (int16)g_selected_hero_slot;
	(void)sprite_draw_current((int16)(uint8)g_hero_slots[w].state, SPRITE_DRAW_MASKED,
	               (int16)(int16)g_hero_slots[w].x >> 3,
	               (int16)g_hero_slots[w].y);
	w = slot;
	(void)sprite_draw_current(SPRITE_HERO_CURSOR, SPRITE_DRAW_SOLID,
	               (int16)(int16)g_hero_slots[w].x >> 3,
	               (int16)g_hero_slots[w].y);

	kind = (int16)(int16)g_hero_slots[slot].state;
	rec = CHAR_REC(character_at(g_record_addr[(int16)(uint8)kind]));

	if (kind == CHAR_GOLMOON) {
		g_game_state.golmoon_spell_count[10] = 1;	/* 0x2246 */
		labels = g_name_offset + 73;		/* 0xE16 */
		codes = g_game_state.golmoon_spell_count;	/* 0x223C */
	} else if (kind == CHAR_RAISTLIN) {
		labels = g_name_offset + 64;		/* 0xE04 */
		codes = g_game_state.raistlin_spell_count;	/* 0x2254 */
	} else if (kind == CHAR_GILTHANAS) {
		labels = g_name_offset + 59;		/* 0xDFA */
		codes = g_game_state.gilthanas_spell_count;	/* 0x225C */
	} else if (kind == CHAR_ELISTAN) {
		g_game_state.elistan_spell_count[10] = 1;	/* 0x2252 */
		labels = g_name_offset + 73;		/* 0xE16 */
		codes = g_game_state.elistan_spell_count;	/* 0x2248 */
	} else {
		labels = 0;
		codes = 0;
	}

	win = &g_actor_table[slot];
	if (command == QPANEL_CMD_NONE) {
		x = (int16)win->x + (int16)win->y + 8;
		y = ((int16)win->y >> 1)
		    - ((int16)win->x >> 1) + 0x24;
		if (x > PANEL_X_CAP)
			x = PANEL_X_CAP;
		x &= PIXEL_COLUMN_MASK;
		if (y < 8)
			y = 8;
		layout_nine_buttons(x + 6, y + 6);
		(void)sprite_draw_current(SPRITE_PORTRAIT_FRAME, SPRITE_DRAW_MASKED, x >> 3, y);
		sel = (int16)rec->hand_weapon.icon;
		if (sel != 0)
			sprite_blit_offset(x + 0xe, y + 0xe, sel);
		sel = (int16)rec->ranged_weapon.icon;
		if (sel == 6)
			++sel;
		if (sel != 0)
			sprite_blit_offset(x + 0x20, y + 0xe, sel);
		if (win->health != 0)
			sprite_draw_dispatch(x + 0x18, y + 6, SPRITE_ACTOR_ALIVE);
		if (labels != 0) {
			sprite_draw_dispatch(x + 0x2a, y + 6, SPRITE_ACTOR_SPELLBOOK);
			if (win->health != 0)
				sprite_draw_dispatch(x + 0x2a, y + 6, SPRITE_ACTOR_ALIVE);
		}
		while (mouse_button_read() != 0) { }
		while (mouse_button_read() == 0)
			(void)mouse_poll_moved(1);
		while (mouse_button_read() != 0) { }
		(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
		sel = rect_scan(9, (uint8 *)g_hot_spots);
	} else {
		sel = QPANEL_ACTION(command);
	}
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	if (sel != -1)
		RECT_STATE_HI(&g_hero_slot_boxes[slot]) &= 0x7f;

	switch (sel) {
	case QPANEL_HOLD:
		win->motion_state = ACTOR_MOTION_SEEK;
		break;

	case QPANEL_QUERY:
		if (win->health != 0)
			break;
		picked = state_query(slot);
		if (picked == 0)
			break;
		g_cursor_mode = CURSOR_MODE_TARGET;
		wait_key_drain();
		(void)hero_window_point_draw(slot, picked);
		win->health = (uint8)((int16)g_hold_length * 3);
		while (mouse_button_read() != 0) { }
		while (shadow_key_read() == 0) { }
		break;

	case QPANEL_MENU:
		if (win->health != 0 || labels == 0)
			break;
		taken = 0;
		if (kind == CHAR_RAISTLIN) {
			if (g_raistlin_menu_choice != 0) {
				picked = (int16)g_raistlin_menu_choice;
				g_raistlin_menu_choice = 0;
				taken = 1;
			}
		}
		if (!taken && kind == CHAR_GOLMOON && g_golmoon_menu_choice != 0) {
			picked = (int16)g_golmoon_menu_choice;
			taken = 1;
		}
		if (!taken && kind == CHAR_ELISTAN && g_elistan_menu_choice != 0) {
			picked = (int16)g_elistan_menu_choice;
			taken = 1;
		}
		if (!taken)
			picked = box_menu(labels, codes, BOX_MENU_NO_DRAW, 0);
		if (picked == -1)
			break;
		win->health = (uint8)((int16)g_hold_length * 3);
		codes += picked;
		if (*codes == 0)
			break;
		--*codes;
		if (kind == CHAR_GOLMOON || kind == CHAR_ELISTAN) {
			picked = setting_apply_choice(picked);
			if (picked == -1) {
				g_game_state.search_active = 1;	/* 0x227B */
				g_game_state.search_x =
					(int16)g_actor_table[slot].x;	/* 0x227C */
				g_game_state.search_y =
					(int16)g_actor_table[slot].y;	/* 0x227E */
				picked = 0;
			}
		} else if (kind == CHAR_RAISTLIN) {
			picked = setting_apply_checked(picked);
		} else if (kind == CHAR_GILTHANAS) {
			if (picked == 3)
				picked = 5;
			picked = setting_apply_checked(picked);
		}
		if (picked == 0)
			break;
		g_cursor_mode = CURSOR_MODE_TARGET;
		wait_key_drain();
		(void)hero_window_point_draw(slot, picked);
		while (mouse_button_read() != 0) { }
		break;

	case QPANEL_PICK:
		if (g_map_mode_char == MAP_STONE_FORT && g_site_table[SITE_SLOT_SLEIGH].id == SITE_ID_SLEIGH) {
			show_text_panel(TEXT_STONE_FORT);
		} else {
			g_search_center_x = (int16)g_actor_table[slot].x;
			g_search_center_y = (int16)g_actor_table[slot].y;
			player_pick_nearby(slot);
		}
		break;

	case QPANEL_USE:
		g_search_center_x = (int16)g_actor_table[slot].x;
		g_search_center_y = (int16)g_actor_table[slot].y;
		(void)character_command_panel(slot, 1, command);
		break;

	case QPANEL_MAP_EVENT:
		if (g_overworld_hold != 0) {
			if (kind == CHAR_TASSLEHOFF) {
				(void)event_raise(EVENT_HOLD_STARTED);
				g_screen_hold = 20 * (int16)g_hold_length;
			} else if (g_map_mode_char == MAP_NEIDAR_VILLAGE && g_game_state.map_event_done == 0) {
				show_text_panel(TEXT_NEIDAR_VILLAGE);
				g_map_working.exit_top_map = TERRAIN_RUNE_STONES;	/* 0xA1AD */
				g_map_records[MAP_NEIDAR_VILLAGE - 1].exit_top_map = TERRAIN_RUNE_STONES;
				g_actor_table[5].state = 0;	/* 0x29FE */
				g_actor_table[4].state = 0;	/* 0x29E0 */
			} else if (g_map_mode_char == MAP_NEIDAR_AUDIENCE && g_game_state.map_event_done == 0) {
				show_text_panel(TEXT_NEIDAR_AUDIENCE);
				g_map_working.exit_left_guard = 3;	/* 0xA1B2 */
				g_map_working.exit_left_map = TERRAIN_RUNE_STONES;	/* 0xA1B3 */
				g_actor_table[5].state = 0;
				g_actor_table[4].state = 0;
			} else {
				(void)event_raise(EVENT_HOLD_BLOCKED);
			}
		} else if (g_map_mode_char == MAP_WIZARD_EVENT) {
			if (kind == CHAR_FLINT) {
				(void)event_raise(EVENT_WIZARD_LEVEL_RESET);
				rec->stats.armor_class = 0;
			} else {
				(void)event_raise(EVENT_WIZARD_NO_RESET);
			}
		}
		if (g_map_mode_char == MAP_STONE_FORT && g_site_table[SITE_SLOT_SLEIGH].id == SITE_ID_SLEIGH) {
			screen_refresh();
			(void)event_raise(EVENT_SLEIGH_END);
			tile_set_position();
			(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
			walk_steps();
			(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
			show_text_panel(TEXT_SLEIGH_RIDE_END);
			terrain_change();
			g_site_table[SITE_SLOT_SLEIGH].id = 0;
		}
		break;

	case QPANEL_SELECT:
		g_selected_hero_slot = (uint16)slot;
		break;

	case QPANEL_GROUP_MOVE:
		win->motion_state = ACTOR_MOTION_GOTO;
		screen_refresh();
		g_cursor_mode = CURSOR_MODE_MOVE;
		wait_key_drain();
		for (sel = 0; sel != 4; ++sel) {
			RECT_STATE_HI(&g_hero_slot_boxes[sel]) &= 0x7f;
			g_actor_table[sel].motion_state = ACTOR_MOTION_GOTO;
			g_actor_table[sel].pixel_x =
			    (uint16)(((int16)g_cursor_tile_x << 4) + 8);
			g_actor_table[sel].pixel_y =
			    (uint16)(((int16)g_cursor_tile_y << 4) + 8);
		}
		break;

	case QPANEL_CONTROL:
		(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
		RECT_STATE_HI(&g_hero_slot_boxes[slot]) += 0x80;
		draw_hero_overlay();
		(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
		break;

	default:
		break;
	}

	if (command == QPANEL_CMD_NONE)
		while (mouse_button_read() != 0) { }
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	g_cursor_mode = CURSOR_MODE_DEFAULT;
	(void)mouse_poll_moved(0);
	(void)shadow_video_page(SHADOW_PAGE_PREVIOUS);
	draw_hero_gauges();
}

static const char g_cp_alpha[6][36] = {
	{
		42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
		54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
		66, 67, 250, 251, 252, 253, 254, 255, 0, 1, 2, 249,
	},
	".882=8>.5;;.;.>26*-99/" "*+*6=.55;12B7*",
	"C85281.>?78-<0.-0AB@2<288;*;5;*+;182",
	";;2,9=22>.5265162A*,0;255;>72668;220",
	"5**<;-*2252-<>0*<*..8.012<.522=.<828",
	",<1<2<.;;;7<1<<=<<771<17,@57;=.<<,.-",
};

static const uint8 g_cp_off[18] = {
	0x1C, 0x17, 0x19, 0x14, 0x12, 0x12, 0x10, 0x0E, 0x0B,
	0x09, 0x07, 0x05, 0x03, 0x01, 0x00, 0x22, 0x20, 0x1E,
};

static const uint8 g_cp_which[18] = {
	5, 3, 2, 5, 0, 4, 2, 3, 2, 5, 2, 3, 4, 5, 0, 2, 3, 2,
};

/* 0xA89C */
void copy_protection(void)
{
	static const char blank_prompt[16] = "Enter\r\r        ";
	char prompt[16];
	int16 attempt, r, a, b, pos;
	uint8 c1, c2, expected;
	int16 key;

	shadow_video_page(SHADOW_PAGE_DEFAULT);
	for (attempt = 0; attempt < 3; attempt++) {
		r = rnd(18);
		for (a = 0; a < 16; a++)
			prompt[a] = blank_prompt[a];

		a = rnd(36);
		b = rnd(36);

		c1 = (uint8)(g_cp_alpha[0][a] + 0x37);
		prompt[8] = (char)c1;
		if (c1 >= 'a')
			prompt[8] &= (char)0xdf;

		c2 = (uint8)(g_cp_alpha[0][b] + 0x37);
		prompt[9] = (char)c2;
		if (c2 >= 'a')
			prompt[9] &= (char)0xdf;

		prompt[0x0B] = ' ';
		format_uint((uint16)(r + 1), prompt + 0x0C);

		pos = (((g_cp_off[r] + a) - b + 0x24) % 0x24 + 0x19) % 0x24;
		expected = (uint8)(g_cp_alpha[g_cp_which[r]][pos] + 0x37);

		draw_text_window(15, 80, 10, 5);
		shadow_text_draw(TEXT_COLOUR_NORMAL, 0x10, 0x58, prompt);

		key = shadow_getkey_noecho();
		key |= 0x20;
		if ((uint16)(key & 0xFF) == expected) {
			shadow_video_page(SHADOW_PAGE_PREVIOUS);
			return;
		}
	}

	for (;;)
		;
}

/* 0x2A71 */
int16 monster_experience_list(void)
{
	extern int16 g_hw_config;
	int16 scroll = 0;
	int16 i, y, state;
	uint16 key;
	int16 dir = 0;
	char field[16];

	do {
		y = 0x30;
		for (i = 0; i < 10; i += 2) {
			(void)shadow_text_draw(TEXT_COLOUR_HIGHLIGHT, 6, y,
			                       class_name_text(g_name_offset[95 + i + scroll]));
			(void)shadow_text_draw(TEXT_COLOUR_HIGHLIGHT, 0x15, y,
			                       class_name_text(g_name_offset[96 + i + scroll]));
			y += 8;
		}

		y = 0x30;
		for (i = 0; i < 10; i += 2) {
			field[0] = ' ';
			field[1] = ' ';
			field[2] = ' ';
			field[3] = 0;
			format_uint(g_game_state.kill_count[5 + i + scroll],
			         field + 2);
			(void)shadow_text_draw(TEXT_COLOUR_HIGHLIGHT, 0x10, y, field);

			field[0] = ' ';
			field[1] = ' ';
			field[2] = ' ';
			field[3] = 0;
			format_uint(g_game_state.kill_count[5 + i + 1 + scroll],
			         field + 2);
			(void)shadow_text_draw(TEXT_COLOUR_HIGHLIGHT, 0x1F, y, field);
			y += 8;
		}

		if (scroll == 0) {
			union { uint32 l; uint16 w[2]; } total;
			uint16 lo, hi;

			total.l = 0;
			/* 0x00FF */
			(void)shadow_text_draw(TEXT_COLOUR_HIGHLIGHT, 6, 0x20,
			                       "Party Experience Points for\r     Combat:");

			for (i = 0; i < 10; i++) {
				state = (int16)(int16)g_hero_slots[i].state;
				if (state >= 0 && state < 0x17) {
					struct character_rec *rec =
						character_at(g_record_addr[state]);
					lo = (uint16)rec->experience;
					hi = (uint16)(rec->experience >> 16);
					total.w[0] += lo;
					if (total.w[0] < lo)
						total.w[1]++;
					total.w[1] += hi;
				}
			}

			for (i = 0; i < 10; i++)
				field[i] = ' ';
			field[10] = 0;
			format_ulong(total.l, field + 9);
			(void)shadow_text_draw(TEXT_COLOUR_HIGHLIGHT, 0x12, 0x28, field);
		}

		if (g_hw_config != HW_CONFIG_21)
			(void)shadow_page_flip();

		do {
			key = (uint16)shadow_key_read();
		} while (key == 0);

		key &= 0xFF;

		if (dir != 0) {
			if (dir < 1) {
				if (scroll > 0)
					scroll -= 2;
			} else if (scroll < 8) {
				scroll += 2;
			}
		}
	} while (key != KEY_R && key != KEY_ESC);
	return 0;
}

/* 0xB4BA */

int16 event_raise(int16 v)
{
	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);
	(void)shadow_video_page(SHADOW_PAGE_DEFAULT);
	show_text_panel(v);
	return shadow_video_page(SHADOW_PAGE_PREVIOUS);
}
