/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#ifndef SERVICE_H
#define SERVICE_H

struct reg_slot {
	uint16 off;
	uint16 seg;
};

extern struct reg_slot g_reg_slots[16];

int16 service_lookup(uint16 slot, uint16 request, uint16 *off, uint16 *seg);

#endif
