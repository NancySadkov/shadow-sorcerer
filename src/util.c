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

extern int16 shadow_region_clear(int16 and_mask, int16 column, int16 row, int16 width_bytes, int16 height_rows);	/* 0xD45D */
extern int16 shadow_text_draw(int16 attr, int16 col, int16 row, const char *text);		/* 0xD340 */

/* 0x0EE0 */

void text_copy(char *dest, const char *src)
{
	while ((*dest = *src) != 0)
	{
		dest++;
		src++;
	}
}

extern int16 shadow_region_clear(int16 and_mask, int16 column, int16 row, int16 width_bytes, int16 height_rows);	/* 0xD45D */
extern int16 shadow_video_page(int16);					/* 0xD443 */
extern uint8 g_rect_menubar[];			/* 0x0b4e */
extern int16 sprite_draw_current(int16 sprite, int16 mode, int16 column, int16 row);			/* 0x9CA9 */
extern void sprite_draw_anchored(int16, int16, int16);				/* 0xA206 */
extern uint16 g_clock_hour;					/* 0x2286 */

const int8 g_ofs_odd_ring[38] = {		/* 0x0fda */
	0, 0, 0, -1, 1, 0, 0, 1, -1, 1,
	-1, 0, -1, -1, 0, -2, 1, -2, 1, -1,
	2, 0, 1, 1, 1, 2, 0, 2, -1, 2,
	-2, 1, -2, 0, -2, -1, -1, -2,
};

const int8 g_ofs_even_ring[38] = {	/* 0x1024 */
	0, 0, 0, -1, 1, -1, 1, 0, 1, 1,
	0, 1, -1, 0, 0, -2, 1, -2, 2, -1,
	2, 0, 2, 1, 1, 2, 0, 2, -1, 2,
	-1, 1, -2, 0, -1, -1, -1, -2,
};

/* 0x0EAA */

const int8 g_pair_data_0eaa[52] = {
	0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, 1, 0, -1, -1,
	0, 0, 0, 0, 0, 0, 1, 0, -1, -1, 1, 0, 1, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0,
};

/* 0x0F00 */

const int8 g_pair_data_0f00[24] = {
	2, 10, 0, 0, 8, 1, 9, 0, 0, 0, 3, 0,
	7, 11, 13, 5, 12, 6, 4, 0, 0, 16, 0, 0,
};

/* 0x1000 */

const int8 g_ofs_odd_ring3[36] = {	/* 0x1000 */
	0, -3, 1, -3, 2, -2, 2, -1, 3, 0, 2, 1,
	2, 2, 1, 3, 0, 3, -1, 3, -2, 3, -2, 2,
	-3, 1, -3, 0, -3, -1, -2, -2, -2, -3, -1, -3,
};

const int8 g_ofs_even_ring3[36] = {	/* 0x104A */
	0, -3, 1, -3, 2, -3, 2, -2, 3, -1, 3, 0,
	3, 1, 2, 2, 2, 3, 1, 3, 0, 3, -1, 3,
	-2, 2, -2, 1, -3, 0, -2, -1, -2, -2, -1, -3,
};

/* 0xB20 */

char *g_menubar_labels[24] = {
	"Options", "Party", "Refugee", "Spells", "Speed", "About",
	"---------", "Heal", "Regroup", "---------", "Load Game", "Save Game",
	"---------", "Quit", "", "Move", "Status", "",
	"Attack", "Ranged", "Flee", "Cast", "Memorize",
	0
};

/* 0xEFF */

int16 text_length(char *s)
{
	char *p = s;				/* F05-F08 */

	while (*p) {				/* F0B-F16 */
		p++;
	}
	return (int16)(p - s);			/* F18-F1B */
}

/* 0xD42 */

