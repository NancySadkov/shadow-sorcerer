/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "cshadow.h"

static void restore_ega(void)
{
	outpw(0x3C4, 0x0F02);
	outpw(0x3CE, 0xFF08);
	outpw(0x3CE, 0x0005);
	outpw(0x3CE, 0x0003);
	outpw(0x3CE, 0x0F07);
}

static uint8 far *display_ptr(uint16 off)
{
	return MK_FP(sd_display_seg, off);
}

void int86(int no, union regs *in, union regs *out)
{
	switch (no) {
	case 0x10: bios10(in, out, (struct sregs *)0); break;
	case 0x16: bios16(in, out, (struct sregs *)0); break;
	case 0x21: bios21(in, out, (struct sregs *)0); break;
	case 0x33: bios33(in, out, (struct sregs *)0); break;
	}
}

void int86x(int no, union regs *in, union regs *out, struct sregs *s)
{
	switch (no) {
	case 0x10: bios10(in, out, s); break;
	case 0x16: bios16(in, out, s); break;
	case 0x21: bios21(in, out, s); break;
	case 0x33: bios33(in, out, s); break;
	}
}

void intdos(union regs *in, union regs *out)
{
	bios21(in, out, (struct sregs *)0);
}

void c_svc00(struct cframe far *f)
{
	uint16 n = (uint16)(0x14 * (uint16)(f->cx & 0xFF));
	uint16 far *p0 = MK_FP(sd_page0_seg, 0);

	outpw(0x3CE, 0xFF08);
	outpw(0x3CE, 0x0005);
	outpw(0x3CE, 0x0003);
	p0[0] = 0;
	outpw(0x3CE, 0x0105);
	sd_far_movsw(sd_page0_seg, 2, sd_page0_seg, 0, (uint16)(n - 1));
	sd_far_movsw(sd_page1_seg, 0, sd_page0_seg, 0, n);
	outpw(0x3CE, 0x0005);
	outpw(0x3CE, 0x0F07);
	f->ax = 0x0F07;
}

void c_svc02(struct cframe far *f)
{
	union regs r;
	struct sregs s;

	segread(&s);
	s.es = f->ds;
	r.x.ax = 0x1002;
	r.x.dx = f->dx;
	int86x(0x10, &r, &r, &s);
}

void c_svc05(struct cframe far *f)
{
	sd_sprite_seg = f->ds;
	sd_sprite_off = f->bx;
}

void c_svc06(struct cframe far *f) { (void)f; }
void c_svc07(struct cframe far *f) { (void)f; }
void c_svc0E(struct cframe far *f) { (void)f; }
void c_svc12(struct cframe far *f) { (void)f; }
void c_svc13(struct cframe far *f) { (void)f; }
void c_svc16(struct cframe far *f) { (void)f; }

void c_svc0D(struct cframe far *f)
{
	sd_cli();
	sd_caller_ds = f->ds;
	sd_counter_ptr = f->si;
	sd_sti();
}

void c_svc0F(struct cframe far *f)
{
	f->ax = sd_display_seg;
}

void c_svc10(struct cframe far *f)
{
	uint16 v;

	sd_joy_enable = 1;
	do {
		v = sd_joy_x;
	} while (v == 0);
	if (v == 0xFFFF)
		sd_joy_enable = 0;
	sd_joy_cx = (uint16)(v >> 1);
	sd_joy_cy = (uint16)(sd_joy_y >> 1);
	f->ax = sd_joy_enable;
}

void c_svc15(struct cframe far *f)
{
	union regs r;

	r.x.ax = 0x0700;
	intdos(&r, &r);
	f->ax = (uint16)(r.x.ax & 0x7F);
}

void c_svc17(struct cframe far *f)
{
	f->ax = 0x21;
}

