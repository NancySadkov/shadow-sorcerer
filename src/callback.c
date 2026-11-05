/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include <stdio.h>
#include <stdlib.h>
#ifndef __EMSCRIPTEN__
#include <process.h>
#else
#include <emscripten.h>
#endif
#include "ids.h"

extern volatile uint16 g_shadow_counters[4];	/* 0xA8A4..0xA8AA */
#define tick_count g_shadow_counters[3]
extern FILE *g_text_stream;             /* 0xA2CE */

extern char g_play_mode;             /* 0x9E0C */

extern void tick_uninstall(void);  /* 0xD108 */
extern void driver_resume_and_call(int16 entry, int16 arg);  /* 0xD2C7 */
extern void driver_resume(void);                 /* 0xD309 */
extern void driver_stop(void);                   /* 0x9A9D */

/* 0x64A3 */
void terminate_callback(void)
{
#ifdef SS_SDL
	if (g_text_stream != NULL) {
		fclose(g_text_stream);
		g_text_stream = NULL;
	}
#ifdef __EMSCRIPTEN__
	/* A browser has no process to exit to.  Flush pending saves and reload
	   the page so Quit returns to the title screen instead of aborting the
	   Emscripten runtime. */
	EM_ASM({
		function restart() { window.location.reload(); }
		try {
			FS.syncfs(false, restart);
		} catch (e) {
			restart();
		}
	});
	for (;;)
		emscripten_sleep(100);
#else
	exit(0);
#endif
#else
	if (g_play_mode < 2) {
		tick_uninstall();
	} else {
		driver_resume_and_call(DRIVER_ENTRY_QUEUE, DRIVER_SEQ_SILENCE);

		tick_count = 0;
		while (tick_count < 10)
			;

		driver_resume();
		driver_stop();
	}

	fclose(g_text_stream);
	_exit(1);
#endif
}
