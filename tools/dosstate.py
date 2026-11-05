#!/usr/bin/env python3
"""Capture the original Shadow Sorcerer's game globals for port comparisons.

GAME.EXE keeps all of its state in one 64 KiB DGROUP.  This tool finds that
segment inside a running DOSBox-X (the credits/monster data pins it down),
dumps it, and prints the milestone-relevant fields.  The port's own structures
use the same layouts, so a dump from a matching milestone can be diffed
against it.

Examples:
    # launch the release, skip the intro, and dump once the game starts
    python tools/dosstate.py --out tmp/dosgt/start.bin

    # attach to a DOSBox-X you are playing by hand, at a world-map or site
    # milestone (find the PID in Task Manager)
    python tools/dosstate.py --pid 12345 --out tmp/dosgt/site.bin

The dump is DGROUP as DS:0000..DS:FFFF.  Useful offsets (src/names.h):
    0x13A6 terrain grid      0x23A0 hero slots       0x2968 actor table
    0x2B48 party table       0x2C92 creature grid    0x2260 overworld phase
    0x227A map mode          0x9F2E VMPL bank paras   0xA28C VMON bank paras
"""
import argparse
import ctypes
import ctypes.wintypes as w
import os
import re
import struct
import subprocess
import sys
import time

REPO = os.path.normpath(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))))
DOSBOX = os.environ.get("DOSBOX_X", r"C:\soft\dosbox\dosbox-x.exe")
DEFAULT_OUT = os.path.join(REPO, "tmp", "dosgt", "dgroup.bin")

# g_creature_grid, the 700-byte table at DGROUP 0x2C92.
def creature_pattern():
    src = open(os.path.join(REPO, "src", "actor.c"), encoding="utf-8").read()
    i = src.index("uint8 g_creature_grid[TERRAIN_CELLS] = {")
    j = src.index("};", i)
    body = src[src.index("{", i) + 1:j]
    return bytes(int(x) & 0xFF for x in re.findall(r"\b\d+\b", body))


class MEMORY_BASIC_INFORMATION(ctypes.Structure):
    _fields_ = [("BaseAddress", ctypes.c_void_p),
                ("AllocationBase", ctypes.c_void_p),
                ("AllocationProtect", w.DWORD),
                ("RegionSize", ctypes.c_size_t),
                ("State", w.DWORD),
                ("Protect", w.DWORD),
                ("Type", w.DWORD)]


k32 = ctypes.WinDLL("kernel32", use_last_error=True)
k32.OpenProcess.restype = w.HANDLE
k32.OpenProcess.argtypes = [w.DWORD, w.BOOL, w.DWORD]
k32.ReadProcessMemory.argtypes = [w.HANDLE, ctypes.c_void_p, ctypes.c_void_p,
                                  ctypes.c_size_t, ctypes.POINTER(ctypes.c_size_t)]
k32.VirtualQueryEx.argtypes = [w.HANDLE, ctypes.c_void_p,
                               ctypes.POINTER(MEMORY_BASIC_INFORMATION),
                               ctypes.c_size_t]


def read_mem(ph, addr, size):
    buf = ctypes.create_string_buffer(size)
    got = ctypes.c_size_t()
    if not k32.ReadProcessMemory(ph, ctypes.c_void_p(addr), buf, size,
                                 ctypes.byref(got)):
        return None
    return buf.raw[:got.value]


def iter_regions(ph):
    mbi = MEMORY_BASIC_INFORMATION()
    addr = 0
    while addr < 0x7FFF_FFFF_FFFF:
        if not k32.VirtualQueryEx(ph, ctypes.c_void_p(addr),
                                  ctypes.byref(mbi), ctypes.sizeof(mbi)):
            break
        if mbi.State == 0x1000 and (mbi.Protect & 0xFF) not in (0x01, 0x00):
            yield mbi.BaseAddress or 0, mbi.RegionSize
        addr = (mbi.BaseAddress or 0) + mbi.RegionSize
        if mbi.RegionSize == 0:
            break


def find_dgroup(ph, pattern):
    """Locate the loaded GAME.EXE data segment.

    The credits block sits at DGROUP:0x8D77 and the creature grid at
    DGROUP:0x2C92; matching both pins the real data segment (the host file
    cache and other allocations hold unrelated copies of the same bytes).
    """
    marker = b"           Shadow Sorcerer"
    for base, size in iter_regions(ph):
        data = read_mem(ph, base, size)
        if not data:
            continue
        idx = 0
        while True:
            off = data.find(marker, idx)
            if off < 0:
                break
            idx = off + 1
            if off >= 0x8D77:
                start = off - 0x8D77
                if data[start + 0x2C92:start + 0x2C92 + len(pattern)] == pattern:
                    return base + start
            else:
                dg = base + off - 0x8D77
                if dg >= 0 and read_mem(ph, dg + 0x2C92, len(pattern)) == pattern:
                    return dg
    return None


