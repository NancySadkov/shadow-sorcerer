/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"
#include <dos.h>
#include <stdio.h>
#include <io.h>
#include "shadow.h"
#include "structs.h"

#include "game.h"

uint16 g_text_msg_offset;

/* 0xD0A4 */

/* 0xD0C9 */

/* 0xD108 */
/* 0xD2C7 */
/* 0xD309 */
/* 0x9A9D */

uint16 g_video_mode, g_video_mode_saved;      /* 0xa2ac */
uint16 g_alloc_mark;          /* 0xa8ac */
FILE *g_text_stream;                 /* 0xa2ce */
uint16 g_sprite_record_off;                 /* 0xa2d0 */
extern void show_text_panel(int16);			/* 0x47B2 */
extern void draw_hero_gauge(int16 i);					/* 0x0C0 */
extern int16 shadow_rect_fill(uint16 a, uint16 b, uint16 c, uint16 d);
extern struct rect_rec g_hero_slots[];

char g_name_vmpl[10] = "vmpl0.dat";    /* DGROUP 0xaf4 */
char g_name_vmon[10] = "vmona.dat";    /* DGROUP 0xafe */
uint8 g_vmpl_slot[4];          /* DGROUP 0xa9bc */
uint16 g_vmpl_para[4];               /* DGROUP 0x9f2e */

/* 0x004B */

void load_pane_maps(void)
{
	int16 i;

	for (i = 0; i != 4; i++) {
		uint8 c = (uint8)g_hero_slots[i].state;

		g_name_vmpl[4] = (char)(c + 'A');
		g_vmpl_slot[i] = (uint8)i;
		if (c != HERO_SLOT_NONE) {
			while ((uint16)shadow_load(g_name_vmpl, g_vmpl_para[i]) == 0)
				(void)message_box(MSG_BOX_ERROR, MSG_GAME_DISK);
		}
	}
}

FILE *g_map_file;                    /* 0xa9fe */

struct map_record g_map_records[MAP_RECORD_COUNT];	/* 0x3060 */
uint16 g_text_index[250];			/* 0x9c10 */

/* 0x2F50 */

struct map_record g_map_record_0;

/* 0x981F */
void map_clear_cell_kinds(int16 rec_no)
{
	struct map_record *rec;
	int16 i;

	if (rec_no == 0)
		rec = &g_map_record_0;
	else
		rec = &g_map_records[rec_no - 1];

	for (i = 0; i != MAP_GRID_CELLS; i++)
		rec->grid[i].cell.part.kind = 0;
}

/* 0x5C40 */

void load_map_records(void)
{
	g_map_file = fopen("maped", "rb");
	fread(g_map_records, MAP_RECORD_SIZE, MAP_RECORD_COUNT, g_map_file);
	fclose(g_map_file);
}

/* 0x5C75 */

void map_export_debug(void)
{
	uint16 i;
	struct map_record *rec;
	uint16 w;
	uint8 b;
	uint16 j;

	g_map_file = fopen("maped.st", "wb");
	for (i = 0, rec = g_map_records; i != MAP_RECORD_COUNT; i++, rec++) {
		w = rec->header_word;
		b = (uint8)(w >> 8); (void)fwrite(&b, 1, 1, g_map_file);
		b = (uint8)w;        (void)fwrite(&b, 1, 1, g_map_file);
		w = rec->free_list;
		b = (uint8)(w >> 8); (void)fwrite(&b, 1, 1, g_map_file);
		b = (uint8)w;        (void)fwrite(&b, 1, 1, g_map_file);
		(void)fwrite((uint8 *)rec + 4, 1, 0x44, g_map_file);
		for (j = 0; j != PERCENT; j++) {
			w = rec->grid[j].cell.value;
			b = (uint8)(w >> 8); (void)fwrite(&b, 1, 1, g_map_file);
			b = (uint8)w;        (void)fwrite(&b, 1, 1, g_map_file);
		}
	}
	fclose(g_map_file);
}

uint16 g_text_msg_offset_default = TEXT_MSG_OFFSET_DEFAULT;			/* 0xd58 */

extern int16 mouse_button_read(void);			/* 0x4A36 */
extern uint16 g_mem_next;			/* 0xa9c8 */

/* 0x6144 */

