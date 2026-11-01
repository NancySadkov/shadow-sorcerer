/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#ifndef SOUND_H
#define SOUND_H

#include "types.h"

int16 far sound_init(int16 mode);		/* 0x136C */
void  far sound_stop(void);			/* 0x13DF */
void  far sound_play(int16 song);		/* 0x1421 */
int16 far sound_channel_active(int16 channel);	/* 0x21CE */

void  far sound_entry_14b3(uint16 value);	/* 0x14B3 */
void  far sound_entry_14e3(uint8 value);	/* 0x14E3 */
int16 far sound_entry_14fa(int16 index);	/* 0x14FA */
void  far sound_entry_1517(void);		/* 0x1517 */

#endif
