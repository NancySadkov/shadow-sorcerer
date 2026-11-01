#!/usr/bin/env python3
"""Assemble the recovered SHADOW.COM source and prove it is the shipped bytes.

    python tools/build_shadow.py

The source is a single-segment MASM 5.1 file with ORG 100h.  LINK 3.64 has no
/TINY, so the result is an EXE whose load module starts with the 100h ORG gap;
dropping those bytes gives the COM image.  A COM may not be relocated, so a
non-zero relocation count is a hard failure.
"""
import os
import shutil
import struct
import sys

REPO = os.path.normpath(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = os.path.join(REPO, "src", "shadow", "shadow.asm")
ORIG = os.path.join(REPO, "game", "SHADOW.COM")
WORK = os.path.join(REPO, "tmp", "shbld")
OUT = os.path.join(REPO, "tmp", "shadow.com")

sys.path.insert(0, os.path.join(REPO, "tools"))
import dosrun  # noqa: E402


def main():
    if not os.path.exists(SRC):
        print("missing %s" % SRC)
        return 2
    if os.path.isdir(WORK):
        shutil.rmtree(WORK)
    os.makedirs(WORK)
    shutil.copyfile(SRC, os.path.join(WORK, "shadow.asm"))

    rc, out = dosrun.run(
        [
            r"cd c:\tmp\shbld",
            r"c:\tools\masm51\bin\masm shadow.asm;",
            r"c:\tools\masm51\bin\link shadow.obj,shadow.exe,shadow.map;",
        ],
        timeout=300,
        echo=False,
    )

    low = out.lower()
    import re
    bad = (any(t in low for t in ("error l", "fatal error", "unresolved",
                                  "cannot find", "undefined symbol"))
           or re.search(r"[1-9]\d*\s+(warning|severe)\s+errors", low) is not None)
    if bad:
        print(out)
        print("\nSHADOW BUILD: FAIL (assembler/linker error)")
        return 1

    exe = os.path.join(WORK, "shadow.exe")
    if not os.path.exists(exe):
        print(out)
        print("\nSHADOW BUILD: FAIL (no image)")
        return 1

    data = open(exe, "rb").read()
    h = struct.unpack_from("<14H", data, 0)
    if h[0] != 0x5A4D:
        print("SHADOW BUILD: FAIL (not an MZ image)")
        return 1
    if h[3] != 0:
        print("SHADOW BUILD: FAIL (%d relocations; a COM must have none)" % h[3])
        return 1
    image = data[h[4] * 16:][0x100:]
    open(OUT, "wb").write(image)

    orig = open(ORIG, "rb").read()
    if image != orig:
        first = next((i for i, (a, b) in enumerate(zip(image, orig)) if a != b),
                     min(len(image), len(orig)))
        print("SHADOW BUILD: FAIL (image %d bytes, original %d; first diff %04X)"
              % (len(image), len(orig), first))
        return 1

    print("SHADOW BUILD: PASS  (%d bytes, byte-identical, MASM 5.1/LINK 3.64)"
          % len(image))
    return 0


if __name__ == "__main__":
    sys.exit(main())