void c_svc19(struct cframe far *f)
{
	uint8 mask = (uint8)(f->bx & 0xFF);
	uint16 row = (uint16)(f->dx & 0xFF);
	uint16 col = (uint16)((f->dx >> 8) & 0xFF);
	uint16 w = (uint16)((f->cx >> 8) & 0xFF);
	uint16 h = (uint16)(f->cx & 0xFF);
	uint8 far *p = display_ptr((uint16)(row * 40 + col));
	uint16 y, x;

	outpw(0x3CE, 0x0A05);
	outpw(0x3CE, 0x0003);
	outpw(0x3CE, 0x0007);
	outpw(0x3CE, 0xFF08);
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++)
			p[x] = (uint8)(p[x] & mask);
		p += 0x28;
	}
	restore_ega();
}

void c_svc1A(struct cframe far *f)
{
	uint16 x = f->cx;
	uint16 y = (uint16)(f->dx & 0xFF);
	uint8 colour = (uint8)((f->dx >> 8) & 0xFF);
	uint16 rows = f->bx;
	uint8 far *p;
	uint8 mask;
	uint16 i;

	restore_ega();
	p = display_ptr((uint16)(y * 40 + (x >> 3)));
	mask = (uint8)(0x80 >> (x & 7));

	outpw(0x3CE, (uint16)((uint16)colour << 8));
	outpw(0x3CE, 0x0F01);
	outpw(0x3CE, 0x0003);
	outpw(0x3CE, (uint16)(0x0008 | ((uint16)mask << 8)));
	for (i = 0; i < rows; i++) {
		*p = (uint8)(*p | 0x08);
		p += 0x28;
	}
	outpw(0x3CE, 0x0000);
	outpw(0x3CE, 0x0001);
	restore_ega();
}

void c_svc1E(struct cframe far *f)
{
	int16 page = (int16)f->bx;

	if (page < 0) {
		sd_display_seg = sd_saved_display;
		sd_page0_seg = 0xA000;
		sd_page1_seg = 0xA200;
		return;
	}
	sd_saved_display = sd_display_seg;
	if (page == 0) {
		sd_display_seg = sd_page0_seg;
		if ((sd_page_toggle & 1) != 0)
			sd_display_seg = sd_page1_seg;
		sd_back_seg = sd_display_seg;
		sd_screen_dirty = 0;
		return;
	}
	sd_display_seg = (uint16)(0xA000 + page * 0x200);
	sd_page0_seg = sd_display_seg;
	sd_page1_seg = sd_display_seg;
}

void c_svc1F(struct cframe far *f)
{
	outpw(0x3CE, 0x0105);
	sd_far_movsb(sd_display_seg, 0,
	             (uint16)(0xA000 + (uint16)f->bx * 0x200), 0, 0x1F40);
}

void c_svc20(struct cframe far *f)
{
	uint16 x = f->bx;
	uint16 y = f->dx;
	uint16 off = (uint16)(y * 40 + (x >> 3));
	uint8 colour = (uint8)(f->cx & 0xFF);
	volatile uint8 far *p = MK_FP(0xA000, off);
	uint8 mask = (uint8)(0x80 >> (x & 7));

	outpw(0x3CE, (uint16)(0x0008 | ((uint16)mask << 8)));
	outpw(0x3CE, 0x0205);
	outpw(0x3CE, 0x0003);

	(void)sd_latch(0xA000, off);
	*p = colour;
	outpw(0x3CE, 0xFF08);
	outpw(0x3CE, 0x0005);
	outpw(0x3CE, 0x0003);
}

void c_svc21(struct cframe far *f)
{
	volatile uint16 *p = (volatile uint16 *)(sd_counter_ptr + 4);

	*p = f->cx;
	while ((int16)*p >= 0)
		;
}

void c_svc22(struct cframe far *f)
{
	sd_mouse_x = f->cx;
	sd_mouse_y = f->dx;
}

