#!/usr/bin/env python3
"""Build the reconstructed INTRO.EXE with the embedded sound driver.

    python tools/build_intro.py

The SSI-authored program is compiled with Microsoft C 5.1.  The John Miles
UltraSound driver is recovered as MASM source (src/intro/sounddrv.asm),
assembled with MASM 5.1, and linked into the same _TEXT segment.

The driver is position-fixed: its absolute data operands and its song bank
assume image offset 0x0AC1.  The build therefore links the program, pads to
0x0AC1, then places the driver, exactly where the release puts it.  A first
link pass measures the program size; the padding object is generated from the
result and the link is repeated with the padding in place.

The dead SHADOW wrappers are linked after the driver so they cannot push it
out of position.
"""
import os
import re
import shutil
import struct
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dosrun  # noqa: E402

REPO = dosrun.REPO
BUILD = r"c:\tmp\bintro"
RSP = "intro.rsp"
DRIVER_START = 0x0AC1
DRIVER_END = 0x2DD0
SOUND_INIT = 0x136C
SOUND_INIT_OFF = SOUND_INIT - DRIVER_START


def _image(path):
    """Return the load image of an MZ executable (drop the header)."""
    data = open(path, "rb").read()
    header = struct.unpack_from("<H", data, 8)[0] * 16
    return data[header:]


def _reset_build_dir():
    d = os.path.join(REPO, "tmp", "bintro")
    if os.path.isdir(d):
        shutil.rmtree(d)
    os.makedirs(d)
    return d


def _map_symbol(map_path, name):
    if not os.path.exists(map_path):
        return None
    for line in open(map_path, errors="replace"):
        m = re.match(r"\s*[0-9A-Fa-f]{4}:([0-9A-Fa-f]{4})\s+_?" + name + r"\s*$",
                     line.rstrip())
        if m:
            return int(m.group(1), 16)
    return None


def build():
    src = os.path.join(REPO, "src", "intro")
    if not os.path.isdir(src):
        print("no src/intro/")
        return 2
    # Generate the recovered driver source from the release, so the checked-in
    # copy cannot drift from the binary it was recovered from.
    subprocess.run([sys.executable, os.path.join(REPO, "tools", "mkdriver_asm.py")],
                   check=True)

    _reset_build_dir()
    rsp_path = os.path.join(REPO, "tmp", "bintro", RSP)

    cmds = [
        r"cd " + BUILD,
        r"copy c:\src\types.h . > nul",
        r"copy c:\src\intro\intro.h . > nul",
        r"copy c:\src\intro\lzhuf.h . > nul",
        r"copy c:\src\intro\shadow.h . > nul",
        r"copy c:\src\intro\sound.h . > nul",
        r"copy c:\src\intro\intro.c . > nul",
        r"copy c:\src\intro\lzhuf.c . > nul",
        r"copy c:\src\intro\shadow.c . > nul",
        r"copy c:\src\intro\shadowx.c . > nul",
        r"copy c:\src\intro\sounddrv.asm . > nul",
        r"cl /AS /Gs /Os /W3 /nologo /c intro.c lzhuf.c shadow.c shadowx.c",
        r"c:\tools\masm51\bin\masm sounddrv.asm;",
    ]
    rc, out = dosrun.run(cmds, timeout=600)

    # pass 1: measure where the driver lands without padding
    objects1 = r"intro+lzhuf+shadow+sounddrv+shadowx"
    with open(rsp_path, "w") as fh:
        fh.write("/stack:2048 " + objects1 + ",intro.exe,intro.map,slibce libh,nul.ddd")
    rc, out1 = dosrun.run(
        [r"cd " + BUILD, r"link /MAP @" + RSP], timeout=600)
    mapp = os.path.join(REPO, "tmp", "bintro", "intro.map")
    init_off = _map_symbol(mapp, "sound_init")
    if init_off is None:
        print(out1[-2000:])
        print("\nINTRO BUILD: FAIL (sound_init not in map)")
        return 1
    driver_off = init_off - SOUND_INIT_OFF
    pad = DRIVER_START - driver_off
    print("program+driver pass 1: sound_init=%04X driver_start=%04X pad=%d"
          % (init_off, driver_off, pad))
    if pad < 0:
        print("INTRO BUILD: FAIL (program is %d bytes too large for the "
              "driver's fixed 0x0AC1 origin)" % (-pad))
        return 1

    # pad object
    pad_asm = os.path.join(REPO, "tmp", "bintro", "pad.asm")
    with open(pad_asm, "w", newline="\r\n") as fh:
        fh.write("_TEXT segment byte public 'CODE'\n")
        if pad:
            fh.write("    db %d dup (0)\n" % pad)
        fh.write("_TEXT ends\nend\n")
    rc, outp = dosrun.run(
        [r"cd " + BUILD, r"c:\tools\masm51\bin\masm pad.asm;"], timeout=300)

    objects2 = r"intro+lzhuf+shadow+pad+sounddrv+shadowx"
    with open(rsp_path, "w") as fh:
        fh.write("/stack:2048 " + objects2 + ",intro.exe,intro.map,slibce libh,nul.ddd")
    rc, out2 = dosrun.run(
        [r"cd " + BUILD, r"link /MAP @" + RSP, r"dir intro.exe"], timeout=600)

    built = os.path.join(REPO, "tmp", "bintro", "intro.exe")
    low = (out + out1 + outp + out2).lower()
    failed = any(t in low for t in
                 ("fatal error", " error c", " error l", "unresolved externals",
                  "cannot find library", "object not found"))
    ok = os.path.exists(built) and not failed

    # verify the driver's public entry points against the release offsets
    if ok:
        checks = {"sound_init": 0x136C, "sound_stop": 0x13DF,
                  "sound_play": 0x1421, "sound_channel_active": 0x21CE}
        for name, want in checks.items():
            got = _map_symbol(mapp, name)
            if got != want:
                print("INTRO BUILD: FAIL (%s at %s, want %04X)"
                      % (name, ("%04X" % got) if got is not None else "missing", want))
                ok = False
                break
    # the whole driver region must match the release byte for byte: its
    # absolute data references, tables and song bank are position-fixed
    if ok:
        want = _image(os.path.join(REPO, "game", "INTRO.EXE"))
        got = _image(built)
        a = want[DRIVER_START:DRIVER_END]
        b = got[DRIVER_START:DRIVER_END]
        if len(a) != len(b):
            print("INTRO BUILD: FAIL (driver region is %d bytes, want %d)"
                  % (len(b), len(a)))
            ok = False
        elif a != b:
            diff = next(i for i in range(len(a)) if a[i] != b[i])
            print("INTRO BUILD: FAIL (driver differs from release at %04X)"
                  % (DRIVER_START + diff))
            ok = False
    if ok:
        shutil.copy(built, os.path.join(REPO, "tmp", "intro.exe"))
        print("INTRO BUILD: PASS (driver at 0x%04X, byte-identical, music linked)"
              % DRIVER_START)
    else:
        print("\nINTRO BUILD: FAIL")
        print(out2[-2000:])
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(build())
