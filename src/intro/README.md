# INTRO.EXE reconstruction

`game/INTRO.EXE` is the PC release's title/intro program: 19,499 bytes, MZ,
Microsoft C 5.1, small model. `SHADOW.COM` installs its INT `F8h` driver and
then EXECs this program with the argument tail from the release's `SS.BAT`
(`shadow v a`). A key press (or SIGINT) leaves the intro and SHADOW EXECs
`GAME.EXE`.

This directory reconstructs the SSI-authored half of the program in C, and
recovers the licensed John Miles UltraSound driver the release links into it as
MASM source, so the rebuilt EXE has its music.

## Image map

| image            | what it is |
|------------------|------------|
| `0x0000..0x000F` | 16 pad bytes |
| `0x0010..0x0AC0` | **the program**: `main`, the LZHUF decoder, the INT F8 wrappers, the page reveal |
| `0x0AC1..0x2DCF` | **the embedded John Miles UltraSound driver**, recovered as `sounddrv.asm` |
| `0x2DD0..0x4236` | Microsoft C 5.1 runtime (`_astart` at 0x2DD0) |
| `0x4237..0x423F` | padding before DGROUP |
| `0x4240..`       | DGROUP initialized data; `_end` = DGROUP+0xF300, stack 0x800 |

DGROUP offsets used below are offsets from the load-time DGROUP base. The
shipped binary's initialized data ends at DGROUP+0x07EB; the rest is BSS.

## The program

| address | name | what it does |
|---------|------|--------------|
| `0x0010` | `load_page` | point the decoder at `struct title_page` and decompress |
| `0x0026` | `plot_pixel` | plot one pixel, or read its colour from the page |
| `0x007E` | `dissolve_page` | 17-bit LFSR reveal of the page |
| `0x0119` | `quit_intro` | stop the music, wait, `exit(0)`; also SIGINT handler |
| `0x0154` | `main` | argument parsing and the endless page loop |
| `0x02D6..0x0417` | `shadow_*` | the INT F8h client wrappers |
| `0x0418` | `lz_getbit` | MSB-first bit reader |
| `0x046E` | `lz_getbyte` | byte reader |
| `0x04B2` | `lz_start_huff` | StartHuff |
| `0x053E` | `lz_reconst` | rebuild the adaptive tree |
| `0x06E8` | `lz_update` | increment one symbol and restore the sibling property |
| `0x07E0` | `lz_decode_char` | decode one 0..313 symbol |
| `0x0816` | `lz_decode_position` | static upper-6 + low bits position code |
| `0x0864` | `lzhuf_decode` | the decoder |
| `0x09BE` | `decode_file` | `fopen`/`fread`/`fclose` + `lzhuf_decode` |
| `0x0A00` | `bios_write_pixel` | INT 10h/AH=0Ch; present but never called |
| `0x0A1A` | `page_pixel` | decode one pixel from four interleaved plane words |
| `0x0A54` | `set_vga_palette` | program the EGA registers and the VGA DAC |
| `0x0A95` | `shadow_pixel` | selector 0x20, plot pixel |
| `0x0AAA` | `shadow_command21` | selector 0x21, queue/wait |

### The page loop

`main` reads only the first letter of each argument: `T`=1, `A`=2, `R`=3
select the sound driver mode and `V` selects the VGA title. It then loops
forever:

```
sound_play(mode == 0 ? 4 : 2)
signal(SIGINT, quit_intro)
shadow_config()                 /* always 0x21 */
shadow_video_copy(200)
load_page("etitle1.dat")
palette
dissolve_page(-1)
load_page("etitle2.dat"); dissolve_page(-1)
load_page("etitle3.dat"); dissolve_page(-1)
load_page("etitle4.dat"); dissolve_page(-1)
load_page("etitle5.dat"); dissolve_page(-1)
if (V) { dissolve_page(0); load_page("vtitle6.dat"); palette;
         dissolve_page(-1); }
else   { load_page("etitle6.dat"); dissolve_page(-1); }
```

`dissolve_page` walks a 17-bit LFSR (`lo=1`, feedback 0x2000, high bit = old
bit 0) through its whole non-zero cycle. For each state it plots
`(lo & 0x1FF, (lo >> 9) | (hi << 7))` when that falls inside 320x200; colour
`-1` means "read the page". At the end of the cycle it issues selector 0x21
with 2 and calls `kbhit()`; a key press runs the SIGINT path.

### The decompressed page

The decompressor writes at DGROUP 0x4FA6. `plot_pixel` reads the image at
DGROUP 0x4FC8, exactly 0x22 bytes later, so a page is:

```
+0x00  uint16 reserved          (always 0 in the shipped pages)
+0x02  uint16 palette[16]        (DGROUP 0x4FA8, used by the VGA page)
+0x22  uint8  image[32000]       (DGROUP 0x4FC8)
       ... some pages carry 32 more bytes
```