char *g_messages[33] = {
	"Shadow Sorceror\r\rVersion UK(B) 1.0", "Joystick Calibration:", "Ensure NUM LOCK key is on", "WRITE DISK ERROR\r Please check disk\r before retrying", "FILE READ ERROR\rUnable to read selected file", "Refugees",
	"HIGH SCORES", "Please insert a 'save game'\r  disk in current drive.\r Press button when ready", "Untitled", "IBM joystick Port 'A'\rTANDY Joystick LEFT socket\rCenter joystick then press\rENTER key\rPress ESC to abort", "Please insert 'DATA Disk'\r   in current drive.\rPress button when ready", "Please insert 'GAME Disk'\r   in current drive.\rPress button when ready",
	"You failed in your quest\rScore: 0", "Alert:", "Shadow Sorcerer", "Selected character cannot\rcarry any more items", "There are no items\ravailable", "You cannot carry any more\ritems",
	"Backpack contains nothing.", "You must be with some refugees\rto use this command", "You must be in\rwilderness mode to\rmove the refugees", "Only Raistlin can use\rthis item", "Only Goldmoon can use\rthis item", "You cannot memorize spells\rwhile in combat",
	"Vote to:", "go back", "stop here", "continue", "leave refugees", " ",
	" ", "  Game paused  ", "Graham E Lilley"
};							/* 0x0d42 */

/* 0x8D77 */

struct credits_text {
	char block_a[228];	/* 0x8D77-0x8E5B */
	char block_b[228];	/* 0x8E5B-0x8F3F */
	char block_c[228];	/* 0x8F3F-0x9023 */
	char block_d[39];	/* 0x9023-0x904A */
};

struct credits_text credits_08d77 = {	/* 0x8D77 */
	/* 0x8D77-0x8E5B */
	"           Shadow Sorcerer           \r"
	"(c) 1991 TSR, Inc All Rights Reserved\r"
	" (c) 1991 Strategic Simulations Inc  \r"
	"         All Rights Reserved         \r"
	"      based on Dragons of Hope       \r"
	"          by Tracy Hickman           \r",
	/* 0x8E5B-0x8F3F */
	"                                     \r"
	"           SSI Developers            \r"
	" George MacDonald    Jeff Groteboer  \r"
	"                                     \r"
	"      Program by  U.S. Gold Ltd      \r"
	"    Project Manager  Mike Wilding    \r",
	/* 0x8F3F-0x9023 */
	"      Programming Graham Lilley      \r"
	"             Graphics by             \r"
	" Dimentia Design    Arc Developments \r"
	"  Encounter design by Paul Cockburn  \r"
	"   Additional design and testing by  \r"
	" Graham Lilley  Bob Malin  Kev Bulmer\r",
	/* 0x9023-0x904A */
	"   click mouse button to continue    \r"
	"\x00",
};

/* 0xE37 */

void text_measure(char *s, int16 *w, int16 *h)
{
	int16 len = 0;					/* E3D-E45 */
	int16 c;

	*w = 0;
	*h = 1;						/* E47-E4A */
	while (*s) {					/* E4E-E54 */
		c = *s++;

		if (c == CHAR_CR) {
			(*h)++;			/* E5E-E61 */
			if (*w < len)		/* E63-E6D */
				*w = len;
			len = 0;
		} else {
			len++;
		}
	}
	if (*w < len)
		*w = len;
}

/* 0xEB9A */

void mul16(uint16 a, uint16 b, uint16 *plo, uint16 *phi)
{
	uint16 al = a & 0xff, ah = a >> 8;
	uint16 bl = b & 0xff, bh = b >> 8;
	uint16 p0 = al * bl, p1 = ah * bl, p2 = al * bh, p3 = ah * bh;
	uint16 mid = (p0 >> 8) + (p1 & 0xff) + (p2 & 0xff);

	*plo = (mid << 8) | (p0 & 0xff);
	*phi = p3 + (p1 >> 8) + (p2 >> 8) + (mid >> 8);
}

/* 0xEB9A */
void long_mul(uint16 xl, uint16 xh, uint16 yl, uint16 yh,
              uint16 *plo, uint16 *phi)
{
	uint16 tlo, thi, cx;

	if (xh == 0 && yh == 0) {
		mul16(xl, yl, plo, phi);				/* EBAA-EBB2 */
		return;
	}

	mul16(xh, yl, &tlo, &thi);
	cx = tlo;
	mul16(xl, yh, &tlo, &thi);
	cx += tlo;
	mul16(xl, yl, plo, phi);
	*phi += cx;
}

/* 0xDEA8 */

/* 0xDEA8 */
void word_mul(uint16 *acc, uint16 yl, uint16 yh)
{
	uint16 lo, hi;

	long_mul(acc[0], acc[1], yl, yh, &lo, &hi);			/* DEAE-DEB9 */
	acc[0] = lo;
	acc[1] = hi;
}

