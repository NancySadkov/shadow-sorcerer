# Microsoft Macro Assembler 5.1 (1988)

`BIN/MASM.EXE` and `BIN/LINK.EXE` are the assembler and overlay linker used to
build `src/shadow/shadow.asm`, the resident `SHADOW.COM` driver.

They were extracted from `doc/masm/Microsoft Macro Assembler 5.1
(02-01-1988) (5.25).7z`:

| File | Version banner | Size |
|---|---|---|
| `MASM.EXE` | `Microsoft (R) Macro Assembler Version 5.10`, `Copyright (C) Microsoft Corp 1981, 1988` | 110,703 |
| `LINK.EXE` | `Microsoft (R) Overlay Linker Version 3.64`, `Copyright (C) Microsoft Corp 1983-1988` | 65,475 |

This is the period-correct pair for the driver: the shipped `SHADOW.COM` was
assembled in 1988-1989, and MASM 5.10 reproduces its instruction encodings
exactly, including the `reg,r/m` direction for two-register ALU operations
(`xor ax,ax` = `33 C0`, not `31 C0`) and the `86h`/`87h` operand order for
`xchg`.

MS C 5.1 is a separate tree (`tools/msc51/`) and is used for `GAME.EXE`.

## Building the driver

```
python tools/build_shadow.py
```

The script runs both tools under the vendored DOSBox-X harness
(`tools/dosrun.py`), then converts the linked EXE into the COM image:

* the source is one `_TEXT` segment with `ORG 100h` (MASM 5.1 has no
  `.MODEL TINY`, and LINK 3.64 has no `/TINY`);
* `link shadow.obj;` produces a stack-less EXE with **no relocations**
  (`e_crlc = 0`), which is what a COM requires;
* the load module's first `100h` bytes are the `ORG` gap and are dropped.

The rebuilt image is byte-identical to `game/SHADOW.COM`.