The image is 4 bits per pixel arranged as 20 groups of 16 pixels per
160-byte row. Each group is four plane words; within a plane word pixels 0..7
are the low byte's bits 7..0 and pixels 8..15 the high byte's bits 15..8.
`page_pixel` reproduces the shipped byte swap, and the 16 masks live at
DGROUP 0x00A8.

### The palette - two real traps

The release is started as `shadow v a`, but the argument tail arrives at
INTRO.EXE with the letters uppercased: both the release and the rebuild hold
`g_vga_flag = 1` and `g_intro_mode = 2`. The VGA path is therefore the one
that runs, and `set_vga_palette(g_page.palette)` is the palette call that
matters.

**Word format.** `set_vga_palette` (0x0A54) turns each 16-bit page-palette
entry into an INT 10h/AX=1010 DAC triple. The shipped code is:

```
CL = AH & 0x0F, CL <<= 3      ; low nibble  -> BLUE
CH = AH & 0xF0, CH >>= 1      ; high nibble -> GREEN
DH = AL << 3                  ; low byte    -> RED
```

so the word format is **AL = red, AH low nibble = blue, AH high nibble =
green**. Swapping green and blue produces a completely wrong but still
plausible-looking palette; that was the defect observed while checking the
rebuild.

**The EGA path is a different dependency.** If the EGA branch were taken,
`shadow_set_palette(0x96)` would call SHADOW's selector-2 handler
(`CS:0E1E`), which is only `AX=0x1002 / INT 10h`: BIOS "set all palette
registers", reading 17 bytes from **ES:DX**, with ES set from the *caller's*
DS. The 0x96 is therefore a DGROUP offset, not a SHADOW table index. DGROUP
0x0096 holds:

```
00 3B 3D 3F 00 04 3C 07 38 03 39 3C 06 3A 02 3E 00
```

Passing the literal 0x96 without providing that table at that offset gets
whatever the linker put there. `intro.c` carries the table as
`g_ega_palette` (non-const, so it really lands in DGROUP) and passes its
address.

Both tables must be **non-const**: MS C 5.1 puts `const` arrays in the code
segment, where a near pointer fetched through DS does not reach them.

### LZHUF

The page files are LZHUF (Yoshizaki 1988), not a proprietary format:

* `N=4096`, `F=60`, `THRESHOLD=2`, `N_CHAR=314`, `T=627`, `R=626`;
* first four bytes are the uncompressed size, little endian;
* the ring is filled with 0x20 and starts at `N-F=4036`;
* match lengths are symbol-0xFD, positions use `d_code`/`d_len` (DGROUP
  0x00D0/0x02D0, copied to work arrays 0x01D0/0x03D0).

One detail is easy to get wrong: `DecodePosition` starts its accumulator
from the *first byte*, not from zero, so the low six bits of the position
include the first byte's own low bits:

```c
i = GetByte();
high = d_code[i] << 6;
for (n = d_len[i] - 2; n; n--)
        i = (i << 1) + GetBit();
return high | (i & 0x3F);
```

Starting from zero still produces a plausible stream, but it decodes to
noise.

The `update()` sibling search compares frequencies as **unsigned**: freq[627]
is the 0xFFFF sentinel, and a signed compare reads it as -1, walks off the
table and corrupts `prnt[]`.

## The embedded sound driver

`INTRO.EXE` does not load `SOUND.DRV`. It statically links a copy of the
John Miles UltraSound driver at `0x0AC1..0x2DCF`. Evidence:

* the driver data begins at 0x0AC1 with the 0x50..0x10 palette ramp;
* the first entry is 0x136C, which is `SOUND.DRV`'s word-0 entry 0x0641
  shifted by +0xD2B;
* every run of 24 bytes or more in 0x0D5D..0x2DCF occurs in `SOUND.DRV`
  exactly 0xD2B lower, and no intro-program byte does;
* 0x2DD0 is the Microsoft C `_astart`, and 0x2E82 begins the linked library,
  exactly as in `GAME.EXE`.

Every driver entry saves DS, sets `DS=CS`, uses CS-relative data and returns
FAR. It never calls the CRT. `sound.h` names the four entries the program
uses (`0x136C` init, `0x13DF` stop, `0x1421` play, `0x21CE` channel busy) and
the four unused public entries.

### Recovering it as assembly

`tools/mkdriver_asm.py` walks the driver with a recursive-descent disassembler
starting from every public entry and the driver's own indirect jump tables
(`0x19C1`, `0x1F6A`, `0x2009`, and the 0x71-entry opcode table at `0x25A7`).
It recovers 2,017 instructions / 4,904 code bytes; the only non-code runs are
those four tables. It then writes `src/intro/sounddrv.asm`, which MASM 5.1
assembles with no warnings or errors. The build regenerates the file from the
pristine release first, so the checked-in source cannot drift from it.