/* 0xF24 */

/* 0x0F24 */
void format_uint(uint16 v, char *p)
{
	do {
		*p = (char)(v % 10 + 0x30);
		p--;
		v /= 10;					/* F3C-F43 */
	} while (v != 0);					/* F46-F48 */
}

/* 0xF4E */

uint16 div32_by_10(uint16 *plo, uint16 *phi)
{
	union dword v;
	uint16 q[4];
	uint16 x, rem = 0;

	v.w[0] = *plo;
	v.w[1] = *phi;

	x = v.b[3];                  q[0] = x / 10; rem = x % 10;
	x = (rem << 8) | v.b[2];     q[1] = x / 10; rem = x % 10;
	x = (rem << 8) | v.b[1];     q[2] = x / 10; rem = x % 10;
	x = (rem << 8) | v.b[0];     q[3] = x / 10; rem = x % 10;

	*phi = (q[0] << 8) | q[1];
	*plo = (q[2] << 8) | q[3];
	return rem;
}

/* 0x0F4E */
void format_ulong(uint32 v, char *p)
{
	union dword u;

	u.l = v;
	do {
		uint16 rem = div32_by_10(&u.w[0], &u.w[1]);
		*p = (char)(rem + 0x30);					/* F60-F68 */
		p--;
	} while (u.w[0] != 0 || u.w[1] != 0);
}

/* 0x1E50 */

struct roll_table g_roll_table = {

	{
		{   2,   0,   8,   1,   4,   5,   3 },
		{   4,   0,   4,   1,   2,   5,   4 },
		{   3,   0,   6,   1,   4,   5,   3 },
		{   8,   0,   8,   3,   0,   1,   3 },
		{   6,   0,  13,   2,   1,   1,   3 },
		{   2,   2,  11,   2,   7,   1,   1 },
		{   1,   1,   8,   1,   5,   1,   1 },
		{   1,   0,   6,   1,   6,   1,   0 },
		{   6,   6,   3,   3,   4,   3,   6 },
		{   8,   0,  14,   3,   3,   2,   6 },
		{   2,   0,   8,   1,   8,   1,   3 },
		{   4,   3,   4,   1,   5,   1,   4 },
		{   2,   0,   6,   1,   4,   1,   4 },
		{  10,   0,  36,   5,   2,   2,   6 },
		{   1,   0,   6,   3,   4,   0,   4 },
		{   4,   1,   3,   1,   8,   0,   1 },
		{   0,   0,   4,   1,   8,   0,   3 },
		{   1,   0,   8,   1,   9,   0,   0 },
		{   8,   0,   8,   1,   9,   0,   0 },
		{  14,   0,   8,   1,   9,   0,   0 },
		{   3,   1,   8,   1,   9,   0,   0 },
		{   3,   3,   8,   1,   9,   0,   0 },
		{   5,   0,   8,   1,   9,   0,   0 },
		{   1,  99,   8,   1,   9,   0,   0 },
		{   1,  55,   8,   1,   9,   2,   0 },
		{   1,  74,  36,   5,   2,   2,   6 },
		{   1,  74,  36,   5,   2,   2,   6 },
		{   1,  74,  36,   5,   2,   2,   6 },
	},

	{
		 24,  23,  22,  21,  20,  20,  20,  20,  20,  20,
	},

	{
		 19,  18,  17,  16,  15,  14,  13,  12,  11,  10,   9,   8,   7,   6,   5,   4,
		  3,   2,   1,   0, 255, 254, 253,   0,
	},

	{
		  0,   0,   0,   0,   2,   2,   2,   4,   4,   4,   6,   6,   6,   0,
	},

	{
		  0,   0,   0,   0,   0,   0,   2,   2,   2,   2,   2,   5,   5,   0,
	},

	{
		  0,   1,   1,   3,   3,   5,   5,   7,   7,   9,   9,  11,  11,   0,
	},

	{
		  0,   0,   0,   0,   0,   2,   2,   2,   2,   5,   5,   5,   5,   0,  35,  18,
		 12,   0,
	},

	{
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  0,   1,   1,   1,   2,   2,   3,   3,   4,   0,
	},

	{
		  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		  1,   2,   2,   3,   3,   4,   4,   5,   5,   0,
	},