static uint16 load_file_impl(uint16 dest_seg, uint16 name_seg, uint16 name_off)
{
	union regs r;
	struct sregs s;
	uint16 handle;
	uint16 n;

	sd_file_seg = dest_seg;
	segread(&s);
	s.ds = name_seg;
	r.x.ax = 0x3D00;
	r.x.dx = name_off;
	int86x(0x21, &r, &r, &s);
	if (r.x.cflag) {
		r.x.ax = 0;
		return 0;
	}
	handle = r.x.ax;
	sd_file_handle = handle;

	segread(&s);
	s.ds = dest_seg;
	r.x.ax = 0x3F00;
	r.x.bx = handle;
	r.x.cx = 0xFFFF;
	r.x.dx = 0;
	int86x(0x21, &r, &r, &s);
	n = r.x.ax;

	r.x.ax = 0x3E00;
	r.x.bx = handle;
	intdos(&r, &r);
	return n;
}

void c_svc09(struct cframe far *f)
{
	f->ax = load_file_impl(f->bx, f->ds, f->dx);
}

void c_svc18(struct cframe far *f)
{
	uint16 n = load_file_impl(f->bx, f->ds, f->dx);
	uint16 far *src = MK_FP(sd_file_seg, 0);
	uint16 far *dst = MK_FP(0xA000, f->cx);
	uint16 i;

	f->ax = (uint16)(n >> 2);

	outpw(0x3CE, 0x0805);
	outpw(0x3CE, 0x0003);
	outpw(0x3CE, 0x0007);
	outpw(0x3CE, 0xFF08);
	for (i = 0; i < (uint16)(n >> 3); i++) {
		outpw(0x3C4, 0x0102);
		dst[0] = (uint16)(dst[0] & src[0]);
		outpw(0x3C4, 0x0202);
		dst[0] = (uint16)(dst[0] & src[1]);
		outpw(0x3C4, 0x0402);
		dst[0] = (uint16)(dst[0] & src[2]);
		outpw(0x3C4, 0x0802);
		dst[0] = (uint16)(dst[0] & src[3]);
		src += 4;
		dst += 1;
	}
	restore_ega();
}

void c_svc11(struct cframe far *f)
{
	union regs r;
	struct sregs s;
	uint16 handle;

	sd_file_seg = f->bx;
	segread(&s);
	s.ds = f->ds;
	r.x.ax = 0x3C00;
	r.x.cx = 0;
	r.x.dx = f->dx;
	int86x(0x21, &r, &r, &s);
	if (r.x.cflag) {

		r.x.ax = 0x3E00;
		r.x.bx = sd_file_handle;
		intdos(&r, &r);
		f->ax = 0xFFFF;
		return;
	}
	handle = r.x.ax;
	sd_file_handle = handle;

	segread(&s);
	s.ds = f->bx;
	r.x.ax = 0x4000;
	r.x.bx = handle;
	r.x.cx = f->cx;
	r.x.dx = 0;
	int86x(0x21, &r, &r, &s);
	if (r.x.ax == 0) {
		r.x.ax = 0x3E00;
		r.x.bx = handle;
		intdos(&r, &r);
		f->ax = 0xFFFF;
		return;
	}
	r.x.ax = 0x3E00;
	r.x.bx = handle;
	intdos(&r, &r);
	f->ax = 0;
}

static uint16 bswap16(uint16 v)
{
	return (uint16)((v << 8) | (v >> 8));
}

static void plane_and(uint8 far *dst, uint8 mask, const uint8 d[4])
{
	int16 p;

	outpw(0x3CE, (uint16)(0x0008 | ((uint16)mask << 8)));
	for (p = 3; p >= 0; p--) {
		outpw(0x3C4, (uint16)(((uint16)(1 << p) << 8) | 0x02));
		*dst = (uint8)(*dst & d[p]);
	}
}

static void plane_and_same(uint8 far *dst, uint8 mask, const uint8 d[4])
{
	int16 p;

	outpw(0x3CE, (uint16)(0x0008 | ((uint16)mask << 8)));
	for (p = 3; p >= 0; p--) {
		outpw(0x3C4, (uint16)(((uint16)(1 << p) << 8) | 0x02));
		*dst = (uint8)(*dst & d[p]);
	}
}

