# SHADOW.COM reconstruction

`game/SHADOW.COM` is the resident EGA graphics driver the game is written
against.  It is a flat 8,044-byte COM loaded at `CS:0100`, with an INT `F8h`
dispatcher: the game puts a service number in `AX`, the dispatcher indexes the
35-word selector table at `CS:0D46`, and calls the handler as a near function.

This directory reconstructs the driver in two layers:

* `shadow.asm` - the complete original driver in MASM 5.1 syntax.
* `services.c` / `cstubs.asm` - the incremental C port of the service bodies.

## Gate 1: the assembly is the shipped image

```
python tools/build_shadow.py
SHADOW BUILD: PASS  (8044 bytes, byte-identical, MASM 5.1/LINK 3.64)
```

The build assembles with the vendored 1988 MASM 5.10, links with Overlay Linker
3.64, drops the 100h `ORG` gap from the EXE load module, and compares every byte
with `game/SHADOW.COM`.  This is the clean starting point for migration: if a
hybrid change is wrong, the full-assembly build still gives the exact release
behaviour.

## Gate 2: hybrid COM

```
python tools/build_cshadow.py --deploy
python tools/playtest.py --run h_origsh boot map travel
```

`build_cshadow.py` regenerates a shell from `shadow.asm`, rewriting only the
35-word selector table: a migrated selector points at its C trampoline, every
other entry still points at the original release handler (`Lxxxx`).  The shell,
the C trampolines and the MS C 5.1 OMF object are merged by LINK into one
single-segment COM with no relocations.  The resident-size immediate in the
installer is changed to the end of the linked segment so the appended C code
stays resident.

This means migration is strictly incremental.  A selector that has not been
touched still runs the bytes from the shipped driver, not a second
reconstruction of them.

## Register ABI

The dispatcher calls a table entry with the caller's registers live, and a
handler returns its result in `AX` (some also return `BX`, `CX`, `DX`).  A C
trampoline in `cstubs.asm`:

1. pushes `AX BX CX DX SI DI BP DS ES` on the caller's stack;
2. remembers the caller's `SS:SP`, switches to a private stack in the driver's
   own segment and sets `DS = CS`;
3. passes the saved frame to the C thunk as a far pointer;
4. restores `SS:SP`, pops the frame and returns.

C therefore runs with `SS == DS == CS`.  Near pointers to C locals and to the
resident driver variables are valid, while the caller's registers are only
read/written through the far frame.  `cshadow.h` maps every resident variable
to its original `CS:` offset.

## Resident layout

The original data, entry, installer, INT `09h`/`1Ch` handlers, dispatcher,
selector table and EGA helpers remain at their original offsets.  The C code
and its private stack are appended after the original resident image, and the
installer's shrink request is extended to cover them.

## Migration status

`tools/cshadow_selectors.py` is the single switch: every selector listed there
is reached through a C trampoline, every other table entry still points at the
release handler.  `services.c` holds the C bodies, `cstubs.asm` their
trampolines.

All 35 service bodies are C.  `LEAN = True` makes the generated shell drop the
complete original service area, so the resident assembly is only the entry,
installer, INT `09h`/`1Ch` handlers, INT `F8h` dispatcher, selector table and
EGA helpers.  Everything the table points at is a C trampoline.

Two details matter for exact behaviour:

* the retrace helper reads the CRTC base with a `CS:` override, as the release
  helper does; using `DS:` let a BIOS INT 10h call change the segment and made
  the intro stall after the SSI logo,
* the input primitives (`0A`/`0C`/`1C`) use the no-stack-switch trampoline, so
  the tight game loop keeps the release timing that `combat-baaz` depends on.

`h_origsh` matches the shipped driver on boot, the world map and the scripted
endgame: state, actors and party all agree.  Remaining screenshot differences
(0.2-0.5% of pixels) are animation phase only.  Capturing the map repeatedly
gives frames that are exactly 0-different, interleaved with frames where a
walking sprite or the blinking cursor is at a different phase; there is no
systematic drawing difference.

The last such defect found this way was real: `render_deferred()` cleared
`0386` after drawing.  The release leaves it set so every page flip redraws the
deferred cursor/object into the new back page.  Clearing it made the mouse
cursor and some sprites disappear after the first flip.

The unused selector `14` (tile-window renderer) is represented by the common
16x16 tile path; GAME.EXE and INTRO.EXE never call it (see `doc/INTF8.csv`).
