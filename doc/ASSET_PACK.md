# Asset packing

`tools/assetpack.py` unpacks the release's data files into an editable tree and
bakes that tree back into release files.  `tools/lzhuf.py` is the compatible
intro-page decoder.

```
python tools/assetpack.py unpack --game game --out assets
python tools/assetpack.py bake   --assets assets --out tmp/baked
python tools/assetpack.py verify --game game
```

`verify` unpacks to a temporary directory, bakes it, and compares every result
with `game/`; it currently reports **PASS** for all 58 groups (1,621 files).

## Recovered formats

### Offset-table sprite banks

VOBJPIC, VMAPPIC, VMISSLE (separate layout), VCLIPS and VCIN0..4.

The first word is the end of a word-offset table, and also the offset of the
first record, so the table has `first_word / 2` entries.  Entries may repeat:
a sprite id is an index, not a byte offset, and several ids can share one
record.  A record starts with two words:

```
+0  width   render selector for VOBJPIC/VMAPPIC; 16-pixel units for VCIN/VCLIPS
+2  height  pixels; for VOBJPIC/VMAPPIC the low byte is SHADOW's BL kind
+4  data    width * height * 8 bytes
```

The data length formula was checked against the next table offset in every
bank; the records tile exactly (VOBJPIC's 159 ids reduce to 46 unique records,
VCLIPS's 90 used ids reduce to 32, and so on).

There are two pixel layouts, selected by the SHADOW service the game uses:

* **planar** (`c_svc03`/`c_svc04`, VOBJPIC/VMAPPIC): per row, four planes each
  `width * 2` bytes, plane-major.  A pixel is the four plane bits; index 0 is
  transparent.  This is the layout behind `sprite_draw_dispatch`,
  `sprite_blit_center`, `sprite_blit_offset` and `sprite_draw_far`.
* **block** (`c_svc08`/`blit_block_impl`, VCLIPS/VCIN): per row, `width` units
  of eight bytes, unit-major; each unit is four planes of two bytes and covers
  16 pixels.  This is `sprite_draw_current`/`sprite_draw_block`.

`VMAPPIC` is planar; its first `width` word is a selector code (1 = 16-pixel
`c_svc03`, 2 = 32-pixel `c_svc04`).  `VCLIPS` is block; its first 90 ids are
used by `sprite_draw_current`, and the shared blank record at offset 0x58 sits
inside the table, so the packer leaves that record in the skeleton rather than
zeroing table bytes.

### VMON / VMPL animation banks

```
+0  unknown word
+2  unknown word
+4  x_offset        x is pulled back by x_offset << 3
+6  y_offset        y is pulled back by y_offset; low byte is the blit kind
+8  state_table     seven 6-byte state entries follow
```

Each state entry is `frame_table` (word), `frame_count`, `step_x`, `step_y`,
`frame_limit`.  A frame table holds eight direction offsets; each direction
holds `frame_count` frame offsets.  A frame is raw 32-pixel planar data:
`16 * (y_offset & 0xff)` bytes, no header, because it is drawn by `c_svc04`.
This matches `struct sprite_bank_header` and `sprite_draw_state_frame` in
`src/video.c`.

Directions 0-3 are drawn from those bytes as stored.  Directions 4-7 instead
go through SHADOW's pixel-order remap (the alternate selector path in
`sprite_draw_state_frame`): each 4-byte group is reversed and the bits of each
byte are reversed.  Across a 32-pixel planar row that is exactly a horizontal
mirror, and it is an involution, so applying it twice returns the stored bytes.
The same stored frame is therefore used for one low direction and the opposite
high direction, and the packer emits two PNGs for it:

* `MNF0005.PNG` - the stored bitmap, what directions 0-3 blit;
* `MRF0005.PNG` - the remapped bitmap, what directions 4-7 blit.

The two are mirror images, not two different frames.  A leg that touches one
edge of the raw view touches the opposite edge of the mirrored view; that is
the mirror, not a scanline wrap.  VMON uses `MNx`/`MRx`, VMPL uses
`MPx`/`PRx`.  The manifest marks the mirrored entry with `"remap": true` and
both entries carry the same `offset`.  Baking either view recovers the same
stored bytes; if both views of one frame are edited inconsistently the baker
refuses rather than picking one.

