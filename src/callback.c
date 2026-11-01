/*
  Shadow Sorcerer
  License: Public Domain (CC0)
  Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)
*/

#include "names.h"
#include <stdio.h>
#include <process.h>
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
}
