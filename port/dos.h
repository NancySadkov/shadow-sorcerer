/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Compatibility header for the game sources, which include <dos.h>.
  It only needs to provide the handful of symbols the DOS build used.
*/

#ifndef SS_DOS_COMPAT_H
#define SS_DOS_COMPAT_H

#include "types.h"
#include "ss_platform.h"

#define far
#define near
#define _interrupt
#define interrupt

struct ss_regs_word {
	uint16 ax, bx, cx, dx, si, di, cflag;
};

struct ss_regs_byte {
	uint8 al, ah, bl, bh, cl, ch, dl, dh;
};

union REGS {
	struct ss_regs_word x;
	struct ss_regs_byte h;
};

struct SREGS {
	uint16 es, cs, ss, ds;
};

#define FP_SEG(p) ((uint16)0)
#define FP_OFF(p) ((uint16)(uintptr_t)(p))
#define MK_FP(seg, off) ((void *)(uintptr_t)(off))

int16 ss_inp(uint16 port);
void ss_outp(uint16 port, uint16 value);
void ss_outpw(uint16 port, uint16 value);

/* Mingw's intrin.h owns these names, so route the game's port I/O through
   the PC-speaker/EGA model with macros. */
#define inp(p)       ss_inp((uint16)(p))
#define outp(p, v)   ss_outp((uint16)(p), (uint16)(v))
#define outpw(p, v)  ss_outpw((uint16)(p), (uint16)(v))

static inline void int86(int no, union REGS *in, union REGS *out)
{
	(void)no;
	if (out != in) {
		*out = *in;
	}
}

static inline void int86x(int no, union REGS *in, union REGS *out, struct SREGS *s)
{
	(void)s;
	int86(no, in, out);
}

static inline void intdos(union REGS *in, union REGS *out)
{
	(void)in;
	(void)out;
}

static inline void segread(struct SREGS *s)
{
	if (s != 0) {
		s->es = s->cs = s->ss = s->ds = 0;
	}
}

static inline void _disable(void) { }
static inline void _enable(void) { }

#endif