void load_pane_blocks(void)
{
	uint8 c;
	uint16 base, delta;
	uint32 n;
	int16 i;

	base = (uint16)g_mem_next;			/* 614A-614D */
	g_vmpl_para[0] = base;

	c = (uint8)g_hero_slots[0].state;				/* 6153-6156 */
	g_name_vmpl[4] = (char)(c + 'A');			/* 615B-615D */
	g_vmpl_slot[0] = 0;

	while ((n = shadow_load(g_name_vmpl, (uint16)g_mem_next)) == 0)	/* 6164-6175 */
	{
		shadow_video_page(SHADOW_PAGE_DEFAULT);			/* 617B-6181 */
		shadow_text(SHADOW_TEXT_PAGE_3, DISK_PROMPT_COL, DISK_PROMPT_ROW,
		            g_text_msg_offset_default);		/* 6184-618D */
		do {
			if (shadow_poll() != 0)		/* 619A-619F */
				break;
		} while (mouse_button_read() == 0);			/* 61A1-61A6 */
		while (mouse_button_read() != 0)
			;
		shadow_video_page(SHADOW_PAGE_PREVIOUS);			/* 61B3-61BA */
	}

	g_mem_next += ((uint16)n >> 4) + 1;			/* 61BF-61C7 */
	delta = (uint16)g_mem_next - base;		/* 61CB-61D1 */

	g_vmpl_para[1] = (uint16)g_mem_next;
	g_vmpl_para[2] = delta + g_vmpl_para[1];
	g_vmpl_para[3] = delta + g_vmpl_para[2];
	g_mem_next = delta + g_vmpl_para[3];		/* 61EE-61F5 */

	for (i = 1; i != 4; i++)			/* 61F8-6206 */
	{
		c = (uint8)g_hero_slots[i].state;
		g_name_vmpl[4] = (char)(c + 'A');		/* 621E-6220 */
		g_vmpl_slot[i] = (uint8)i;		/* 6223-6229 */
		(void)shadow_load(g_name_vmpl, g_vmpl_para[i]);
	}
}

/* 0xD75E */

extern uint16 g_mouse_x;			/* 0xa9c2 */
extern uint16 g_mouse_y;			/* 0xa9c4 */

/* 0xA270 */

void ensure_text_open(void)
{
	while ((g_text_stream = fopen("text.dat", "r+b")) == 0)
		message_box(MSG_BOX_ERROR, MSG_GAME_DISK);
}

char g_name_vcin[10] = "vcin0.dat";    /* DGROUP 0x262 */

/* 0x9852 */

void load_vcin_clip(int16 which)
{
	int16 retried = 0;
	uint16 ok;

	g_name_vcin[4] = (char)(which + 0x30);		/* 985D-9862 */

	while ((ok = (uint16)shadow_asset_load(g_name_vcin, g_vmpl_para[0])) == 0) {	/* 9865-9876 */
		(void)message_box(MSG_BOX_ERROR, MSG_DATA_DISK);				/* 987A-9885 */
		retried = 1;
	}
	if (retried != 0)
		(void)message_box(MSG_BOX_ERROR, MSG_GAME_DISK);				/* 9899-98A4 */
}

extern uint16 g_mem_next;			/* 0xA9C8 */
extern int16 shadow_save(void);			/* 0xD413 */

/* 0x9444 */

union panel_template g_panel_template = { {
	"Richard J Lilley\rSarah           \rKevin           \rKate            \rGraham          \r",
	{ 55000ul, 45000ul, 35000ul, 18000ul, 1000ul }
} };

/* 0x9BDD */

void publish_panel_template(int16 arg)
{
	uint16 seg = (uint16)g_mem_next;
	uint8 far *dst;
	int16 i;

	FP_SEG(dst) = (uint16)seg;
	FP_OFF(dst) = 0;

	(void)arg;
	for (i = 0; i < 106; i++)
		dst[i] = g_panel_template.raw[i];
	(void)shadow_save();					/* 9BFA-9BFC */
}

/* 0x4771 */

/* 0x4771 */
void text_resource_read(int16 idx)
{
	FILE *f;

	f = g_text_stream;
	fseek(f, (int32)(int16)g_text_index[idx], 0);	/* 4777-4795 */
	(void)fread(&g_credits, 1, CREDITS_BUFFER_SIZE, f);			/* 4798-47AB */
}

extern uint16 g_mem_next;

/* 0x9C35 */

int16 load_panel_template(char *name)
{
	union REGS r;
	uint8 far *src;
	int16 i;

	r.x.dx = (uint16)name;
	r.x.bx = (uint16)g_mem_next;
	r.x.ax = SHADOW_SEL_LOAD;
	int86(SHADOW_VECTOR, &r, &r);

	if ((int16)r.x.ax == 0)					/* 9C49-9C4B */
		return 0;

	FP_SEG(src) = (uint16)g_mem_next;
	FP_OFF(src) = 0;

	for (i = 0; i < 106; i++)
		g_panel_template.raw[i] = src[i];

	return -1;
}

/* 0xE80C */
