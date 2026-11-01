# Release versions and executable layout

This note compares the PC release in `game/` with the Amiga and Atari ST
releases under `doc/disk/`, and explains why the reconstructed `REBUILD.EXE`
is smaller than the original `GAME.EXE`.

The ST and Amiga main binaries are packed.  They have now been decoded and
imported into Ghidra; the decoded images and the Ghidra listings are stored in
`doc/versions/` (see `doc/versions/README.md`).  The unpacking routines are
kept separately in `doc/versions/unpack/` so they can be shared without the
game assets.

## Summary

| Release | Intro | Loader / EXEC | Main game | Video TSR | Split? |
|---|---|---|---|---|---|
| PC (MS-DOS) | `INTRO.EXE` (19,499 B) | `SHADOW.COM` (8,044 B) | `GAME.EXE` (105,429 B) | `SHADOW.COM` | yes: intro + game |
| Amiga | inside `shadow` | `shadow` (92,340 B packed -> 181,156 B) | `shadow` | none | no: one executable |
| Atari ST | `INTRO.SOR` (2,217 B) | `SHADSORC.PRG` (85,110 B packed -> 298,346 B) | `DL3.SOR` (53,994 B packed -> 119,066 B) | none | yes: intro + EXEC + game |

Only the PC release has `SHADOW.COM`.  It is the PC video/TSR layer that
patches the screen and supplies INT F8h; the Amiga and ST versions drive their
native graphics hardware and do not need it.

The PC and ST releases are split into separate intro and game executables.
The Amiga release is a single `shadow` executable that contains the EXEC-style
loader, the intro/title presentation and the game itself.  `e2` is not an
intro: it is the TMB! decruncher for the Amiga data modules (`s1.dat`,
`s2.dat`).

## PC release

`game/` contains:

- `GAME.EXE` - 105,429 B, MZ, Microsoft C 5.1, small model.
- `INTRO.EXE` - 19,499 B, the title/intro program.
- `SHADOW.COM` - 8,044 B, the video/TSR driver. `GAME.EXE` calls INT F8h 40
  times and `INTRO.EXE` 20 times.
- The `.DAT` asset set (`VMON*`, `VMPL*`, `VMAPPIC.DAT`, `VMISSLE.DAT`,
  `VOBJPIC.DAT`, `VCIN*`, `VCLIPS.DAT`, `TEXT.DAT`, `SSI.DAT`, `MAPED`,
  `SOUND.DRV`, `ADLIB.ADV`, `MT32MPU.ADV`, `SAVEGAME._A_`).

`GAME.EXE` contains the literal strings `maped`, `maped.st`, and `rb`/`wb`.
The export routine at `0x5C75` (295 bytes) is still present but has no caller.
It writes the byte-swapped `maped.st` format.  Nothing in the shipped PC game
reads that file back and no map-editing UI is present.

## Amiga release

The zip contains the extracted contents of the ADF
`Shadow_Sorcerer_v1.0_1991_SSI_cr_CSL_h_MWB.adf` (volume `Shadow2`, 1,760
blocks, 880 KiB) plus the `s/startup-sequence`:

```
e2
shadow
```

Executables:

- `shadow` - 92,340 B Amiga Hunk executable.  It is packed: hunk 0 is a
  0x50-byte stub, hunk 1 is a 181,156-byte BSS sled, hunk 2 is a 496-byte
  relocator/decompressor, hunk 3 is 91,664 B of packed data, and hunk 4 is
  18,688 B of BSS.  The stub calls `helper(1)`, which lands in the zero-filled
  BSS sled; the sled runs to hunk 2, whose relocator/decompressor expands
  hunk 3 into hunk 1.  The decoded image is 181,156 B at `0x10000`.
- `e2` - 1,552 B Amiga Hunk executable.  It is the TMB! decruncher (the
  `TMB!` magic, `0x544D4221`).  Its depacker at `0x10154` reads a backward
  bit stream, folds a running XOR checksum, and expands a stream whose
  trailing header is `[first long][checksum][decrunched size]["TMB!"]`.

Data in the extracted tree includes `init.dat` (27,472 B), `s1.dat` (43,832 B,
TMB!-packed -> 45,601 B), `s2.dat` (17,932 B, TMB!-packed -> 21,220 B),
`i1.dat`..`i6.dat`, `maped` (23,120 B), `text.dat` (32,610 B), the
`VMON*`/`VMPL*` banks, `vmappic.dat`, `vmissle.dat`, `vobjpic.dat`,
`vclips.dat`, `vmplp.dat`, and the `s/startup-sequence`.

`s1.dat` decodes to 68000 code that programs the Paula audio registers
(`$DFF0A0`..`$DFF0D8`), i.e. the sound/music module.  `s2.dat` decodes to a
data table.  The game text (`"Shadow Sorcerer.  The Dwarves ..."`) lives in
`text.dat`.

There is no `SHADOW.COM` and no `GAME.EXE`/`INTRO.EXE`.

