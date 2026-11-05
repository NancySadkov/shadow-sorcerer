# Dungeons & Dragons Shadow Sorcerer — source reconstruction

Reverse engineered **Shadow Sorcerer**, the 1991 SSI / U.S. Gold
Dragonlance RPG, rebuilt from the shipped MS-DOS release.

Author: **[Nancy Sadkov](https://aermia.com/)**

## The game

Shadow Sorcerer is a party-based Dragonlance adventure based on *Dragons of
Hope* by Tracy Hickman. Four heroes travel an overland map, explore sites,
talk, trade and fight monsters, with a spellbook-driven magic system.

## License

CC0 1.0 Universal. See [LICENSE.md](LICENSE.md).

## Architecture

The PC release is three programs:

- **`SHADOW.COM`** — a resident EGA/video TSR. It hooks `INT F8h` and supplies
  the drawing, palette, text and memory services the game calls. Rebuilt from
  MASM source; byte-identical to the release.
- **`GAME.EXE`** — the game itself. Microsoft C 5.1, small model, with the
  original DGROUP layout preserved.
- **`INTRO.EXE`** — the title sequence and the embedded John Miles UltraSound
  driver, fixed at image `0x0AC1` and reproduced byte-for-byte.


## SDL3 port

`port/` contains a native SDL3 front end for `GAME.EXE`.  The game sources in
`src/` are compiled mostly unchanged, with small `#ifdef SS_SDL` branches where
the original code used 16-bit DOS facilities (`int86`, far pointers, segment
arithmetic, near pointers stored in 16-bit fields), plus a few decompilation
fixes that the release disassembly proves apply to both builds.

What the port supplies:

- a flat 1 MiB arena addressed with the original paragraphs, so the asset
  loader, sprite banks and save-image layout keep working;
- eight 320x200 display pages selected by the original page segments
  (`0xA000 + page * 0x200`), the deferred mouse-cursor save/restore, and the
  planar sprite/block/text/region-clear services that `SHADOW.COM` provided;
- the driver's own 8x8 font and edge-mask table, extracted from the release
  `SHADOW.COM`;
- the PC-speaker sound engine (`src/pit.c`, `src/tick.c`) clocked from an
  SDL audio callback, so the original sequences play unchanged;
- the Ogg music tracks (`music/title.ogg`, `overworld.ogg`, `encounter.ogg`
  and `dungeon.ogg`) mixed into the same callback; title is a startup sting,
  overworld plays once on map return, encounter plays on chance encounters,
  and dungeon loops during site exploration;
- keyboard, mouse, palette and window handling through SDL3.

The Adlib/MT-32 driver path (`SOUND.DRV`, `ADLIB.ADV`, `MT32MPU.ADV`) is
16-bit DOS code and is not emulated.  The port plays the Ogg music and the
named Amiga effects; while the named effect bank is present the PC-speaker
emulation stays muted, and only a missing `sfx` directory brings the
original bleeps back as a fallback.  `INTRO.EXE` is a separate program and is
not part of the port.

### Build

Requires CMake 3.16+ and an SDL3 development package.

```sh
cmake -S . -B build -DSDL3_DIR=/path/to/SDL3/lib/cmake/SDL3
cmake --build build
```

The executable is `build/shadow` (`build/shadow.exe` on Windows); `SDL3.dll`
is copied next to it on Windows.  A plain GCC command line also works:

```sh
gcc -DSS_SDL -Iport -Isrc -IC:/soft/SDL3/include \
    src/*.c port/*.c -LC:/soft/SDL3/lib -lSDL3 -o shadow
```

### Browser build

The same sources build for the browser with Emscripten and its SDL3 port
(`-sUSE_SDL=3`).  `web/pre.js` mounts an IndexedDB filesystem at `/game`,
copies the read-only preloaded assets into it on first launch, and installs a
case-insensitive `FS.open` for the DOS-style mixed-case file names.  Saves
written by the game stay in the browser's IndexedDB.

Only the graphics/text/map banks the port loads are packed into the browser
build (`tools/stage_web_assets.py`); the DOS executables, `SHADOW.COM`, the
Adlib/MT-32 driver files and the `INTRO.EXE` title art are not needed.

```sh
python tools/stage_web_assets.py
emcc -DSS_SDL -Iport -Isrc -O2 -sUSE_SDL=3 -sWASM=0 -sASYNCIFY=1 \
    -sALLOW_MEMORY_GROWTH=1 -sINITIAL_MEMORY=268435456 \
    -sSTACK_SIZE=1048576 -sFORCE_FILESYSTEM=1 \
    -sEXPORTED_RUNTIME_METHODS=FS -lidbfs.js --pre-js web/pre.js \
    --shell-file web/shell.html \
    --preload-file build/web_assets/game@/assets/game \
    --preload-file music@/music --preload-file sfx@/sfx \
    @web/sources.rsp -o web/shadow.html
```

`web/sources.rsp` lists the sources for the build; regenerate it when files
are added.  The result is `web/shadow.html`, `web/shadow.js` and
`web/shadow.data`, served from any static file server:

```sh
python -m http.server 8000 --directory web
```

and opened at `http://localhost:8000/shadow.html`.  `-sASYNCIFY` is required
because the game runs a blocking loop and uses `emscripten_sleep` for timing.
Current Emscripten releases do not emit classic asm.js; `-sWASM=0` produces
JavaScript through the wasm2js backend instead, so no `.wasm` file is needed.
The browser also needs one click or key press before audio starts.
`web/shell.html` is a minimal page with just the canvas (no Emscripten
footer or controls).  Quit and Exit flush pending saves and reload the page,
which returns to the title screen; a native build simply closes.

### Run

The game data (`GAME.EXE` release files) is not distributed with the source.
The port looks for it in this order:

1. `--game-dir=<path>` or `SS_GAME_DIR`;
2. `<directory of shadow.exe>/game`;
3. `<current directory>/game`;
4. the current directory itself.

The directory must contain `VCLIPS.DAT`, `VMON*.DAT`, `VMPL*.DAT`,
`VOBJPIC.DAT`, `VMAPPIC.DAT`, `VMISSLE.DAT`, `VCIN*.DAT`, `TEXT.DAT`, `MAPED`
and the sound files.  If none of the candidates has them, the port shows an
error box naming the paths it tried instead of starting on a black screen.
Command line arguments are the release's: `v` selects the VGA asset set (the
default), `a`/`r` are accepted but fall back to PC-speaker sound.  The port
starts the game with a queued Enter because the release expects to be launched
by typing its name.

The named Amiga effect WAVs are looked for in an `sfx` directory next to the
executable, in the working directory, or in a parent of either.  If the
directory is absent the port keeps the original PC-speaker effects.  See
`sfx/README.md` for the slot-to-action mapping.

The Ogg tracks (`title.ogg`, `overworld.ogg`, `encounter.ogg`, `dungeon.ogg`)
are looked for in a `music` directory in the same locations.  `stb_vorbis` is
built into the port, so no external codec DLLs are needed.

Window: 960x600, nearest-neighbour scaled and resizable.  `Esc`/window close
quits.
