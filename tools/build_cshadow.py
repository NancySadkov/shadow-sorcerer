#!/usr/bin/env python3
"""Build the hybrid SHADOW.COM: original MASM shell + migrated C services.

    python tools/build_cshadow.py [--deploy]

The hybrid is a single-segment COM.  The shell is the byte-identical original
source with only its selector table rewritten; unmigrated entries still point
at the release handlers.  C services are MS C 5.1 OMF objects linked after the
shell with the 1988 LINK, which is also what merges the bare assembly entry
with the C code.

--deploy installs the result as tmp/h_origsh/SHADOW.COM next to the pristine
GAME.EXE and writes tmp/h_origsh.conf for tools/playtest.py.
"""
import os
import re
import shutil
import struct
import sys

REPO = os.path.normpath(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = os.path.join(REPO, "src", "shadow")
WORK = os.path.join(REPO, "tmp", "csh")
OUT = os.path.join(REPO, "tmp", "cshadow.com")
RUN = os.path.join(REPO, "tmp", "h_origsh")

sys.path.insert(0, os.path.join(REPO, "tools"))
import dosrun  # noqa: E402
import gen_cshell  # noqa: E402


def _clean_work():
    os.makedirs(WORK, exist_ok=True)
    for name in os.listdir(WORK):
        p = os.path.join(WORK, name)
        if os.path.isfile(p):
            os.remove(p)
    for name in ("cstubs.asm", "ctail.asm", "services.c", "cshadow.h"):
        shutil.copyfile(os.path.join(SRC, name), os.path.join(WORK, name))


def main():
    _clean_work()
    gen_cshell.main(os.path.join(WORK, "cshell.asm"))

    rc, out = dosrun.run(
        [
            r"cd c:\tmp\csh",
            r"c:\tools\masm51\bin\masm cshell.asm;",
            r"c:\tools\masm51\bin\masm cstubs.asm;",
            r"c:\tools\masm51\bin\masm ctail.asm;",
            r"c:\tools\msc51\bin\cl /AS /Gs /Zl /c /Foservices.obj services.c",
            r"c:\tools\masm51\bin\link cshell.obj+cstubs.obj+services.obj+ctail.obj,cshadow.exe,cshadow.map;",
        ],
        timeout=420,
        echo=False,
    )

    low = out.lower()
    bad = (any(t in low for t in ("error c", "error l", "fatal error",
                                  "unresolved", "cannot find",
                                  "symbol defined more than once",
                                  "undefined symbol"))
           or re.search(r"[1-9]\d*\s+severe\s+errors", low) is not None)
    exe = os.path.join(WORK, "cshadow.exe")
    if bad or not os.path.exists(exe):
        print(out)
        print("\nCSHADOW BUILD: FAIL")
        return 1

    data = open(exe, "rb").read()
    h = struct.unpack_from("<14H", data, 0)
    if h[0] != 0x5A4D:
        print("CSHADOW BUILD: FAIL (not an MZ image)")
        return 1
    if h[3] != 0:
        print("CSHADOW BUILD: FAIL (%d relocations; a COM must have none)" % h[3])
        return 1
    image = data[h[4] * 16:][0x100:]
    open(OUT, "wb").write(image)
    print("CSHADOW BUILD: OK  (%d bytes, 0 relocations)" % len(image))

    if "--deploy" in sys.argv:
        deploy(image)
    return 0


def deploy(image):
    os.makedirs(RUN, exist_ok=True)
    for name in os.listdir(os.path.join(REPO, "game")):
        src = os.path.join(REPO, "game", name)
        if os.path.isfile(src):
            shutil.copyfile(src, os.path.join(RUN, name))
    shutil.copyfile(OUT, os.path.join(RUN, "SHADOW.COM"))
    text = ("[dosbox]\nmachine = svga_s3\nmemsize = 4\n\n"
            "[cpu]\ncore = normal\ncputype = 386\ncycles = max\n\n"
            "[sdl]\nfullscreen = false\noutput = surface\n\n"
            "[midi]\nmididevice = none\n\n"
            "[mixer]\nnosound = true\n\n"
            "[autoexec]\n@echo off\nmount c C:/Users/nangl/d/code/shsorc\n"
            "c:\ncd \\tmp\\h_origsh\n")
    open(os.path.join(REPO, "tmp", "h_origsh.conf"), "w",
         newline="\r\n").write(text)
    print("deployed tmp/h_origsh (pristine GAME.EXE + hybrid driver)")


if __name__ == "__main__":
    sys.exit(main())
