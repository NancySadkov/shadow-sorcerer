/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#ifndef CSHADOW_H
#define CSHADOW_H

typedef unsigned char  uint8;
typedef unsigned int   uint16;
typedef signed int     int16;

#define MK_FP(seg,ofs) ((void far *)(((unsigned long)(seg) << 16) | (unsigned)(ofs)))

struct cframe {
	uint16 es, ds, bp, di, si, dx, cx, bx, ax;
};

union regs {
	struct { uint16 ax, bx, cx, dx, si, di, cflag; } x;
	struct { uint8 al, ah, bl, bh, cl, ch, dl, dh; } h;
};

struct sregs {
	uint16 es, cs, ss, ds;
};

#define sd_page_toggle		(*(volatile uint8  *)0x330)
#define sd_saved_display	(*(volatile uint16 *)0x332)
#define sd_display_seg		(*(volatile uint16 *)0x334)
#define sd_back_seg		(*(volatile uint16 *)0x336)
#define sd_page0_seg		(*(volatile uint16 *)0x338)
#define sd_page1_seg		(*(volatile uint16 *)0x33A)
#define sd_tile_seg		(*(volatile uint16 *)0x33C)
#define sd_frame_count		(*(volatile uint16 *)0x33E)
#define sd_frame_stamp		(*(volatile uint16 *)0x340)
#define sd_prev_stamp		(*(volatile uint16 *)0x342)
#define sd_draw_offset		(*(volatile uint16 *)0x344)
#define sd_busy			(*(volatile uint8  *)0x346)
#define sd_saved_mode		(*(volatile uint8  *)0x347)
#define sd_crtc_base		(*(volatile uint16 *)0x34C)
#define sd_mouse_first		(*(volatile uint8  *)0x352)
#define sd_sprite_seg		(*(volatile uint16 *)0x354)
#define sd_sprite_off		(*(volatile uint16 *)0x356)
#define sd_caller_ds		(*(volatile uint16 *)0x358)
#define sd_counter_ptr		(*(volatile uint16 *)0x361)
#define sd_sprite_w		(*(volatile uint16 *)0x36B)
#define sd_sprite_h		(*(volatile uint16 *)0x36D)
#define sd_sprite_mode		(*(volatile uint8  *)0x36F)
#define sd_defer_flag_a		(*(volatile uint8  *)0x370)
#define sd_defer_flag_b		(*(volatile uint8  *)0x371)
#define sd_defer_dest		(*(volatile uint16 *)0x374)
#define sd_screen_dirty		(*(volatile uint8  *)0x376)
#define sd_defer_rows		(*(volatile uint8  *)0x377)
#define sd_mouse_buttons	(*(volatile uint16 *)0x378)
#define sd_mouse_x		(*(volatile uint16 *)0x37A)
#define sd_mouse_y		(*(volatile uint16 *)0x37C)
#define sd_defer_si		(*(volatile uint16 *)0x37E)
#define sd_defer_ds		(*(volatile uint16 *)0x380)
#define sd_defer_cx		(*(volatile uint16 *)0x382)
#define sd_defer_dy		(*(volatile uint16 *)0x384)
#define sd_defer_pending	(*(volatile uint16 *)0x386)
#define sd_defer_h		(*(volatile uint8  *)0x388)
#define sd_joy_enable		(*(volatile uint16 *)0x38A)
#define sd_joy_cx		(*(volatile uint16 *)0x38C)
#define sd_joy_cy		(*(volatile uint16 *)0x38E)
#define sd_joy_x		(*(volatile uint16 *)0x390)
#define sd_joy_y		(*(volatile uint16 *)0x392)
#define sd_joy_saved		(*(volatile uint8  *)0x39C)
#define sd_keys			((volatile uint8  *)0x39D)
#define sd_file_handle		(*(volatile uint16 *)0x3AA)
#define sd_file_seg		(*(volatile uint16 *)0x3AC)
#define sd_clip_tiles		(*(volatile uint16 *)0x3AE)
#define sd_tile_base		(*(volatile uint16 *)0x3B0)
#define sd_tile_x		(*(volatile uint16 *)0x3B2)
#define sd_tile_y		(*(volatile uint16 *)0x3B4)

#define sd_edge_mask		((const uint8 *)0x3C3)
#define sd_font			((const uint8 *)0x3FE)

void outp(uint16 port, uint8 value);
void outpw(uint16 port, uint16 value);
uint8 inp(uint16 port);
void int86(int no, union regs *in, union regs *out);
void int86x(int no, union regs *in, union regs *out, struct sregs *s);
void intdos(union regs *in, union regs *out);
void segread(struct sregs *s);
int16 bios16_poll(void);
void sd_set_page(uint8 page);
uint8 sd_latch(uint16 seg, uint16 off);
void sd_cli(void);
void sd_sti(void);
void sd_wait_retrace_asm(void);
void sd_far_movsw(uint16 dst_seg, uint16 dst_off, uint16 src_seg,
                  uint16 src_off, uint16 words);
void sd_far_movsb(uint16 dst_seg, uint16 dst_off, uint16 src_seg,
                  uint16 src_off, uint16 bytes);

void bios10(union regs *in, union regs *out, struct sregs *s);
void bios16(union regs *in, union regs *out, struct sregs *s);
void bios21(union regs *in, union regs *out, struct sregs *s);
void bios33(union regs *in, union regs *out, struct sregs *s);

void c_svc05(struct cframe far *f);

#endif