void c_svc03(struct cframe far *f)
{
	int16 x = (int16)f->cx;
	int16 y = (int16)f->dx;
	int16 h = (int16)(f->bx & 0xFF);
	const uint8 far *src = MK_FP(f->ds, f->si);
	const uint16 far *srcw = (const uint16 far *)src;
	uint8 far *dst;
	uint8 edge;
	int16 shift;
	int16 row;

	if (x < 0) {
		if (-x > 16)
			return;
	} else if (x >= 320) {
		return;
	}
	if (y < 0) {
		int16 k = (int16)-y;
		if (k >= h || k > 255)
			return;
		h -= k;
		src += 8 * k;
		srcw = (const uint16 far *)src;
		y = 0;
	} else {
		int16 room;
		if (y >= 200)
			return;
		room = (int16)(200 - y);
		if (room <= h)
			h = room;
	}
	edge = sd_edge_mask[4 + (x >> 3)];
	sd_sprite_h = (uint16)h;
	sd_draw_offset = (uint16)(y * 40 + (x >> 3));
	dst = display_ptr(sd_draw_offset);
	shift = (int16)(x & 7);

	outpw(0x3CE, 0x0805);
	outpw(0x3CE, 0x0003);
	outpw(0x3CE, 0x0007);
	outpw(0x3CE, 0xFF08);

	for (row = 0; row < h; row++) {
		uint16 w[4];
		uint16 orw;
		int16 p;

		for (p = 0; p < 4; p++)
			w[p] = srcw[p];
		orw = (uint16)(w[0] | w[1] | w[2] | w[3]);

		if (shift == 0) {
			uint8 d0[4], d1[4];
			uint8 mask0 = (uint8)orw;
			uint8 mask1 = (uint8)(orw >> 8);

			for (p = 0; p < 4; p++) {
				d0[p] = (uint8)(w[p] & 0xFF);
				d1[p] = (uint8)(w[p] >> 8);
			}

			if (edge & 0x10)
				plane_and(dst + 0, mask0, d0);
			if (edge & 0x08)
				plane_and(dst + 1, mask1, d1);
		} else {
			uint8 pd[4][3], maskb[3];
			uint16 ax, bx;

			ax = bswap16(orw);
			bx = (uint16)((ax & 0x00FF) << 8);
			ax >>= shift;
			bx >>= shift;
			maskb[0] = (uint8)(ax >> 8);
			maskb[1] = (uint8)ax;
			maskb[2] = (uint8)bx;

			for (p = 0; p < 4; p++) {
				ax = bswap16(w[p]);
				bx = (uint16)((ax & 0x00FF) << 8);
				ax >>= shift;
				bx >>= shift;
				pd[p][0] = (uint8)(ax >> 8);
				pd[p][1] = (uint8)ax;
				pd[p][2] = (uint8)bx;
			}
			for (p = 0; p < 3; p++) {
				uint8 d[4];
				int16 q;
				for (q = 0; q < 4; q++)
					d[q] = pd[q][p];
				if (edge & (0x10 >> p))
					plane_and(dst + p, maskb[p], d);
			}
		}
		srcw += 4;
		dst += 0x28;
	}
	restore_ega();
}

static void shift4(const uint8 b[4], int16 s, uint8 out[5])
{
	uint16 dx = bswap16((uint16)(b[0] | ((uint16)b[1] << 8)));
	uint16 ax = bswap16((uint16)(b[2] | ((uint16)b[3] << 8)));
	uint16 bx;

	bx = (uint16)(((dx & 0x00FF) << 8) | ((ax >> 8) & 0xFF));
	dx >>= s;
	bx >>= s;
	out[0] = (uint8)(dx >> 8);
	out[1] = (uint8)dx;
	out[2] = (uint8)bx;

	bx = (uint16)((ax & 0x00FF) << 8);
	ax >>= s;
	bx >>= s;
	out[3] = (uint8)ax;
	out[4] = (uint8)bx;
}

