/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#ifndef HW_H
#define HW_H

#ifndef SS_SDL
int16 inp(int16 port);
int16 outp(int16 port, int16 value);
#endif

#define PORT_PIT_CMD     0x43
#define PORT_PIT_CH0     0x40
#define PORT_PIT_CH2     0x42
#define PORT_PIA_CTRL    0x61

#define PIT_DIVISOR_MAX  0xffff

#endif