### VMISSLE

The bank is a table of 8-byte missile descriptors (`frame_table`, `width`,
`height`, `frame_limit`); descriptor 0 is the empty/no-missile slot.  A frame
table holds `frame_limit` frame offsets, and each frame is raw planar data
`width * 8 * height` bytes.  This matches `struct missile_descriptor` and
`sprite_draw_field`.

### Intro pages

ETITLE1..5 and VTITLE6 are LZHUF streams beginning with a little-endian 32-bit
output size.  The decoded `struct title_page` is a 2-byte reserved word, sixteen
palette words at +2, the 320x200 image at +0x22, and up to 32 trailing bytes.

The image is twenty 8-byte groups per row.  Each group is four 16-bit plane
words, and each word is byte-swapped for display: pixel `x & 15` is bit
`15 - (x & 15)` of the swapped word.  The palette word format is the one
`set_vga_palette` uses: low byte red, AH low nibble blue, AH high nibble green.
Each channel is a 4-bit nibble shifted left by three, and the DAC keeps the low
six bits, so the effective 6-bit value is `(nibble << 3) & 0x3f`; the PNG
palette scales that to 8 bits (`value * 255 / 63`).

Only ETITLE1 and VTITLE6 call `set_vga_palette`.  ETITLE2..ETITLE5 inherit
ETITLE1's DAC palette, so the packer renders those four with ETITLE1's palette
while preserving the unused per-page palette in the file.  Baking an edited
ETITLE2..5 pixel keeps that stored palette; editing ETITLE1's palette changes
the active colours for all four.

`tools/lzhuf.py` is a direct port of the reconstructed decoder and reads its
position tables from `src/intro/lzhuf.c`.

### TEXT.DAT

250 little-endian word offsets followed by NUL-terminated strings.  The packer
writes one raw CP437 file per record (`TXTnnnn.TXT`).  Changed strings are
rebaked with a fresh offset table.

### SSI.DAT

A 32-entry table of little-endian 32-bit offsets to Standard MIDI Files.  The
game uses entries 1..6; entry 7 and the duplicate entries 8..31 point at the
last file.  The packer writes each distinct file as `SSInnnn.MID` and preserves
the table.  Changed MIDI files are repacked and the table offsets are rewritten.

### SFX

`GAME.EXE` contains the 16-sequence PC-speaker fallback bank: a 16-entry table
of absolute addresses at image 0xC850, and 1,494 bytes of stream data at image
0xC890.  `tick_handler_type0/1/2` in `src/tick.c` define the three stream
kinds (noise, tone, melody/loop).  The packer writes the raw slices as
`SFXnnnn.BIN` and renders approximate WAVs with a square-wave PIT model.  The
Adlib/MT-32 sequences are not PCM and live inside `SOUND.DRV`/`ADLIB.ADV`/
`MT32MPU.ADV`; those are preserved byte-for-byte in `RAW/`.

## Naming

Every editable file is 7+3.  Prefixes are in `assets/README.md`; the number is
the sprite id, frame index or file index.  `manifest.json` carries the recovered
label where one exists, including the monster names from the VMON suffix
(Baaz, Bozak, ...), the `VOBJPIC`/`VCLIPS` `SPRITE_*` names from `src/ids.h`,
and the `VMPL_*` presentation frame names from `src/world.c`.

## Round-trip and limitations

* All sprite banks, intro pages, TEXT.DAT, SSI.DAT, raw files and GAME.EXE
  rebuild byte-for-byte when untouched.
* Intro pages are recompressed when edited.  The encoder is compatible with the
  intro's LZHUF decoder; the page reserved word and trailing bytes are kept.
  For ETITLE1/VTITLE6 the PNG palette is used when it differs; ETITLE2..5 keep
  their stored palette because the game inherits ETITLE1's active palette.  A
  page that recompresses over the intro's 16,000-byte read buffer is refused.
* SFX WAVs are approximations of the PC-speaker fallback; the driver-based
  Adlib/MT-32 sound is not rendered.
* TEXT files are raw CP437 bytes.
