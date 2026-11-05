/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Loads the named Amiga effect WAVs from the ``sfx`` directory and mixes
  one-shots into the game's audio callback.  The original PC-speaker effect
  remains the fallback when a file is not present.
*/

#define SDL_MAIN_HANDLED 1
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ss_platform.h"
#include "ss_sfx.h"

#define SFX_RATE 44100
#define SFX_VOICES 10

struct sfx_sample {
	short *pcm;
	int frames;
	int rate;
};

struct sfx_voice {
	int sample;
	double pos;
	double step;
	int active;
};

static struct sfx_sample samples[SS_SFX_COUNT];
static struct sfx_voice voices[SFX_VOICES];
static SDL_SpinLock sfx_lock;
static Uint64 last_step_ms;
static Uint64 last_menu_ms;

static const char *const sample_names[SS_SFX_COUNT] = {
	"arrow_hit.wav",
	"menu.wav",
	"hurt.wav",
	"death.wav",
	"footstep.wav",
	"attack_swing.wav",
	"arrow_hit_plus2.wav",
	"sling_stone.wav",
	"fireball.wav",
	"spellcast.wav",
	"explosion.wav"
};

static unsigned int rd32(const unsigned char *p)
{
	return (unsigned int)p[0] | ((unsigned int)p[1] << 8) |
	       ((unsigned int)p[2] << 16) | ((unsigned int)p[3] << 24);
}

static unsigned int rd16(const unsigned char *p)
{
	return (unsigned int)p[0] | ((unsigned int)p[1] << 8);
}

static int load_wav(const char *path, struct sfx_sample *out)
{
	size_t size = 0;
	unsigned char *data = (unsigned char *)SDL_LoadFile(path, &size);
	int channels = 0, bits = 0, rate = 0;
	const unsigned char *pcm = NULL;
	size_t pcm_bytes = 0;
	unsigned int pos = 12;

	if (data == NULL || size < 44) {
		if (data) SDL_free(data);
		return 0;
	}
	if (memcmp(data, "RIFF", 4) != 0 || memcmp(data + 8, "WAVE", 4) != 0) {
		SDL_free(data);
		return 0;
	}
	while (pos + 8 <= size) {
		unsigned int len = rd32(data + pos + 4);
		if (memcmp(data + pos, "fmt ", 4) == 0 && pos + 8 + 16 <= size) {
			channels = (int)rd16(data + pos + 10);
			rate = (int)rd32(data + pos + 12);
			bits = (int)rd16(data + pos + 22);
		} else if (memcmp(data + pos, "data", 4) == 0) {
			pcm = data + pos + 8;
			pcm_bytes = len;
			if (pos + 8 + len > size)
				pcm_bytes = size - (pos + 8);
		}
		pos += 8 + len + (len & 1);
	}
	if (pcm == NULL || channels <= 0 || rate <= 0 ||
	    (bits != 8 && bits != 16)) {
		SDL_free(data);
		return 0;
	}

	{
		size_t bytes_per = (size_t)bits / 8;
		size_t frame_bytes = bytes_per * (size_t)channels;
		size_t frames;
		short *dst;
		size_t i;

		if (frame_bytes == 0) {
			SDL_free(data);
			return 0;
		}
		frames = pcm_bytes / frame_bytes;
		if (frames == 0) {
			SDL_free(data);
			return 0;
		}
		dst = (short *)malloc(frames * sizeof(short));
		if (dst == NULL) {
			SDL_free(data);
			return 0;
		}
		for (i = 0; i < frames; i++) {
			const unsigned char *s = pcm + i * frame_bytes;
			int value = 0;
			int c;
			for (c = 0; c < channels; c++) {
				if (bits == 16) {
					value += (int)(short)rd16(s + c * 2);
				} else {
					value += ((int)s[c] - 128) << 8;
				}
			}
			value /= channels;
			if (value > 32767) value = 32767;
			if (value < -32768) value = -32768;
			dst[i] = (short)value;
		}
		out->pcm = dst;
		out->frames = (int)frames;
		out->rate = rate;
	}
	SDL_free(data);
	return 1;
}