void c_svc04(struct cframe far *f)
{
	int16 x = (int16)f->cx;
	int16 y = (int16)f->dx;
	int16 h = (int16)(f->bx & 0xFF);
	const uint8 far *src = MK_FP(f->ds, f->si);
	uint8 far *dst;
	uint8 edge;
	int16 shift;
	int16 row;

	if (x < 0) {
		if (-x > 32)
			return;
	} else if (x >= 320) {
		return;
	}
	if (y < 0) {
		int16 k = (int16)-y;
		if (k >= h || k > 255)
			return;
		h -= k;
		src += 16 * k;
		y = 0;
	} else {
		int16 room;
		if (y >= 200)
			return;
		room = (int16)(200 - y);
		if (room <= h)
			h = room;
	}
	edge = sd_edge_mask[4 + (x >> 3)];
	sd_sprite_h = (uint16)h;
	sd_draw_offset = (uint16)(y * 40 + (x >> 3));
	dst = display_ptr(sd_draw_offset);
	shift = (int16)(x & 7);

	outpw(0x3CE, 0x0805);
	outpw(0x3CE, 0x0003);
	outpw(0x3CE, 0x0007);
	outpw(0x3CE, 0xFF08);

	for (row = 0; row < h; row++) {
		uint8 b[16];
		uint8 ob[20];
		int16 i;

		for (i = 0; i < 16; i++) {
			b[i] = src[i];
			ob[i] = 0;
		}

		if (shift == 0) {
			uint16 evenor = 0, oddor = 0;
			uint8 mask[4];
			uint8 d[4];
			int16 k, p;

			for (p = 0; p < 4; p++) {
				oddor |= (uint16)(b[p * 4 + 0] | ((uint16)b[p * 4 + 1] << 8));
				evenor |= (uint16)(b[p * 4 + 2] | ((uint16)b[p * 4 + 3] << 8));
			}
			mask[0] = (uint8)oddor;
			mask[1] = (uint8)(oddor >> 8);
			mask[2] = (uint8)evenor;
			mask[3] = (uint8)(evenor >> 8);
			for (k = 0; k < 4; k++) {
				for (p = 0; p < 4; p++)
					d[p] = b[p * 4 + k];
				if (edge & (0x10 >> k))
					plane_and(dst + k, mask[k], d);
			}
		} else {
			uint8 maskb[5];
			uint8 ob4[4];
			uint8 po[4][5];
			int16 k, p;

			for (k = 0; k < 4; k++) {
				ob4[k] = 0;
				for (p = 0; p < 4; p++)
					ob4[k] |= b[p * 4 + k];
			}
			shift4(ob4, shift, maskb);
			for (p = 0; p < 4; p++) {
				uint8 p4[4];
				int16 q;
				for (q = 0; q < 4; q++)
					p4[q] = b[p * 4 + q];
				shift4(p4, shift, po[p]);
			}
			for (k = 0; k < 5; k++) {
				uint8 d[4];
				for (p = 0; p < 4; p++)
					d[p] = po[p][k];
				if (edge & (0x10 >> k))
					plane_and(dst + k, maskb[k], d);
			}
		}
		src += 16;
		dst += 0x28;
	}
	restore_ega();
}

