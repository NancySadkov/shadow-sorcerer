/*
  Shadow Sorcerer - SDL3 port
  License: Public Domain (CC0)

  Entry point.  The game's own main() is compiled as game_main(); the port
  brings up SDL first and supplies a VGA mode letter when none was given.

  The program is linked as a GUI-subsystem executable so no console window
  appears (and steals focus) next to the game.
*/

#define SDL_MAIN_HANDLED 1
#include <SDL3/SDL.h>
#include <stdlib.h>

#include "ss_platform.h"

extern int game_main(int argc, char **argv);

static void ss_setenv(const char *name, const char *value)
{
#ifdef _WIN32
	_putenv_s(name, value);
#else
	setenv(name, value, 1);
#endif
}

static int shadow_main(int argc, char **argv)
{
	int game_argc = 0;
	char **game_argv;
	int i, n = 0;

	/* Port options are consumed here; everything else is the game's. */
	for (i = 1; i < argc; i++) {
		if (SDL_strcmp(argv[i], "--headless") == 0) {
			ss_setenv("SS_HEADLESS", "1");
		} else if (SDL_strncmp(argv[i], "--game-dir=", 11) == 0) {
			ss_setenv("SS_GAME_DIR", argv[i] + 11);
		} else {
			argv[1 + n++] = argv[i];
		}
	}
	argc = 1 + n;
	argv[argc] = NULL;

	ss_init(argc, argv);
	game_argv = ss_build_argv(argc, argv, &game_argc);
	(void)game_main(game_argc, game_argv);
	ss_shutdown();
	return 0;
}

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmdline, int show)
{
	(void)hInst;
	(void)hPrev;
	(void)cmdline;
	(void)show;
	return shadow_main(__argc, __argv);
}
#else
int main(int argc, char **argv)
{
	return shadow_main(argc, argv);
}
#endif
