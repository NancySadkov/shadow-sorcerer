/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  SDL3 front end: window/renderer, input, timing, asset loading and the
  flat memory arena that stands in for DOS conventional memory.
*/

#define SDL_MAIN_HANDLED 1
#include <SDL3/SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define SS_DELAY_MS(ms) emscripten_sleep(ms)
#else
#define SS_DELAY_MS(ms) SDL_Delay(ms)
#endif

#include "names.h"
#include "ss_platform.h"
#include "ss_ega.h"
#include <stdarg.h>
#ifdef _WIN32
#include <direct.h>
#define ss_chdir _chdir
#define ss_getcwd _getcwd
#else
#include <unistd.h>
#define ss_chdir chdir
#define ss_getcwd getcwd
#endif

uint8 *ss_arena;
uint8 ss_screen[SS_SCREEN_W * SS_SCREEN_H];

static SDL_Window *ss_window;
static SDL_Renderer *ss_renderer;
static SDL_Texture *ss_texture;
static uint32 ss_pixels[SS_SCREEN_W * SS_SCREEN_H];
static uint32 ss_pal_rgb[16];

static char ss_game_dir[512];
static char ss_out_dir[512];
static char ss_searched_dir[512];
static int ss_data_ok;

/* Title-menu settings. */
int ss_crt_filter = 0;
int ss_music_volume = 100;
int ss_sfx_volume = 100;
int ss_difficulty = SS_DIFF_NORMAL;
int ss_pending_load = 0;
int ss_pending_debug = 0;
int ss_debug_map = 0;
int ss_debug_world_x = -1;
int ss_debug_world_y = -1;
int ss_debug_monster = -1;
int ss_debug_item = -1;

int ss_scale_monster_damage(int amount)
{
	if (amount <= 0)
		return amount;
	if (ss_difficulty == SS_DIFF_EASY) {
		amount /= 2;
		return (amount < 1) ? 1 : amount;
	}
	if (ss_difficulty == SS_DIFF_HARD)
		return (amount * 3) / 2;
	return amount;
}

int ss_score_percent(void)
{
	if (ss_difficulty == SS_DIFF_EASY)
		return 50;
	if (ss_difficulty == SS_DIFF_HARD)
		return 150;
	return 100;
}

int ss_show_title(void)
{
	return SDL_getenv("SS_SKIP_TITLE") == NULL &&
	       SDL_getenv("SS_HEADLESS") == NULL;
}

static Uint64 ss_tick_next;
static Uint64 ss_frame_next;

/* SHADOW.COM's INT 1Ch handler advances the game counters once per BIOS
   tick.  The intro runs at the BIOS rate (17-18 Hz), but once GAME.EXE's
   sound tick is driving PIT channel 0 the counter runs at about 24 Hz
   (PLAYTEST.md: 184,196,208,220,232,245 over 2.5 s = 24.4/s).  The DOS
   build's wait loops and combat timers are calibrated for that. */
#define SS_TICK_MS 41u
/* Video refresh is still 60 Hz; only the game counters run at the BIOS
   timer rate.  Page flips and walk animations stay at the release's
   60 Hz presentation rate. */
#define SS_FRAME_MS 16u

static int ss_quit;
static int ss_headless;
static int ss_key_head, ss_key_tail;
static int16 ss_keys[64];
static uint16 ss_mouse_btn;
static uint16 ss_mouse_events;   /* buttons as last reported by events */
static int ss_mouse_seen;        /* a motion/button event has arrived */
static float ss_mouse_fx = -1.0f, ss_mouse_fy = -1.0f;

/* A fast click can deliver both BUTTON_DOWN and BUTTON_UP between two game
   polls.  Keep the down edge latched until the game has read it and a frame
   boundary has passed, so quick real clicks are not lost (the DOS loop is
   slower than SDL's event pump too). */
static uint8  ss_mouse_latch;
static uint8  ss_mouse_latch_seen;
static Uint64 ss_mouse_latch_time;
static Uint32 ss_mouse_latch_present;
static Uint64 ss_last_down_time[3];
static float  ss_last_down_x[3], ss_last_down_y[3];
static uint8  ss_double_click[3];
static Uint64 ss_double_time[3];
static Uint32 ss_present_stamp;

extern volatile uint16 g_shadow_counters[4];
extern void terminate_callback(void);

/* screenshot/test hooks */
static int ss_dump_at = -1;
static int ss_dump_exit;
static char ss_dump_path[512];
static int ss_frames;
static const char *ss_render_dump;
static int ss_render_dump_at = 120;
static Uint64 ss_dump_time = (Uint64)-1;
static Uint64 ss_click_at[8];
static int ss_click_count;
static float ss_click_x = 160.0f, ss_click_y = 100.0f;
static float ss_click_px[8], ss_click_py[8];
static int ss_click_pos_count;
static Uint64 ss_click_every;
static Uint64 ss_click_next;
static Uint64 ss_key_every;
static Uint64 ss_key_next;
static char ss_key_char;
static int16 ss_hold_dir = -1;
static Uint64 ss_hold_from, ss_hold_to;

/* Scripted SDL events for automated input tests (see SS_SYNTH). */
struct ss_synth_ev {
	Uint64 at;
	char kind;                 /* 'm' motion, 'b' button, 'k' key */
	float x, y;
	int a, b;
};
static struct ss_synth_ev ss_synth[128];
static int ss_synth_n;
static int ss_synth_i;
static Uint64 ss_last_click = (Uint64)-1;

static int ss_log_on;

static void ss_log(const char *fmt, ...)
{
	va_list ap;
	FILE *f;
	char path[600];

	if (!ss_log_on) {
		return;
	}
	SDL_snprintf(path, sizeof(path), "%s/tmp/ss.log", ss_out_dir);
	f = fopen(path, "a");
	if (f == NULL) {
		return;
	}
	va_start(ap, fmt);
	vfprintf(f, fmt, ap);
	va_end(ap);
	fclose(f);
}

/* Debug tracing used while reconciling the port with the release. */

static void ss_fail(const char *what)
{
	SDL_Log("shadow: %s: %s", what, SDL_GetError());
}

/* ------------------------------------------------------------------ */
/* files                                                              */
/* ------------------------------------------------------------------ */