static void blit_block_impl(int16 col, int16 row, const uint8 far *src,
                            uint8 mode)
{
	uint16 width, height;
	uint8 far *dst = display_ptr((uint16)(row * 40 + col));

	width = (uint16)(src[0] | ((uint16)src[1] << 8));
	height = (uint16)(src[2] | ((uint16)src[3] << 8));
	src += 4;
	sd_sprite_w = width;
	sd_sprite_h = height;
	outpw(0x3CE, 0x0805);
	outpw(0x3CE, 0x0003);
	outpw(0x3CE, 0x0007);
	outpw(0x3CE, 0xFF08);

	while (height != 0) {
		uint16 n;
		uint8 far *line = dst;
		const uint8 far *s = src;

		for (n = 0; n < width; n++) {
			if ((mode & 1) == 0) {
				int16 p;
				for (p = 0; p < 4; p++) {
					uint16 v;
					outpw(0x3C4, (uint16)(((uint16)(1 << p) << 8) | 0x02));
					v = (uint16)(s[0] | ((uint16)s[1] << 8));
					v = bswap16(v);
					*((uint16 far *)line) =
						(uint16)(*((uint16 far *)line) & v);
					s += 2;
				}
				line += 2;
			} else {
				uint16 orw = (uint16)((s[0] | ((uint16)s[1] << 8)) |
				                      (s[2] | ((uint16)s[3] << 8)) |
				                      (s[4] | ((uint16)s[5] << 8)) |
				                      (s[6] | ((uint16)s[7] << 8)));
				uint8 d[4];
				int16 p;
				for (p = 0; p < 4; p++)
					d[p] = s[p * 2 + 1];
				plane_and_same(line + 0, (uint8)(orw >> 8), d);
				for (p = 0; p < 4; p++)
					d[p] = s[p * 2 + 0];
				plane_and_same(line + 1, (uint8)orw, d);
				line += 2;
				s += 8;
			}
		}
		src += (uint16)(width * ((mode & 1) ? 8 : 8));
		dst += 0x28;
		height--;
	}
	restore_ega();
}

void c_svc08(struct cframe far *f)
{
	blit_block_impl((int16)((f->dx >> 8) & 0xFF),
	                (int16)(f->dx & 0xFF),
	                MK_FP(f->ds, f->si),
	                (uint8)(f->cx & 0xFF));
}

void c_svc0A(struct cframe far *f)
{
	f->ax = bios16_poll();
}

void c_svc0B(struct cframe far *f)
{
	uint16 colour = f->bx;
	uint16 row = (uint16)(f->dx & 0xFF);
	uint16 col = (uint16)((f->dx >> 8) & 0xFF);
	const uint8 far *s = MK_FP(f->ds, f->si);
	uint8 far *dst = display_ptr((uint16)(row * 40 + col));
	uint16 off = (uint16)(row * 40 + col);
	uint8 fg = (uint8)(colour & 0xFF);
	uint8 bg = (uint8)(colour >> 8);

	outpw(0x3CE, 0x0A05);
	outpw(0x3CE, 0x0003);
	outpw(0x3CE, 0x0007);
	for (;;) {
		uint8 ch = *s++;
		if (ch == 0)
			break;
		if (ch == 0x0D) {
			off = (uint16)(off + 0x140);
			sd_draw_offset = off;
			dst = display_ptr(off);
			continue;
		}
		{
			const uint8 *glyph = sd_font + (uint16)((ch - 0x20) * 8);
			uint8 far *p = dst;
			uint16 i;
			for (i = 0; i < 8; i++) {
				uint8 bits = glyph[i];
				outpw(0x3CE, (uint16)(0x0008 | ((uint16)bits << 8)));
				*p = (uint8)(*p & fg);
				if (bg != 0xFF) {
					outpw(0x3CE, (uint16)(0x0008 | ((uint16)(uint8)~bits << 8)));
					*p = (uint8)(*p & bg);
				}
				p += 0x28;
			}
		}
		dst++;
	}
	restore_ega();
}

static uint16 direction_read_impl(void)
{
	uint16 v;

	if (sd_joy_enable & 1) {
		uint8 al = (uint8)(~sd_joy_saved);
		uint8 bl;
		uint16 ax;
		al &= 0x30;
		bl = (uint8)(al << 2);
		ax = sd_joy_cx;
		if (ax >= sd_joy_x)
			bl |= 4;
		ax = (uint16)((ax << 1) + sd_joy_cx);
		if (ax > sd_joy_x)
			bl |= 8;
		ax = sd_joy_cy;
		if (ax >= sd_joy_y)
			bl |= 1;
		ax = (uint16)((ax << 1) + sd_joy_cy);
		if (ax > sd_joy_y)
			bl |= 2;
		if (bl != 0)
			return bl;
	}
	v = 0;
	if (sd_keys[3] & 1) v |= 0x40;
	if (sd_keys[7] & 1) v |= 0x80;
	if (sd_keys[0] & 1) { v |= 0x05; return v; }
	if (sd_keys[2] & 1) { v |= 0x09; return v; }
	if (sd_keys[8] & 1) { v |= 0x06; return v; }
	if (sd_keys[10] & 1) { v |= 0x0A; return v; }
	if (sd_keys[1] & 1) v |= 0x01;
	else if (sd_keys[9] & 1) v |= 0x02;
	if (sd_keys[4] & 1) { v |= 0x04; return v; }
	if (sd_keys[6] & 1) v |= 0x08;
	return v;
}

