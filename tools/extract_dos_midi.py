#!/usr/bin/env python3
"""Extract the DOS release's music sequences as Standard MIDI Files.

SSI.DAT is a table of 32 little-endian 32-bit offsets followed by Standard
MIDI Files.  The game calls AIL_report_long() with SSI.DAT + offset; sequence
0 is the empty/silence entry and sequences 7 and 8 share the endgame file.

Usage:
    python tools/extract_dos_midi.py --game-dir ../shsorc/game \
        --out assets/dos_music
"""
import argparse
import os
import struct

NAMES = {
    1: "refugee_vote",
    2: "eight_monsters",
    3: "scene_monster",
    4: "random_encounter",
    5: "dragon_attack",
    6: "overworld",
    7: "endgame",
    8: "endgame",
}


def midi_length(d, off):
    if d[off:off + 4] != b"MThd":
        raise ValueError("no MThd at %X" % off)
    hlen = struct.unpack_from(">I", d, off + 4)[0]
    ntrk = struct.unpack_from(">H", d, off + 10)[0]
    p = off + 8 + hlen
    for _ in range(ntrk):
        if d[p:p + 4] != b"MTrk":
            raise ValueError("no MTrk at %X" % p)
        ln = struct.unpack_from(">I", d, p + 4)[0]
        p += 8 + ln
    return p - off


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--game-dir", default=os.path.join("..", "shsorc", "game"))
    ap.add_argument("--out", default=os.path.join("assets", "dos_music"))
    args = ap.parse_args()
    src = os.path.join(args.game_dir, "SSI.DAT")
    d = open(src, "rb").read()
    os.makedirs(args.out, exist_ok=True)
    seen = {}
    for seq in range(32):
        off = struct.unpack_from("<I", d, seq * 4)[0]
        if off == 0 or off in seen:
            continue
        seen[off] = seq
        n = midi_length(d, off)
        name = NAMES.get(seq)
        if name is None:
            name = "seq%02d" % seq
        path = os.path.join(args.out, "%s.mid" % name)
        open(path, "wb").write(d[off:off + n])
        print("%s: sequence %d, offset 0x%X, %d bytes" %
              (path, seq, off, n))


if __name__ == "__main__":
    main()