static void ss_join(const char *name, char *out, int size)
{
	const char *dir = ss_game_dir;
	int n = 0;

	if (dir[0] == 0) {
		SDL_snprintf(out, size, "%s", name);
		return;
	}
	while (dir[n] != 0 && n < size - 2) {
		out[n] = dir[n];
		n++;
	}
	if (n > 0 && out[n - 1] != '/' && out[n - 1] != '\\') {
		out[n++] = '/';
	}
	SDL_snprintf(out + n, (size_t)(size - n), "%s", name);
}

int ss_path_for(const char *dos_name, char *out, int out_size)
{
	char buf[32];
	int i;

	/* DOS names are 8.3; keep it short and terminate. */
	for (i = 0; i < (int)sizeof(buf) - 1 && dos_name[i] != 0; i++) {
		buf[i] = dos_name[i];
	}
	buf[i] = 0;

	ss_join(buf, out, out_size);
	return 1;
}

int32 ss_load_asset(const char *name, uint16 dest_para)
{
	char namebuf[16];
	char path[512];
	FILE *f;
	long size;
	uint32 base = SS_SEG_ADDR(dest_para);
	uint32 room;

	if (dest_para == 0) {
		return 0;
	}
	strncpy(namebuf, name, sizeof(namebuf) - 1);
	namebuf[sizeof(namebuf) - 1] = 0;

	ss_path_for(namebuf, path, sizeof(path));
	f = fopen(path, "rb");
	if (f == NULL) {
		return 0;
	}
	if (fseek(f, 0, SEEK_END) != 0) {
		fclose(f);
		return 0;
	}
	size = ftell(f);
	if (size <= 0 || size > 0xFFFF) {
		fclose(f);
		return 0;
	}
	if (fseek(f, 0, SEEK_SET) != 0) {
		fclose(f);
		return 0;
	}
	room = SS_SEG_BYTES - base;
	if ((uint32)size > room) {
		size = (long)room;
	}
	if (fread(ss_arena + base, 1, (size_t)size, f) != (size_t)size) {
		fclose(f);
		return 0;
	}
	fclose(f);
	return (int32)size;
}

/* ------------------------------------------------------------------ */
/* video                                                              */
/* ------------------------------------------------------------------ */

void ss_set_dac(int index, int r, int g, int b)
{
	ss_ega_set_dac(index, r, g, b);
}

static void ss_refresh_palette(void)
{
	int i;
	for (i = 0; i < 16; i++) {
		int rr, gg, bb;
		ss_ega_color(i, &rr, &gg, &bb);
		ss_pal_rgb[i] = ((uint32)((rr * 255 + 31) / 63) << 16) |
		                ((uint32)((gg * 255 + 31) / 63) << 8) |
		                (uint32)((bb * 255 + 31) / 63);
	}
}

static void ss_dump_screen(void)
{
	if (ss_dump_path[0] == 0) {
		return;
	}
	{
		char raw[600];
		FILE *f;
		SDL_snprintf(raw, sizeof(raw), "%s.idx", ss_dump_path);
		f = fopen(raw, "wb");
		if (f != NULL) {
			fwrite(ss_screen, 1, sizeof(ss_screen), f);
			fclose(f);
		}
	}
	{
		SDL_Surface *s = SDL_CreateSurface(SS_SCREEN_W, SS_SCREEN_H,
		                                   SDL_PIXELFORMAT_XRGB8888);
		if (s != NULL) {
			int y;
			for (y = 0; y < SS_SCREEN_H; y++) {
				uint32 *dst = (uint32 *)((uint8 *)s->pixels + (size_t)y * s->pitch);
				int x;
				for (x = 0; x < SS_SCREEN_W; x++) {
					dst[x] = ss_pal_rgb[ss_screen[y * SS_SCREEN_W + x] & 0x0F];
				}
			}
			SDL_SaveBMP(s, ss_dump_path);
			SDL_DestroySurface(s);
		}
	}
}