## Atari ST release

Both `.st` files are raw FAT12 floppy images.

Disk 1 (`819,200 B`, 10 sectors/track, 2 sides):

- `SHADSORC.PRG` - 85,110 B Atari ST PRG, text 85,082 B, data 0, BSS 0.
  It is packed with a self-decompressing stub: `PEA`/`TRAP #14`
  (XBIOS `Supexec`) calls the decoder at `0x100AA`; the decoder expands a
  backward LZSS stream from `0x1020A` to `0x1060A`.  The decoded PRG has text
  `0x48D6A` (298,346 B) and 587 relocations.  The decoded program is the
  Clipper EXEC V2.00 loader (`"EXEC  V2.00"`,
  `"Copyright (c) 1988-1989 Clipper Computer Products"`,
  `"Loading Shadow Sorcerer"`, `"*** MOUSE version ***"`).
- `INTRO.SOR` - 2,217 B PRG, text 1,344 B, data 112 B, BSS 264,810 B.
  The strings include `title.dat`, `safesng2.SOR`, `dungeons.set`,
  `title.4v`, and loader/display/key/joystick/file helpers.  This is the
  intro/title loader.
- `DL3.SOR` - 53,994 B PRG, packed with the same stub; it decodes to a PRG
  with text `0x1160E` (71,182 B) and data `0xBB0C` (47,884 B), 119,066 B in
  all.  This is the game module: it contains `"Shadow Sorceror"`,
  `"Version UK(B) 1.0"`, `"Program by  U.S. Gold Ltd"`,
  `"Programming Graham Lilley"`, `"Goldmoon 7C"` and `"Graham E Lilley"`.
- `TITLE.DAT` - 192,204 B title/intro data.
- `DUNGEONS.SET`, `SHADSORC.SOL`, `SAFESNG2.SOR`, `SHAD_SOR.DOC`,
  `DESKTOP.INF`, and the small `TITLE.4V`/`S2.DAT` files.

Disk 2 (`755,712 B`, 9 sectors/track, 2 sides):

- `INIT.DAT` (27,472 B), `FEXT.DAT`, `GEXT.DAT`, `IEXT.DAT`,
  `MAPED` (23,120 B), `TEXT` data, `VMON*`, `VMPL*`, `VMISSLE.DAT`,
  `VMAPPIC.DAT`, `VOBJPIC.DAT`, `VCLIPS.DAT`, `VCIN*`.

There is no `SHADOW.COM`, no `GAME.EXE`, and no `INTRO.EXE`.  The ST release is
split into `INTRO.SOR` (intro), `SHADSORC.PRG` (EXEC V2.00 loader) and
`DL3.SOR` (game).

The `.st` suffix of `maped.st` is therefore consistent with the Atari ST
development origin: the PC release retained the editor/export code and its
literal output name, but never shipped the editor UI or the exported file.

## Decoded executables and Ghidra structure

The packed executables have been decoded to flat images and imported into
Ghidra with `tools/ghidra/DumpRaw.java`:

| Image | Bytes | Functions | Instructions | Source |
|---|---:|---:|---:|---|
| ST EXEC loader (`SHADSORC_img.bin`) | 298,346 | 49 | 4,303 | `SHADSORC.PRG` |
| ST game (`DL3_img.bin`) | 119,066 | 279 | 17,025 | `DL3.SOR` |
| Amiga game (`shadow_game.bin`) | 181,156 | 283 | 14,039 | `shadow` |
| Amiga sound module (`s1.bin`) | 45,601 | 3 | 432 | `s1.dat` |
| Amiga data module (`s2.bin`) | 21,220 | 1 | 64 | `s2.dat` |

Ghidra output directories (under `doc/versions/ghidra/`):

- `ghidra_st_exec/`
- `ghidra_st_game/`
- `ghidra_amiga_game_full/`
- `ghidra_amiga_game/` (sound module)
- `ghidra_amiga_game_s2/` (data module)

### ST packing scheme

`SHADSORC.PRG` and `DL3.SOR` use the same decoder at `0x100AA`.  It is a
backward bit-serial LZSS decoder:

- `a0` starts at the end of the packed data and is read backwards.
- `a1` starts at the end of the unpacked data and is written backwards.
- `d7` is a 32-bit shift register refilled with big-endian longs read
  backwards; a forced `1` in bit 0 acts as a sentinel.
- A 2-bit selector chooses a 4/8/12/16-bit value.  A literal run copies that
  many 8-bit bytes; a match copies `value+3` bytes at distance `value`.
- After the text/data comes a byte-delta relocation table.  A delta of `1`
  means `+0xFE` to the text pointer (a run of `1` bytes skips `0xFE` bytes
  each); any other delta is added to the text pointer and `0x10000` is added
  to the long there.

The decoder consumed the whole packed stream exactly (the source pointer ends
on the first packed byte) and produced a valid PRG header, which is a strong
check on the decode.