void c_svc0C(struct cframe far *f)
{
	f->ax = direction_read_impl();
}

void c_svc1C(struct cframe far *f)
{
	union regs r;
	int16 cx, dx;
	uint8 al, b;

	r.x.ax = 0x0B;
	int86(0x33, &r, &r);
	cx = (int16)r.x.cx;
	dx = (int16)r.x.dx;

	al = (uint8)direction_read_impl();
	if (sd_mouse_first == 0) {
		sd_mouse_first = 1;
		if (al & 1) dx -= 4;
		else if (al & 2) dx += 4;
		if (al & 4) cx -= 4;
		else if (al & 8) cx += 4;
	}
	b = (uint8)(al & 0xC0);
	b = (uint8)((b << 2) | (b >> 6));

	cx = (int16)(cx + (int16)sd_mouse_x);
	dx = (int16)(dx + (int16)sd_mouse_y);
	if (cx < 0) cx = 0;
	else if (cx > 0x13F) cx = 0x13F;
	if (dx < 0) dx = 0;
	else if (dx > 0xC7) dx = 0xC7;
	sd_defer_cx = (uint16)cx;
	sd_defer_dy = (uint16)dx;
	sd_mouse_x = (uint16)cx;
	sd_mouse_y = (uint16)dx;

	r.x.ax = 0x03;
	int86(0x33, &r, &r);
	f->bx = (uint16)((r.x.bx & 0xFF00) | ((r.x.bx | b) & 0xFF));
	f->cx = (uint16)cx;
	f->dx = (uint16)dx;
	f->ax = b;
}

void c_svc03_frame(uint16 x, uint16 y, uint16 h, const uint8 far *src);

static void render_deferred(void)
{
	uint16 h, x, y, off, i, saved;
	uint8 far *save, *src;

	if (sd_defer_pending == 0)
		return;
	outpw(0x3CE, 0x0105);
	sd_screen_dirty = 1;
	h = sd_defer_h;
	x = sd_defer_cx;
	y = sd_defer_dy;
	if ((int16)(200 - y) < (int16)h)
		h = (uint16)(200 - y);
	sd_defer_rows = (uint8)h;
	off = (uint16)(y * 40 + (x >> 3));
	sd_defer_dest = off;
	save = MK_FP(sd_back_seg, 0x1F40);
	src = MK_FP(sd_back_seg, off);
	for (i = 0; i < h; i++) {
		save[0] = src[0]; save[1] = src[1]; save[2] = src[2];
		save[3] = src[3]; save[4] = src[4];
		save += 5;
		src += 0x28;
	}
	saved = sd_display_seg;
	sd_display_seg = sd_back_seg;
	c_svc03_frame(x, y, h, MK_FP(sd_defer_ds, sd_defer_si));
	sd_display_seg = saved;

}

void c_svc03_frame(uint16 x, uint16 y, uint16 h, const uint8 far *src)
{
	struct cframe fr;
	fr.cx = x;
	fr.dx = y;
	fr.bx = h;
	fr.ds = (uint16)((unsigned long)src >> 16);
	fr.si = (uint16)(unsigned)src;
	c_svc03(&fr);
}

