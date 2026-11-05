#!/usr/bin/env python3
"""Stage the game files the SDL port actually reads for the browser build.

The port reimplements SHADOW.COM and the sound driver, and uses its own title
art, so the DOS executables (GAME.EXE, INTRO.EXE, SHADOW.COM), the launcher
(SS.BAT), the Adlib/MT-32 drivers (SOUND.DRV, ADLIB.ADV, MT32MPU.ADV,
FAT.ADB, SSI.DAT) and the INTRO title banks (ETITLE*.DAT, VTITLE6.DAT) are not
needed at runtime.  Leaving them out keeps the .data download (and the itch.io
zip) much smaller.

Usage:
    python tools/stage_web_assets.py [--game game] [--out build/web_assets/game]
"""
from __future__ import annotations

import argparse
import os
import shutil


def required_names():
    names = [
        "VCLIPS.DAT", "VMISSLE.DAT", "VOBJPIC.DAT", "VMAPPIC.DAT",
        "TEXT.DAT", "MAPED",
    ]
    # Monster banks: A..Z plus the two digit banks the sprite loader uses for
    # the high-numbered monsters.
    names += ["VMON%d.DAT" % i for i in (0, 1)]
    names += ["VMON%s.DAT" % chr(ord("A") + i) for i in range(26)]
    # Party sprite banks selected by the hero slot state.
    names += ["VMPL%s.DAT" % chr(ord("A") + i) for i in range(16)]
    # Site/event presentation clips.
    names += ["VCIN%d.DAT" % i for i in range(5)]
    return names


def find_ci(root, name):
    """Return the path of name in root, ignoring case (keeps the DOS archive
    readable from Linux without renaming files)."""
    direct = os.path.join(root, name)
    if os.path.exists(direct):
        return direct
    lower = name.lower()
    for entry in os.listdir(root):
        if entry.lower() == lower:
            return os.path.join(root, entry)
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--game", default="game", help="directory with the DOS release files")
    ap.add_argument("--out", default=os.path.join("build", "web_assets", "game"),
                    help="staging directory for --preload-file")
    args = ap.parse_args()

    if not os.path.isdir(args.game):
        ap.error("game directory not found: %s" % args.game)
    if os.path.isdir(args.out):
        shutil.rmtree(args.out)
    os.makedirs(args.out)

    missing = []
    total = 0
    staged = 0
    for name in required_names():
        src = find_ci(args.game, name)
        if src is None:
            missing.append(name)
            continue
        shutil.copy2(src, os.path.join(args.out, name))
        total += os.path.getsize(src)
        staged += 1

    print("staged %d files, %d bytes -> %s" % (staged, total, args.out))
    if missing:
        print("missing: " + ", ".join(missing))
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
