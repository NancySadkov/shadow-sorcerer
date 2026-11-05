/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Display pages and palette.

  SHADOW.COM talks to an EGA: four bit planes in a 64 KiB window, with pages
  selected by segment (0xA000 + page * 0x200).  The port keeps the same page
  segments and hands the game plain 8-bit indexed pages; the drawing services
  decode the planar sprite data themselves.  This keeps every game-side
  address calculation (and the deferred cursor save area) unchanged.
*/

#ifndef SS_EGA_H
#define SS_EGA_H

#include "types.h"

#define SS_VRAM_PAGES 8

void ss_ega_reset(void);

/* Page backing store for a VRAM segment (0xA000 + page * 0x200). */
uint8 *ss_page(uint16 seg);

/* Copy the page at `seg` into the visible 320x200 index buffer. */
void ss_ega_compose(uint16 seg);

/* Whole-page copy used by video_copy / screen_restore (bytes -> rows). */
void ss_vram_copy(uint16 dst_seg, uint16 src_seg, uint16 bytes);

void ss_ega_set_dac(int index, int r, int g, int b);
void ss_ega_color(int index, int *r, int *g, int *b);
void ss_ega_set_palette_identity(void);

#endif
