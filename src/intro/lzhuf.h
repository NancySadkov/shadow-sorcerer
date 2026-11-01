/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#ifndef LZHUF_H
#define LZHUF_H

#include "types.h"

/* 0x0864 - dec */
void lzhuf_decode(void);

/* DGROUP 0x0A36 */
extern uint8 *g_lz_in;			/* 0xF0F4 */
extern uint8 *g_decode_dst;		/* 0x4FA0 */
extern uint32 g_lz_size;		/* 0x00C8 */
extern uint16 g_lz_bitbuf;		/* 0x04D0 */
extern uint8  g_lz_bitcnt;		/* 0x04D2 */

#endif