### Amiga packing scheme

`shadow` uses a different, Imploder-style decoder:

- Hunk 0 stub `0x10000` calls `helper(1)`; `helper(n)` walks the AmigaDOS
  segment list (`[next BPTR][data]` blocks) and returns the data address of
  hunk `n`.
- `helper(1)` lands in the 181,156-byte BSS hunk, which is a zero sled
  (`0x0000` = `ORI.B #0,D0`).  The sled runs into hunk 2 at `0x3C3F4`.
- Hunk 2 relocates the loaded hunks and, when the relocation stream wraps,
  falls into the decoder at `0x3C46A`.
- The decoder uses separate literal (`a0`) and bit (`a2`) pointers, both read
  backwards, a `0x4900`-byte window (`d4`), token-base tables at `0x3C51A`
  and match-offset tables at `0x3C56C`/`0x3C574`.

Emulating the stub and decoder in `machine68k` produced the 181,156-byte
image.  It contains the EXEC-style loader at `0x10000`
(`FindTask`, `OpenLibrary("dos.library")`, `OpenLibrary("intuition.library")`)
and the game/credits data from `0x1F000`:
`"(c) 1991 TSR, Inc All Rights Reserved"`,
`"(c) 1991 Strategic Simulations Inc"`,
`"based on Dragons of Hope"`,
`"SSI Developers"`, `"Program by  U.S. Gold Ltd"`,
`"Programming Graham Lilley"`, `"Shadow Sorceror"`,
`"Version UK(B) 1.0"`, `"Goldmoon"`, `"Graham E Lilley"`.

### TMB! packing scheme (`e2`, `s1.dat`, `s2.dat`)

The TMB! stream is read backwards from just before the trailing `"TMB!"`
magic:

```
[packed data ...][first long][checksum][decrunched size]["TMB!"]
```

Bits are consumed LSB-first from 32-bit words read backwards.  The decoder
folds a running XOR checksum (`d5 ^= d0`) and uses a 2-bit sub-opcode after a
set type bit to select a short match, a longer match, or a literal run.
Decoded sizes: `s1.dat` -> 45,601 B, `s2.dat` -> 21,220 B.  `s1.bin` starts
with valid 68000 code (`movem.l d0-d5/a0-a6,-(a7)` ...) and writes the Paula
audio registers; `s2.bin` starts with a table of 16-bit offsets.

### Reproduction

The unpacking routines are in `doc/versions/unpack/` (code only, no game
assets):

- `unpack_all.py` runs the whole pipeline:
  `python unpack_all.py --input <release_dir> --output <out_dir>`.
- ST: `unpack/st/lzss_st.py` (LZSS) then `unpack/st/unpack_st_prg.py`
  (relocation table and final image).
- Amiga: `unpack/amiga/run_shadow.py` builds the AmigaDOS segment layout and
  runs the real stub and decoder on `machine68k`, then dumps the BSS hunk.
- TMB! decode: `unpack/amiga/tmb_decode.py`.
- Ghidra: `tools/ghidra/DumpRaw.java` with `analyzeHeadless`, base `0x10000`.

The decoded artifacts are in `doc/versions/`; see
`doc/versions/README.md` for the file map.

## What differs between the releases

- **Executable container.** PC uses MZ; ST uses the Atari PRG format with a
  `0x601A` header; Amiga uses AmigaDOS Hunk executables.
- **Packing.** The PC `GAME.EXE` is a plain Microsoft C 5.1 MZ image.  The ST
  `SHADSORC.PRG` and `DL3.SOR` use a backward LZSS stub with an XBIOS
  `Supexec` bootstrap and a byte-delta relocation table.  The Amiga `shadow`
  uses an Imploder-style stub with a BSS zero sled, segment-list helper and
  a `0x4900`-byte window; the Amiga data modules use the TMB! cruncher.
- **Split.** PC: `INTRO.EXE` + `GAME.EXE` (`SHADOW.COM` is a TSR).  ST:
  `INTRO.SOR` + `SHADSORC.PRG` (EXEC V2.00 loader) + `DL3.SOR` (game).
  Amiga: one `shadow` executable containing loader, intro and game, plus
  `e2` as the TMB! data decruncher.
- **Video/TSR layer.** Only the PC has `SHADOW.COM`, the EGA/VGA INT F8h
  driver.  The ST and Amiga versions use native graphics.
- **Sound.** The PC uses `SOUND.DRV`, `ADLIB.ADV`, `MT32MPU.ADV` and
  `SSI.DAT` MIDI sequences.  The ST and Amiga versions drive their native
  sound hardware directly; the Amiga sound module is `s1.dat`.
- **Text/assets.** `TEXT.DAT` is common in content but the Amiga and ST
  asset files differ in size (for example `VCIN4.DAT` 24,028 B on Amiga vs
  24,020 B on PC, and `VMPLA.DAT` 11,854 B vs 11,850 B), reflecting the
  different graphics formats.