void c_svc1D(struct cframe far *f)
{
	if ((int16)f->bx < 0) {
		sd_screen_dirty = 0;
		sd_defer_pending = 0;
		return;
	}
	if (sd_screen_dirty != 0) {
		uint8 far *dst = MK_FP(sd_back_seg, sd_defer_dest);
		uint8 far *save = MK_FP(sd_back_seg, 0x1F40);
		uint16 i;
		outpw(0x3CE, 0x0105);
		for (i = 0; i < sd_defer_rows; i++) {
			dst[0] = save[0]; dst[1] = save[1]; dst[2] = save[2];
			dst[3] = save[3]; dst[4] = save[4];
			dst += 0x28;
			save += 5;
		}
		sd_screen_dirty = 0;
	}
	if ((f->bx & 0x4000) != 0)
		return;
	sd_defer_pending = 1;
	sd_defer_h = (uint8)f->bx;
	sd_defer_si = f->si;
	sd_defer_cx = f->cx;
	sd_defer_dy = f->dx;
	sd_defer_ds = f->ds;
	render_deferred();
}

void c_svc01(struct cframe far *f)
{
	sd_page_toggle ^= 1;
	if (sd_page_toggle != 0) {
		sd_display_seg = sd_page0_seg;
		sd_back_seg = sd_page1_seg;
		sd_set_page(1);
	} else {
		sd_display_seg = sd_page1_seg;
		sd_back_seg = sd_page0_seg;
		sd_set_page(0);
	}
	sd_frame_count++;
	sd_prev_stamp = sd_frame_stamp;
	sd_wait_retrace_asm();
	render_deferred();
	f->ax = 0;
}

void c_svc1B(struct cframe far *f)
{
	uint8 far *block = MK_FP(f->ds, f->si);
	uint8 far *p = block + 0x22;
	unsigned long i;
	union regs r;
	struct sregs s;

	segread(&s);
	s.es = f->ds;
	r.x.ax = 0x1002;
	r.x.dx = (uint16)(f->si + 2);
	int86x(0x10, &r, &r, &s);

	for (i = 0; i < 32000UL; i += 2) {
		uint8 t = p[i];
		p[i] = p[i + 1];
		p[i + 1] = t;
	}

	*((uint16 far *)(block + 0x1E)) = 20;
	*((uint16 far *)(block + 0x20)) = 200;
	blit_block_impl((int16)((f->dx >> 8) & 0xFF),
	                (int16)(f->dx & 0xFF),
	                block + 0x1E,
	                (uint8)(f->cx & 0xFF));
}

static void tile_strip(uint16 tile, uint16 src_col, uint16 rows,
                       uint8 far *dst, uint8 step)
{
	const uint8 far *src;
	uint16 y;

	if (tile == 0)
		return;
	tile--;
	src = MK_FP(sd_tile_seg, (uint16)(sd_tile_base +
	          tile * 32 + src_col * 2));
	for (y = 0; y < rows; y++) {
		dst[0] = src[0];
		if (step == 2)
			dst[1] = src[1];
		src += 2;
		dst += 0x28;
	}
}

void c_svc14(struct cframe far *f)
{
	const uint16 far *map = MK_FP(f->ds, f->si);
	uint16 width = *map++;
	uint16 height = *map++;
	int16 px = (int16)f->cx;
	int16 py = (int16)f->dx;
	uint16 tx, ty, row, col, rows, cols, r, c;
	uint8 far *dst;

	tx = (uint16)(px & 15);
	ty = (uint16)(py & 15);
	col = (uint16)(px >> 4);
	row = (uint16)(py >> 4);
	if (row >= height || col >= width)
		return;
	rows = (uint16)(16 - ty);
	if (rows > 16) rows = 16;
	cols = (uint16)(width - col);
	if (cols > 10) cols = 10;

	dst = MK_FP(sd_display_seg, (uint16)(f->di + ty * 40 + tx));
	for (r = 0; r < rows; r++) {
		const uint16 far *m = map + (row + r) * width + col;
		uint8 far *d = dst;
		for (c = 0; c < cols; c++, d += 2) {
			uint16 v = *m++;
			tile_strip((uint16)(v & 0x0FFF),
			           (r == 0 ? tx : 0),
			           16, d, 2);
		}
		dst += 0x28;
	}
	restore_ega();
}
