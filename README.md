# Dungeons & Dragons Shadow Sorcerer — source reconstruction

Reverse engineered **Shadow Sorcerer**, the 1991 SSI / U.S. Gold
Dragonlance RPG, rebuilt from the shipped MS-DOS release.

Author: **[Nancy Sadkov](https://aermia.com/)**

## The game

Shadow Sorcerer is a party-based Dragonlance adventure based on *Dragons of
Hope* by Tracy Hickman. Four heroes travel an overland map, explore sites,
talk, trade and fight monsters, with a spellbook-driven magic system.

## Architecture

The PC release is three programs:

- **`SHADOW.COM`** — a resident EGA/video TSR. It hooks `INT F8h` and supplies
  the drawing, palette, text and memory services the game calls. Rebuilt from
  MASM source; byte-identical to the release.
- **`GAME.EXE`** — the game itself. Microsoft C 5.1, small model, with the
  original DGROUP layout preserved.
- **`INTRO.EXE`** — the title sequence and the embedded John Miles UltraSound
  driver, fixed at image `0x0AC1` and reproduced byte-for-byte.

## License

CC0 1.0 Universal. See [LICENSE.md](LICENSE.md).
