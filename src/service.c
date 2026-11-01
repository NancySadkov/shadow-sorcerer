/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"

#include <dos.h>
#include "service.h"

struct reg_slot g_reg_slots[16];

union far_pointer {
	uint16 far *pw;
	struct { uint16 off, seg; } r;
};

int16 service_lookup(uint16 slot, uint16 request, uint16 *off, uint16 *seg)
{
	union far_pointer table;
	uint16 i;

	if (slot >= 16)
		goto none;

	table.r.off = g_reg_slots[slot].off;
	table.r.seg = g_reg_slots[slot].seg;

	if (table.r.off == 0 && table.r.seg == 0)
		goto none;

	for (i = 0; ; i += 2) {
		uint16 key = table.pw[i];

		if (key == request) {
			*off = table.pw[i + 1];
			*seg = table.r.seg;
			return 1;
		}
		if (key == SERVICE_NONE)
			break;
	}

none:
	*off = 0;
	*seg = 0;
	return 0;
}

uint16 g_service_state[16];
uint16 g_service_id[16];
uint16 g_service_flag[16];
uint16 g_service_count;
uint16 g_service_index;

/* 0xF9AC */

static void service_unregister(uint16 id)
{
	if (id >= 16 || g_service_state[id] == 0)
		return;

	g_service_state[id] = 0;
	g_service_count--;

}

/* 0xFCA5 */

static void service_release(uint16 idx)
{
	uint16 armed;

	if (idx >= 16)
		return;

	armed = g_service_flag[idx];
	g_service_flag[idx] = 0;
	if (armed == 0)
		return;

	if (g_service_id[idx] != SERVICE_NONE)
		service_unregister(g_service_id[idx]);

}

/* 0xF8AC */
void service_registry_reset(int16 a, int16 b)
{
	uint16 i;

	(void)a;
	(void)b;

	_disable();				/* F8B2-F8B3 */

	g_service_index = 0;
	for (i = 0; i < 16; i++) {
		if ((g_reg_slots[i].off | g_reg_slots[i].seg) == 0)
			continue;		/* F8C9-F8D3 */
		if (g_service_id[i] != SERVICE_NONE)	/* F8D5-F8DF */
			service_unregister(g_service_id[i]);
		service_release(i);		/* F8E2-F8F1 */
		g_reg_slots[i].off = 0;
		g_reg_slots[i].seg = 0;
	}

	for (i = 16; i-- > 0;)
		service_unregister(i);

	_enable();
}
