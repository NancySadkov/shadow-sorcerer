# AIL 2.14 - the John Miles Audio Interface Library source

This is the **nearest publicly available source** for the John Miles sound
driver that `GAME.EXE` loads as `SOUND.DRV` and that `INTRO.EXE` links
statically.

## Provenance

* IBM Audio Interface Library, Release **2.14 of 14-Nov-92**, by John Miles /
  Miles Design, Inc.
* Open-sourced by John Miles on 26 May 2000: "all source code and compiled
  object code in this package may be considered open-source freeware, usable
  by anyone for any purpose, commercial or otherwise, without restriction or
  limitation."  See `READ.ME` and `LICENSE.TXT`.
* The full distribution is now kept in the repository at `doc/ail/`
  (`A214_D1.ZIP` .. `A214_D4.ZIP`, plus `REL105.ZIP` for AIL/32 1.05).
  This directory holds the real-mode source subset extracted from
  `A214_D3.ZIP`, byte for byte; the extracted files hash-identical to the
  VGMPF mirror of the official `AIL2.ZIP` linked from
  <http://www.ke5fx.com/>.

Only the real-mode driver/API sources and the two drivers this release uses
are kept here; the 32-bit protected-mode tree, tools and demo programs in the
full distribution are not.

| File | What it is |
|---|---|
| `AIL.ASM` | the AIL API/timer/queue module (V2.00 derived from V1.02) |
| `AIL.H`, `AIL.INC`, `AIL.MAC` | its C/assembly interfaces |
| `YAMAHA.INC` | Ad Lib / OPL2 / OPL3 driver source (V1.00 "replaces YM3812.INC v1.04") |
| `MT32.INC`, `MPU401.INC` | Roland MT-32 and MPU-401 driver source |
| `XMIDI.ASM` | the XMIDI sequencer |
| `SPKR.INC` | PC-speaker driver |
| `REFERENCE_ADLIB.ADV`, `REFERENCE_MT32MPU.ADV` | the 2.14 built drivers, for binary comparison |
| `READ.ME`, `LICENSE.TXT`, `NOTES.TXT` | the release notes, licence and API notes |

## This is NOT the exact version Shadow Sorcerer uses

Shadow Sorcerer's drivers are **AIL 1.x (1991)**, not AIL 2.14.  The evidence
is in the shipped files:

| | Shadow Sorcerer (1991) | AIL 2.14 |
|---|---|---|
| ADV header length | `0x20` | `0x2D` |
| copyright string | `Copyright (C) 1991 John Miles` | `Copyright (C) 1991,1992 Miles Design, Inc.` |
| interface tag at +0x1A | `0x0014` | `0x0064` |
| `ADLIB.ADV` size | 18,191 | 14,775 |
| `MT32MPU.ADV` size | 11,638 | 10,707 |
| byte match (`ADLIB.ADV`) | - | 27.5% |
| byte match (`MT32MPU.ADV`) | - | 16.4% |

The AIL 2.14 sources themselves name the exact predecessors:

* `AIL.ASM`: "Version 2.00 of 24-Sep-91: Initial V2.X version (derived from
  V1.02)".
* `YAMAHA.INC`: "Version 1.00 of 15-Feb-92: Init version (replaces
  YM3812.INC v1.04)".
* `MT32.INC`: "Version 1.00 of 27-Sep-91: Initial version for AIL V2.0
  release".

So the matching source would be the AIL 1.x tree with `YM3812.INC` v1.04 and
`MT32.INC` v1.00.  `doc/ail/` contains only AIL 2.14 and the separate
32-bit AIL/32 1.05 (19-Oct-93); the 1991 real-mode 1.x tree is not in it.

## How to use this tree

It is a **reference**, not a drop-in:

* `AIL.ASM` shows the timer/queue/sequencer architecture that `SOUND.DRV`
  implements in its 1991 form.
* `YAMAHA.INC` and `MT32.INC` show the device-driver structure behind
  `ADLIB.ADV` / `MT32MPU.ADV`; the 1991 bodies are the v1.04/v1.00
  predecessors named in their own headers.
* The `REFERENCE_*.ADV` binaries let you diff a 2.14 build against the
  release's 1991 drivers and separate code that stayed the same (large
  identical runs, e.g. 2,822 bytes in `ADLIB.ADV`) from code that changed.

To make `INTRO.EXE` behaviourally complete, either:

1. recover the AIL 1.x delta from the shipped `game/INTRO.EXE` /
   `game/SOUND.DRV` using this tree as the guide, or
2. link the shipped `0x0AC1..0x2DCF` driver image at its original offset
   (the licensed-external route currently described in `../README.md`).
