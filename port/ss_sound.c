/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  PC-speaker sound.  The game's sound engine (src/tick.c, src/pit.c) drives a
  real 8253/8254: channel 0 paces the sequence handler, channel 2 makes the
  tone, and port 0x61 gates the speaker.  Instead of approximating the
  sequences, this file emulates those ports and clocks the same handlers from
  the audio callback, so the existing PIT code produces the original bleeps.
  When the named sample bank (port/ss_sfx.c) is present the tone output is
  muted, and the game's sound requests are routed to those samples instead.
*/

#define SDL_MAIN_HANDLED 1
#include <SDL3/SDL.h>

#include "names.h"
#include "types.h"
#include "ss_platform.h"
#include "ss_music.h"
#include "ss_sfx.h"

extern void (*volatile g_disp_handler)(void);   /* 0xCE82 */
extern volatile uint16 g_disp_last;             /* 0xCE7E */
extern volatile uint16 g_tick_enabled;          /* 0x98CA */
extern void tick_handler_type0(void);
extern void tick_handler_type1(void);
extern void tick_handler_type2(void);

#define PIT_HZ 1193182.0

static SDL_AudioStream *ss_audio;

/* 8253 state, only what the game programs. */
static uint32 ch0_count = 0xFFFF;
static uint32 ch0_reload = 0xFFFF;
static uint32 ch2_count = 0xFFFF;
static uint32 ch2_reload = 0xFFFF;
static double ch0_frac, ch2_frac;
static uint8 ch2_out;
static uint8 port61;

/* Control-word byte counters: 0x43 selects a channel and a LSB/MSB order. */
static int pit_write_ch = -1;
static int pit_write_lo;
static uint8 pit_write_value;

int16 ss_inp(uint16 port)
{
	switch ((uint16)port) {
	case 0x61:
		return (int16)port61;
	case 0x60:
		return 0;
	default:
		return 0xFF;
	}
}

void ss_outp(uint16 port, uint16 value)
{
	ss_internal_outport(port, value);
}

void ss_outpw(uint16 port, uint16 value)
{
	ss_internal_outport(port, (uint16)(value & 0xFF));
	ss_internal_outport((uint16)(port + 1), (uint16)(value >> 8));
}

void ss_internal_outport(uint16 port, uint16 value)
{
	uint8 v = (uint8)value;

	switch (port) {
	case 0x43:
		pit_write_ch = (v >> 6) & 3;
		pit_write_lo = 1;
		pit_write_value = 0;
		break;
	case 0x40:
	case 0x42:
		if (pit_write_lo) {
			pit_write_value = v;
			pit_write_lo = 0;
		} else {
			uint16 d = (uint16)(pit_write_value | ((uint16)v << 8));
			if (d == 0) {
				d = 0xFFFF;
			}
			if (port == 0x40) {
				ch0_reload = d;
				ch0_count = d;
			} else {
				ch2_reload = d;
				ch2_count = d;
			}
			pit_write_lo = 1;
		}
		break;
	case 0x61:
		port61 = v;
		break;
	default:
		break;
	}
}

static int16 ss_next_sample(void)
{
	int guard = 0;

	/* Channel 0 paces the game's sequence handlers. */
	ch0_frac += PIT_HZ / 44100.0;
	while (ch0_frac >= 1.0) {
		ch0_frac -= 1.0;
		if (ch0_count > 0) {
			ch0_count--;
		}
		if (ch0_count == 0) {
			ch0_count = ch0_reload;
			if (g_disp_last != 0 && g_disp_handler != 0) {
				g_disp_handler();
			}
		}
		if (++guard > 128) {
			break;
		}
	}

	/* Channel 2 is the tone generator (mode 3: square wave). */
	ch2_frac += PIT_HZ / 44100.0;
	while (ch2_frac >= 1.0) {
		ch2_frac -= 1.0;
		if (ch2_count > 0) {
			ch2_count--;
		}
		if (ch2_count == 0) {
			ch2_count = ch2_reload;
			ch2_out ^= 1;
		}
		if (++guard > 256) {
			break;
		}
	}

	if ((port61 & 0x03) == 0x03) {
		int16 v;
		int vol;
		if (ss_sfx_available())
			return 0;			/* named samples replace the beeper */
		v = ch2_out ? 6000 : -6000;
		vol = ss_sound_is_music() ? ss_music_volume : ss_sfx_volume;
		return (int16)((v * vol) / 100);
	}
	if ((port61 & 0x03) == 0x02) {
		int vol;
		if (ss_sfx_available())
			return 0;
		vol = ss_sound_is_music() ? ss_music_volume : ss_sfx_volume;
		return (int16)((6000 * vol) / 100);          /* speaker data high, gate off */
	}
	return 0;
}

static void ss_audio_callback(void *userdata, SDL_AudioStream *stream,
                              int additional_amount, int total_amount)
{
	static int16 buf[2048];

	(void)userdata;
	(void)total_amount;
	while (additional_amount > 0) {
		int n = additional_amount / 2;
		int i;

		if (n > (int)(sizeof(buf) / sizeof(buf[0]))) {
			n = (int)(sizeof(buf) / sizeof(buf[0]));
		}
		if (n <= 0) {
			break;
		}
		for (i = 0; i < n; i++) {
			buf[i] = ss_next_sample();
		}
		ss_sfx_mix(buf, n);
		ss_music_mix(buf, n);
		SDL_PutAudioStreamData(stream, buf, n * 2);
		additional_amount -= n * 2;
	}
}

void ss_sound_init(void)
{
	SDL_AudioSpec spec;

	ss_music_init();
	ss_sfx_init();

	SDL_zero(spec);
	spec.format = SDL_AUDIO_S16;
	spec.channels = 1;
	spec.freq = 44100;

	g_tick_enabled = 1;
	ss_audio = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK,
	                                     &spec, ss_audio_callback, NULL);
	if (ss_audio != NULL) {
		SDL_ResumeAudioStreamDevice(ss_audio);
	}
}

void ss_sound_shutdown(void)
{
	if (ss_audio != NULL) {
		SDL_DestroyAudioStream(ss_audio);
		ss_audio = NULL;
	}
	ss_sfx_shutdown();
	ss_music_shutdown();
}