static int try_path(const char *dir, const char *name, char *out, size_t size)
{
	FILE *f;
	if (dir[0] == 0) {
		SDL_snprintf(out, size, "sfx/%s", name);
	} else {
		SDL_snprintf(out, size, "%s/sfx/%s", dir, name);
	}
	f = fopen(out, "rb");
	if (f == NULL)
		return 0;
	fclose(f);
	return 1;
}

static int load_named(int id, const char *base)
{
	char path[1024];
	static const char *const prefixes[] = { "", "..", "../..", "../../.." };
	size_t i;

	for (i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); i++) {
		if (try_path(prefixes[i], sample_names[id], path, sizeof(path)) &&
		    load_wav(path, &samples[id])) {
			return 1;
		}
	}
	if (base != NULL) {
		char dir[1024];
		SDL_snprintf(dir, sizeof(dir), "%s", base);
		for (i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); i++) {
			char combined[1200];
			SDL_snprintf(combined, sizeof(combined), "%s/%s", dir, prefixes[i]);
			if (try_path(combined, sample_names[id], path, sizeof(path)) &&
			    load_wav(path, &samples[id])) {
				return 1;
			}
		}
	}
	return 0;
}

void ss_sfx_init(void)
{
	const char *base;
	int i;

	memset(samples, 0, sizeof(samples));
	memset(voices, 0, sizeof(voices));
	base = SDL_GetBasePath();
	for (i = 0; i < SS_SFX_COUNT; i++)
		load_named(i, base);
}

void ss_sfx_shutdown(void)
{
	int i;

	for (i = 0; i < SS_SFX_COUNT; i++) {
		if (samples[i].pcm != NULL) {
			free(samples[i].pcm);
			samples[i].pcm = NULL;
		}
	}
}

int ss_sfx_play(int id)
{
	int i;
	int slot = -1;

	if (id < 0 || id >= SS_SFX_COUNT || samples[id].pcm == NULL)
		return 0;
	if (id == SS_SFX_MENU) {
		Uint64 now = SDL_GetTicks();
		if (now - last_menu_ms < 80)
			return 1;
		last_menu_ms = now;
	}
	SDL_LockSpinlock(&sfx_lock);
	for (i = 0; i < SFX_VOICES; i++) {
		if (!voices[i].active) {
			slot = i;
			break;
		}
	}
	if (slot < 0) {
		slot = 0;
	}
	voices[slot].sample = id;
	voices[slot].pos = 0.0;
	voices[slot].step = (double)samples[id].rate / (double)SFX_RATE;
	voices[slot].active = 1;
	SDL_UnlockSpinlock(&sfx_lock);
	return 1;
}

int ss_sfx_play_effect(int kind)
{
	switch (kind) {
	case 1:				/* EFFECT_KIND_MARKER */
	case 3:				/* EFFECT_KIND_CREATURE */
		return ss_sfx_play(SS_SFX_EXPLOSION);
	case 4:				/* recover throwing axe */
	case 5:				/* recover spear */
		return ss_sfx_play(SS_SFX_HURT);
	case 6:				/* clear arrows */
		return ss_sfx_play(SS_SFX_ATTACK_SWING);
	case 7:				/* clear arrows +2 */
		return ss_sfx_play(SS_SFX_ARROW_HIT_PLUS2);
	case 8:				/* clear stones */
		return ss_sfx_play(SS_SFX_SLING_STONE);
	case 10:			/* charm */
	case 11:			/* sleep */
	case 13:			/* web */
	case 14:			/* burning hands */
		return ss_sfx_play(SS_SFX_SPELLCAST);
	case 12:			/* fireball */
		return ss_sfx_play(SS_SFX_FIREBALL);
	case 20:			/* hit bozak */
	case 24:			/* large effect */
		return ss_sfx_play(SS_SFX_EXPLOSION);
	case 21:			/* default hit */
		return ss_sfx_play(SS_SFX_HURT);
	default:
		return 0;
	}
}

