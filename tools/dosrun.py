#!/usr/bin/env python3
"""Run DOS commands under the vendored DOSBox-X harness and capture their output.

Only paths inside this repository are ever mounted into the guest.
"""
import os
import subprocess
import sys

REPO = os.path.normpath(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
DOSBOX = os.path.join(REPO, "emulator", "dosbox-x-mod.exe")
TC = r"c:\tools\msc51"
WORK = os.path.join(REPO, "tmp")
LOG = r"c:\tmp\dosrun.log"

ENV = [
    r"set PATH=%s\BIN" % TC,
    r"set INCLUDE=%s\INCLUDE" % TC,
    r"set LIB=%s\LIB" % TC,
    r"set TMP=c:\tmp",
    r"set TEMP=c:\tmp",
]


def _host(p):
    """Host path with forward slashes, which is what DOSBox's mount wants."""
    return p.replace("\\", "/")


def run(lines, timeout=180, extra_mounts=None, echo=True):
    """Run `lines` (DOS commands) in the guest; return (rc, captured_text)."""
    os.makedirs(WORK, exist_ok=True)
    bat = os.path.join(WORK, "dosrun.bat")
    log = os.path.join(WORK, "dosrun.log")
    err = os.path.join(WORK, "dosbox.log")
    for p in (log, err):
        if os.path.exists(p):
            os.remove(p)

    def emit(f, cmd):
        # A command that already redirects is left alone; everything else is
        # appended to the shared log so ordering is preserved. ` < nul` matters:
        # LINK and CL prompt when unhappy, and would otherwise consume the rest
        # of this batch as their answers.
        f.write((cmd if (">" in cmd or cmd.startswith("@"))
                 else cmd + " < nul >> " + LOG) + "\r\n")

    with open(bat, "w", newline="\r\n") as f:
        f.write("@echo off\r\n")
        for ln in ENV:
            f.write(ln + "\r\n")
        emit(f, "echo === begin ===")
        for ln in lines:
            emit(f, ln)
        emit(f, "echo === end ===")

    conf = os.path.join(WORK, "dosrun.conf")
    with open(conf, "w", newline="\r\n") as f:
        f.write("[dosbox]\nmachine=svga_s3\nmemsize=16\n")
        f.write("[cpu]\ncore=normal\ncputype=386\ncycles=30000\n")
        f.write("[sdl]\nfullscreen=false\noutput=surface\n")
        f.write("[midi]\nmididevice=none\n")
        f.write("[mixer]\nnosound=true\n")
        # Optional host-visible side channel. Everything the guest prints goes to
        # stdout only, and the game draws to the framebuffer instead, so a run of
        # the release is otherwise invisible past SHADOW.COM's banner. Setting
        # DOSRUN_LPT=<host file> wires LPT1 to that file, and a guest program can
        # then log through it with INT 17h. Off by default: nothing changes for
        # the build gate.
        lpt = os.environ.get("DOSRUN_LPT")
        par = os.environ.get("DOSRUN_PARALLEL")
        if par:
            f.write("[parallel]\n%s\n" % par)
        elif lpt:
            f.write("[parallel]\nparallel1=file file:%s timeout:1000\n" % _host(lpt))
        f.write("[autoexec]\n@echo off\n")
        f.write("mount c %s\n" % _host(REPO))
        for drive, hostdir in (extra_mounts or []):
            f.write("mount %s %s\n" % (drive, _host(hostdir)))
        f.write("c:\n")
        f.write("if not exist c:\\tmp mkdir c:\\tmp\n")
        f.write("call c:\\tmp\\dosrun.bat\n")
        f.write("exit\n")

    if not os.path.exists(DOSBOX):
        return 2, "DOSBox harness not found at %s" % DOSBOX
    env = dict(os.environ, SDL_AUDIODRIVER="dummy")
    try:
        with open(err, "wb") as ef:
            subprocess.run([DOSBOX, "-conf", _host(conf)], timeout=timeout, env=env,
                           stdout=ef, stderr=subprocess.STDOUT)
    except subprocess.TimeoutExpired:
        return 3, "DOSBox timed out after %ss" % timeout

    out = ""
    if os.path.exists(log):
        out = open(log, "rb").read().decode("cp437", "replace")
    if not out and os.path.exists(err):
        out = "[no guest output]\n" + open(err, "rb").read().decode("cp437", "replace")[-3000:]
    if echo:
        sys.stdout.write(out)
    return 0, out


if __name__ == "__main__":
    rc, _ = run(sys.argv[1:] or ["ver"])
    print("\n[rc=%d]" % rc)