def describe(mem):
    def u8(off):
        return mem[off]

    def u16(off):
        return struct.unpack_from("<H", mem, off)[0]

    print("overworld phase %u  map mode 0x%02x  clock hour %u"
          % (u16(0x2260), u8(0x227A), u16(0x2286)))
    print("party[0] grid %u,%u  target %u,%u  cooldown %u"
          % (u16(0x2B48 + 2), u16(0x2B48 + 4),
             u16(0x2B48 + 6), u16(0x2B48 + 8), u16(0x2B48 + 10)))
    print("hero slots:", end="")
    for i in range(4):
        print(" %u" % u16(0x23A0 + i * 10), end="")
    print()
    print("actors:", end="")
    for i in range(8):
        print(" %u" % u8(0x2968 + i * 30), end="")
    print()
    print("VMPL paras: %04x %04x %04x %04x"
          % tuple(struct.unpack_from("<4H", mem, 0x9F2E)))
    print("VMON paras: %04x %04x %04x %04x"
          % tuple(struct.unpack_from("<4H", mem, 0xA28C)))


def capture(pid, out):
    ph = k32.OpenProcess(0x0410, False, pid)  # VM_READ | QUERY
    if not ph:
        raise SystemExit("cannot open pid %d (error %d)"
                         % (pid, ctypes.get_last_error()))
    try:
        dg = find_dgroup(ph, creature_pattern())
        if dg is None:
            raise SystemExit("DGROUP not found; is GAME.EXE running?")
        mem = read_mem(ph, dg, 0x10000)
    finally:
        k32.CloseHandle(ph)
    os.makedirs(os.path.dirname(os.path.abspath(out)), exist_ok=True)
    open(out, "wb").write(mem)
    print("DGROUP %08x -> %s" % (dg, out))
    describe(mem)


def default_conf():
    """Write a minimal DOSBox-X config that mounts this repository."""
    path = os.path.join(REPO, "tmp", "dosgt", "dosstate.conf")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    host = REPO.replace("\\", "/")
    with open(path, "w", newline="\r\n") as f:
        f.write("[dosbox]\nmachine=svga_s3\nmemsize=16\n\n")
        f.write("[cpu]\ncore=normal\ncputype=386\ncycles=30000\n\n")
        f.write("[sdl]\nfullscreen=false\noutput=surface\n")
        f.write("windowposition=-3000,-3000\n\n")
        f.write("[midi]\nmididevice=none\n\n[mixer]\nnosound=true\n\n")
        f.write("[autoexec]\n@echo off\nmount c %s\nc:\ncd \\game\n" % host)
        # INTRO.EXE runs first and waits for a key; a few scripted presses
        # get GAME.EXE loaded without needing host input.
        f.write("addkey p3000 space\naddkey p6000 space\n")
        f.write("addkey p9000 space\naddkey p12000 space\n")
        f.write("shadow v a\n")
    return path


def launch_and_capture(out, conf, wait):
    if not os.path.exists(DOSBOX):
        raise SystemExit("DOSBox-X not found: %s (set DOSBOX_X)" % DOSBOX)
    if conf is None:
        conf = default_conf()
    if not os.path.exists(conf):
        raise SystemExit("configuration not found: %s" % conf)
    proc = subprocess.Popen(
        [DOSBOX, "-conf", conf],
        env=dict(os.environ, SDL_AUDIODRIVER="dummy"),
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
        creationflags=0x08000000)  # CREATE_NO_WINDOW
    try:
        deadline = time.time() + wait
        while time.time() < deadline:
            time.sleep(0.5)
            ph = k32.OpenProcess(0x0410, False, proc.pid)
            if not ph:
                continue
            dg = find_dgroup(ph, creature_pattern())
            k32.CloseHandle(ph)
            if dg is not None:
                break
        else:
            raise SystemExit("GAME.EXE did not start within %ss" % wait)
        capture(proc.pid, out)
    finally:
        proc.terminate()
        try:
            proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            proc.kill()


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--pid", type=int, help="attach to this DOSBox-X process")
    ap.add_argument("--out", default=DEFAULT_OUT, help="snapshot path")
    ap.add_argument("--conf", help="DOSBox-X config to launch (default tmp/dosgt/gt.conf)")
    ap.add_argument("--wait", type=float, default=30.0,
                    help="seconds to wait for GAME.EXE to start")
    args = ap.parse_args()
    if args.pid:
        capture(args.pid, args.out)
    else:
        launch_and_capture(args.out, args.conf, args.wait)
    return 0


if __name__ == "__main__":
    sys.exit(main())