void ss_present(void)
{
	int i, n = SS_SCREEN_W * SS_SCREEN_H;

	ss_refresh_palette();
	if (!ss_headless && ss_renderer != NULL) {
		for (i = 0; i < n; i++) {
			ss_pixels[i] = ss_pal_rgb[ss_screen[i] & 0x0F];
		}
		SDL_UpdateTexture(ss_texture, NULL, ss_pixels, SS_SCREEN_W * 4);
		SDL_RenderClear(ss_renderer);
		SDL_RenderTexture(ss_renderer, ss_texture, NULL, NULL);
		if (ss_crt_filter) {
			int y;
			SDL_SetRenderDrawBlendMode(ss_renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(ss_renderer, 0, 0, 0, 160);
			for (y = 1; y < SS_SCREEN_H; y += 2) {
				SDL_FRect r;
				r.x = 0.0f;
				r.y = (float)y;
				r.w = (float)SS_SCREEN_W;
				r.h = 1.0f;
				SDL_RenderFillRect(ss_renderer, &r);
			}
			SDL_SetRenderDrawBlendMode(ss_renderer, SDL_BLENDMODE_NONE);
		}
		if (ss_render_dump &&
		    (ss_frames + 1) >= (ss_render_dump_at > 0 ? ss_render_dump_at : 1)) {
			SDL_Surface *shot = SDL_RenderReadPixels(ss_renderer, NULL);
			if (shot != NULL) {
				SDL_SaveBMP(shot, ss_render_dump);
				SDL_DestroySurface(shot);
			}
			ss_render_dump = NULL;
		}
		SDL_RenderPresent(ss_renderer);
	}

	ss_frames++;
	if (ss_dump_time != (Uint64)-1 && SDL_GetTicks() >= ss_dump_time) {
		ss_dump_time = (Uint64)-1;
		ss_dump_screen();
		if (ss_dump_exit) {
			ss_quit = 1;
		}
	}
	if (ss_dump_at >= 0 && ss_frames >= ss_dump_at) {
		ss_dump_screen();
		ss_dump_at = -1;
		if (ss_dump_exit) {
			ss_quit = 1;
		}
	}
}

int ss_quit_requested(void)
{
	return ss_quit;
}

/* ------------------------------------------------------------------ */
/* input                                                              */
/* ------------------------------------------------------------------ */

/* Physical scan codes (not key symbols), so Caps Lock, Shift and non-US
   layouts do not change which DOS key the game sees.  The numeric keypad is
   mapped to the same scan codes the release's INT 09 handler recorded. */
static const struct {
	SDL_Scancode sc;
	uint8 scan;
	uint8 ascii;
} ss_keymap[] = {
	{ SDL_SCANCODE_ESCAPE, 0x01, 0x1B },
	{ SDL_SCANCODE_1, 0x02, '1' }, { SDL_SCANCODE_2, 0x03, '2' },
	{ SDL_SCANCODE_3, 0x04, '3' }, { SDL_SCANCODE_4, 0x05, '4' },
	{ SDL_SCANCODE_5, 0x06, '5' }, { SDL_SCANCODE_6, 0x07, '6' },
	{ SDL_SCANCODE_7, 0x08, '7' }, { SDL_SCANCODE_8, 0x09, '8' },
	{ SDL_SCANCODE_9, 0x0A, '9' }, { SDL_SCANCODE_0, 0x0B, '0' },
	{ SDL_SCANCODE_MINUS, 0x0C, '-' }, { SDL_SCANCODE_EQUALS, 0x0D, '=' },
	{ SDL_SCANCODE_BACKSPACE, 0x0E, 0x08 }, { SDL_SCANCODE_TAB, 0x0F, 0x09 },
	{ SDL_SCANCODE_Q, 0x10, 'q' }, { SDL_SCANCODE_W, 0x11, 'w' },
	{ SDL_SCANCODE_E, 0x12, 'e' }, { SDL_SCANCODE_R, 0x13, 'r' },
	{ SDL_SCANCODE_T, 0x14, 't' }, { SDL_SCANCODE_Y, 0x15, 'y' },
	{ SDL_SCANCODE_U, 0x16, 'u' }, { SDL_SCANCODE_I, 0x17, 'i' },
	{ SDL_SCANCODE_O, 0x18, 'o' }, { SDL_SCANCODE_P, 0x19, 'p' },
	{ SDL_SCANCODE_LEFTBRACKET, 0x1A, '[' },
	{ SDL_SCANCODE_RIGHTBRACKET, 0x1B, ']' },
	{ SDL_SCANCODE_RETURN, 0x1C, 0x0D },
	{ SDL_SCANCODE_KP_ENTER, 0x1C, 0x0D },
	{ SDL_SCANCODE_LCTRL, 0x1D, 0 }, { SDL_SCANCODE_RCTRL, 0x1D, 0 },
	{ SDL_SCANCODE_A, 0x1E, 'a' }, { SDL_SCANCODE_S, 0x1F, 's' },
	{ SDL_SCANCODE_D, 0x20, 'd' }, { SDL_SCANCODE_F, 0x21, 'f' },
	{ SDL_SCANCODE_G, 0x22, 'g' }, { SDL_SCANCODE_H, 0x23, 'h' },
	{ SDL_SCANCODE_J, 0x24, 'j' }, { SDL_SCANCODE_K, 0x25, 'k' },
	{ SDL_SCANCODE_L, 0x26, 'l' }, { SDL_SCANCODE_SEMICOLON, 0x27, ';' },
	{ SDL_SCANCODE_APOSTROPHE, 0x28, '\'' }, { SDL_SCANCODE_GRAVE, 0x29, '`' },
	{ SDL_SCANCODE_LSHIFT, 0x2A, 0 },
	{ SDL_SCANCODE_BACKSLASH, 0x2B, '\\' },
	{ SDL_SCANCODE_Z, 0x2C, 'z' }, { SDL_SCANCODE_X, 0x2D, 'x' },
	{ SDL_SCANCODE_C, 0x2E, 'c' }, { SDL_SCANCODE_V, 0x2F, 'v' },
	{ SDL_SCANCODE_B, 0x30, 'b' }, { SDL_SCANCODE_N, 0x31, 'n' },
	{ SDL_SCANCODE_M, 0x32, 'm' }, { SDL_SCANCODE_COMMA, 0x33, ',' },
	{ SDL_SCANCODE_PERIOD, 0x34, '.' }, { SDL_SCANCODE_SLASH, 0x35, '/' },
	{ SDL_SCANCODE_RSHIFT, 0x36, 0 },
	{ SDL_SCANCODE_KP_MULTIPLY, 0x37, '*' },
	{ SDL_SCANCODE_LALT, 0x38, 0 }, { SDL_SCANCODE_RALT, 0x38, 0 },
	{ SDL_SCANCODE_SPACE, 0x39, 0x20 }, { SDL_SCANCODE_CAPSLOCK, 0x3A, 0 },
	{ SDL_SCANCODE_F1, 0x3B, 0 }, { SDL_SCANCODE_F2, 0x3C, 0 },
	{ SDL_SCANCODE_F3, 0x3D, 0 }, { SDL_SCANCODE_F4, 0x3E, 0 },
	{ SDL_SCANCODE_F5, 0x3F, 0 }, { SDL_SCANCODE_F6, 0x40, 0 },
	{ SDL_SCANCODE_F7, 0x41, 0 }, { SDL_SCANCODE_F8, 0x42, 0 },
	{ SDL_SCANCODE_F9, 0x43, 0 }, { SDL_SCANCODE_F10, 0x44, 0 },
	{ SDL_SCANCODE_NUMLOCKCLEAR, 0x45, 0 },
	{ SDL_SCANCODE_HOME, 0x47, 0 }, { SDL_SCANCODE_UP, 0x48, 0 },
	{ SDL_SCANCODE_PAGEUP, 0x49, 0 },
	{ SDL_SCANCODE_KP_MINUS, 0x4A, '-' },
	{ SDL_SCANCODE_LEFT, 0x4B, 0 }, { SDL_SCANCODE_KP_5, 0x4C, '5' },
	{ SDL_SCANCODE_RIGHT, 0x4D, 0 }, { SDL_SCANCODE_KP_PLUS, 0x4E, '+' },
	{ SDL_SCANCODE_END, 0x4F, 0 }, { SDL_SCANCODE_DOWN, 0x50, 0 },
	{ SDL_SCANCODE_PAGEDOWN, 0x51, 0 }, { SDL_SCANCODE_INSERT, 0x52, 0 },
	{ SDL_SCANCODE_DELETE, 0x53, 0 },
	{ SDL_SCANCODE_KP_7, 0x47, '7' }, { SDL_SCANCODE_KP_8, 0x48, '8' },
	{ SDL_SCANCODE_KP_9, 0x49, '9' },
	{ SDL_SCANCODE_KP_4, 0x4B, '4' }, { SDL_SCANCODE_KP_6, 0x4D, '6' },
	{ SDL_SCANCODE_KP_1, 0x4F, '1' }, { SDL_SCANCODE_KP_2, 0x50, '2' },
	{ SDL_SCANCODE_KP_3, 0x51, '3' }, { SDL_SCANCODE_KP_0, 0x52, '0' },
	{ SDL_SCANCODE_KP_PERIOD, 0x53, '.' },
	{ SDL_SCANCODE_KP_DIVIDE, 0x35, '/' },
};

static void ss_push_key(int16 v)
{
	int next = (ss_key_tail + 1) % 64;

	if (next == ss_key_head) {
		return;
	}
	ss_keys[ss_key_tail] = v;
	ss_key_tail = next;
	g_shadow_counters[0]++;
}

static void ss_handle_key(const SDL_KeyboardEvent *kev)
{
	size_t i;
	uint8 scan = 0;
	uint8 ascii = 0;

	for (i = 0; i < sizeof(ss_keymap) / sizeof(ss_keymap[0]); i++) {
		if (ss_keymap[i].sc == kev->scancode) {
			scan = ss_keymap[i].scan;
			break;
		}
	}
	if (scan == 0) {
		return;
	}
	if ((kev->mod & SDL_KMOD_ALT) == 0) {
		/* Prefer the produced character (Shift/CapsLock/layout aware). */
		if (kev->key >= 32 && kev->key < 127) {
			ascii = (uint8)kev->key;
		} else if (i < sizeof(ss_keymap) / sizeof(ss_keymap[0])) {
			ascii = ss_keymap[i].ascii;
		}
		/* With Num Lock off the keypad acts as the navigation keys. */
		if ((kev->mod & SDL_KMOD_NUM) == 0 &&
		    kev->scancode >= SDL_SCANCODE_KP_1 &&
		    kev->scancode <= SDL_SCANCODE_KP_0) {
			ascii = 0;
		}
	}
	ss_push_key((int16)(((uint16)scan << 8) | ascii));
}

/* Held direction keys: arrows plus the numeric keypad, like the original's
   joystick/numpad movement.  Returns 0..7 (0 = up, then clockwise) or -1. */
/* Held direction keys: the numeric keypad gives the exact 8-way direction
   (0 = up, then clockwise); the arrow keys give a screen cardinal (8 = up,
   9 = right, 10 = down, 11 = left) which the walker resolves to the nearest
   available neighbour, because an isometric grid has no tile straight above. */
int16 ss_direction_step(void)
{
	static const SDL_Scancode kp[8] = {
		SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_9, SDL_SCANCODE_KP_6,
		SDL_SCANCODE_KP_3, SDL_SCANCODE_KP_2, SDL_SCANCODE_KP_1,
		SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_7,
	};
	static const SDL_Scancode arrows[4] = {
		SDL_SCANCODE_UP, SDL_SCANCODE_RIGHT,
		SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT,
	};
	static int16 held = -1;
	static Uint64 next;
	const bool *ks = SDL_GetKeyboardState(NULL);
	Uint64 now = SDL_GetTicks();
	int i;
	int16 d = -1;

	if (ss_hold_dir >= 0 && now >= ss_hold_from && now < ss_hold_to) {
		d = ss_hold_dir;
	} else {
		for (i = 0; i < 8; i++) {
			if (ks[kp[i]]) {
				d = (int16)i;
				break;
			}
		}
		if (d < 0) {
			for (i = 0; i < 4; i++) {
				if (ks[arrows[i]]) {
					d = (int16)(8 + i);
					break;
				}
			}
		}
	}
	if (d < 0) {
		held = -1;
		next = 0;
		return -1;
	}
	if (d != held) {
		held = d;
		next = now + 300;          /* first step, then auto-repeat */
		return d;
	}
	if (now >= next) {
		next = now + 130;
		return d;
	}
	return -1;
}

static void ss_mouse_from_window(float wx, float wy)
{
	float lx = wx, ly = wy;

	if (ss_renderer != NULL) {
		if (!SDL_RenderCoordinatesFromWindow(ss_renderer, wx, wy, &lx, &ly)) {
			return;
		}
	}
	ss_mouse_fx = lx;
	ss_mouse_fy = ly;
	ss_mouse_seen = 1;
}

static void ss_update_mouse(void)
{
	uint16 base = ss_mouse_events;   /* buttons as last reported by events */

	/* The pointer position comes from mouse events; SDL_GetMouseState is only
	   used until the first event arrives (its value can be stale or refer to a
	   different window, which used to make clicks land elsewhere). */
	if (!ss_mouse_seen) {
		float mx = 0.0f, my = 0.0f;
		SDL_MouseButtonFlags btn = SDL_GetMouseState(&mx, &my);

		if (ss_window != NULL &&
		    (SDL_GetWindowFlags(ss_window) & SDL_WINDOW_INPUT_FOCUS) != 0) {
			ss_mouse_from_window(mx, my);
			base = 0;
			if ((btn & SDL_BUTTON_LMASK) != 0) {
				base |= 1;
			}
			if ((btn & SDL_BUTTON_RMASK) != 0) {
				base |= 2;
			}
		}
	}
	ss_mouse_btn = base;
	{
		int i;
		Uint64 now = SDL_GetTicks();
		for (i = 0; i < ss_click_count; i++) {
			if (now >= ss_click_at[i] && now < ss_click_at[i] + 260u) {
				int k = (ss_click_pos_count > 0)
				        ? (i < ss_click_pos_count ? i : ss_click_pos_count - 1) : -1;
				ss_mouse_btn |= 1;
				ss_mouse_fx = (k >= 0) ? ss_click_px[k] : ss_click_x;
				ss_mouse_fy = (k >= 0) ? ss_click_py[k] : ss_click_y;
			}
		}
		if (ss_click_every != 0 && now >= ss_click_next) {
			ss_click_next = now + ss_click_every;
			ss_last_click = now;
		}
		if (ss_click_every != 0 && now >= ss_last_click && now < ss_last_click + 260u) {
			ss_mouse_btn |= 1;
			ss_mouse_fx = ss_click_x;
			ss_mouse_fy = ss_click_y;
		}
	}
}

void ss_mouse_read(uint16 *x, uint16 *y, uint16 *buttons)
{
	static int in_pump;

	if (!in_pump) {
		/* Mouse polling happens in tight wait loops too, so service SDL
		   events here as well as in shadow_poll(). */
		in_pump = 1;
		ss_pump();
		in_pump = 0;
	}
	ss_update_mouse();
	{
		float fx = ss_mouse_fx;
		float fy = ss_mouse_fy;
		int ix, iy;

		if (fx < 0) fx = 0;
		if (fy < 0) fy = 0;
		ix = (int)(fx + 0.5f);
		iy = (int)(fy + 0.5f);
		if (ix > SS_SCREEN_W - 1) ix = SS_SCREEN_W - 1;
		if (iy > SS_SCREEN_H - 1) iy = SS_SCREEN_H - 1;
		if (x != NULL) *x = (uint16)ix;
		if (y != NULL) *y = (uint16)iy;
	}
	if (buttons != NULL) {
		uint16 out = ss_mouse_btn;

		if (ss_mouse_latch != 0) {
			Uint64 now = SDL_GetTicks();

			if (ss_mouse_latch_seen == 0) {
				ss_mouse_latch_seen = 1;
				/* Survive the frame in which it was first seen and the
				   next one, so a click arriving just after the menu check
				   is still seen by the following iteration. */
				ss_mouse_latch_present = ss_present_stamp + 1;
				/* Start the hold window when the game first sees it,
				   not when the host event arrived. */
				ss_mouse_latch_time = now;
			}
			if (ss_present_stamp > ss_mouse_latch_present ||
			    now - ss_mouse_latch_time >= 120u) {
				/* The game has had a frame (or plenty of polls) to see
				   the press; let the release through now. */
				ss_mouse_latch = 0;
			} else {
				out |= 1;
			}
		}
		*buttons = out;
	}
}

void ss_mouse_peek(uint16 *x, uint16 *y)
{
	float fx, fy;
	int ix, iy;

	ss_update_mouse();
	fx = ss_mouse_fx;
	fy = ss_mouse_fy;
	if (fx < 0) fx = 0;
	if (fy < 0) fy = 0;
	ix = (int)(fx + 0.5f);
	iy = (int)(fy + 0.5f);
	if (ix > SS_SCREEN_W - 1) ix = SS_SCREEN_W - 1;
	if (iy > SS_SCREEN_H - 1) iy = SS_SCREEN_H - 1;
	if (x != NULL) *x = (uint16)ix;
	if (y != NULL) *y = (uint16)iy;
}

/* Returns and clears the double-click flag for button 1 (left) or 2 (right). */
int16 ss_take_double_click(int16 button)
{
	int16 r;

	if (button < 1 || button > 2)
		return 0;
	r = (int16)ss_double_click[button];
	if (r != 0 && SDL_GetTicks() - ss_double_time[button] > 200u)
		r = 0;
	ss_double_click[button] = 0;
	return r;
}

void ss_set_mouse(int16 x, int16 y)
{
	if (ss_window == NULL) {
		return;
	}
	{
		float wx = 0, wy = 0;
		if (ss_renderer != NULL) {
			SDL_RenderCoordinatesToWindow(ss_renderer, (float)x, (float)y, &wx, &wy);
		}
		SDL_WarpMouseInWindow(ss_window, wx, wy);
	}
}

int16 ss_key_poll(void)
{
	if (ss_key_head == ss_key_tail) {
		return 0;
	}
	{
		int16 v = ss_keys[ss_key_head];
		ss_key_head = (ss_key_head + 1) % 64;
		return v;
	}
}

int16 ss_key_wait(void)
{
	int16 k;

	for (;;) {
		ss_pump();
		k = ss_key_poll();
		if (k != 0) {
			return k;
		}
		SS_DELAY_MS(1);
	}
}

/* ------------------------------------------------------------------ */
/* timing / events                                                    */
/* ------------------------------------------------------------------ */

static void ss_fire_synth(Uint64 now)
{
	while (ss_synth_i < ss_synth_n && now >= ss_synth[ss_synth_i].at) {
		struct ss_synth_ev *e = &ss_synth[ss_synth_i++];
		SDL_Event ev;

		SDL_zero(ev);
		if (e->kind == 'm') {
			ev.type = SDL_EVENT_MOUSE_MOTION;
			ev.motion.x = e->x;
			ev.motion.y = e->y;
		} else if (e->kind == 'b') {
			ev.type = (e->b != 0) ? SDL_EVENT_MOUSE_BUTTON_DOWN
			                      : SDL_EVENT_MOUSE_BUTTON_UP;
			ev.button.button = (Uint8)e->a;
			ev.button.down = (bool)(e->b != 0);
			ev.button.x = e->x;
			ev.button.y = e->y;
		} else if (e->kind == 'k') {
			ev.type = SDL_EVENT_KEY_DOWN;
			ev.key.key = (SDL_Keycode)e->a;
			ev.key.scancode = (SDL_Scancode)e->b;
			ev.key.repeat = 0;
		}
		SDL_PushEvent(&ev);
		ss_log("synth %c %d %d\n", e->kind, e->a, e->b);
	}
}

void ss_pump(void)
{
	SDL_Event ev;
	int had_event = 0;
	Uint64 now;

	ss_fire_synth(SDL_GetTicks());

	while (SDL_PollEvent(&ev)) {
		had_event = 1;
		switch (ev.type) {
		case SDL_EVENT_QUIT:
			ss_quit = 1;
			break;
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			ss_quit = 1;
			break;
		case SDL_EVENT_KEY_DOWN:
			if (ev.key.repeat == 0) {
				ss_handle_key(&ev.key);
			}
			break;
		case SDL_EVENT_MOUSE_MOTION:
			ss_mouse_from_window(ev.motion.x, ev.motion.y);
			ss_log("motion %g,%g -> %g,%g\n", (double)ev.motion.x,
			       (double)ev.motion.y, (double)ss_mouse_fx, (double)ss_mouse_fy);
			ss_mouse_events = 0;
			if ((ev.motion.state & SDL_BUTTON_LMASK) != 0) {
				ss_mouse_events |= 1;
			}
			if ((ev.motion.state & SDL_BUTTON_RMASK) != 0) {
				ss_mouse_events |= 2;
			}
			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP: {
			uint16 bit = (uint16)((ev.button.button == SDL_BUTTON_LEFT) ? 1 :
			                      (ev.button.button == SDL_BUTTON_RIGHT) ? 2 : 0);
			ss_mouse_from_window(ev.button.x, ev.button.y);
			if (ev.button.down) {
				ss_mouse_events |= bit;
				if (bit != 0) {
					Uint64 now = SDL_GetTicks();
					float dx = ss_mouse_fx - ss_last_down_x[bit];
					float dy = ss_mouse_fy - ss_last_down_y[bit];

					if (dx < 0) dx = -dx;
					if (dy < 0) dy = -dy;
					if (ss_last_down_time[bit] != 0
					    && now - ss_last_down_time[bit] <= 350u
					    && dx <= 6.0f && dy <= 6.0f) {
						ss_double_click[bit] = 1;
						ss_double_time[bit] = now;
					}
					ss_last_down_time[bit] = now;
					ss_last_down_x[bit] = ss_mouse_fx;
					ss_last_down_y[bit] = ss_mouse_fy;
				}
				if ((bit & 1) != 0) {
					ss_mouse_latch = 1;
					ss_mouse_latch_seen = 0;
					ss_mouse_latch_time = SDL_GetTicks();
				}
			} else {
				ss_mouse_events &= (uint16)~bit;
			}
			g_shadow_counters[0]++;
			break;
		}
		default:
			break;
		}
	}
	if (ss_quit) {
		terminate_callback();
	}

	now = SDL_GetTicks();
	if (ss_key_every != 0 && now >= ss_key_next) {
		unsigned char ch = (unsigned char)ss_key_char;
		ss_key_next = now + ss_key_every;
		ss_push_key((int16)((((uint16)0x39) << 8) | ch));
	}
	if (now >= ss_tick_next) {
		Uint64 n = (now - ss_tick_next) / SS_TICK_MS + 1u;
		if (n > 4) {
			n = 4;
		}
		ss_tick_next += n * SS_TICK_MS;
		g_shadow_counters[3] = (uint16)(g_shadow_counters[3] + n);
		g_shadow_counters[0] = (uint16)(g_shadow_counters[0] + n);
	} else if (!had_event) {
		SS_DELAY_MS(1);
	}

	if (ss_dump_time != (Uint64)-1 && now >= ss_dump_time) {
		ss_dump_screen();
		ss_dump_time = (Uint64)-1;
		if (ss_dump_exit) {
			ss_quit = 1;
		}
	}
}

void ss_wait_frame(void)
{
	Uint64 now = SDL_GetTicks();

	ss_present_stamp++;
	/* Keep input and the pointer real-time while the game is waiting for
	   its next (possibly 24 Hz) logic tick. */
	while (now < ss_frame_next) {
		ss_pump();
		ss_cursor_tick();
		SS_DELAY_MS(1);
		now = SDL_GetTicks();
	}
	ss_frame_next += SS_FRAME_MS;
	if (ss_frame_next < now) {
		ss_frame_next = now + SS_FRAME_MS;
	}
}

void ss_wait_ticks(int32 ticks)
{
	uint16 start = g_shadow_counters[3];

	while ((uint16)(g_shadow_counters[3] - start) < (uint16)ticks) {
		ss_pump();
		SS_DELAY_MS(1);
	}
}

/* ------------------------------------------------------------------ */
/* startup                                                            */
/* ------------------------------------------------------------------ */

static int ss_file_present(const char *dir, const char *name)
{
	char path[1024];
	FILE *f;

	if (dir[0] == 0) {
		SDL_snprintf(path, sizeof(path), "%s", name);
	} else {
		SDL_snprintf(path, sizeof(path), "%s/%s", dir, name);
	}
	f = fopen(path, "rb");
	if (f == NULL) {
		return 0;
	}
	fclose(f);
	return 1;
}

static int ss_dir_has_data(const char *dir)
{
	return ss_file_present(dir, "vclips.dat") ||
	       ss_file_present(dir, "VCLIPS.DAT");
}

static void ss_read_env(void)
{
	const char *p;
	char exe_dir[512];
	char candidate[1024];

#ifdef __EMSCRIPTEN__
	if (SDL_getenv("SS_GAME_DIR") == NULL)
		setenv("SS_GAME_DIR", "/game", 1);
#endif

	if (ss_getcwd(ss_out_dir, sizeof(ss_out_dir)) == NULL) {
		ss_out_dir[0] = 0;
	}

	/* Data search order: explicit option/environment, the executable's
	   own directory, then the working directory. */
	ss_searched_dir[0] = 0;
	exe_dir[0] = 0;
	{
		const char *base = SDL_GetBasePath();
		if (base != NULL) {
			SDL_snprintf(exe_dir, sizeof(exe_dir), "%s", base);
			/* drop the trailing separator SDL adds */
			{
				size_t n = SDL_strlen(exe_dir);
				while (n > 1 && (exe_dir[n - 1] == '/' || exe_dir[n - 1] == '\\')) {
					exe_dir[--n] = 0;
				}
			}
		}
	}

	p = SDL_getenv("SS_GAME_DIR");
	if (p != NULL && p[0] != 0) {
		SDL_snprintf(candidate, sizeof(candidate), "%s", p);
		if (ss_dir_has_data(candidate)) {
			SDL_snprintf(ss_game_dir, sizeof(ss_game_dir), "%s", candidate);
			ss_data_ok = 1;
		} else if (ss_searched_dir[0] == 0) {
			SDL_snprintf(ss_searched_dir, sizeof(ss_searched_dir), "%s", candidate);
		}
	}
	if (!ss_data_ok && exe_dir[0] != 0) {
		SDL_snprintf(candidate, sizeof(candidate), "%s/game", exe_dir);
		if (ss_dir_has_data(candidate)) {
			SDL_snprintf(ss_game_dir, sizeof(ss_game_dir), "%s", candidate);
			ss_data_ok = 1;
		} else if (ss_searched_dir[0] == 0) {
			SDL_snprintf(ss_searched_dir, sizeof(ss_searched_dir), "%s", candidate);
		}
	}
	if (!ss_data_ok && ss_out_dir[0] != 0) {
		SDL_snprintf(candidate, sizeof(candidate), "%s/game", ss_out_dir);
		if (ss_dir_has_data(candidate)) {
			SDL_snprintf(ss_game_dir, sizeof(ss_game_dir), "%s", candidate);
			ss_data_ok = 1;
		} else if (ss_searched_dir[0] == 0) {
			SDL_snprintf(ss_searched_dir, sizeof(ss_searched_dir), "%s", candidate);
		}
	}
	if (!ss_data_ok && ss_out_dir[0] != 0) {
		/* The executable may have been copied into the data directory. */
		if (ss_dir_has_data(ss_out_dir)) {
			SDL_snprintf(ss_game_dir, sizeof(ss_game_dir), "%s", ss_out_dir);
			ss_data_ok = 1;
		}
	}
	if (!ss_data_ok && ss_searched_dir[0] == 0) {
		SDL_snprintf(ss_searched_dir, sizeof(ss_searched_dir), "%s/game", ss_out_dir);
	}

	/* The release runs with its own directory as the current directory:
	   MAPED, TEXT.DAT and the save files are opened by bare name. */
	if (ss_data_ok && ss_chdir(ss_game_dir) == 0) {
		ss_game_dir[0] = 0;
	} else if (ss_data_ok) {
		ss_log("cannot chdir to %s\n", ss_game_dir);
	}

	p = SDL_getenv("SS_SCREENSHOT");
	if (p != NULL && p[0] != 0) {
		if (p[0] == '/' || (p[1] == ':')) {
			SDL_snprintf(ss_dump_path, sizeof(ss_dump_path), "%s", p);
		} else {
			SDL_snprintf(ss_dump_path, sizeof(ss_dump_path), "%s/%s", ss_out_dir, p);
		}
		ss_dump_at = 120;
		p = SDL_getenv("SS_SCREENSHOT_AT");
		if (p != NULL) {
			ss_dump_at = SDL_atoi(p);
		}
		p = SDL_getenv("SS_SCREENSHOT_EXIT");
		if (p != NULL && SDL_atoi(p) != 0) {
			ss_dump_exit = 1;
		}
		p = SDL_getenv("SS_SCREENSHOT_TIME");
		if (p != NULL) {
			ss_dump_time = (Uint64)SDL_atoi(p);
		}
	}
	p = SDL_getenv("SS_RENDER_DUMP");
	if (p != NULL && p[0] != 0) {
		ss_render_dump = p;
		p = SDL_getenv("SS_RENDER_DUMP_AT");
		if (p != NULL) {
			ss_render_dump_at = SDL_atoi(p);
		}
	}
	p = SDL_getenv("SS_CLICK_AT");	if (p != NULL) {
		char buf[128];
		char *tok;
		char *save = NULL;

		SDL_snprintf(buf, sizeof(buf), "%s", p);
		tok = strtok_r(buf, ",", &save);
		while (tok != NULL && ss_click_count < 8) {
			ss_click_at[ss_click_count++] = (Uint64)SDL_atoi(tok);
			tok = strtok_r(NULL, ",", &save);
		}
	}
	p = SDL_getenv("SS_CLICK_POS");
	if (p != NULL) {
		char buf[256];
		char *save = NULL;
		char *tok;

		SDL_snprintf(buf, sizeof(buf), "%s", p);
		if (strchr(buf, ';') == NULL) {
			int x = 160, y = 100;
			if (sscanf(buf, "%d,%d", &x, &y) == 2) {
				ss_click_x = (float)x;
				ss_click_y = (float)y;
			}
		} else {
			tok = strtok_r(buf, ";", &save);
			while (tok != NULL && ss_click_pos_count < 8) {
				int x = 160, y = 100;
				if (sscanf(tok, "%d,%d", &x, &y) == 2) {
					ss_click_px[ss_click_pos_count] = (float)x;
					ss_click_py[ss_click_pos_count] = (float)y;
					ss_click_pos_count++;
				}
				tok = strtok_r(NULL, ";", &save);
			}
		}
	}
	p = SDL_getenv("SS_CLICK_EVERY");
	if (p != NULL) {
		ss_click_every = (Uint64)SDL_atoi(p);
		ss_click_next = (Uint64)SDL_atoi(SDL_getenv("SS_CLICK_FROM") ? SDL_getenv("SS_CLICK_FROM") : "1000");
	}
	p = SDL_getenv("SS_KEY_EVERY");
	if (p != NULL) {
		ss_key_every = (Uint64)SDL_atoi(p);
		ss_key_next = (Uint64)SDL_atoi(SDL_getenv("SS_KEY_FROM") ? SDL_getenv("SS_KEY_FROM") : "1000");
		ss_key_char = ' ';
		p = SDL_getenv("SS_KEY_CHAR");
		if (p != NULL && p[0] != 0) {
			ss_key_char = p[0];
		}
	}
	p = SDL_getenv("SS_HOLD_DIR");
	if (p != NULL) {
		ss_hold_dir = (int16)SDL_atoi(p);
		ss_hold_from = (Uint64)SDL_atoi(SDL_getenv("SS_HOLD_FROM") ? SDL_getenv("SS_HOLD_FROM") : "0");
		ss_hold_to = (Uint64)SDL_atoi(SDL_getenv("SS_HOLD_TO") ? SDL_getenv("SS_HOLD_TO") : "600000");
	}
	p = SDL_getenv("SS_SYNTH");
	if (p != NULL) {
		char buf[4096];
		char *save = NULL;
		char *tok;

		SDL_snprintf(buf, sizeof(buf), "%s", p);
		tok = strtok_r(buf, ",", &save);
		while (tok != NULL && ss_synth_n < 128) {
			struct ss_synth_ev *e = &ss_synth[ss_synth_n];
			int t = 0;
			char kind = 0;
			int a = 0, b = 0, c = 0, d = 0;

			if (sscanf(tok, "%d:%c:%d:%d:%d:%d", &t, &kind, &a, &b, &c, &d) >= 2) {
				e->at = (Uint64)t;
				e->kind = kind;
				e->a = a;
				e->b = b;
				e->x = (float)a;
				e->y = (float)b;
				if (kind == 'b') {
					/* b:<button>:<down>:<window x>:<window y> */
					e->x = (float)c;
					e->y = (float)d;
				}
				ss_synth_n++;
			}
			tok = strtok_r(NULL, ",", &save);
		}
	}
}

static void ss_check_data(void)
{
	char msg[1400];

	if (ss_data_ok) {
		return;
	}
	SDL_snprintf(msg, sizeof(msg),
	             "Shadow Sorcerer could not find its data files.\n\n"
	             "Looked in:\n  %s\n\n"
	             "Run with --game-dir=<path> (or set SS_GAME_DIR) pointing at a "
	             "directory that contains VCLIPS.DAT, VMON*.DAT, VMPL*.DAT, "
	             "VOBJPIC.DAT, VMAPPIC.DAT, VMISSLE.DAT, VCIN*.DAT, TEXT.DAT, "
	             "MAPED and the sound files.\n\n"
	             "You can also put the files in a 'game' directory next to the "
	             "executable.",
	             ss_searched_dir[0] ? ss_searched_dir : "(none)");
	if (ss_headless) {
		ss_log("%s\n", msg);
		fprintf(stderr, "%s\n", msg);
	} else {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shadow Sorcerer",
		                         msg, NULL);
	}
	exit(1);
}

