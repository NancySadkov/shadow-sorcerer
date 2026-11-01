#!/usr/bin/env python3
"""Build with the vendored Microsoft C 5.1 chain, under DOSBox-X.

    python tools/build.py            # toolchain smoke test
    python tools/build.py game       # build the reconstructed game (needs src/)

The guest sees only paths inside this repository: the repo is mounted as C:,
so the compiler lives at C:\TOOLS\MSC51 and scratch work in C:\TMP.

This target is a COMPILE-AND-LINK gate. It deliberately does not run the game:
main() really does spin on shadow_poll(), which is INT 0F8h, and with SHADOW.COM
absent that never returns.
"""
import os
import shutil
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dosrun  # noqa: E402

REPO = dosrun.REPO
BUILD = r"c:\tmp\build"
RSP = "link.rsp"

# DOS caps a command line at 127 characters, and COMMAND.COM enforces that on
# batch lines too. Exceeding it does NOT error - the tail is silently dropped, so
# files vanish from a compile and LINK then reports a missing object, or sees a
# mangled library name and falls through to a prompt while still emitting a file.
# Every command built here is kept under this budget.
CMD_BUDGET = 110


def _reset_local_build_dir(name):
    """Wipe and recreate repo/tmp/build so the guest has a clean writable tree."""
    d = os.path.join(REPO, "tmp", "build")
    if os.path.isdir(d):
        shutil.rmtree(d)
    os.makedirs(d)
    return d


def _chunked(compiler_prefix, names, budget=CMD_BUDGET):
    """Split a file list into commands whose command lines stay under the DOS cap.

    Silent truncation is the failure mode being defended against, so this errs
    toward more, shorter commands."""
    cmds, cur, cur_len = [], compiler_prefix, len(compiler_prefix)
    for n in names:
        add = len(n) + 1
        if cur_len + add > budget:
            cmds.append(cur.rstrip())
            cur, cur_len = compiler_prefix, len(compiler_prefix)
        cur += n + " "
        cur_len += add
    if cur.rstrip() != compiler_prefix.rstrip():
        cmds.append(cur.rstrip())
    return cmds


def smoke():
    """Prove the vendored compiler and linker work at all."""
    _reset_local_build_dir("smoke")
    cmds = [
        r"cd " + BUILD,
        r"copy c:\tools\smoke\hello.c . > nul",
        r"cl /AS /W3 /nologo hello.c",
        r"dir hello.*",
        r"hello",
    ]
    rc, out = dosrun.run(cmds, timeout=180)
    ok = ("sum=84" in out) and ("hello.exe" in out.lower())
    print("\nTOOLCHAIN: " + ("PASS" if ok else "FAIL") + "  (expected sum=84)")
    return 0 if ok else 1


def game():
    src = os.path.join(REPO, "src")
    if not os.path.isdir(src):
        print("no src/ yet - nothing to build")
        return 2
    _reset_local_build_dir("game")
    names = sorted(f for f in os.listdir(src) if f.lower().endswith(".c"))
    if not names:
        print("no .c files in src/")
        return 2
    bases = [os.path.splitext(n)[0] for n in names]

    # LINK gets a response file. All five of its fields must be filled or it
    # prompts and swallows the rest of the batch, and enumerating the objects on
    # the command line grows past the DOS cap as src/ grows. Never write `nul` for
    # the libraries field: that IS the NUL device, so LINK reads it as a library
    # and dies with L1102 unexpected end-of-file. A nonexistent *filename* is
    # merely absent, which LINK tolerates. Wildcards were rejected: LINK 5.01 does
    # not expand them, it took `*.OBJ` literally and failed with L1093.
    # LINK 5.01 caps a response line at roughly 150 characters. By the nineteenth
    # source file the enumerated object names reached 159 and LINK died with L1022,
    # so objects are renamed to one-letter aliases immediately before linking.
    # Link ORDER is preserved exactly - which is what the CRT's startup chunk
    # position depends on - and the line now grows by ~2 characters per module
    # instead of ~9, deferring the wall to around forty files. Aliasing rather than
    # shortening the source names keeps src/ readable. Single letters cannot collide
    # with an object on this tree (every stem is >= 3 characters), and a rename whose
    # source is missing fails the link with "object not found", which the gate below
    # already catches.
    # already catches. The fifth field keeps its dotted name `nul.ddd`: unlike the
    # map field, LINK OPENS the module-definitions file for reading, and an
    # extension-less alias there died with L1092 cannot-open-module-definitions.
    codes = [chr(ord("a") + i) if i < 26 else "z%d" % i for i in range(len(bases))]
    rsp_path = os.path.join(REPO, "tmp", "build", "link.rsp")
    with open(rsp_path, "w") as fh:
        fh.write("/stack:2048 " + "+".join(codes) + ",rebuild.exe,rebuild.map,slibce libh,nul.ddd")

    cmds = [
        r"cd " + BUILD,
        r"copy c:\src\*.c . > nul",
        r"copy c:\src\*.h . > nul",
    ]
    # Small model (/AS) to match the original. Chunked for the command-line cap:
    # at thirteen files the single enumerated line reached 137 characters and two
    # sources vanished from the compile without any error being reported.
    cmds += _chunked(r"cl /AS /Gs /Os /W3 /nologo /c ", names)
    cmds += ["ren %s.obj %s.obj" % (b, c) for b, c in zip(bases, codes)]
    cmds += [
        r"link /MAP @" + RSP,
        r"dir rebuild.exe",
    ]
    rc, out = dosrun.run(cmds, timeout=600)

    built = os.path.join(REPO, "tmp", "build", "rebuild.exe")
    # Existence alone is NOT enough. LINK can print "error L2029: Unresolved
    # externals" and still leave an executable on disk, so a gate that only stat()s
    # the file reports PASS on a broken build. Read the log as well.
    low = out.lower()
    failed = any(t in low for t in
                 ("fatal error", "error c", "error l", "unresolved externals",
                  "cannot find library", "object not found"))
    ok = os.path.exists(built) and not failed
    if ok:
        shutil.copy(built, os.path.join(REPO, "tmp", "rebuild.exe"))
    print("\nBUILD: " + ("PASS" if ok else "FAIL"))
    if not ok:
        print(out[-1500:])
    return 0 if ok else 1


if __name__ == "__main__":
    target = sys.argv[1] if len(sys.argv) > 1 else "smoke"
    sys.exit({"smoke": smoke, "game": game}[target]())