int ss_sfx_play_request(int request)
{
	switch (request) {
	case 1:
		return ss_sfx_play(SS_SFX_ARROW_HIT);
	case 3:
		return ss_sfx_play(SS_SFX_HURT);
	case 4:
		return ss_sfx_play(SS_SFX_DEATH);
	case 6:
		return ss_sfx_play(SS_SFX_ATTACK_SWING);
	case 7:
		return ss_sfx_play(SS_SFX_ARROW_HIT_PLUS2);
	case 8:
		return ss_sfx_play(SS_SFX_SLING_STONE);
	case 10:
		return ss_sfx_play(SS_SFX_FIREBALL);
	case 13:
		return ss_sfx_play(SS_SFX_MENU);
	case 14:
		return ss_sfx_play(SS_SFX_EXPLOSION);
	default:
		return 0;
	}
}

/* The values in g_tick_by_kind are the PC-speaker sequence ids the effect
   kinds used in the DOS release.  They are mostly spell/weapon impacts, so
   they map to the effect samples rather than to the menu one. */
int ss_sfx_play_tick(int tick)
{
	switch (tick) {
	case 1:
		return ss_sfx_play(SS_SFX_ARROW_HIT);
	case 3:
		return ss_sfx_play(SS_SFX_HURT);
	case 4:
		return ss_sfx_play(SS_SFX_HURT);
	case 6:
		return ss_sfx_play(SS_SFX_ATTACK_SWING);
	case 7:
		return ss_sfx_play(SS_SFX_ARROW_HIT_PLUS2);
	case 8:
		return ss_sfx_play(SS_SFX_SLING_STONE);
	case 10:
		return ss_sfx_play(SS_SFX_FIREBALL);
	case 13:
		return ss_sfx_play(SS_SFX_SPELLCAST);
	case 14:
		return ss_sfx_play(SS_SFX_EXPLOSION);
	default:
		return 0;
	}
}

int ss_sfx_available(void)
{
	int i;

	for (i = 0; i < SS_SFX_COUNT; i++) {
		if (samples[i].pcm != NULL)
			return 1;
	}
	return 0;
}

void ss_sfx_step(void)
{
	Uint64 now = SDL_GetTicks();
	if (now - last_step_ms < 120)
		return;
	last_step_ms = now;
	(void)ss_sfx_play(SS_SFX_STEP);
}

void ss_sfx_mix(short *samples_out, int n)
{
	int i, v;

	SDL_LockSpinlock(&sfx_lock);
	for (v = 0; v < SFX_VOICES; v++) {
		struct sfx_voice *voice = &voices[v];
		const struct sfx_sample *s;
		double gain;

		if (!voice->active)
			continue;
		s = &samples[voice->sample];
		gain = 9000.0 * (double)ss_sfx_volume / 100.0;
		for (i = 0; i < n; i++) {
			int idx = (int)voice->pos;
			double frac;
			double value;
			if (idx < 0 || idx >= s->frames) {
				voice->active = 0;
				break;
			}
			if (idx + 1 < s->frames) {
				frac = voice->pos - idx;
				value = s->pcm[idx] * (1.0 - frac) +
				        s->pcm[idx + 1] * frac;
			} else {
				value = s->pcm[idx];
			}
			{
				int mixed = samples_out[i] + (int)(value * gain / 32768.0);
				if (mixed > 32767) mixed = 32767;
				if (mixed < -32768) mixed = -32768;
				samples_out[i] = (short)mixed;
			}
			voice->pos += voice->step;
		}
	}
	SDL_UnlockSpinlock(&sfx_lock);
}
