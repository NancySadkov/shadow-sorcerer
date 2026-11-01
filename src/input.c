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

uint16 g_mouse_last_x, g_mouse_last_y, g_cursor_mode;

int16 sprite_draw_far(int16 id, int16 x, int16 y);		/* 0x9D07 */

/* 0x49BC */

int16 mouse_poll_moved(int16 a)
{
	shadow_mouse_read();
	if (a != 0 && g_mouse_last_x == g_mouse_x && g_mouse_last_y == g_mouse_y)
		return 0;

	g_mouse_last_x = g_mouse_x;
	g_mouse_last_y = g_mouse_y;
	if ((int16)g_mouse_x < 0)     g_mouse_x = 0;
	if ((int16)g_mouse_y < 0)     g_mouse_y = 0;
	if ((int16)g_mouse_x > SCREEN_MAX_X) g_mouse_x = SCREEN_MAX_X;
	if ((int16)g_mouse_y > SCREEN_MAX_Y)  g_mouse_y = SCREEN_MAX_Y;
	return sprite_draw_far((int16)g_cursor_mode, (int16)g_mouse_x, (int16)g_mouse_y);
}

extern uint16 g_mouse_x;           /* 0xa9c2 */
extern uint16 g_mouse_y;           /* 0xa9c4 */

/* 0xCDC */

int16 rect_scan(int16 count, uint8 *t)
{
	int16 i;
	for (i = 0; i != count; ++i, t += 10) {
		int16 dx = (int16)g_mouse_x - (int16)RECT(t)->x;
		int16 dy = (int16)g_mouse_y - (int16)RECT(t)->y;
		if (dx < 0)
			continue;
		if (dy < 0)
			continue;
		if ((int16)RECT(t)->w < dx)
			continue;
		if ((int16)RECT(t)->h < dy)
			continue;
		return i;
	}
	return -1;
}

int16  character_detail_panel(int16 a, int16 b, int16 c);
int16  character_command_panel(int16 slot, int16 mode, int16 flag);
/* 0xB4E7 */

int16 hit_test_buttons(void)
{
	int16 best = -1;
	int16 best_y = 0;
	int16 i;

	for (i = 0; i != 5; i++) {
		struct actor_rec *r = &g_actor_table[i];
		int16 x, y;

		if (r->state == 0)
			continue;
		x = (int16)r->x + (int16)r->y - 8;
		y = ((int16)r->y >> 1)
		    - ((int16)r->x >> 1) + 0x7c;            /* B524-B533 */

		if (x >= (int16)g_mouse_x)
			continue;
		if (x + 16 <= (int16)g_mouse_x)
			continue;
		if (y <= (int16)g_mouse_y)
			continue;
		if (y - 24 >= (int16)g_mouse_y)
			continue;
		if (y > best_y) {
			best = i;
			best_y = y;
		}
	}
	return best;
}

extern int16 mouse_button_read(void);					/* 0x4A36 */
extern int16 shadow_poll(void);					/* 0xD374 */
extern void camera_publish_cells(int16);					/* 0x5B50 */

/* 0x10FE */

/* 0x10FE */
void wait_key_drain(void)
{
	screen_refresh();
	(void)mouse_poll_moved(0);
	while (mouse_button_read() == 0)						/* 110D-1112 */
		(void)mouse_poll_moved(1);
	camera_publish_cells(0);								/* 1120-1126 */
	while (shadow_poll() != 0)
		;
}

/* 0x5D9C */

int16 read_3digit(void)
{
	int16 d1 = (int16)(int8)((uint8)shadow_getkey_noecho() - 0x30);	/* 5DA2-5DA7 */
	int16 d2 = shadow_getkey_noecho() - 0x30;		/* 5DB8-5DBB */
	int16 d3 = shadow_getkey_noecho() - 0x30;		/* 5DCA-5DCD */

	return ((d1 * 10) + d2) * 10 + d3;			/* 5DAD-5DD0 */
}

/* 0x5DDC */

int16 read_2digit(void)
{
	int16 c = shadow_getkey_noecho();			/* 5DE2-5DE5 */
	int16 val;

	if ((uint8)c == KEY_MINUS)
		return -1;

	val = (int16)(int8)c * 10 - CHAR_ZERO * 10;		/* 5DF1-5DFF */
	return val + (shadow_getkey_noecho() - CHAR_ZERO);	/* 5E05-5E0B */
}

uint16 g_sticky_click_flag;				/* 0xa288 */

/* 0x1EF2 */

void hero_slot_click_arm(void)
{
	struct rect_rec *base = g_hero_slots;
	int16 sel;

	screen_refresh();
	g_cursor_mode = CURSOR_MODE_TARGET;

	while (mouse_button_read() != 0)				/* 1F07-1F0E */
		;

	while (mouse_button_read() == 0)
		(void)mouse_poll_moved(1);

	(void)shadow_video_flags(SHADOW_BLIT_FLUSH);				/* 1F23-1F2A */

	while (mouse_button_read() != 0)				/* 1F2D-1F34 */
		;

	g_cursor_mode = CURSOR_MODE_DEFAULT;

	sel = rect_scan(4, (uint8 *)base);
	if (sel < 0)					/* 1F4D-1F51 */
		return;

	if ((int16)(base + sel)->state < 0)	/* 1F53-1F68 */
		g_sticky_click_flag = 1;
	if ((int16)(base + sel)->state > 0x16)
		g_sticky_click_flag = 1;

	if ((int16)g_sticky_click_flag != 0)
		return;

	g_actor_table[sel].timer = TICKS_PER_SECOND;
}

/* 0x4A36 */
int16 mouse_button_read(void)
{
	shadow_mouse_read();
	return (int16)g_mouse_button;
}
