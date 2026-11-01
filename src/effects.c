/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include "ids.h"
#include "shadow.h"
#include "game.h"
#include "structs.h"

/* 0x9696 */
struct effect_rec g_effect_table[EFFECT_COUNT];

/* 0xBDCF - effec */
void effect_clear_flags(void)
{
	int16 i;

	for (i = 0; i != EFFECT_COUNT; i++)
		EFFECT_KIND(&g_effect_table[i]) = 0;
}
