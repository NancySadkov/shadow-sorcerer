#!/usr/bin/env python3
"""LZHUF decoder compatible with the intro's ETITLE/VTITLE files.

The constants and the adaptive tree are those recovered in src/intro/lzhuf.c:
N=4096, F=60, N_CHAR=314, T=627.  The 256-byte position-code tables are read
from that source, so this port cannot silently diverge from the reconstructed
decoder.
"""
import os
import re
import struct

ROOT = os.path.normpath(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SOURCE = os.path.join(ROOT, "src", "intro", "lzhuf.c")

N = 4096
F = 60
THRESHOLD = 2
N_CHAR = 256 - THRESHOLD + F
T = N_CHAR * 2 - 1
R = T - 1
MAX_FREQ = 0x8000


def _table(name):
    text = open(SOURCE, encoding="utf-8", errors="replace").read()
    m = re.search(r"%s\[256\]\s*=\s*\{(.*?)\};" % re.escape(name), text, re.S)
    if not m:
        raise SystemExit("cannot find %s in %s" % (name, SOURCE))
    vals = [int(x, 16) for x in re.findall(r"0x([0-9a-fA-F]{2})", m.group(1))]
    if len(vals) != 256:
        raise SystemExit("%s has %d entries, want 256" % (name, len(vals)))
    return vals


D_CODE = _table("g_lz_d_code")
D_LEN = _table("g_lz_d_len")

# Encoder-side upper-6-bit position tables from the reference LZHUF.C.
P_LEN = (
    0x03, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
)
P_CODE = (
    0x00, 0x20, 0x30, 0x40, 0x50, 0x58, 0x60, 0x68,
    0x70, 0x78, 0x80, 0x88, 0x90, 0x94, 0x98, 0x9C,
    0xA0, 0xA4, 0xA8, 0xAC, 0xB0, 0xB4, 0xB8, 0xBC,
    0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
    0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE,
    0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
)


class _Reader:
    def __init__(self, data):
        self.data = data
        self.pos = 0
        self.bitbuf = 0
        self.bitcnt = 0

    def getbit(self):
        while self.bitcnt <= 8:
            if self.pos >= len(self.data):
                byte = 0
            else:
                byte = self.data[self.pos]
                self.pos += 1
            self.bitbuf = (self.bitbuf | (byte << (8 - self.bitcnt))) & 0xFFFF
            self.bitcnt += 8
        v = self.bitbuf
        self.bitbuf = (self.bitbuf << 1) & 0xFFFF
        self.bitcnt -= 1
        return 1 if (v & 0x8000) else 0

    def getbyte(self):
        while self.bitcnt <= 8:
            if self.pos >= len(self.data):
                byte = 0
            else:
                byte = self.data[self.pos]
                self.pos += 1
            self.bitbuf = (self.bitbuf | (byte << (8 - self.bitcnt))) & 0xFFFF
            self.bitcnt += 8
        v = self.bitbuf
        self.bitbuf = (self.bitbuf << 8) & 0xFFFF
        self.bitcnt -= 8
        return (v >> 8) & 0xFF


def decode(data: bytes) -> bytes:
    """Decode one intro page.  data starts with the 32-bit output size."""
    size = struct.unpack_from("<I", data, 0)[0]
    if size == 0:
        return b""
    rd = _Reader(data[4:])
    freq = [0] * (T + 1)
    son = [0] * T
    prnt = [0] * (T + N_CHAR)

    for i in range(N_CHAR):
        freq[i] = 1
        son[i] = i + T
        prnt[i + T] = i
    i, j = 0, N_CHAR
    while j <= R:
        freq[j] = freq[i] + freq[i + 1]
        son[j] = i
        prnt[i] = prnt[i + 1] = j
        i += 2
        j += 1
    freq[T] = 0xFFFF
    prnt[R] = 0

    text = bytearray(N)
    text[:N - F] = b" " * (N - F)
    r = N - F

    def reconst():
        nonlocal freq, son, prnt
        j = 0
        for i in range(T):
            if son[i] >= T:
                freq[j] = (freq[i] + 1) >> 1
                son[j] = son[i]
                j += 1
        i = 0
        for j in range(N_CHAR, T):
            f = freq[i] + freq[i + 1]
            freq[j] = f
            k = j - 1
            while freq[k] > f:
                k -= 1
            k += 1
            for m in range(j, k, -1):
                freq[m] = freq[m - 1]
                son[m] = son[m - 1]
            freq[k] = f
            son[k] = i
            prnt[i] = prnt[i + 1] = j
            i += 2
        for i in range(T):
            k = son[i]
            if k < T:
                prnt[k] = prnt[k + 1] = i
            else:
                prnt[k] = i
        prnt[R] = 0

    def update(sym):
        nonlocal freq, son, prnt
        if freq[R] == MAX_FREQ:
            reconst()
        c = prnt[sym + T]
        while True:
            k = freq[c] + 1
            freq[c] = k
            l = c + 1
            if k > freq[l]:
                while k > freq[l]:
                    l += 1
                l -= 1
                freq[c] = freq[l]
                freq[l] = k
                i = son[c]
                prnt[i] = l
                if i < T:
                    prnt[i + 1] = l
                j = son[l]
                son[l] = i
                prnt[j] = c
                if j < T:
                    prnt[j + 1] = c
                son[c] = j
                c = l
            c = prnt[c]
            if c == 0:
                break

    def decode_char():
        c = son[R]
        while c < T:
            c = son[c + rd.getbit()]
        c -= T
        update(c)
        return c

    def decode_position():
        i = rd.getbyte()
        high = D_CODE[i] << 6
        bits = D_LEN[i] - 2
        acc = i
        while bits > 0:
            acc = ((acc << 1) + rd.getbit()) & 0xFFFF
            bits -= 1
        return (high | (acc & 0x3F)) & 0xFFFF

    out = bytearray()
    while len(out) < size:
        c = decode_char()
        if c < 256:
            out.append(c)
            text[r] = c
            r = (r + 1) & (N - 1)
        else:
            pos = (r - decode_position() - 1) & (N - 1)
            length = c - 255 + THRESHOLD
            for k in range(length):
                ch = text[(pos + k) & (N - 1)]
                out.append(ch)
                text[r] = ch
                r = (r + 1) & (N - 1)
    return bytes(out[:size])


def encode(data: bytes) -> bytes:
    """Compatible LZHUF encoder (experimental).

    The Huffman code for a symbol is found by walking the same tree the decoder
    walks from ``son[R]``, rather than by following the leaf's parent chain:
    the parent of an odd leaf is not its tree parent in this LZHUF variant.
    """
    size = len(data)
    freq = [0] * (T + 1)
    son = [0] * T
    prnt = [0] * (T + N_CHAR)

    for i in range(N_CHAR):
        freq[i] = 1
        son[i] = i + T
        prnt[i + T] = i
    i, j = 0, N_CHAR
    while j <= R:
        freq[j] = freq[i] + freq[i + 1]
        son[j] = i
        prnt[i] = prnt[i + 1] = j
        i += 2
        j += 1
    freq[T] = 0xFFFF
    prnt[R] = 0

    def reconst():
        j = 0
        for i in range(T):
            if son[i] >= T:
                freq[j] = (freq[i] + 1) >> 1
                son[j] = son[i]
                j += 1
        i = 0
        for j in range(N_CHAR, T):
            f = freq[i] + freq[i + 1]
            freq[j] = f
            k = j - 1
            while freq[k] > f:
                k -= 1
            k += 1
            for m in range(j, k, -1):
                freq[m] = freq[m - 1]
                son[m] = son[m - 1]
            freq[k] = f
            son[k] = i
            prnt[i] = prnt[i + 1] = j
            i += 2
        for i in range(T):
            k = son[i]
            if k < T:
                prnt[k] = prnt[k + 1] = i
            else:
                prnt[k] = i
        prnt[R] = 0

    def update(sym):
        if freq[R] == MAX_FREQ:
            reconst()
        c = prnt[sym + T]
        while True:
            k = freq[c] + 1
            freq[c] = k
            l = c + 1
            if k > freq[l]:
                while k > freq[l]:
                    l += 1
                l -= 1
                freq[c] = freq[l]
                freq[l] = k
                i = son[c]
                prnt[i] = l
                if i < T:
                    prnt[i + 1] = l
                j = son[l]
                son[l] = i
                prnt[j] = c
                if j < T:
                    prnt[j + 1] = c
                son[c] = j
                c = l
            c = prnt[c]
            if c == 0:
                break

    class Writer:
        def __init__(self):
            self.buf = 0
            self.cnt = 0
            self.out = bytearray()

        def bit(self, b):
            self.buf = ((self.buf << 1) | (b & 1)) & 0xFFFF
            self.cnt += 1
            if self.cnt == 8:
                self.out.append(self.buf & 0xFF)
                self.buf = 0
                self.cnt = 0

        def byte(self, b):
            for i in range(7, -1, -1):
                self.bit((b >> i) & 1)

        def flush(self):
            while self.cnt:
                self.bit(0)

    wr = Writer()

    def putcode(l, c):
        for b in range(15, 15 - l, -1):
            wr.bit((c >> b) & 1)

    def enc_char(c):
        # Reference EncodeChar: walk the parent chain to R, using the
        # node-address parity as the bit.  R itself is not coded.
        i = 0
        j = 0
        k = prnt[c + T]
        while True:
            i >>= 1
            if k & 1:
                i += 0x8000
            j += 1
            k = prnt[k]
            if k == R:
                break
        putcode(j, i & 0xFFFF)
        update(c)

    def enc_pos(pos):
        i = (pos >> 6) & 0x3F
        putcode(P_LEN[i], (P_CODE[i] << 8) & 0xFFFF)
        putcode(6, (pos & 0x3F) << 10)

    history = {}
    ring = bytearray(N)
    ring[:N - F] = b" " * (N - F)
    r = N - F
    i = 0

    def add(pos):
        if pos + 2 < size:
            key = data[pos:pos + 3]
            lst = history.setdefault(key, [])
            lst.append(pos)
            if len(lst) > 64:
                del lst[0]

    while i < size:
        best_pos, best_len = -1, 0
        if i + 2 < size:
            key = data[i:i + 3]
            for cand in reversed(history.get(key, [])):
                if i - cand > N - 1:
                    break
                limit = min(F, size - i, i - cand)
                if limit < 3:
                    continue
                ln = 3
                while ln < limit and data[cand + ln] == data[i + ln]:
                    ln += 1
                if ln > best_len:
                    best_len, best_pos = ln, cand
                    if ln == F:
                        break
        if best_len >= 3:
            enc_char(best_len + 253)
            enc_pos((i - best_pos - 1) & (N - 1))
            for k in range(best_len):
                ring[r] = data[i + k]
                r = (r + 1) & (N - 1)
                add(i + k)
            i += best_len
        else:
            enc_char(data[i])
            ring[r] = data[i]
            r = (r + 1) & (N - 1)
            add(i)
            i += 1

    wr.flush()
    return struct.pack("<I", size) + bytes(wr.out)
