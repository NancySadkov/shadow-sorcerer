/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Sample-based sound effects.  The Amiga effect bank is exported to the
  ``sfx`` directory with gameplay names; this module loads those WAVs and
  mixes one-shots into the SDL audio callback.  If a file is missing the
  caller falls back to the original PC-speaker effect.
*/

#ifndef SS_SFX_H
#define SS_SFX_H

enum {
	SS_SFX_ARROW_HIT = 0,
	SS_SFX_MENU,
	SS_SFX_HURT,
	SS_SFX_DEATH,
	SS_SFX_STEP,
	SS_SFX_ATTACK_SWING,
	SS_SFX_ARROW_HIT_PLUS2,
	SS_SFX_SLING_STONE,
	SS_SFX_FIREBALL,
	SS_SFX_SPELLCAST,
	SS_SFX_EXPLOSION,
	SS_SFX_COUNT
};

void ss_sfx_init(void);
void ss_sfx_shutdown(void);

/* Returns 1 when the named sample is loaded and started. */
int ss_sfx_play(int id);

/* Map an EFFECT_KIND_* value to a sample.  Returns 0 when the effect has no
   dedicated sample. */
int ss_sfx_play_effect(int kind);

/* Map a DOS SOUND_REQUEST_* value to a sample. */
int ss_sfx_play_request(int request);

/* Map the DOS tick/sequence request that g_tick_by_kind carries for an
   effect kind.  Effect kinds without a dedicated role use this so every
   original beep still gets a sample. */
int ss_sfx_play_tick(int tick);

/* 1 when the named sample bank is loaded; the PC-speaker emulation stays
   muted while it is. */
int ss_sfx_available(void);

/* Party/hero movement; has a short cooldown so held movement keys do not
   machine-gun the sample. */
void ss_sfx_step(void);

/* Mix active one-shots into n mono samples. */
void ss_sfx_mix(short *samples, int n);

#endif