The source is deliberately **position-fixed**. Absolute data operands stay
numeric (they are segment offsets in the original, and must not be relocated),
while branch targets, jump-table entries and the callback pointers the driver
stores in its channel records are labels, so MASM emits relocations for them.
The build pads the program to `0x0AC1` and links `sounddrv.asm` there, exactly
where the release puts it. MASM canonicalises `xchg` to `(r/m, r)` and would
otherwise pick the opposite `xchg al,ah` encoding; the generator swaps the
operands so the assembled bytes match the shipped ones.

The result is byte-for-byte identical to the release for the whole
`0x0AC1..0x2DCF` region: data, code, the embedded tables, the opcode jump
table and the song bank. The driver's absolute references are never patched at
run time, so placing it anywhere else would break the music.

### The open-source release is AIL 2.14, not this driver

John Miles open-sourced the Audio Interface Library in 2000, but the release
is **2.14 (14-Nov-92)**. Shadow Sorcerer's 1991 drivers are **AIL 1.x**:

| | Shadow Sorcerer | AIL 2.14 |
|---|---|---|
| ADV header length | `0x20` | `0x2D` |
| copyright | `Copyright (C) 1991 John Miles` | `Copyright (C) 1991,1992 Miles Design, Inc.` |
| interface tag | `0x0014` | `0x0064` |
| `ADLIB.ADV` match | - | 27.5% |

The AIL 2.14 sources name the exact predecessors: `AIL.ASM` V2.00 is
"derived from V1.02", and `YAMAHA.INC` V1.00 "replaces YM3812.INC v1.04".
That 1.x tree is not in the open-source package and no public copy was found.

The full AIL 2.14 distribution is kept at `doc/ail/`; its real-mode source
subset is extracted under `ail214/` as the closest reference: `AIL.ASM` for
the timer/queue architecture, `YAMAHA.INC` and `MT32.INC` for the ADV
structure. See `ail214/README.md` for provenance, the licence, and the
version evidence.

## Build

```
python tools/build_intro.py
INTRO BUILD: PASS
```

The gate compiles `intro.c`, `lzhuf.c`, `shadow.c` and `shadowx.c` with the
vendored `cl /AS /Gs /Os`, assembles `sounddrv.asm` with MASM 5.1, and links
them against `slibce + libh` with `/stack:2048` into `tmp/bintro/intro.exe`.

Linking the driver needs two passes. The first link measures where
`sound_init` would land; the gate then generates a padding object of
`0x0AC1 - measured_start` zero bytes and relinks, putting the driver at its
fixed origin. `shadow.c` holds only the five wrappers the intro actually
calls; the dead library wrappers moved to `shadowx.c` and are linked *after*
the driver, so they cannot push it out of position. If the pre-driver program
ever grows past `0x0AC1`, the gate fails and says by how much rather than
silently moving the driver.

The gate also checks the map: `sound_init`, `sound_stop`, `sound_play` and
`sound_channel_active` must be at `0x136C`, `0x13DF`, `0x1421` and `0x21CE`,
and the built image's `0x0AC1..0x2DCF` region must equal the release's byte for
byte. It does not require binary identity for the rest of the program; page
rendering and music are checked at run time.

To watch the rebuild, put `tmp/bintro/intro.exe` in a copy of the release
directory as `INTRO.EXE` and start it the release's way:

```
cd \tmp\yourcopy
shadow v a
```

SHADOW installs and EXECs the local `INTRO.EXE`; the pages, palette and
dissolve come from the reconstruction, while the INT F8 services come from
the resident `SHADOW.COM` exactly as on the release disk.

## Verification performed

1. **LZHUF against the release.** A DOS test program built from `lzhuf.c`
   decodes `ETITLE1.DAT` and `VTITLE6.DAT`; the full declared output matches
   an independent reference decoder byte for byte (32,066 bytes each).
2. **Decompressor inside the rebuilt intro.** The rebuilt EXE was run under
   SHADOW.COM, the CPU halted, and `g_page` dumped from the address in the
   rebuild's map. It matched the reference decode of the page then loaded
   (ETITLE2, 32,034/32,034 bytes). The map address moves when the source
   changes; take it from `tmp/bintro/intro.map`, not from this note.
3. **Page rendering.** The reference decode rendered at image offset +0x22
   produces the release's screens: the SSI logo (ETITLE1) and the
   "Advanced Dungeons & Dragons / the SAGA / Shadow Sorcerer" title
   (ETITLE3).
