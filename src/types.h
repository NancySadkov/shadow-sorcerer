/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#ifndef TYPES_H
#define TYPES_H

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <stdint.h>
typedef int8_t   int8;
typedef uint8_t  uint8;
typedef int16_t  int16;
typedef uint16_t uint16;
typedef int32_t  int32;
typedef uint32_t uint32;
#else
typedef signed char   int8;
typedef unsigned char uint8;

typedef int           int16;
typedef unsigned int  uint16;
typedef long          int32;
typedef unsigned long uint32;
#endif

typedef char type_size_int8  [(sizeof(int8)  == 1) ? 1 : -1];
typedef char type_size_uint8 [(sizeof(uint8) == 1) ? 1 : -1];
typedef char type_size_int16 [(sizeof(int16) == 2) ? 1 : -1];
typedef char type_size_uint16[(sizeof(uint16) == 2) ? 1 : -1];
typedef char type_size_int32 [(sizeof(int32) == 4) ? 1 : -1];
typedef char type_size_uint32[(sizeof(uint32) == 4) ? 1 : -1];

#endif
