/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Ogg Vorbis music playback.  TITLE, OVERWORLD and ENCOUNTER are one-shot
  stings; DUNGEON loops while the party is inside a site.  The files live in
  the `music` directory next to the executable or in a parent directory.
*/

#define SDL_MAIN_HANDLED 1
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_VORBIS_HEADER_ONLY
#include "third_party/stb_vorbis.c"

#include "ss_platform.h"
#include "ss_music.h"

#define MUSIC_RATE 44100

struct music_track {
	short *pcm;
	int frames;
	int rate;
};

static struct music_track tracks[SS_MUSIC_DUNGEON + 1];

static const char *const track_names[SS_MUSIC_DUNGEON + 1] = {
	NULL,
	"title.ogg",
	"overworld.ogg",
	"encounter.ogg",
	"dungeon.ogg"
};

static volatile int music_wanted;
static int music_applied;
static int music_current;
static int music_playing;
static int music_one_shot;
static int dungeon_pending;
static double music_pos;
static double music_step;

static int file_exists(const char *path)
{
	FILE *f = fopen(path, "rb");
	if (f == NULL)
		return 0;
	fclose(f);
	return 1;
}

static int find_music(const char *name, const char *base, char *out, size_t size)
{
	static const char *const prefixes[] = { "", "..", "../..", "../../.." };
	size_t i;

	for (i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); i++) {
		if (prefixes[i][0] == 0)
			SDL_snprintf(out, size, "music/%s", name);
		else
			SDL_snprintf(out, size, "%s/music/%s", prefixes[i], name);
		if (file_exists(out))
			return 1;
	}
	if (base != NULL) {
		for (i = 0; i < sizeof(prefixes) / sizeof(prefixes[0]); i++) {
			char dir[1200];
			if (prefixes[i][0] == 0)
				SDL_snprintf(dir, sizeof(dir), "%smusic/%s", base, name);
			else
				SDL_snprintf(dir, sizeof(dir), "%s%s/music/%s", base,
				             prefixes[i], name);
			if (file_exists(dir)) {
				SDL_snprintf(out, size, "%s", dir);
				return 1;
			}
		}
	}
	return 0;
}

static int load_track(const char *path, struct music_track *track)
{
	short *decoded = NULL;
	int channels = 0;
	int rate = 0;
	int frames;
	short *mono;
	int i;

	frames = stb_vorbis_decode_filename(path, &channels, &rate, &decoded);
	if (frames <= 0 || decoded == NULL || channels <= 0 || rate <= 0) {
		if (decoded) free(decoded);
		return 0;
	}
	mono = (short *)malloc((size_t)frames * sizeof(short));
	if (mono == NULL) {
		free(decoded);
		return 0;
	}
	if (channels == 1) {
		memcpy(mono, decoded, (size_t)frames * sizeof(short));
	} else {
		for (i = 0; i < frames; i++) {
			int sum = 0;
			int c;
			for (c = 0; c < channels; c++)
				sum += decoded[i * channels + c];
			mono[i] = (short)(sum / channels);
		}
	}
	free(decoded);
	track->pcm = mono;
	track->frames = frames;
	track->rate = rate;
	return 1;
}

void ss_music_init(void)
{
	const char *base = SDL_GetBasePath();
	int i;

	memset(tracks, 0, sizeof(tracks));
	for (i = SS_MUSIC_TITLE; i <= SS_MUSIC_DUNGEON; i++) {
		char path[1400];
		if (find_music(track_names[i], base, path, sizeof(path)))
			(void)load_track(path, &tracks[i]);
	}
	music_wanted = SS_MUSIC_NONE;
	music_applied = SS_MUSIC_NONE;
	music_current = SS_MUSIC_NONE;
	music_playing = 0;
	music_one_shot = 0;
	dungeon_pending = 0;
}

void ss_music_shutdown(void)
{
	int i;

	for (i = 0; i <= SS_MUSIC_DUNGEON; i++) {
		if (tracks[i].pcm != NULL) {
			free(tracks[i].pcm);
			tracks[i].pcm = NULL;
		}
	}
}