void ss_init(int argc, char **argv)
{
	const char *p;

	(void)argc;
	(void)argv;

	ss_arena = (uint8 *)calloc(SS_SEG_BYTES + 0x2000u, 1);
	if (ss_arena == NULL) {
		fprintf(stderr, "shadow: cannot allocate arena\n");
		exit(1);
	}
	ss_ega_reset();
	ss_log_on = (SDL_getenv("SS_LOG") != NULL);
	p = SDL_getenv("SS_HEADLESS");
	ss_headless = (p != NULL && p[0] != 0 && SDL_atoi(p) != 0);
	ss_read_env();
	if (ss_headless) {
		/* No window at all: useful for automated runs and screenshots. */
		SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy");
		SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "dummy");
		if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_AUDIO | SDL_INIT_VIDEO)) {
			ss_fail("SDL_Init");
			exit(1);
		}
		ss_check_data();
		ss_tick_next = SDL_GetTicks() + SS_TICK_MS;
		ss_frame_next = SDL_GetTicks() + SS_FRAME_MS;
		ss_refresh_palette();
		ss_sound_init();
		ss_push_key((int16)0x1C0D);
		return;
	}
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
		ss_fail("SDL_Init");
		exit(1);
	}
	ss_check_data();

	ss_window = SDL_CreateWindow("Shadow Sorcerer", SS_SCREEN_W * 3,
	                             SS_SCREEN_H * 3,
	                             SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY |
	                             ((SDL_getenv("SS_WINDOW_HIDDEN") != NULL) ? SDL_WINDOW_HIDDEN : 0));
	if (ss_window == NULL) {
		ss_fail("SDL_CreateWindow");
		exit(1);
	}
