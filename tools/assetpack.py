#!/usr/bin/env python3
"""Unpack and bake Shadow Sorcerer's asset files.

The release stores graphics as EGA planar sprite banks.  Each output PNG is an
indexed 16-colour image with palette index 0 transparent, matching SHADOW's
blitter (a pixel is drawn only when at least one of its four plane bits is set).

The packer keeps a per-bank skeleton: the original file with its pixel ranges
cleared.  Baking copies the skeleton and writes the PNG pixels back at their
recorded offsets, so an untouched bank rebuilds byte-for-byte and an edited PNG
changes only the pixels it represents.  Animation banks keep the original frame
bytes instead, because one stored frame can appear as two PNG views (raw and
mirrored); the baker compares against those bytes to see which view was edited.

Usage:
    python tools/assetpack.py unpack [--game game] [--out assets]
    python tools/assetpack.py bake   [--assets assets] [--out tmp/baked]
    python tools/assetpack.py verify [--game game] [--assets assets]
"""
from __future__ import annotations

import argparse
import hashlib
import json
import math
import os
import struct
import sys
import wave
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

from PIL import Image

import lzhuf

ROOT = os.path.normpath(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

# g_pal_rgb in src/video.c: a 12-bit 0xRGB word, channel order R, G, B.
_PAL_WORDS = (
    0x000, 0x410, 0x630, 0x777, 0x000, 0x222, 0x047, 0x555,
    0x333, 0x030, 0x040, 0x150, 0x500, 0x741, 0x753, 0x770,
)
PALETTE = tuple(
    (((w >> 8) & 0xF) * 17, ((w >> 4) & 0xF) * 17, (w & 0xF) * 17)
    for w in _PAL_WORDS
)


def page_word_to_rgb(w: int) -> Tuple[int, int, int]:
    """Intro/VGA palette word: low byte red, AH low nibble blue, high green.

    set_vga_palette shifts each 4-bit channel left by three and the DAC takes
    the low six bits, so the effective 6-bit value is (nibble << 3) & 0x3f.
    """
    r6 = ((w & 0xFF) << 3) & 0x3F
    g6 = (((w >> 8) & 0xF0) >> 1) & 0x3F
    b6 = (((w >> 8) & 0x0F) << 3) & 0x3F
    return (r6 * 255 // 63, g6 * 255 // 63, b6 * 255 // 63)


def rgb_to_page_word(rgb: Tuple[int, int, int]) -> int:
    r, g, b = rgb
    r4 = (((r * 63 + 127) // 255) >> 3) & 0xF
    g4 = (((g * 63 + 127) // 255) >> 3) & 0xF
    b4 = (((b * 63 + 127) // 255) >> 3) & 0xF
    return r4 | (b4 << 8) | (g4 << 12)

# Monster names in shipped order, verbatim from the 0x095A name pool in
# src/actor.c.  src/ids.h maps the VMON suffix to this same sequence: request
# 1..17 -> A..Q, 18..26 -> R..Z, 27/28 -> 0/1.  (The actor state/kind is the
# request + 4, which is why e.g. Bozak, kind 6, is bank B.)
MONSTER_LABELS = {
    "A": "Baaz", "B": "Bozak", "C": "Kapak", "D": "Aurak", "E": "Sivak",
    "F": "Spider", "G": "Hobgoblin", "H": "Skeleton", "I": "Troll",
    "J": "Haunt", "K": "Zombie", "L": "Wight", "M": "Ghoul", "N": "Ghost",
    "O": "Giant Wasp", "P": "Ogre", "Q": "Aghar", "R": "Dknight",
    "S": "Golem", "T": "Hydra", "U": "Lizard", "V": "Slig",
    "W": "Spectral", "X": "Dwarves", "Y": "Lich", "Z": "RDRAGON",
    "0": "BDRAGON", "1": "GDRAGON",
}

# Known sprite-id labels from src/ids.h.  These are manifest labels only; the
# PNG filename stays the stable bank+id name.
VOID_LABELS = {
    0x3A: "CELL_CURSOR", 0x3C: "ACTOR_SPELLBOOK", 0x46: "ACTOR_ALIVE",
    0x4D: "STATUS_FOOD", 0x4E: "STATUS_SCORE", 0x4F: "STATUS_REFUGEES",
    0x50: "BUTTON_MARK", 0x51: "BUTTON_MARK_SET", 0x58: "HERO_GAUGE_FRAME",
    0x59: "PANEL_CORNER",
}
CLIP_LABELS = {
    0x1A: "CONTROL_ICON", 0x1D: "MAP_SCROLL_ARROW", 0x1E: "PANEL_BACKGROUND",
    0x1F: "CONTROL_OVERLAY", 0x20: "SCROLL_ARROW_BASE", 0x21: "ARROW_UP",
    0x22: "ARROW_DOWN", 0x24: "FRAME_RING", 0x25: "PORTRAIT_FRAME",
    0x27: "MENU_RULE", 0x28: "HERO_CURSOR",
}
VCIN_LABELS = {
    "VCIN0": {0: "DWARF_SCENE", 1: "DWARF_WAKE_1", 2: "DWARF_WAKE_2",
              3: "DWARF_WAKE_3", 4: "DWARF_WAKE_4", 5: "DWARF_SIT_1",
              6: "DWARF_SIT_2", 7: "DWARF_SIT_3", 8: "DWARF_SIT_4",
              9: "DWARF_SIT_5", 10: "DWARF_SIT_6", 11: "DWARF_SIT_7"},
    "VCIN1": {0: "FEAST_SCENE", 1: "FEAST_DISH_1", 2: "FEAST_DISH_2",
              3: "FEAST_DISH_3", 4: "FEAST_DISH_4", 5: "FEAST_TABLE_1",
              6: "FEAST_TABLE_2", 7: "FEAST_TABLE_3", 8: "FEAST_TABLE_4",
              9: "FEAST_TABLE_5", 10: "FEAST_TABLE_6", 11: "FEAST_TABLE_7",
              12: "FEAST_TABLE_8"},
    "VCIN2": {0: "DOOR_SCENE", 1: "DOOR_OPEN_1", 2: "DOOR_OPEN_2",
              3: "DOOR_OPEN_3", 4: "DOOR_OPEN_4", 5: "DOOR_OPEN_5",
              6: "DOOR_OPEN_6"},
    "VCIN3": {0: "GATE_SCENE", 1: "GATE_FLOOR_1", 2: "GATE_FLOOR_2",
              3: "GATE_FLOOR_3", 4: "GATE_FLOOR_4", 5: "GATE_FLOOR_5",
              6: "GATE_FLOOR_6", 7: "GATE_DOOR_1", 8: "GATE_DOOR_2",
              9: "GATE_DOOR_3", 10: "GATE_DOOR_4", 11: "GATE_DOOR_5"},
    "VCIN4": {0: "COUNCIL_FRAME_1", 1: "COUNCIL_FRAME_2",
              2: "COUNCIL_FRAME_3"},
}


def label_for(source: str, ids: List[int]) -> str:
    stem = source.rsplit(".", 1)[0].upper()
    if stem.startswith("VMON"):
        letter = stem[4:5]
        return MONSTER_LABELS.get(letter, stem) + " sprite"
    if stem == "VOBJPIC" and ids:
        return VOID_LABELS.get(ids[0], "")
    if stem == "VCLIPS" and ids:
        return CLIP_LABELS.get(ids[0], "")
    if stem in VCIN_LABELS and ids:
        return VCIN_LABELS[stem].get(ids[0], "")
    return ""


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def read_u16(data: bytes, off: int) -> int:
    return struct.unpack_from("<H", data, off)[0]


def write_u16(data: bytearray, off: int, value: int) -> None:
    struct.pack_into("<H", data, off, value & 0xFFFF)


# ---------------------------------------------------------------------------
# PNG helpers
# ---------------------------------------------------------------------------

def palette_image(width: int, height: int, pixels: List[List[int]]) -> Image.Image:
    """Build an indexed PNG image; index 0 is transparent."""
    im = Image.new("P", (width, height))
    flat = bytearray(width * height)
    for y, row in enumerate(pixels):
        flat[y * width:(y + 1) * width] = bytes(row)
    im.frombytes(bytes(flat))
    im.putpalette([c for rgb in PALETTE for c in rgb])
    im.info["transparency"] = 0
    return im


def read_palette_image(path: str) -> Tuple[int, int, List[List[int]]]:
    """Read an indexed PNG and return its 0..15 palette indices."""
    im = Image.open(path)
    if im.mode != "P":
        im = im.convert("RGBA")
        # nearest palette entry, with alpha 0 -> index 0
        w, h = im.size
        px = im.load()
        out = [[0] * w for _ in range(h)]
        for y in range(h):
            for x in range(w):
                r, g, b, a = px[x, y]
                if a < 128:
                    out[y][x] = 0
                    continue
                best, bestd = 0, 1 << 30
                for i, (pr, pg, pb) in enumerate(PALETTE):
                    d = (r - pr) ** 2 + (g - pg) ** 2 + (b - pb) ** 2
                    if d < bestd:
                        best, bestd = i, d
                out[y][x] = best
        return w, h, out
    im.load()
    w, h = im.size
    raw = list(im.getdata())
    return w, h, [raw[y * w:(y + 1) * w] for y in range(h)]


# ---------------------------------------------------------------------------
# Sprite codecs
# ---------------------------------------------------------------------------

REV_BITS = bytes(int(format(i, "08b")[::-1], 2) for i in range(256))


def remap_bytes(src: bytes, height: int, grp: int = 4) -> bytes:
    """The 16-colour SHADOW remap: reverse each grp-byte group and its bits."""
    out = bytearray(len(src))
    n = height * 4
    for i in range(n):
        for j in range(grp):
            out[i * grp + j] = REV_BITS[src[i * grp + (grp - 1 - j)]]
    return bytes(out)


def decode_planar(data: bytes, off: int, width: int, height: int) -> List[List[int]]:
    """Selectors 3/4: 4 planes, each width*2 bytes, plane-major per row."""
    row_plane = width * 2
    row_bytes = row_plane * 4
    w = width * 16
    out: List[List[int]] = []
    p = off
    for _ in range(height):
        row = data[p:p + row_bytes]
        p += row_bytes
        line = [0] * w
        for x in range(w):
            byte = x >> 3
            bit = 7 - (x & 7)
            v = 0
            for pl in range(4):
                if (row[pl * row_plane + byte] >> bit) & 1:
                    v |= 1 << pl
            line[x] = v
        out.append(line)
    return out


def encode_planar(pixels: List[List[int]], width: int, height: int) -> bytes:
    row_plane = width * 2
    row_bytes = row_plane * 4
    out = bytearray(row_bytes * height)
    for y in range(height):
        base = y * row_bytes
        for x in range(width * 16):
            v = pixels[y][x] & 0xF
            if v == 0:
                continue
            byte = base + (x >> 3)
            bit = 7 - (x & 7)
            for pl in range(4):
                if (v >> pl) & 1:
                    out[byte + pl * row_plane] |= 1 << bit
    return bytes(out)


def decode_block(data: bytes, off: int, width: int, height: int) -> List[List[int]]:
    """Selector 8: width units of 8 bytes per row, unit-major, 4 planes x 2 B."""
    row_bytes = width * 8
    w = width * 16
    out: List[List[int]] = []
    p = off
    for _ in range(height):
        row = data[p:p + row_bytes]
        p += row_bytes
        line = [0] * w
        for u in range(width):
            base = u * 8
            words = [row[base + 2 * pl] | (row[base + 2 * pl + 1] << 8)
                     for pl in range(4)]
            for x in range(16):
                bit = 15 - x
                v = 0
                for pl in range(4):
                    if (words[pl] >> bit) & 1:
                        v |= 1 << pl
                line[u * 16 + x] = v
        out.append(line)
    return out


def encode_block(pixels: List[List[int]], width: int, height: int) -> bytes:
    row_bytes = width * 8
    out = bytearray(row_bytes * height)
    for y in range(height):
        base = y * row_bytes
        for u in range(width):
            words = [0, 0, 0, 0]
            for x in range(16):
                v = pixels[y][u * 16 + x] & 0xF
                if v == 0:
                    continue
                bit = 15 - x
                for pl in range(4):
                    if (v >> pl) & 1:
                        words[pl] |= 1 << bit
            for pl in range(4):
                off = base + u * 8 + pl * 2
                out[off] = words[pl] & 0xFF
                out[off + 1] = (words[pl] >> 8) & 0xFF
    return bytes(out)


# ---------------------------------------------------------------------------
# Offset-table banks
# ---------------------------------------------------------------------------

@dataclass
class Record:
    offset: int
    width: int
    height: int
    layout: str          # "planar" or "block"
    ids: List[int] = field(default_factory=list)
    png: str = ""
    label: str = ""

    @property
    def data_offset(self) -> int:
        return self.offset + 4

    @property
    def data_length(self) -> int:
        return self.width * self.height * 8

    @property
    def end(self) -> int:
        return self.data_offset + self.data_length


def parse_offset_bank(data: bytes, layout: str, id_limit: Optional[int] = None) -> List[Record]:
    """Parse a bank whose first word is the end of its word-offset table."""
    first = read_u16(data, 0)
    if first < 2 or first > len(data):
        return []
    n = first // 2
    if id_limit is not None:
        n = min(n, id_limit)
    by_off: Dict[int, Record] = {}
    for sid in range(n):
        off = read_u16(data, 2 * sid)
        if off < first or off + 4 > len(data):
            continue
        width, height = read_u16(data, off), read_u16(data, off + 2)
        # Plausibility: the banks use 1..4 (planar) or 1..32 (block).  A
        # record must not overlap the table unless it is a deliberate shared
        # blank inside it; those are skipped by the caller.
        if layout == "planar":
            if not (1 <= width <= 8 and 1 <= height <= 256):
                continue
        else:
            if not (1 <= width <= 64 and 1 <= height <= 256):
                continue
        end = off + 4 + width * height * 8
        if end > len(data):
            continue
        rec = by_off.get(off)
        if rec is None:
            rec = Record(off, width, height, layout)
            by_off[off] = rec
        rec.ids.append(sid)
    return sorted(by_off.values(), key=lambda r: r.offset)


def decode_offset_bank(data: bytes, layout: str, id_limit: Optional[int] = None):
    records = parse_offset_bank(data, layout, id_limit)
    return records


# ---------------------------------------------------------------------------
# VMON / VMPL animation banks
# ---------------------------------------------------------------------------

@dataclass
class AnimFrame:
    offset: int
    height: int
    ids: List[str] = field(default_factory=list)
    png: str = ""
    remap: bool = False


def parse_anim_bank(data: bytes) -> Tuple[dict, List[AnimFrame], List[dict]]:
    """Return (header, unique frames, state/direction table description)."""
    if len(data) < 10:
        return {}, [], []
    u0, u1, xo, yo, st = struct.unpack_from("<5H", data, 0)
    height = yo & 0xFF
    header = {"unknown_00": u0, "unknown_02": u1, "x_offset": xo,
              "y_offset": yo, "height": height, "state_table": st}
    states = []
    frame_by_key: Dict[Tuple[int, bool], AnimFrame] = {}
    for si in range(7):
        off = st + 6 * si
        if off + 6 > len(data):
            break
        ft, fc, sx, sy, lim = struct.unpack_from("<HBBBB", data, off)
        state = {"state": si, "frame_table": ft, "frame_count": fc,
                 "step_x": sx, "step_y": sy, "frame_limit": lim,
                 "directions": []}
        if ft + 16 <= len(data):
            dirs = list(struct.unpack_from("<8H", data, ft))
            state["directions"] = dirs
            for di, doff in enumerate(dirs):
                # g_node_selector is direction+1.  Directions 4..7 take the
                # remap path in sprite_draw_state_frame; 0..3 draw raw.
                remap = di >= 4
                for fi in range(fc):
                    pos = doff + 2 * fi
                    if pos + 2 > len(data):
                        continue
                    fo = read_u16(data, pos)
                    if fo == 0 or fo + 16 * height > len(data):
                        continue
                    fr = frame_by_key.get((fo, remap))
                    if fr is None:
                        fr = AnimFrame(fo, height, remap=remap)
                        frame_by_key[(fo, remap)] = fr
                    fr.ids.append("%d/%d/%d" % (si, di, fi))
        states.append(state)
    return header, sorted(frame_by_key.values(),
                          key=lambda f: (f.offset, f.remap)), states


# ---------------------------------------------------------------------------
# VMISSLE frame bank
# ---------------------------------------------------------------------------

@dataclass
class MissileRec:
    index: int
    frame_table: int
    width: int
    height: int
    frame_limit: int
    frames: List[AnimFrame] = field(default_factory=list)


def parse_missile_bank(data: bytes) -> List[MissileRec]:
    recs: List[MissileRec] = []
    idx = 0
    while idx * 8 + 8 <= len(data):
        off = idx * 8
        ft, width, height, limit = struct.unpack_from("<4H", data, off)
        if idx == 0:
            # The first descriptor is the empty/no-missile slot.
            idx += 1
            continue
        if not (1 <= width <= 4 and 1 <= height <= 256 and limit <= 64):
            break
        if ft + 2 * limit > len(data):
            break
        frames = []
        for fi in range(limit):
            fo = read_u16(data, ft + 2 * fi)
            if fo == 0 or fo + width * 8 * height > len(data):
                continue
            frames.append(AnimFrame(fo, height, ["%d/%d" % (idx, fi)]))
        recs.append(MissileRec(idx, ft, width, height, limit, frames))
        idx += 1
    return recs


# ---------------------------------------------------------------------------
# Pack / unpack a bank directory
# ---------------------------------------------------------------------------

def _unique_records(records: List[Record]) -> List[Record]:
    seen = {}
    for r in records:
        if r.offset not in seen:
            seen[r.offset] = r
    return [seen[k] for k in sorted(seen)]


def unpack_bank(name: str, data: bytes, outdir: str, kind: str,
                label_prefix: str, layout: str = "planar",
                id_limit: Optional[int] = None) -> dict:
    os.makedirs(outdir, exist_ok=True)
    if kind == "offset":
        records = _unique_records(decode_offset_bank(data, layout, id_limit))
        entries = []
        for r in records:
            png = "%s%04X.PNG" % (label_prefix, min(r.ids))
            r.png = png
            pixels = (decode_planar if layout == "planar" else decode_block)(
                data, r.data_offset, r.width, r.height)
            image = palette_image(r.width * 16, r.height, pixels)
            image.save(os.path.join(outdir, png), optimize=True)
            entries.append({
                "offset": r.offset, "data_offset": r.data_offset,
                "width": r.width, "height": r.height,
                "layout": layout, "ids": r.ids, "png": png,
                "label": label_for(name, r.ids),
            })
        skeleton = bytearray(data)
        for r in records:
            if r.offset >= read_u16(data, 0):
                skeleton[r.data_offset:r.end] = b"\0" * r.data_length
        manifest = {
            "format": "offset-bank", "source": name, "size": len(data),
            "sha256": sha256(data), "layout": layout,
            "table_bytes": read_u16(data, 0),
            "entries": entries,
        }
    elif kind == "anim":
        header, frames, states = parse_anim_bank(data)
        if label_prefix.startswith("MN"):
            remap_prefix = "MR" + label_prefix[2:]
        elif label_prefix.startswith("MP"):
            remap_prefix = "PR" + label_prefix[2:]
        else:
            remap_prefix = "R" + label_prefix[1:]
        entries = []
        offset_index = {off: i for i, off in enumerate(
            sorted({fr.offset for fr in frames}))}
        for fr in frames:
            prefix = remap_prefix if fr.remap else label_prefix
            png = "%s%04d.PNG" % (prefix, offset_index[fr.offset])
            fr.png = png
            if fr.remap:
                src = remap_bytes(data[fr.offset:fr.offset + 16 * fr.height],
                                  fr.height)
                pixels = decode_planar(src, 0, 2, fr.height)
            else:
                pixels = decode_planar(data, fr.offset, 2, fr.height)
            palette_image(32, fr.height, pixels).save(
                os.path.join(outdir, png), optimize=True)
            label = label_for(name, [])
            if fr.remap:
                label = (label + " mirrored").strip() if label else ""
            entries.append({"offset": fr.offset,
                            "data_offset": fr.offset, "width": 2,
                            "height": fr.height, "layout": "planar",
                            "remap": fr.remap,
                            "ids": fr.ids, "png": png,
                            "label": label})
        skeleton = bytearray(data)
        # Animation frames keep their original bytes: a frame can be stored
        # once and drawn twice (raw for directions 0-3, remapped for 4-7), so
        # the baker needs the pristine bytes to tell which of the two PNG
        # views the user edited.
        manifest = {
            "format": "anim-bank", "source": name, "size": len(data),
            "sha256": sha256(data), "header": header, "states": states,
            "entries": entries,
        }
    elif kind == "missile":
        recs = parse_missile_bank(data)
        unique = {}
        for rec in recs:
            for fr in rec.frames:
                key = (fr.offset, rec.width, rec.height)
                if key not in unique:
                    unique[key] = {"offset": fr.offset, "width": rec.width,
                                   "height": rec.height, "ids": []}
                unique[key]["ids"].extend(fr.ids)
        entries = []
        for n, key in enumerate(sorted(unique)):
            e = unique[key]
            png = "%s%04X.PNG" % (label_prefix, n)
            pixels = decode_planar(data, e["offset"], e["width"], e["height"])
            palette_image(e["width"] * 16, e["height"], pixels).save(
                os.path.join(outdir, png), optimize=True)
            entries.append({"offset": e["offset"], "data_offset": e["offset"],
                            "width": e["width"], "height": e["height"],
                            "layout": "planar", "ids": e["ids"], "png": png,
                            "label": ""})
        skeleton = bytearray(data)
        for e in entries:
            n = e["width"] * 8 * e["height"]
            skeleton[e["offset"]:e["offset"] + n] = b"\0" * n
        manifest = {
            "format": "missile-bank", "source": name, "size": len(data),
            "sha256": sha256(data),
            "missiles": [{"index": r.index, "frame_table": r.frame_table,
                          "width": r.width, "height": r.height,
                          "frame_limit": r.frame_limit} for r in recs],
            "entries": entries,
        }
    else:
        raise ValueError(kind)

    with open(os.path.join(outdir, "skeleton.bin"), "wb") as fh:
        fh.write(skeleton)
    with open(os.path.join(outdir, "manifest.json"), "w") as fh:
        json.dump(manifest, fh, indent=1)
    return manifest


def bake_bank(bankdir: str, outdir: str) -> str:
    manifest = json.load(open(os.path.join(bankdir, "manifest.json")))
    data = bytearray(open(os.path.join(bankdir, "skeleton.bin"), "rb").read())
    # A frame used by both halves of the direction table has two PNGs: the
    # stored bitmap (directions 0-3) and its SHADOW remap/mirror (4-7).  Both
    # bake to the same stored bytes, so group the views and compare each
    # against the pristine bytes to decide which view the user edited.
    groups: Dict[Tuple[int, int], List[Tuple[str, bytes]]] = {}
    for entry in manifest["entries"]:
        png = os.path.join(bankdir, entry["png"])
        w, h, pixels = read_palette_image(png)
        if (w, h) != (entry["width"] * 16, entry["height"]):
            raise SystemExit("%s: image is %dx%d, expected %dx%d" % (
                png, w, h, entry["width"] * 16, entry["height"]))
        enc = (encode_planar if entry["layout"] == "planar" else encode_block)(
            pixels, entry["width"], entry["height"])
        if entry.get("remap"):
            # The PNG is the mirrored/remapped visual; the remap is an
            # involution, so applying it recovers the stored bytes.
            enc = remap_bytes(enc, entry["height"])
        off = entry.get("data_offset", entry["offset"] + 4)
        groups.setdefault((off, len(enc)), []).append((png, enc))
    for (off, n), views in groups.items():
        if off + n > len(data):
            raise SystemExit("record at %04X runs past end of file" % off)
        if len(views) == 1:
            data[off:off + n] = views[0][1]
            continue
        base = bytes(data[off:off + n])
        changed = [(png, enc) for png, enc in views if enc != base]
        if not changed:
            continue
        chosen_png, chosen = changed[0]
        for png, enc in changed[1:]:
            if enc != chosen:
                raise SystemExit(
                    "%s and %s are two different edits of one frame; "
                    "make the raw and mirrored views agree" %
                    (chosen_png, png))
        data[off:off + n] = chosen
    out = os.path.join(outdir, manifest["source"])
    os.makedirs(outdir, exist_ok=True)
    with open(out, "wb") as fh:
        fh.write(data)
    return out


# ---------------------------------------------------------------------------
# Intro pages (LZHUF-compressed 320x200 EGA pages)
# ---------------------------------------------------------------------------

INTRO_FILES = ["ETITLE1.DAT", "ETITLE2.DAT", "ETITLE3.DAT", "ETITLE4.DAT",
               "ETITLE5.DAT", "VTITLE6.DAT"]


def decode_intro_image(page: bytes) -> Tuple[List[Tuple[int, int, int]], List[List[int]]]:
    words = list(struct.unpack_from("<16H", page, 2))
    palette = [page_word_to_rgb(w) for w in words]
    img = page[0x22:0x22 + 32000]
    pixels = [[0] * 320 for _ in range(200)]
    for y in range(200):
        row = img[y * 160:(y + 1) * 160]
        for g in range(20):
            raw = struct.unpack_from("<4H", row, g * 8)
            planes = [((w >> 8) | ((w & 0xFF) << 8)) & 0xFFFF for w in raw]
            for x in range(16):
                bit = 15 - x
                v = 0
                for p in range(4):
                    if (planes[p] >> bit) & 1:
                        v |= 1 << p
                pixels[y][g * 16 + x] = v
    return palette, pixels


def encode_intro_image(pixels: List[List[int]]) -> bytes:
    """Inverse of decode_intro_image: 20 groups of four byte-swapped words."""
    out = bytearray()
    for y in range(200):
        for g in range(20):
            words = [0, 0, 0, 0]
            for x in range(16):
                v = pixels[y][g * 16 + x] & 0xF
                if v == 0:
                    continue
                bit = 15 - x
                for p in range(4):
                    if (v >> p) & 1:
                        words[p] |= 1 << bit
            for w in words:
                swapped = ((w >> 8) | ((w & 0xFF) << 8)) & 0xFFFF
                out.extend(struct.pack("<H", swapped))
    return bytes(out)


def palette_words_from_rgb(palette: List[Tuple[int, int, int]]) -> List[int]:
    return [rgb_to_page_word(rgb) for rgb in palette[:16]]


def png_palette_rgb(path: str) -> List[Tuple[int, int, int]] | None:
    im = Image.open(path)
    if im.mode != "P":
        return None
    raw = im.getpalette() or []
    raw = raw + [0] * max(0, 48 - len(raw))
    return [tuple(raw[i * 3:i * 3 + 3]) for i in range(16)]


def unpack_intro(game: str, outroot: str) -> None:
    d = os.path.join(outroot, "INTRO")
    os.makedirs(d, exist_ok=True)
    loaded = []
    for name in INTRO_FILES:
        path = os.path.join(game, name)
        if not os.path.exists(path):
            continue
        data = open(path, "rb").read()
        page = lzhuf.decode(data)
        _, pixels = decode_intro_image(page)
        own = list(struct.unpack_from("<16H", page, 2))
        loaded.append((name, data, page, own, pixels))
    if not loaded:
        return
    master = next((p[3] for p in loaded if p[0] == "ETITLE1.DAT"), loaded[0][3])
    manifest = {"format": "intro-pages", "pages": []}
    for n, (name, data, page, own, pixels) in enumerate(loaded):
        # The intro sets a palette only for ETITLE1 and VTITLE6; ETITLE2..5
        # inherit ETITLE1's palette.  Render those pages with the active
        # palette, not the unused copy stored in the file.
        own_palette = name in ("ETITLE1.DAT", "VTITLE6.DAT")
        display = own if own_palette else master
        palette = [page_word_to_rgb(w) for w in display]
        png = "ETI%04d.PNG" % (n + 1)
        image = Image.new("P", (320, 200))
        image.putpalette([c for rgb in palette for c in rgb])
        flat = bytearray(320 * 200)
        for y in range(200):
            flat[y * 320:(y + 1) * 320] = bytes(pixels[y])
        image.frombytes(bytes(flat))
        image.save(os.path.join(d, png), optimize=True)
        manifest["pages"].append({
            "source": name, "png": png, "compressed_size": len(data),
            "decoded_size": len(page),
            "reserved": read_u16(page, 0),
            "palette": own,
            "display_palette": display,
            "palette_source": name if own_palette else "ETITLE1.DAT",
            "sha256": sha256(data),
        })
    json.dump(manifest, open(os.path.join(d, "manifest.json"), "w"), indent=1)


def bake_intro(bankdir: str, outroot: str, game: str) -> List[str]:
    manifest = json.load(open(os.path.join(bankdir, "manifest.json")))
    out = []
    for page in manifest["pages"]:
        src = page["source"]
        # Byte-identical when the PNG still matches the release page.  Intro
        # recompression is a separate step; refuse to silently drop edits.
        original = None
        for base in (game,):
            p = os.path.join(base, src)
            if os.path.exists(p):
                original = open(p, "rb").read()
                break
        if original is None:
            raise SystemExit("missing original %s" % src)
        decoded = lzhuf.decode(original)
        _, pixels = decode_intro_image(decoded)
        w, h, edited = read_palette_image(os.path.join(bankdir, page["png"]))
        if (w, h) != (320, 200):
            raise SystemExit("%s: expected 320x200" % page["png"])

        orig_palette = page.get(
            "palette", list(struct.unpack_from("<16H", decoded, 2)))
        display_palette = page.get("display_palette", orig_palette)
        display_rgb = [page_word_to_rgb(w) for w in display_palette]
        png_pal = png_palette_rgb(os.path.join(bankdir, page["png"]))
        own_palette = page.get("palette_source", src) == src
        palette_changed = (own_palette and png_pal is not None and
                           png_pal != display_rgb)

        if edited != pixels or palette_changed:
            # Recompress the page.  The reserved word and trailing bytes come
            # from the release page; the image and (for ETITLE1/VTITLE6, the
            # pages that actually set a palette) the palette come from the PNG.
            reserved = read_u16(decoded, 0)
            palette = (palette_words_from_rgb(png_pal)
                       if palette_changed else orig_palette)
            tail = decoded[0x22 + 32000:]
            page_data = (struct.pack("<H", reserved) +
                         struct.pack("<16H", *palette) +
                         encode_intro_image(edited) + tail)
            enc = lzhuf.encode(page_data)
            if len(enc) > 16000:
                raise SystemExit(
                    "%s: recompressed page is %d bytes, over the intro's "
                    "16000-byte read buffer" % (page["png"], len(enc)))
            p = os.path.join(outroot, src)
            os.makedirs(outroot, exist_ok=True)
            open(p, "wb").write(enc)
            out.append(p)
            continue
        p = os.path.join(outroot, src)
        os.makedirs(outroot, exist_ok=True)
        open(p, "wb").write(original)
        out.append(p)
    return out


# ---------------------------------------------------------------------------
# TEXT.DAT
# ---------------------------------------------------------------------------

TEXT_INDEX_WORDS = 250


def unpack_text(game: str, outroot: str) -> None:
    data = open(os.path.join(game, "TEXT.DAT"), "rb").read()
    d = os.path.join(outroot, "TEXT")
    os.makedirs(d, exist_ok=True)
    entries = []
    for i in range(TEXT_INDEX_WORDS):
        off = read_u16(data, 2 * i)
        end = data.find(b"\0", off)
        if end < 0:
            end = len(data)
        text = data[off:end]
        png = "TXT%04d.TXT" % i
        open(os.path.join(d, png), "wb").write(text)
        entries.append({"index": i, "offset": off, "file": png,
                        "sha256": sha256(text)})
    json.dump({"format": "text", "source": "TEXT.DAT", "size": len(data),
               "sha256": sha256(data), "entries": entries},
              open(os.path.join(d, "manifest.json"), "w"), indent=1)


def bake_text(bankdir: str, outroot: str, game: str) -> str:
    manifest = json.load(open(os.path.join(bankdir, "manifest.json")))
    original = open(os.path.join(game, manifest["source"]), "rb").read()
    texts = []
    changed = False
    for e in manifest["entries"]:
        raw = open(os.path.join(bankdir, e["file"]), "rb").read()
        texts.append(raw)
        if raw != original[e["offset"]:e["offset"] + len(raw)] or \
                (e["offset"] + len(raw) < len(original) and
                 original[e["offset"] + len(raw)] != 0):
            changed = True
    if not changed:
        out = bytearray(original)
    else:
        out = bytearray(original[:2 * TEXT_INDEX_WORDS])
        for i, raw in enumerate(texts):
            write_u16(out, 2 * i, len(out))
            out.extend(raw)
            out.append(0)
    p = os.path.join(outroot, manifest["source"])
    os.makedirs(outroot, exist_ok=True)
    open(p, "wb").write(out)
    return p


# ---------------------------------------------------------------------------
# SSI.DAT - 32-entry table of Standard MIDI Files
# ---------------------------------------------------------------------------

def unpack_music(game: str, outroot: str) -> None:
    data = open(os.path.join(game, "SSI.DAT"), "rb").read()
    d = os.path.join(outroot, "MUSIC")
    os.makedirs(d, exist_ok=True)
    table = list(struct.unpack_from("<32I", data, 0))
    starts = sorted({v for v in table if 0 < v < len(data)})
    entries = []
    for n, off in enumerate(starts):
        end = starts[n + 1] if n + 1 < len(starts) else len(data)
        mid = data[off:end]
        name = "SSI%04d.MID" % (n + 1)
        open(os.path.join(d, name), "wb").write(mid)
        entries.append({"offset": off, "file": name, "size": len(mid),
                        "sha256": sha256(mid)})
    json.dump({"format": "midi-container", "source": "SSI.DAT",
               "size": len(data), "sha256": sha256(data),
               "table": table, "entries": entries},
              open(os.path.join(d, "manifest.json"), "w"), indent=1)


def bake_music(bankdir: str, outroot: str, game: str) -> str:
    manifest = json.load(open(os.path.join(bankdir, "manifest.json")))
    original = open(os.path.join(game, manifest["source"]), "rb").read()
    changed = False
    for e in manifest["entries"]:
        raw = open(os.path.join(bankdir, e["file"]), "rb").read()
        if raw != original[e["offset"]:e["offset"] + e["size"]]:
            changed = True
            break
    if not changed:
        out = bytearray(original)
    else:
        table = list(manifest["table"])
        body = bytearray(128)
        last = 128
        for n, e in enumerate(manifest["entries"]):
            raw = open(os.path.join(bankdir, e["file"]), "rb").read()
            table[n + 1] = len(body)
            last = len(body)
            body.extend(raw)
        for i in range(len(manifest["entries"]) + 1, 32):
            table[i] = last
        struct.pack_into("<32I", body, 0, *table)
        out = body
    p = os.path.join(outroot, manifest["source"])
    os.makedirs(outroot, exist_ok=True)
    open(p, "wb").write(out)
    return p


# ---------------------------------------------------------------------------
# Raw external data
# ---------------------------------------------------------------------------

RAW_FILES = ["MAPED", "FAT.ADB", "SAVEGAME._A_", "SOUND.DRV",
             "ADLIB.ADV", "MT32MPU.ADV", "GAME.EXE", "INTRO.EXE",
             "SHADOW.COM", "SS.BAT"]


def unpack_raw(game: str, outroot: str) -> None:
    d = os.path.join(outroot, "RAW")
    os.makedirs(d, exist_ok=True)
    entries = []
    for name in RAW_FILES:
        p = os.path.join(game, name)
        if not os.path.exists(p):
            continue
        data = open(p, "rb").read()
        open(os.path.join(d, name), "wb").write(data)
        entries.append({"source": name, "size": len(data), "sha256": sha256(data)})
    json.dump({"format": "raw", "entries": entries},
              open(os.path.join(d, "manifest.json"), "w"), indent=1)


def bake_raw(bankdir: str, outroot: str) -> List[str]:
    manifest = json.load(open(os.path.join(bankdir, "manifest.json")))
    out = []
    for e in manifest["entries"]:
        src = os.path.join(bankdir, e["source"])
        p = os.path.join(outroot, e["source"])
        os.makedirs(outroot, exist_ok=True)
        open(p, "wb").write(open(src, "rb").read())
        out.append(p)
    return out


# ---------------------------------------------------------------------------
# PC-speaker SFX (the 16-sequence fallback bank inside GAME.EXE)
# ---------------------------------------------------------------------------

SFX_BANK_IMAGE = 0xC890
SFX_BANK_BYTES = 1494
SFX_TABLE_IMAGE = 0xC850
SFX_COUNT = 16
PIT_HZ = 1193182.0
SFX_TICK_HZ = 18.2


def _game_image(path: str) -> bytes:
    data = open(path, "rb").read()
    if data[:2] != b"MZ":
        return data
    header = struct.unpack_from("<H", data, 8)[0] * 16
    return data[header:]


def _square(freq: float, seconds: float, sr: int = 44100,
            amp: int = 12000) -> bytes:
    n = max(0, int(seconds * sr))
    if n == 0:
        return b""
    out = bytearray(n * 2)
    if freq <= 0:
        return bytes(out)
    period = sr / freq
    fade = min(int(sr * 0.005), n // 2)
    for i in range(n):
        env = 1.0
        if i < fade:
            env = i / fade
        elif i >= n - fade:
            env = (n - i) / fade
        v = amp if (i % period) < (period / 2) else -amp
        struct.pack_into("<h", out, 2 * i, int(v * env))
    return bytes(out)


def _render_sfx(kind: int, data: bytes) -> bytes:
    chunks = []
    if kind == 1:                      # tone: one PIT divisor per tick
        for i in range(0, len(data) - 1, 2):
            div = read_u16(data, i)
            freq = PIT_HZ / div if div else 0
            chunks.append(_square(freq, 1.0 / SFX_TICK_HZ))
            if len(chunks) > int(SFX_TICK_HZ * 8):
                break
    elif kind != 0:                    # melody/loop: duration, frequency
        pos = 0
        elapsed = 0.0
        while pos + 3 < len(data) and elapsed < 8.0:
            dur = read_u16(data, pos)
            freqdiv = read_u16(data, pos + 2)
            pos += 4
            if dur == 0:
                break
            seconds = dur / SFX_TICK_HZ
            if elapsed + seconds > 8.0:
                seconds = max(0.0, 8.0 - elapsed)
            freq = PIT_HZ / freqdiv if freqdiv else 0
            chunks.append(_square(freq, seconds))
            elapsed += seconds
    else:                              # noise voice: LFSR-driven PIT
        if len(data) < 8:
            return b""
        repeat = read_u16(data, 0)
        mask = read_u16(data, 2)
        orv = read_u16(data, 4)
        period = read_u16(data, 6)
        sr = 0
        ticks = 0
        max_ticks = int(SFX_TICK_HZ * 8)
        for _ in range(max(1, repeat)):
            for _ in range(max(1, period)):
                sr = (sr + 0x9248) & 0xFFFF
                sr = ((sr >> 3) | (sr << 13)) & 0xFFFF
                f = (sr & mask) | orv
                chunks.append(_square(PIT_HZ / f if f else 0,
                                      1.0 / SFX_TICK_HZ))
                ticks += 1
                if ticks >= max_ticks:
                    break
            if ticks >= max_ticks:
                break
    pcm = b"".join(chunks)
    if len(pcm) > 44100 * 2 * 8:
        pcm = pcm[:44100 * 2 * 8]
    return pcm


def unpack_sfx(game: str, outroot: str) -> None:
    image = _game_image(os.path.join(game, "GAME.EXE"))
    d = os.path.join(outroot, "SFX")
    os.makedirs(d, exist_ok=True)
    bank = image[SFX_BANK_IMAGE:SFX_BANK_IMAGE + SFX_BANK_BYTES]
    entries = []
    starts = []
    for i in range(SFX_COUNT):
        abs_off = read_u16(image, SFX_TABLE_IMAGE + 4 * i)
        kind = read_u16(image, SFX_TABLE_IMAGE + 4 * i + 2)
        starts.append((abs_off - SFX_BANK_IMAGE, kind))
    for i, (off, kind) in enumerate(starts):
        end = min([o for o, _ in starts if o > off] + [SFX_BANK_BYTES])
        raw = bank[off:end]
        bin_name = "SFX%04d.BIN" % i
        wav_name = "SFX%04d.WAV" % i
        open(os.path.join(d, bin_name), "wb").write(raw)
        pcm = _render_sfx(kind, raw)
        with wave.open(os.path.join(d, wav_name), "wb") as w:
            w.setnchannels(1)
            w.setsampwidth(2)
            w.setframerate(44100)
            w.writeframes(pcm)
        entries.append({"index": i, "offset": off, "kind": kind,
                        "binary": bin_name, "wav": wav_name,
                        "label": "" if kind == 0 else
                                 ("tone" if kind == 1 else "melody")})
    json.dump({"format": "sfx-embedded", "source": "GAME.EXE",
               "bank_image": SFX_BANK_IMAGE, "bank_bytes": SFX_BANK_BYTES,
               "table_image": SFX_TABLE_IMAGE,
               "entries": entries},
              open(os.path.join(d, "manifest.json"), "w"), indent=1)


def bake_sfx(bankdir: str, outroot: str, game: str) -> List[str]:
    manifest = json.load(open(os.path.join(bankdir, "manifest.json")))
    src = os.path.join(game, manifest["source"])
    data = bytearray(open(src, "rb").read())
    header = struct.unpack_from("<H", data, 8)[0] * 16
    image = bytearray(data[header:])
    base = manifest["bank_image"]
    for e in manifest["entries"]:
        raw = open(os.path.join(bankdir, e["binary"]), "rb").read()
        off = base + e["offset"]
        if off + len(raw) > base + manifest["bank_bytes"]:
            raise SystemExit("%s: edited SFX runs past the bank" % e["binary"])
        image[off:off + len(raw)] = raw
    data[header:] = image
    out = os.path.join(outroot, manifest["source"])
    os.makedirs(outroot, exist_ok=True)
    open(out, "wb").write(data)
    return [out]


# ---------------------------------------------------------------------------
# File registry
# ---------------------------------------------------------------------------

SPRITE_BANKS = {
    "VOBJPIC.DAT": ("VOB", "planar", "offset", None),
    "VMAPPIC.DAT": ("VMA", "planar", "offset", 76),
    "VCLIPS.DAT": ("VCL", "block", "offset", 128),
    "VCIN0.DAT": ("VC0", "block", "offset", None),
    "VCIN1.DAT": ("VC1", "block", "offset", None),
    "VCIN2.DAT": ("VC2", "block", "offset", None),
    "VCIN3.DAT": ("VC3", "block", "offset", None),
    "VCIN4.DAT": ("VC4", "block", "offset", None),
}

ANIM_BANKS = {}
for _letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
    ANIM_BANKS["VMON%s.DAT" % _letter] = ("MN" + _letter, "anim")
for _n in "01":
    ANIM_BANKS["VMON%s.DAT" % _n] = ("MN" + _n, "anim")
for _letter in "ABCDEFGHIJKLMNOP":
    ANIM_BANKS["VMPL%s.DAT" % _letter] = ("MP" + _letter, "anim")


ASSET_README = """# Shadow Sorcerer asset tree

Generated by `tools/assetpack.py unpack`.  Every group has a `manifest.json`
that records the original file, table offsets, record sizes, PNG filenames and
the recovered labels; `skeleton.bin` is the original file with sprite pixel
ranges cleared.  Animation banks keep their frame bytes, because one stored
frame can appear as two PNG views (raw and mirrored).  `tools/assetpack.py
bake` copies the skeleton and writes the PNG pixels back, so untouched assets
rebuild byte-for-byte.

## Naming

All editable files use a 7+3 DOS name.  The three-letter prefixes are:

| prefix | bank |
|---|---|
| VOB | VOBJPIC.DAT (VOBJPIC) |
| VMA | VMAPPIC.DAT |
| VMI | VMISSLE.DAT |
| VCL | VCLIPS.DAT |
| VC0..VC4 | VCIN0..VCIN4.DAT |
| MNA..MNZ, MN0, MN1 | VMONA..VMONZ, VMON0, VMON1 |
| MRA..MRZ, MR0, MR1 | the same VMON banks, mirrored views |
| MPA..MPP | VMPLA..VMPLP |
| PRA..PRP | the same VMPL banks, mirrored views |
| ETI | ETITLE1..5 / VTITLE6 intro pages |
| SSI | SSI.DAT Standard MIDI Files |
| SFX | PC-speaker SFX bank in GAME.EXE |
| TXT | TEXT.DAT strings |

The number after the prefix is the sprite id (offset banks), the frame index
(animation banks), or the file index.  The manifest's `label` carries the
recovered name where one is known (e.g. `Baaz`, `STATUS_FOOD`,
`DWARF_WAKE_1`).

## PNG format

Sprites are indexed 16-colour PNGs.  Palette entry 0 is transparent; a pixel is
drawn only when at least one of its four EGA plane bits is set, which is exactly
index 0 here.  The palette is the game's `g_pal_rgb` table in `src/video.c`.
Record widths are in 16-pixel units, so a `width` of 2 is a 32-pixel image.
VMON/VMPL frames are always 32 pixels wide; VMISSLE frames use their descriptor
width.

Paint with the exact 16 palette colours.  Transparent pixels are index 0.  If
an editor saves RGB/RGBA instead of indexed PNG, the baker maps each pixel to
the nearest palette entry and treats alpha < 128 as transparent.

VMON/VMPL animation frames have two views.  Directions 0-3 blit the stored
bitmap; directions 4-7 run it through SHADOW's pixel-order remap, which over a
32-pixel row is an exact horizontal mirror.  A frame used by both halves of the
direction table therefore appears twice:

* `MNF0005.PNG` is the stored bitmap (directions 0-3);
* `MRF0005.PNG` is the same frame mirrored (directions 4-7).

The views are mirrors of one another, not different frames.  A limb touching
one edge of the raw view touches the opposite edge of the mirrored view; that
is the mirror, not a scanline wrap.  Both files bake back to the same stored
bytes, so edit either one (or keep them consistent); the baker rejects a pair
that was edited into two different frames.  VMON uses `MNx`/`MRx`, VMPL uses
`MPx`/`PRx`.

Intro pages use their own 6-bit page palette.  A page word's channels are
4-bit nibbles that `set_vga_palette` shifts left by three, and the DAC keeps
the low six bits; the packer scales that to 8 bits.  ETITLE2..5 inherit
ETITLE1's palette because the intro only programs the DAC for ETITLE1 and
VTITLE6, so those four PNGs use ETITLE1's palette while their files keep the
unused per-page palette.

## Baking

```
python tools/assetpack.py bake --assets assets --out tmp/baked
```

`tmp/baked` then contains the release files.  Unedited groups are byte-identical
to `game/`; edited sprite banks change only the pixels represented by the
edited PNG.  `python tools/assetpack.py verify` performs an unpack/bake cycle
and reports byte equality for every release file.

## Limitations

* Intro pages (`INTRO/`) are recompressed on bake.  The recompressor is
  compatible with the intro's decoder; the reserved word and trailing bytes are
  preserved.  For ETITLE1 and VTITLE6 the page palette is taken from the PNG
  when it differs; ETITLE2..5 keep their stored (unused) palette because the
  game inherits ETITLE1's active palette.  A page that recompresses over the
  intro's 16,000-byte read buffer is refused.
* `SFX/*.WAV` are approximations rendered from the PC-speaker sequence bank in
  `GAME.EXE`; the Adlib/MT-32 sequences live inside `SOUND.DRV`/`ADLIB.ADV`/
  `MT32MPU.ADV` and are preserved as raw files.  `SFX/*.BIN` can be baked back
  into the fixed bank inside `GAME.EXE`.
* `TEXT/*.TXT` are raw CP437 bytes, not UTF-8.  Use a CP437-aware editor.
* `RAW/` is copied verbatim; it holds MAPED, savegames, sound drivers and the
  programs needed to run the baked tree.
"""


def write_asset_readme(outroot: str) -> None:
    open(os.path.join(outroot, "README.md"), "w", encoding="utf-8").write(
        ASSET_README)
    groups = []
    for d in sorted(os.listdir(outroot)):
        p = os.path.join(outroot, d, "manifest.json")
        if os.path.isdir(os.path.join(outroot, d)) and os.path.exists(p):
            m = json.load(open(p))
            groups.append({"directory": d, "format": m.get("format"),
                           "source": m.get("source")})
    json.dump({"format": "shadow-sorcerer-assets", "groups": groups},
              open(os.path.join(outroot, "INDEX.json"), "w"), indent=1)


def cmd_unpack(args):
    outroot = args.out
    os.makedirs(outroot, exist_ok=True)
    count = 0
    for name, (prefix, layout, kind, limit) in SPRITE_BANKS.items():
        path = os.path.join(args.game, name)
        if not os.path.exists(path):
            continue
        data = open(path, "rb").read()
        d = os.path.join(outroot, name.replace(".", "_"))
        unpack_bank(name, data, d, "offset", prefix, layout, limit)
        count += 1
    for name, (prefix, kind) in ANIM_BANKS.items():
        path = os.path.join(args.game, name)
        if not os.path.exists(path):
            continue
        data = open(path, "rb").read()
        d = os.path.join(outroot, name.replace(".", "_"))
        unpack_bank(name, data, d, kind, prefix)
        count += 1
    path = os.path.join(args.game, "VMISSLE.DAT")
    if os.path.exists(path):
        data = open(path, "rb").read()
        unpack_bank("VMISSLE.DAT", data,
                    os.path.join(outroot, "VMISSLE_DAT"), "missile", "VMI")
        count += 1
    unpack_intro(args.game, outroot)
    unpack_text(args.game, outroot)
    unpack_music(args.game, outroot)
    unpack_raw(args.game, outroot)
    unpack_sfx(args.game, outroot)
    write_asset_readme(outroot)
    print("unpacked %d sprite banks plus intro/text/music/raw into %s"
          % (count, outroot))
    return 0


BANK_FORMATS = ("offset-bank", "anim-bank", "missile-bank")


def bake_one(bankdir, outroot, game):
    m = json.load(open(os.path.join(bankdir, "manifest.json")))
    fmt = m.get("format")
    if fmt in BANK_FORMATS:
        return [bake_bank(bankdir, outroot)]
    if fmt == "intro-pages":
        return bake_intro(bankdir, outroot, game)
    if fmt == "text":
        return [bake_text(bankdir, outroot, game)]
    if fmt == "midi-container":
        return [bake_music(bankdir, outroot, game)]
    if fmt == "raw":
        return bake_raw(bankdir, outroot)
    if fmt == "sfx-embedded":
        return bake_sfx(bankdir, outroot, game)
    raise SystemExit("unknown manifest format %r in %s" % (fmt, bankdir))


def cmd_bake(args):
    outroot = args.out
    os.makedirs(outroot, exist_ok=True)
    count = 0
    for d in sorted(os.listdir(args.assets)):
        bankdir = os.path.join(args.assets, d)
        if not os.path.isdir(bankdir) or not os.path.exists(
                os.path.join(bankdir, "manifest.json")):
            continue
        bake_one(bankdir, outroot, args.game)
        count += 1
    print("baked %d asset groups into %s" % (count, outroot))
    return 0


def cmd_verify(args):
    import tempfile
    tmp = tempfile.mkdtemp(prefix="assetpack-")
    cmd_unpack(argparse.Namespace(game=args.game, out=tmp))
    baked = os.path.join(tmp, "baked")
    cmd_bake(argparse.Namespace(assets=tmp, out=baked, game=args.game))
    ok = True
    for dirpath, _, files in os.walk(baked):
        for name in sorted(files):
            out = os.path.join(dirpath, name)
            src = os.path.join(args.game, name)
            if not os.path.exists(src):
                continue
            a, b = open(src, "rb").read(), open(out, "rb").read()
            same = a == b
            print("%-14s %6d -> %6d %s" % (name, len(a), len(b),
                                           "OK" if same else "DIFF"))
            ok &= same
    print("VERIFY:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__)
    sub = ap.add_subparsers(dest="cmd", required=True)
    up = sub.add_parser("unpack")
    up.add_argument("--game", default=os.path.join(ROOT, "game"))
    up.add_argument("--out", default=os.path.join(ROOT, "assets"))
    bk = sub.add_parser("bake")
    bk.add_argument("--assets", default=os.path.join(ROOT, "assets"))
    bk.add_argument("--out", default=os.path.join(ROOT, "tmp", "baked"))
    bk.add_argument("--game", default=os.path.join(ROOT, "game"))
    vf = sub.add_parser("verify")
    vf.add_argument("--game", default=os.path.join(ROOT, "game"))
    vf.add_argument("--assets", default=os.path.join(ROOT, "assets"))
    args = ap.parse_args(argv)
    if args.cmd == "unpack":
        return cmd_unpack(args)
    if args.cmd == "bake":
        return cmd_bake(args)
    return cmd_verify(args)


if __name__ == "__main__":
    sys.exit(main())
