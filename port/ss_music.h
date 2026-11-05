/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Music playback from the `music` directory (Ogg Vorbis).
*/

#ifndef SS_MUSIC_H
#define SS_MUSIC_H

enum {
	SS_MUSIC_NONE = 0,
	SS_MUSIC_TITLE,
	SS_MUSIC_OVERWORLD,
	SS_MUSIC_ENCOUNTER,
	SS_MUSIC_DUNGEON
};

void ss_music_init(void);
void ss_music_shutdown(void);

/* Start a track.  TITLE/OVERWORLD/ENCOUNTER are one-shot stings; DUNGEON
   loops until another track replaces it or ss_music_stop() is called. */
void ss_music_play(int track);
void ss_music_stop(void);
int  ss_music_is_playing(void);
int  ss_music_current(void);

/* Map the DOS SOUND_SEQUENCE_* ids to the Ogg tracks. */
void ss_music_play_sequence(int sequence);

/* Mix the active track into n mono samples. */
void ss_music_mix(short *samples, int n);

#endif