	{
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
		{ { 249, 251, 253, 255,   0,   0,   1,   1,   1 },   1,   6,   1,   6 },
		{ { 251, 253, 255,   1,   2,   2,   3,   3,   3 },   1,   8,   1,   8 },
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
		{ { 254, 255,   0,   0,   0,   0,   0,   1,   2 },   1,   8,   1,  12 },
		{ { 255,   0,   1,   2,   2,   3,   3,   3,   4 },   4,  10,   4,   9 },
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
		{ { 250, 252, 254,   0,   1,   2,   3,   2,   4 },   3,   8,   3,   6 },
		{ {   5,   5,   5,   5,   6,   6,   6,   4,   3 },   4,  13,   6,  19 },
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
		{ { 255,   0,   1,   1,   1,   1,   1,   2,   3 },   2,   9,   2,  13 },
		{ { 254,   1,   0,   0,   1,   1,   2,   2,   3 },   2,   9,   2,   9 },
		{ {   0,   1,   2,   2,   2,   2,   2,   3,   4 },   3,  10,   3,  14 },
		{ { 251, 253, 255,   1,   2,   2,   3,   3,   3 },   3,   8,   3,   8 },
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
		{ {   0,   1,   2,   2,   3,   3,   4,   4,   5 },   4,  11,   4,  11 },
		{ {   5,   5,   5,   5,   6,   6,   6,   4,   3 },   4,  13,   6,  19 },
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
		{ {   0,   0,   0,   0,   0,   0,   0,   0,   0 },   0,   1,   0,   1 },
	}
};

/* 0x7EB */

int16 roll_dice_amount(int16 i)
{
	int16 dice, bonus, sum = 0, n;
	struct roll_entry *r = &g_roll_table.rolls[i];		/* 7FB-808 */

	dice = (int16)r->dice;
	if (dice == 0) {						/* 810-812 */
		dice = ROLL_D4();					/* 814-81F */
	}

	bonus = (int16)r->bonus;					/* 822-834 */

	for (n = 0; n != dice; ++n) {					/* 83C-857 */
		sum += ROLL_D6();
	}

	sum += bonus;							/* 859-85C */
	if (sum <= 0)							/* 85F-865 */
		sum = 1;
	return sum;
}

/* 0x9BD2 */
int16 word_xor(int16 a, int16 b)
{
	return (int16)(a ^ b);
}

/* 0x0EBB */

void mem_copy_bytes(uint8 *dst, const uint8 *src, int16 n)
{
	while (n-- != 0) {
		*dst++ = *src++;						/* EC9-ED9 */
	}
}

extern uint16 g_sound_request;				/* 0x9F6A */

/* 0x07D6 */

/* 0x07D6 */
void sound_request_max(int16 request)
{
	if (request > (int16)g_sound_request)
		g_sound_request = (uint16)request;	/* 07E1-07E4 */
}

/* 0xA875 */

/* 0xA875 */
void mem_zero(uint8 *p, int16 n)
{
	int16 i;

	for (i = 0; i != n; ++i) {				/* A885-0A8B */
		*p = 0;						/* A88D-0A93 */
		++p;
	}
}

/* 0x0E8B */

int16 text_equal(uint8 *a, uint8 *b)
{
	uint8 c;

	while ((c = *a) == *b) {			/* 0E9C-0EA6 */
		if (c == 0)				/* 0EA8-0EAE */
			return 0;
		++a;					/* 0E93-0E96 */
		++b;
	}
	return 1;					/* 0EA1-0EA6 */
}

/* 0x9BC4 */

int16 abs_w(int16 v)
{
	if (v < 0)
		return (int16)-v;
	return v;
}

/* 0xA232 */

int16 isqrt_word(uint16 lo, uint16 hi)
{
	uint16 seed = (((lo >> 8) | (lo & 0xffu)) & 0xffu);
	int16 d;

	(void)hi;					/* 0xA23B */

	if (seed == 0)					/* 0xA248-0xA24B */
		seed = 1;

	for (;;) {
		d = (int16)((lo / seed) - seed);	/* 0xA254-0xA256 */

		if (d == 0 || d == 1 || d == -1)		/* 0xA258-0xA260 */
			break;

		seed = (uint16)((int16)seed + (d >> 1));	/* 0xA262-0xA264 */
	}

	return (int16)seed;				/* 0xA26C */
}