4. **Palette.** The release and the rebuild were each caught on page 1 and
   their VGA DAC palettes compared entry by entry; after the green/blue fix
   all 16 entries are identical. The earlier green background and the
   mismatched entries were the two defects described above.
5. **Recovered driver vs the release.** The assembled `sounddrv.asm` was
   compared against the shipped image: the whole `0x0AC1..0x2DCF` region
   (8,975 bytes: data, code, tables and song bank) is identical. The linked
   rebuild reproduces the four entry points at their shipped offsets.
6. **Music at run time.** The release and the rebuild were run under
   `SHADOW.COM` in the same emulator and the driver inspected through the
   harness. Both install the recovered ISR as INT 8h (`0A65:1267`), read
   mode 2, queue song 2, consume it (`read == write == 1`), set the busy flag,
   keep the same song-bank base (`0x263B`), advance the timer countdown, and
   call the OPL write helper (`0x2188`) from the same internal call sites.
   A framebuffer dump of the rebuild shows the "THE HEROES" intro page. The
   rebuilt program's page phase drifts from the release's because it is
   recompiled C, so a screenshot taken at the same wall-clock second need not
   be the same page.

## Audio capture and waveform comparison

The emulator harness now records the mixer. Set `HARNESS_WAV=<path>` in the
emulator environment to capture from power-on, or, with the CPU halted, use
the harness commands

```
wavstart <path>     start a WAV at the next mixer block
wavstate            report whether capture is armed
wavstop             flush the last block and finalize the WAV
```

`[mixer] nosound=true` still renders the internal mixer, so no host audio
device is needed. The writer is `CAPTURE_ArmWaveTo`/`CAPTURE_StartWaveTo`
in `emulator/dosbox-x/src/hardware/hardware.cpp`; the commands are in
`emulator/harness_patch/harness.cpp`. It writes 16-bit stereo PCM at the
`[mixer] rate`. Re-apply notes for the ignored DOSBox-X tree are in
`emulator/harness_patch/dosbox-audio-capture.txt`. Rebuild the emulator with

```
cd emulator
PATH=C:\soft\w64devkit\bin;%PATH%
make -f Makefile.shsorc
```

The release and the rebuild were recorded from power-on with
`cycles=fixed 100000`, `[mixer] rate=44100`, and the first non-silent sample
used as the alignment point. Over the first 8 s after that point:

| pair | waveform corr | RMS diff / RMS signal | 1 kHz LP corr | STFT corr |
|---|---|---|---|---|
| release vs rebuild | 0.9945 | 9.9% | 0.9971 | 0.9944 |
| release vs release | 0.9965 | 7.9% | 0.9986 | 0.9966 |
| rebuild vs rebuild | 0.9967 | 7.8% | 0.9985 | 0.9931 |

So the waveforms are **not bit-identical**, and it would be wrong to claim
they are. The control is the important part: two runs of the *same release
binary* are also not bit-identical. The emulator's mixer/OPL sample phase and
the guest's music-start instant vary between processes, which perturbs the
rendered samples even when the driver and song are identical. The
release-versus-rebuild difference is slightly larger than the same-binary
spread but of the same order; the note timing does not drift (the best lag is
constant across the window) and the spectra track each other closely. The
driver region is byte-identical and both sides select song 2 from the same
bank, so the remaining difference is in rendering/start phase and in the
page-loop instant that re-queues the song, not in the tune.

The trimmed recordings (`release_intro.wav`, `rebuild_intro.wav`), the raw
captures and the waveform images are in `tmp/audio/`. Sample-exact equality
would require a capture trigger tied to the first OPL register write and a
deterministic OPL/mixer reset at the same emulated instant; the current
harness does not yet provide that.

## Remaining work

* `doc/ASSET_PACK.md` and `tools/assetpack.py` now unpack and bake the release
  asset set (sprite banks, VMON/VMPL animations, intro pages, TEXT.DAT, SSI.DAT
  MIDI, PC-speaker SFX and raw files).  Intro pages are decoded for viewing but
  LZHUF recompression is not implemented, so edited pages are refused by the
  baker.
* The 0x02D6..0x0417 wrappers that the intro never calls carry their DGROUP
  variables at offset 0 in the shipped image; their original source is not
  recoverable from the code alone. The reconstruction gives them ordinary
  storage because nothing reads them. They live in `shadowx.c`, after the
  driver, so the dead code cannot move it.
* The 32 extra bytes some pages decode past the image are preserved as
  `struct title_page.tail`; the program never reads them.
* The driver's 1991 AIL 1.x source is still not available (see `ail214/`),
  but the driver itself is now recovered from the shipped binary. The same
  object is `SOUND.DRV` when `GAME.EXE` loads it; `SOUND.DRV`'s byte layout
  differs only by the 0xD2B relocation documented above.