#ifdef __EMSCRIPTEN__
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "software");
#endif
	ss_renderer = SDL_CreateRenderer(ss_window, NULL);
	if (ss_renderer == NULL) {
		ss_fail("SDL_CreateRenderer");
		exit(1);
	}
	SDL_SetRenderVSync(ss_renderer, 1);
	SDL_SetRenderLogicalPresentation(ss_renderer, SS_SCREEN_W, SS_SCREEN_H,
	                                 SDL_LOGICAL_PRESENTATION_LETTERBOX);
	ss_texture = SDL_CreateTexture(ss_renderer, SDL_PIXELFORMAT_XRGB8888,
	                               SDL_TEXTUREACCESS_STREAMING,
	                               SS_SCREEN_W, SS_SCREEN_H);
	if (ss_texture == NULL) {
		ss_fail("SDL_CreateTexture");
		exit(1);
	}
	SDL_SetTextureScaleMode(ss_texture, SDL_SCALEMODE_NEAREST);
	SDL_SetTextureBlendMode(ss_texture, SDL_BLENDMODE_NONE);
	SDL_HideCursor();
	ss_log("video ready\n");

	ss_tick_next = SDL_GetTicks() + SS_TICK_MS;
	ss_frame_next = SDL_GetTicks() + SS_FRAME_MS;

	ss_refresh_palette();
	ss_sound_init();

	/* The release is always started by typing its command, so GAME.EXE's
	   first shadow_poll() finds the terminating Enter still queued.  Keep
	   that behaviour or the startup wait never ends. */
	ss_push_key((int16)0x1C0D);
}

void ss_shutdown(void)
{
	ss_sound_shutdown();
	if (ss_texture != NULL) SDL_DestroyTexture(ss_texture);
	if (ss_renderer != NULL) SDL_DestroyRenderer(ss_renderer);
	if (ss_window != NULL) SDL_DestroyWindow(ss_window);
	SDL_Quit();
	free(ss_arena);
	ss_arena = NULL;
}

char **ss_build_argv(int argc, char **argv, int *out_argc)
{
	static char *av[8];
	static char mode[] = "v";
	int i, n = 0;
	int have_mode = 0;

	for (i = 0; i < argc && n < 7; i++) {
		av[n++] = argv[i];
		if (i > 0) {
			char c = argv[i][0];
			if (c >= 'A' && c <= 'Z') {
				c = (char)(c - 'A' + 'a');
			}
			if (c == 'a' || c == 'e' || c == 'r' || c == 't' || c == 'v') {
				have_mode = 1;
			}
		}
	}
	if (!have_mode) {
		av[n++] = mode;
	}
	av[n] = NULL;
	*out_argc = n;
	return av;
}
