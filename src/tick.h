/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#ifndef TICK_H
#define TICK_H

struct noise_event {
	uint16 mask;
	uint16 or_value;
	uint16 period;
};
struct tone_event {
	uint16 divisor;
};
struct note_event {
	uint16 duration;
	uint16 frequency;
};

extern void tick_handler_type0(void);
extern void tick_handler_type1(void);
extern void tick_handler_type2(void);

#endif