static void start_track(int track)
{
	const struct music_track *t;

	if (track <= SS_MUSIC_NONE || track > SS_MUSIC_DUNGEON) {
		music_playing = 0;
		music_current = SS_MUSIC_NONE;
		return;
	}
	t = &tracks[track];
	if (t->pcm == NULL) {
		music_playing = 0;
		music_current = SS_MUSIC_NONE;
		return;
	}
	music_current = track;
	music_pos = 0.0;
	music_step = (double)t->rate / (double)MUSIC_RATE;
	music_one_shot = (track != SS_MUSIC_DUNGEON);
	music_playing = 1;
}

static void apply_music(int track)
{
	if (track == SS_MUSIC_DUNGEON) {
		/* Do not interrupt an encounter sting; resume when it ends. */
		if (music_playing && music_current == SS_MUSIC_ENCOUNTER) {
			dungeon_pending = 1;
			return;
		}
		dungeon_pending = 0;
		start_track(SS_MUSIC_DUNGEON);
		return;
	}
	if (track == SS_MUSIC_NONE) {
		dungeon_pending = 0;
		music_playing = 0;
		music_current = SS_MUSIC_NONE;
		return;
	}
	if (track == SS_MUSIC_TITLE || track == SS_MUSIC_OVERWORLD)
		dungeon_pending = 0;
	else if (track == SS_MUSIC_ENCOUNTER && music_current == SS_MUSIC_DUNGEON)
		dungeon_pending = 1;
	start_track(track);
}

void ss_music_play(int track)
{
	music_wanted = track;
}

void ss_music_stop(void)
{
	music_wanted = SS_MUSIC_NONE;
}

void ss_music_play_sequence(int sequence)
{
	switch (sequence) {
	case 6:				/* SOUND_SEQUENCE_OVERWORLD */
		ss_music_play(SS_MUSIC_OVERWORLD);
		break;
	case 2:				/* EIGHT_MONSTERS */
	case 3:				/* SCENE_MONSTER */
	case 4:				/* RANDOM_ENCOUNTER */
	case 5:				/* DRAGON_ATTACK */
		ss_music_play(SS_MUSIC_ENCOUNTER);
		break;
	default:
		break;
	}
}

int ss_music_is_playing(void)
{
	return music_playing;
}

int ss_music_current(void)
{
	return music_playing ? music_current : SS_MUSIC_NONE;
}

void ss_music_mix(short *samples, int n)
{
	int i;
	const struct music_track *t;

	if (music_wanted != music_applied) {
		music_applied = music_wanted;
		apply_music(music_applied);
	}
	if (!music_playing)
		return;
	t = &tracks[music_current];
	if (t->pcm == NULL)
		return;

	for (i = 0; i < n; i++) {
		int idx = (int)music_pos;
		double frac;
		double value;
		int mixed;

		if (idx >= t->frames) {
			if (!music_one_shot) {
				music_pos -= t->frames;
				idx = (int)music_pos;
			} else {
				music_playing = 0;
				music_current = SS_MUSIC_NONE;
				if (dungeon_pending) {
					dungeon_pending = 0;
					start_track(SS_MUSIC_DUNGEON);
					music_wanted = SS_MUSIC_DUNGEON;
					music_applied = SS_MUSIC_DUNGEON;
					t = &tracks[music_current];
					if (!music_playing)
						break;
				} else {
					music_wanted = SS_MUSIC_NONE;
					music_applied = SS_MUSIC_NONE;
					break;
				}
			}
		}
		if (idx + 1 < t->frames) {
			frac = music_pos - idx;
			value = t->pcm[idx] * (1.0 - frac) + t->pcm[idx + 1] * frac;
		} else {
			value = t->pcm[idx];
		}
		mixed = samples[i] + (int)(value * 0.30 *
		                          (double)ss_music_volume / 100.0);
		if (mixed > 32767) mixed = 32767;
		if (mixed < -32768) mixed = -32768;
		samples[i] = (short)mixed;
		music_pos += music_step;
	}
}
