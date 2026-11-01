; Shadow Sorcerer
; License: Public Domain (CC0)
; Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)

_TEXT   SEGMENT BYTE PUBLIC 'CODE'
        ASSUME  CS:_TEXT, DS:_TEXT, ES:_TEXT, SS:_TEXT
        ORG     100h

; CS:0100
L0100:
        jmp L0AFF
; CS:0103 CS:0AFE

        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0ah, 0dh, 45h, 58h
        DB      45h, 43h, 20h, 20h, 56h, 33h, 2eh, 30h, 30h, 20h, 28h, 45h, 47h, 41h, 20h, 31h
        DB      36h, 20h, 43h, 6fh, 6ch, 29h, 0ah, 0dh, 24h, 43h, 6fh, 70h, 79h, 72h, 69h, 67h
        DB      68h, 74h, 20h, 28h, 63h, 29h, 20h, 31h, 39h, 38h, 38h, 2dh, 31h, 39h, 38h, 39h
        DB      0ah, 0dh, 43h, 6ch, 69h, 70h, 70h, 65h, 72h, 20h, 43h, 6fh, 6dh, 70h, 75h, 74h
        DB      65h, 72h, 20h, 50h, 72h, 6fh, 64h, 75h, 63h, 74h, 73h, 0ah, 0dh, 33h, 34h, 20h
        DB      54h, 68h, 65h, 20h, 48h, 75h, 72h, 73h, 74h, 0ah, 0dh, 4dh, 6fh, 73h, 65h, 6ch
        DB      65h, 79h, 20h, 20h, 42h, 69h, 72h, 6dh, 69h, 6eh, 67h, 68h, 61h, 6dh, 0ah, 0dh
        DB      0ah, 0dh, 4ch, 6fh, 61h, 64h, 69h, 6eh, 67h, 20h, 53h, 68h, 61h, 64h, 6fh, 77h
        DB      0ah, 0dh, 24h, 0ah, 0dh, 0ah, 0dh, 2dh, 50h, 72h, 6fh, 67h, 72h, 61h, 6dh, 20h
        DB      54h, 65h, 72h, 6dh, 69h, 6eh, 61h, 74h, 69h, 6fh, 6eh, 2dh, 0ah, 0dh, 24h, 2dh
        DB      4dh, 61h, 63h, 68h, 69h, 6eh, 65h, 20h, 48h, 61h, 6ch, 74h, 65h, 64h, 2dh, 0ah
        DB      0dh, 24h, 69h, 6eh, 74h, 72h, 6fh, 2eh, 65h, 78h, 65h, 0h, 67h, 61h, 6dh, 65h
        DB      2eh, 65h, 78h, 65h, 0h, 6h, 20h, 20h, 20h, 20h, 20h, 20h, 0h, 0h, 0h, 0h
        DB      0h, 6dh, 65h, 6dh, 6fh, 72h, 79h, 20h, 61h, 72h, 65h, 6eh, 61h, 20h, 74h, 72h
        DB      61h, 73h, 68h, 65h, 64h, 0dh, 0ah, 24h, 4eh, 6fh, 74h, 20h, 65h, 6eh, 6fh, 75h
        DB      67h, 68h, 20h, 6dh, 65h, 6dh, 6fh, 72h, 79h, 20h, 74h, 6fh, 20h, 6ch, 6fh, 61h
        DB      64h, 20h, 70h, 72h, 6fh, 67h, 72h, 61h, 6dh, 2eh, 0dh, 0ah, 24h, 41h, 73h, 73h
        DB      69h, 67h, 6eh, 65h, 64h, 20h, 70h, 72h, 6fh, 67h, 72h, 61h, 6dh, 20h, 69h, 73h
        DB      20h, 6eh, 6fh, 74h, 20h, 6fh, 66h, 20h, 65h, 78h, 65h, 63h, 75h, 74h, 61h, 62h
        DB      6ch, 65h, 20h, 66h, 6fh, 72h, 6dh, 61h, 74h, 0dh, 0ah, 24h, 90h, 0h, 0h, 0a8h
        DB      2h, 0h, 0h, 5ch, 0h, 0h, 0h, 6ch, 0h, 0h, 0h, 0h, 0h, 0h, 90h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0a0h, 0h, 0a2h, 0h, 0a0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      90h, 0h, 0h, 0h, 0h, 0h, 0h, 63h, 6ch, 6fh, 63h, 6bh, 2dh, 3eh, 63h, 3h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 1h, 0h, 1h, 0h, 1h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0a0h, 0h, 64h, 0h, 0h, 0h, 0h, 0h, 0a0h
        DB      0h, 64h, 0h, 0h, 0h, 0h, 90h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      1h, 3h, 7h, 0fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh
        DB      1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh
        DB      1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1fh, 1eh, 1ch, 18h, 10h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      0h, 0h, 0h, 30h, 78h, 78h, 30h, 30h, 0h, 30h, 0h, 6ch, 6ch, 6ch, 0h, 0h
        DB      0h, 0h, 0h, 0h, 70h, 57h, 74h, 0h, 3fh, 20h, 24h, 0h, 0eh, 0eah, 2eh, 0h
        DB      0fch, 4h, 24h, 24h, 20h, 3fh, 0h, 74h, 57h, 70h, 0h, 24h, 4h, 0fch, 0h, 2eh
        DB      0eah, 0eh, 0h, 60h, 60h, 0c0h, 0h, 0h, 0h, 0h, 0h, 18h, 30h, 60h, 60h, 60h
        DB      30h, 18h, 0h, 60h, 30h, 18h, 18h, 18h, 30h, 60h, 0h, 0h, 7eh, 7eh, 7eh, 7eh
        DB      7eh, 7eh, 0h, 0h, 30h, 30h, 0fch, 30h, 30h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      30h, 30h, 60h, 0h, 0h, 0h, 0fch, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0h
        DB      30h, 30h, 0h, 6h, 0ch, 18h, 30h, 60h, 0c0h, 80h, 0h, 0h, 3ch, 6eh, 0d6h, 0d6h
        DB      0ech, 78h, 0h, 30h, 0f0h, 30h, 30h, 30h, 34h, 0fch, 0h, 0h, 7ch, 0c6h, 0c6h, 0ch
        DB      38h, 0feh, 0h, 0feh, 0ch, 18h, 3ch, 6h, 0c6h, 7ch, 0h, 1ch, 3ch, 2ch, 6ch, 4ch
        DB      0feh, 0ch, 0h, 3eh, 6ch, 0c0h, 0fch, 0eh, 0cch, 78h, 0h, 6h, 1eh, 70h, 0deh, 0c6h
        DB      0eeh, 7ch, 0h, 0feh, 0ch, 1ch, 18h, 38h, 30h, 30h, 0h, 3ch, 7eh, 0c6h, 7ch, 0c6h
        DB      0eeh, 7ch, 0h, 7ch, 0ceh, 86h, 0f6h, 66h, 1ch, 78h, 0h, 0h, 30h, 30h, 0h, 0h
        DB      30h, 30h, 0h, 0h, 30h, 30h, 0h, 0h, 30h, 30h, 60h, 18h, 30h, 60h, 0c0h, 60h
        DB      30h, 18h, 0h, 0h, 0h, 0fch, 0h, 0h, 0fch, 0h, 0h, 60h, 30h, 18h, 0ch, 18h
        DB      30h, 60h, 0h, 78h, 0cch, 0ch, 18h, 30h, 0h, 30h, 0h, 7ch, 0c6h, 0deh, 0deh, 0dch
        DB      0c0h, 7ch, 0h, 38h, 7ch, 6eh, 0c6h, 0ceh, 0f6h, 66h, 0h, 0dch, 0ech, 0c8h, 0fch, 0c6h
        DB      0ceh, 0fch, 0h, 3eh, 64h, 0c0h, 0c0h, 0c0h, 0c2h, 7eh, 0h, 0f8h, 6ch, 66h, 66h, 0c6h
        DB      0ceh, 0fch, 0h, 7eh, 64h, 60h, 0fch, 0c0h, 0c2h, 0feh, 0h, 7ch, 66h, 60h, 0f0h, 6ch
        DB      60h, 60h, 0h, 3eh, 64h, 0c0h, 0ceh, 0deh, 0c6h, 7eh, 0h, 0c6h, 0c6h, 0deh, 0e6h, 0c6h
        DB      0ceh, 0dch, 0h, 78h, 30h, 30h, 30h, 30h, 30h, 78h, 0h, 0feh, 6h, 6h, 6h, 0c6h
        DB      0c6h, 7ch, 0h, 0c6h, 0cch, 0d8h, 0f0h, 0f8h, 0dch, 0ceh, 0h, 0c0h, 0c0h, 0c0h, 0c0h, 0c0h
        DB      0c4h, 0fch, 0h, 0eeh, 0feh, 0d6h, 0c6h, 0c6h, 0c6h, 0c6h, 0h, 0ech, 0eeh, 0f6h, 0d6h, 0deh
        DB      0ceh, 0dch, 0h, 3ch, 6eh, 0c6h, 0c6h, 0c6h, 0ech, 78h, 0h, 0dch, 0e6h, 0c6h, 0c6h, 0e6h
        DB      0dch, 0c0h, 0h, 3ch, 6eh, 0c6h, 0c6h, 0d6h, 0ech, 7eh, 0h, 0dch, 0e6h, 0c6h, 0c6h, 0f8h
        DB      0cch, 0c6h, 0h, 7eh, 0e2h, 78h, 3ch, 0eh, 8eh, 0fch, 0h, 0feh, 70h, 0c0h, 0c0h, 0c0h
        DB      0e6h, 7ch, 0h, 0f6h, 66h, 0c6h, 0c6h, 0c6h, 0ceh, 76h, 0h, 0ech, 66h, 0c6h, 0c6h, 0c6h
        DB      0cch, 78h, 0h, 0e6h, 62h, 0cah, 0cah, 0dah, 7eh, 24h, 0h, 0ceh, 66h, 2ch, 38h, 6ch
        DB      6ch, 0ceh, 0h, 0e6h, 66h, 6ch, 2ch, 18h, 18h, 30h, 0h, 0feh, 8ch, 8h, 18h, 30h
        DB      60h, 0feh, 0h, 0h, 1h, 1h, 1h, 1h, 1h, 1h, 0h, 0h, 0ffh, 0h, 0h, 0h
        DB      0h, 0ffh, 0h, 0h, 80h, 80h, 80h, 80h, 80h, 80h, 0h, 0h, 0h, 0f0h, 0f0h, 0f0h
        DB      0f0h, 0h, 0h, 0h, 0h, 0ffh, 0ffh, 0ffh, 0ffh, 0h, 0h, 30h, 30h, 18h, 0h, 0h
        DB      0h, 0h, 0h, 0h, 3ah, 6eh, 0c6h, 0c6h, 0ceh, 76h, 0h, 0c0h, 0c0h, 0c0h, 0fch, 0c6h
        DB      0ceh, 0fch, 0h, 0h, 3eh, 64h, 0c0h, 0c0h, 0c2h, 7eh, 0h, 0f8h, 1ch, 6eh, 0c6h, 0c6h
        DB      0ceh, 7ch, 0h, 0h, 3eh, 64h, 0feh, 0c0h, 0c2h, 7eh, 0h, 3ch, 66h, 60h, 60h, 0fch
        DB      60h, 60h, 0h, 0h, 3eh, 66h, 0c6h, 76h, 86h, 0fch, 0h, 0c0h, 0dch, 0e6h, 0c6h, 0c6h
        DB      0cch, 0deh, 0h, 30h, 0h, 70h, 30h, 30h, 30h, 30h, 0h, 6h, 0h, 6h, 6h, 0c6h
        DB      0c6h, 7ch, 0h, 0c0h, 0cch, 0d8h, 0f0h, 0f8h, 0dch, 0ceh, 0h, 30h, 30h, 60h, 60h, 0c0h
        DB      0c4h, 78h, 0h, 0h, 0ach, 0d6h, 0d6h, 0d6h, 0d4h, 0d6h, 0h, 0h, 0dch, 0e6h, 0c6h, 0c6h
        DB      0cch, 0deh, 0h, 0h, 3ch, 6eh, 0c6h, 0c6h, 0ech, 78h, 0h, 0h, 0dch, 0e6h, 0c6h, 0e6h
        DB      0dch, 0c0h, 0h, 0h, 3ch, 6eh, 0c6h, 0c6h, 0ech, 7eh, 0h, 0h, 0dch, 0e6h, 0c0h, 0c0h
        DB      0c0h, 0c0h, 0h, 0h, 7eh, 0e2h, 78h, 3ch, 8eh, 0fch, 0h, 60h, 0fch, 60h, 0c0h, 0c0h
        DB      0e6h, 7ch, 0h, 0h, 0f6h, 66h, 0c6h, 0c6h, 0ceh, 76h, 0h, 0h, 0ech, 66h, 0c6h, 0c6h
        DB      0cch, 78h, 0h, 0h, 0e6h, 62h, 0cah, 0cah, 6eh, 34h, 0h, 0h, 76h, 0bch, 38h, 38h
        DB      6ch, 0ceh, 0h, 0h, 0e6h, 66h, 6ch, 2ch, 98h, 70h, 0h, 0h, 0feh, 8ch, 18h, 30h
        DB      60h, 0feh, 0h, 1ch, 30h, 30h, 0e0h, 30h, 30h, 1ch, 0h, 24h, 24h, 24h, 10h, 8h
        DB      24h, 24h, 24h, 0e0h, 30h, 30h, 1ch, 30h, 30h, 0e0h, 0h, 0h, 0h, 0e7h, 10h, 8h
        DB      0e7h, 0h, 0h, 0h, 10h, 38h, 6ch, 0c6h, 0c6h, 0feh, 0h, 78h, 0cch, 0c0h, 0cch, 78h
        DB      18h, 0ch, 78h, 0h, 0cch, 0h, 0cch, 0cch, 0cch, 7eh, 0h, 1ch, 0h, 78h, 0cch, 0fch
        DB      0c0h, 78h, 0h, 7eh, 0c3h, 3ch, 6h, 3eh, 66h, 3fh, 0h, 0cch, 0h, 78h, 0ch, 7ch
        DB      0cch, 7eh, 0h, 0e0h, 0h, 78h, 0ch, 7ch, 0cch, 7eh, 0h, 30h, 30h, 78h, 0ch, 7ch
        DB      0cch, 7eh, 0h, 0h, 0h, 78h, 0c0h, 0c0h, 78h, 0ch, 38h, 7eh, 0c3h, 3ch, 66h, 7eh
        DB      60h, 3ch, 0h, 0cch, 0h, 78h, 0cch, 0fch, 0c0h, 78h, 0h, 0e0h, 0h, 78h, 0cch, 0fch
        DB      0c0h, 78h, 0h, 0cch, 0h, 70h, 30h, 30h, 30h, 78h, 0h, 7ch, 0c6h, 38h, 18h, 18h
        DB      18h, 3ch, 0h, 0e0h, 0h, 70h, 30h, 30h, 30h, 78h, 0h, 0c6h, 38h, 6ch, 0c6h, 0feh
        DB      0c6h, 0c6h, 0h, 30h, 30h, 0h, 78h, 0cch, 0fch, 0cch, 0h, 1ch, 0h, 0fch, 60h, 78h
        DB      60h, 0fch, 0h, 0h, 0h, 7fh, 0ch, 7fh, 0cch, 7fh, 0h, 3eh, 6ch, 0cch, 0feh, 0cch
        DB      0cch, 0ceh, 0h, 78h, 0cch, 0h, 78h, 0cch, 0cch, 78h, 0h, 0h, 0cch, 0h, 78h, 0cch
        DB      0cch, 78h, 0h, 0h, 0e0h, 0h, 78h, 0cch, 0cch, 78h, 0h, 78h, 0cch, 0h, 0cch, 0cch
        DB      0cch, 7eh, 0h, 0h, 0e0h, 0h, 0cch, 0cch, 0cch, 7eh, 0h, 0h, 0cch, 0h, 0cch, 0cch
        DB      7ch, 0ch, 0f8h, 0c3h, 18h, 3ch, 66h, 66h, 3ch, 18h, 0h, 0cch, 0h, 0cch, 0cch, 0cch
        DB      0cch, 78h, 0h, 18h, 18h, 7eh, 0c0h, 0c0h, 7eh, 18h, 18h, 38h, 6ch, 60h, 0f0h, 60h
        DB      0e6h, 0fch, 0h, 0cch, 0cch, 78h, 0fch, 30h, 0fch, 30h, 30h, 0f8h, 0cch, 0cch, 0fah, 0c6h
        DB      0cfh, 0c6h, 0c7h, 0eh, 1bh, 18h, 3ch, 18h, 18h, 0d8h, 70h, 1ch, 0h, 78h, 0ch, 7ch
        DB      0cch, 7eh, 0h, 38h, 0h, 70h, 30h, 30h, 30h, 78h, 0h, 0h, 1ch, 0h, 78h, 0cch
        DB      0cch, 78h, 0h, 0h, 1ch, 0h, 0cch, 0cch, 0cch, 7eh, 0h, 0h, 0f8h, 0h, 0f8h, 0cch
        DB      0cch, 0cch, 0h, 0fch, 0h, 0cch, 0ech, 0fch, 0dch, 0cch, 0h, 3ch, 6ch, 6ch, 3eh, 0h
        DB      7eh, 0h, 0h, 38h, 6ch, 6ch, 38h, 0h, 7ch, 0h, 0h, 30h, 0h, 30h, 60h, 0c0h
        DB      0cch, 78h, 0h, 0h, 0h, 0h, 0fch, 0c0h, 0c0h, 0h, 0h, 0h, 0h, 0h, 0fch, 0ch
        DB      0ch, 0h, 0h, 0c3h, 0c6h, 0cch, 0deh, 33h, 66h, 0cch, 0fh, 0c3h, 0c6h, 0cch, 0dbh, 37h
        DB      6fh, 0cfh, 3h, 18h, 18h, 0h, 18h, 18h, 18h, 0h, 0h, 0h, 33h, 66h, 0cch, 66h
        DB      33h, 0h, 0h, 0h, 0cch, 66h, 33h, 66h, 0cch, 0h, 0h, 22h, 88h, 22h, 88h, 22h
        DB      88h, 22h, 88h, 0aah, 55h, 0aah, 55h, 0aah, 55h, 0aah, 55h, 0dbh, 77h, 0dbh, 0eeh, 0dbh
        DB      77h, 0dbh, 0eeh, 18h, 18h, 18h, 18h, 18h, 18h, 18h, 18h, 18h, 18h, 18h, 18h, 0f8h
        DB      18h, 18h, 18h, 18h, 18h, 0f8h, 18h, 0f8h, 18h, 18h, 18h, 36h, 36h, 36h, 36h, 0f6h
        DB      36h, 36h, 36h, 0h, 0h, 0h, 0h, 0feh, 36h, 36h, 36h, 0h, 0h, 0f8h, 18h, 0f8h
        DB      18h, 18h, 18h, 36h, 36h, 0f6h, 6h, 0f6h, 36h, 36h, 36h, 36h, 36h, 36h, 36h, 36h
        DB      36h, 36h, 36h, 0h, 0h, 0feh, 6h, 0f6h, 36h, 36h, 36h, 36h, 36h, 0f6h, 6h, 0feh
        DB      0h, 0h, 0h, 36h, 36h, 36h, 36h, 0feh, 0h, 0h, 0h, 18h, 18h, 0f8h, 18h, 0f8h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 0h, 0f8h, 18h, 18h, 18h, 18h, 18h, 18h, 18h, 1fh
        DB      0h, 0h, 0h, 18h, 18h, 18h, 18h, 0ffh, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 0ffh
        DB      18h, 18h, 18h, 18h, 18h, 18h, 18h, 1fh, 18h, 18h, 18h, 0h, 0h, 0h, 0h, 0ffh
        DB      0h, 0h, 0h, 18h, 18h, 18h, 18h, 0ffh, 18h, 18h, 18h, 18h, 18h, 1fh, 18h, 1fh
        DB      18h, 18h, 18h, 36h, 36h, 36h, 36h, 37h, 36h, 36h, 36h, 36h, 36h, 37h, 30h, 3fh
        DB      0h, 0h, 0h, 0h, 0h, 3fh, 30h, 37h, 36h, 36h, 36h, 36h, 36h, 0f7h, 0h, 0ffh
        DB      0h, 0h, 0h, 0h, 0h, 0ffh, 0h, 0f7h, 36h, 36h, 36h, 36h, 36h, 37h, 30h, 37h
        DB      36h, 36h, 36h, 0h, 0h, 0ffh, 0h, 0ffh, 0h, 0h, 0h, 36h, 36h, 0f7h, 0h, 0f7h
        DB      36h, 36h, 36h, 18h, 18h, 0ffh, 0h, 0ffh, 0h, 0h, 0h, 36h, 36h, 36h, 36h, 0ffh
        DB      0h, 0h, 0h, 0h, 0h, 0ffh, 0h, 0ffh, 18h, 18h, 18h, 0h, 0h, 0h, 0h, 0ffh
        DB      36h, 36h, 36h, 36h, 36h, 36h, 36h, 3fh, 0h, 0h, 0h, 36h, 36h, 36h, 36h, 3fh
        DB      0h, 0h, 0h, 0h, 0h, 1fh, 18h, 1fh, 18h, 18h, 18h, 0h, 0h, 0h, 0h, 3fh
        DB      36h, 36h, 36h, 36h, 36h, 36h, 36h, 0ffh, 36h, 36h, 36h, 18h, 18h, 0ffh, 18h, 0ffh
        DB      18h, 18h, 18h, 18h, 18h, 18h, 18h, 0f8h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 1fh
        DB      18h, 18h, 18h, 0ffh, 0ffh, 0ffh, 0ffh, 0ffh, 0ffh, 0ffh, 0ffh, 0h, 0h, 0h, 0h, 0ffh
        DB      0ffh, 0ffh, 0ffh, 0f0h, 0f0h, 0f0h, 0f0h, 0f0h, 0f0h, 0f0h, 0f0h, 0fh, 0fh, 0fh, 0fh, 0fh
        DB      0fh, 0fh, 0fh, 0ffh, 0ffh, 0ffh, 0ffh, 0h, 0h, 0h, 0h, 0h, 0h, 76h, 0dch, 0c8h
        DB      0dch, 76h, 0h, 0h, 78h, 0cch, 0f8h, 0cch, 0f8h, 0c0h, 0c0h, 0h, 0fch, 0cch, 0c0h, 0c0h
        DB      0c0h, 0c0h, 0h, 0h, 0feh, 6ch, 6ch, 6ch, 6ch, 6ch, 0h, 0fch, 0cch, 60h, 30h, 60h
        DB      0cch, 0fch, 0h, 0h, 0h, 7eh, 0d8h, 0d8h, 0d8h, 70h, 0h, 0h, 66h, 66h, 66h, 66h
        DB      7ch, 60h, 0c0h, 0h, 76h, 0dch, 18h, 18h, 18h, 18h, 0h, 0fch, 30h, 78h, 0cch, 0cch
        DB      78h, 30h, 0fch, 38h, 6ch, 0c6h, 0feh, 0c6h, 6ch, 38h, 0h, 38h, 6ch, 0c6h, 0c6h, 6ch
        DB      6ch, 0eeh, 0h, 1ch, 30h, 18h, 7ch, 0cch, 0cch, 78h, 0h, 0h, 0h, 7eh, 0dbh, 0dbh
        DB      7eh, 0h, 0h, 6h, 0ch, 7eh, 0dbh, 0dbh, 7eh, 60h, 0c0h, 38h, 60h, 0c0h, 0f8h, 0c0h
        DB      60h, 38h, 0h, 78h, 0cch, 0cch, 0cch, 0cch, 0cch, 0cch, 0h, 0h, 0fch, 0h, 0fch, 0h
        DB      0fch, 0h, 0h, 30h, 30h, 0fch, 30h, 30h, 0h, 0fch, 0h, 60h, 30h, 18h, 30h, 60h
        DB      0h, 0fch, 0h, 18h, 30h, 60h, 30h, 18h, 0h, 0fch, 0h, 0eh, 1bh, 1bh, 18h, 18h
        DB      18h, 18h, 18h, 18h, 18h, 18h, 18h, 18h, 0d8h, 0d8h, 70h, 30h, 30h, 0h, 0fch, 0h
        DB      30h, 30h, 0h, 0h, 76h, 0dch, 0h, 76h, 0dch, 0h, 0h, 38h, 6ch, 6ch, 38h, 0h
        DB      0h, 0h, 0h, 0h, 0h, 0h, 18h, 18h, 0h, 0h, 0h, 0h, 0h, 0h, 0h, 10h
        DB      0h, 0h, 0h, 0fh, 0ch, 0ch, 0ch, 0ech, 6ch, 3ch, 1ch, 78h, 6ch, 6ch, 6ch, 6ch
        DB      0h, 0h, 0h, 70h, 18h, 30h, 60h, 78h, 0h, 0h, 0h, 0h, 0h, 3ch, 3ch, 3ch
        DB      3ch, 0h, 0h, 0f0h, 80h, 0e0h, 8fh, 88h, 0eh, 8h, 8h, 0h
; CS:0AFF CS:0D45
; CS:0AFF
L0AFF:
        cli
L0B00:
        mov sp, 1cbh
L0B03:
        sti
L0B04:
        mov bx, 1f6ch
L0B07:
        shr bx, 1
L0B09:
        shr bx, 1
L0B0B:
        shr bx, 1
L0B0D:
        shr bx, 1
L0B0F:
        inc bx
L0B10:
        inc bx
L0B11:
        mov ah, 4ah
L0B13:
        int 21h
L0B15:
        jae L0B21
L0B17:
        mov dx, 2b4h
L0B1A:
        mov ah, 9
L0B1C:
        int 21h
L0B1E:
        jmp short L0B92
L0B20:
        nop
L0B21:
        call L0BA0
L0B24:
        mov bx, 5dh
L0B27:
        mov al, byte ptr cs:[bx]
L0B2A:
        mov byte ptr cs:[2aah], al
L0B2E:
        mov bx, 6dh
L0B31:
        mov al, byte ptr cs:[bx]
L0B34:
        mov byte ptr cs:[2ach], al
L0B38:
        mov ax, cs
L0B3A:
        mov ds, ax
L0B3C:
        mov es, ax
L0B3E:
        mov word ptr ds:[324h], ax
L0B41:
        mov word ptr ds:[328h], ax
L0B44:
        mov word ptr ds:[32ch], ax
L0B47:
        mov dx, 295h
L0B4A:
        mov bx, 320h
L0B4D:
        mov ah, 4bh
L0B4F:
        mov al, 0
L0B51:
        int 21h
L0B53:
        mov ax, cs
L0B55:
        mov ds, ax
L0B57:
        mov es, ax
L0B59:
        mov word ptr ds:[324h], ax
L0B5C:
        mov word ptr ds:[328h], ax
L0B5F:
        mov word ptr ds:[32ch], ax
L0B62:
        mov dx, 29fh
L0B65:
        mov bx, 320h
L0B68:
        mov ah, 4bh
L0B6A:
        mov al, 0
L0B6C:
        int 21h
L0B6E:
        mov dx, 266h
L0B71:
        mov ah, 9
L0B73:
        int 21h
L0B75:
        push cs
L0B76:
        pop ds
L0B77:
        push ds
L0B78:
        lds dx, dword ptr ds:[348h]
L0B7C:
        mov ax, 2509h
L0B7F:
        int 21h
L0B81:
        pop ds
L0B82:
        push ds
L0B83:
        mov dx, word ptr ds:[34eh]
L0B87:
        mov ds, word ptr ds:[350h]
L0B8B:
        mov ah, 25h
L0B8D:
        mov al, 1ch
L0B8F:
        int 21h
L0B91:
        pop ds
L0B92:
        mov al, byte ptr ds:[347h]
L0B95:
        call L1F17
L0B98:
        xor ax, ax
L0B9A:
        int 33h
L0B9C:
        mov ah, 4ch
L0B9E:
        int 21h
; CS:0BA0
L0BA0:
        mov word ptr ds:[358h], ds
L0BA4:
        mov ah, 0fh
L0BA6:
        int 10h
L0BA8:
        mov byte ptr ds:[347h], al
L0BAB:
        mov al, 0dh
L0BAD:
        call L1F17
L0BB0:
        mov dx, 1dfh
L0BB3:
        mov ah, 9
L0BB5:
        int 21h
L0BB7:
        mov dx, 0d2ah
L0BBA:
        mov ah, 25h
L0BBC:
        mov al, 0f8h
L0BBE:
        int 21h
L0BC0:
        mov ah, 35h
L0BC2:
        mov al, 9
L0BC4:
        int 21h
L0BC6:
        mov word ptr ds:[348h], bx
L0BCA:
        mov word ptr ds:[34ah], es
L0BCE:
        mov dx, 0c5ch
L0BD1:
        mov ah, 25h
L0BD3:
        mov al, 9
L0BD5:
        int 21h
L0BD7:
        push es
L0BD8:
        mov ax, 40h
L0BDB:
        mov es, ax
L0BDD:
        mov dx, word ptr es:[63h]
L0BE2:
        mov word ptr ds:[34ch], dx
L0BE6:
        pop es
L0BE7:
        mov ah, 35h
L0BE9:
        mov al, 1ch
L0BEB:
        int 21h
L0BED:
        mov word ptr ds:[34eh], bx
L0BF1:
        mov word ptr ds:[350h], es
L0BF5:
        mov dx, 0ca9h
L0BF8:
        mov ah, 25h
L0BFA:
        mov al, 1ch
L0BFC:
        int 21h
L0BFE:
        call L1F16
L0C01:
        mov cl, 0c7h
L0C03:
        call L0D8F
L0C06:
        call L0DDB
L0C09:
        call L0DDB
L0C0C:
        push es
L0C0D:
        mov ax, 3533h
L0C10:
        int 21h
L0C12:
        mov ax, es
L0C14:
        pop es
L0C15:
        or ax, bx
L0C17:
        je L0C5B
L0C19:
        mov ax, 0
L0C1C:
        int 33h
L0C1E:
        and ax, ax
L0C20:
        je L0C5B
L0C22:
        mov cx, 0
L0C25:
        mov dx, 13fh
L0C28:
        mov ax, 7
L0C2B:
        int 33h
L0C2D:
        mov cx, 0
L0C30:
        mov dx, 0c7h
L0C33:
        mov ax, 8
L0C36:
        int 33h
L0C38:
        mov cx, 0a0h
L0C3B:
        mov dx, 64h
L0C3E:
        mov ax, 4
L0C41:
        int 33h
L0C43:
        mov cx, 8
L0C46:
        mov dx, 8
L0C49:
        mov ax, 0fh
L0C4C:
        int 33h
L0C4E:
        mov cx, 0ffffh
L0C51:
        mov dx, 0d19h
L0C54:
        push es
L0C55:
        push cs
L0C56:
        pop es
L0C57:
        mov ax, 0ch
L0C5A:
        pop es
L0C5B:
        ret
; CS:0C5C
L0C5C:
        push ds
L0C5D:
        push ax
L0C5E:
        push cs
L0C5F:
        pop ds
L0C60:
        in al, 60h
L0C62:
        mov ah, al
L0C64:
        and al, 7fh
L0C66:
        cmp al, 52h
L0C68:
        jge L0C6E
L0C6A:
        cmp al, 47h
L0C6C:
        jge L0C79
L0C6E:
        mov byte ptr cs:[0afeh], al
L0C72:
        pushf
L0C73:
        call dword ptr ds:[348h]
L0C77:
        jmp short L0CA6
L0C79:
        push ax
L0C7A:
        in al, 61h
L0C7C:
        mov ah, al
L0C7E:
        or al, 80h
L0C80:
        out 61h, al
L0C82:
        xchg ah, al
L0C84:
        out 61h, al
L0C86:
        mov al, 20h
L0C88:
        out 20h, al
L0C8A:
        pop ax
L0C8B:
        push bx
L0C8C:
        sub al, 47h
L0C8E:
        mov bl, al
L0C90:
        xor bh, bh
L0C92:
        add bx, 39dh
L0C96:
        xor al, al
L0C98:
        not al
L0C9A:
        mov byte ptr [bx], al
L0C9C:
        not al
L0C9E:
        test ah, 80h
L0CA1:
        je L0CA5
L0CA3:
        mov byte ptr [bx], al
L0CA5:
        pop bx
L0CA6:
        pop ax
L0CA7:
        pop ds
L0CA8:
        iret
; CS:0CA9
L0CA9:
        push ds
L0CAA:
        push dx
L0CAB:
        push ax
L0CAC:
        push si
L0CAD:
        mov ax, cs
L0CAF:
        mov ds, ax
L0CB1:
        mov byte ptr ds:[352h], 0
L0CB6:
        test word ptr ds:[38ah], 1
L0CBC:
        je L0CFB
L0CBE:
        mov dx, 201h
L0CC1:
        mov word ptr ds:[390h], 0
L0CC7:
        mov word ptr ds:[392h], 0
L0CCD:
        in al, dx
L0CCE:
        mov byte ptr ds:[39ch], al
L0CD1:
        xor ax, ax
L0CD3:
        out dx, al
L0CD4:
        in al, dx
L0CD5:
        and al, 3
L0CD7:
        je L0CFB
L0CD9:
        test al, 1
L0CDB:
        je L0CEB
L0CDD:
        inc word ptr ds:[390h]
L0CE1:
        jne L0CEB
L0CE3:
        mov word ptr ds:[390h], 0ffffh
L0CE9:
        jmp short L0CFB
L0CEB:
        test al, 2
L0CED:
        je L0CD4
L0CEF:
        inc word ptr ds:[392h]
L0CF3:
        jne L0CD4
L0CF5:
        mov word ptr ds:[392h], 0ffffh
L0CFB:
        mov si, word ptr ds:[361h]
L0CFF:
        mov ds, word ptr ds:[358h]
L0D03:
        inc word ptr [si]
L0D05:
        add si, 2
L0D08:
        inc word ptr [si]
L0D0A:
        add si, 2
L0D0D:
        dec word ptr [si]
L0D0F:
        add si, 2
L0D12:
        inc word ptr [si]
L0D14:
        pop si
L0D15:
        pop ax
L0D16:
        pop dx
L0D17:
        pop ds
L0D18:
        iret
; CS:0D19
L0D19:
        retf
L0D1A:
        mov word ptr cs:[378h], bx
L0D1F:
        mov word ptr cs:[37ah], cx
L0D24:
        mov word ptr cs:[37ch], dx
L0D29:
        retf
; CS:0D2A
L0D2A:
        mov byte ptr cs:[346h], 1
L0D30:
        sti
L0D31:
        shl ax, 1
L0D33:
        push bx
L0D34:
        mov bx, 0d46h
L0D37:
        add bx, ax
L0D39:
        mov ax, word ptr cs:[bx]
L0D3C:
        pop bx
L0D3D:
        call ax
L0D3F:
        mov byte ptr cs:[346h], 0
L0D45:
        iret
; CS:0D46 CS:0D8E
        DB      8fh, 0dh, 0dbh, 0dh, 1eh, 0eh, 28h, 0eh, 80h, 10h, 3bh, 14h, 46h, 14h, 47h, 14h
        DB      48h, 14h, 5fh, 15h, 94h, 15h, 0a5h, 15h, 26h, 16h, 0e6h, 16h, 0f3h, 16h, 0f4h, 16h
        DB      0f9h, 16h, 27h, 17h, 6ch, 17h, 6dh, 17h, 6eh, 17h, 52h, 1bh, 5ah, 1bh, 5bh, 1bh
        DB      5fh, 1bh, 0b9h, 1bh, 0h, 1ch, 53h, 1ch, 87h, 1ch, 0b4h, 1dh, 2eh, 1eh, 8bh, 1eh
        DB      0b3h, 1eh, 0f7h, 1eh, 0bh, 1fh, 8eh, 0dh, 0c3h
; CS:0D8F CS:1F6B
; CS:0D8F
L0D8F:
        push es
L0D90:
        push ds
L0D91:
        push si
L0D92:
        push di
L0D93:
        mov dx, 3ceh
L0D96:
        mov ax, 0ff08h
L0D99:
        out dx, ax
L0D9A:
        mov ax, 5
L0D9D:
        out dx, ax
L0D9E:
        mov al, 3
L0DA0:
        out dx, ax
L0DA1:
        mov ax, word ptr cs:[338h]
L0DA5:
        mov es, ax
L0DA7:
        mov ds, ax
L0DA9:
        xor si, si
L0DAB:
        xor di, di
L0DAD:
        xor ax, ax
L0DAF:
        stosw
L0DB0:
        mov ax, 105h
L0DB3:
        out dx, ax
L0DB4:
        mov ax, 14h
L0DB7:
        mul cl
L0DB9:
        mov cx, ax
L0DBB:
        push cx
L0DBC:
        dec cx
L0DBD:
        rep movsw
L0DBF:
        pop cx
L0DC0:
        mov ax, word ptr cs:[33ah]
L0DC4:
        mov es, ax
L0DC6:
        mov si, 0
L0DC9:
        mov di, 0
L0DCC:
        rep movsw
L0DCE:
        mov ax, 5
L0DD1:
        out dx, ax
L0DD2:
        mov ax, 0f07h
L0DD5:
        out dx, ax
L0DD6:
        pop di
L0DD7:
        pop si
L0DD8:
        pop ds
L0DD9:
        pop es
L0DDA:
        ret
L0DDB:
        push ds
L0DDC:
        mov ax, cs
L0DDE:
        mov ds, ax
L0DE0:
        mov al, byte ptr ds:[330h]
L0DE3:
        xor al, 1
L0DE5:
        mov byte ptr ds:[330h], al
L0DE8:
        je L0DFB
L0DEA:
        mov ax, word ptr ds:[338h]
L0DED:
        mov word ptr ds:[334h], ax
L0DF0:
        mov ax, word ptr ds:[33ah]
L0DF3:
        mov word ptr ds:[336h], ax
L0DF6:
        mov ax, 501h
L0DF9:
        jmp short L0E0A
L0DFB:
        mov ax, word ptr ds:[33ah]
L0DFE:
        mov word ptr ds:[334h], ax
L0E01:
        mov ax, word ptr ds:[338h]
L0E04:
        mov word ptr ds:[336h], ax
L0E07:
        mov ax, 500h
L0E0A:
        int 10h
L0E0C:
        inc word ptr ds:[33eh]
L0E10:
        mov ax, word ptr ds:[340h]
L0E13:
        mov word ptr ds:[342h], ax
L0E16:
        pop ds
L0E17:
        call L1F56
L0E1A:
        call L1D17
L0E1D:
        ret
L0E1E:
        push es
L0E1F:
        push ds
L0E20:
        pop es
L0E21:
        mov ax, 1002h
L0E24:
        int 10h
L0E26:
        pop es
L0E27:
        ret
L0E28:
        and cx, cx
L0E2A:
        jns L0E37
L0E2C:
        neg cx
L0E2E:
        cmp cx, 10h
L0E31:
        jg L0E6D
L0E33:
        neg cx
L0E35:
        jmp short L0E3D
L0E37:
        cmp cx, 140h
L0E3B:
        jge L0E6D
L0E3D:
        and dx, dx
L0E3F:
        jns L0E58
L0E41:
        neg dx
L0E43:
        and dh, dh
L0E45:
        jne L0E6D
L0E47:
        cmp dl, bl
L0E49:
        jge L0E6D
L0E4B:
        sub bl, dl
L0E4D:
        mov ax, 8
L0E50:
        mul dl
L0E52:
        add si, ax
L0E54:
        xor dx, dx
L0E56:
        jmp short L0E6E
L0E58:
        mov ax, dx
L0E5A:
        sub ax, 0c8h
L0E5D:
        jns L0E6D
L0E5F:
        je L0E6D
L0E61:
        neg ax
L0E63:
        xor bh, bh
L0E65:
        cmp ax, bx
L0E67:
        jg L0E6E
L0E69:
        mov bl, al
L0E6B:
        jmp short L0E6E
L0E6D:
        ret
L0E6E:
        push bp
L0E6F:
        mov bp, sp
L0E71:
        sub sp, 14h
L0E74:
        push bx
L0E75:
        mov bx, cx
L0E77:
        sar bx, 1
L0E79:
        sar bx, 1
L0E7B:
        sar bx, 1
L0E7D:
        add bx, 3c7h
L0E81:
        mov bl, byte ptr cs:[bx]
L0E84:
        mov byte ptr [bp - 11h], bl
L0E87:
        pop bx
L0E88:
        xor ah, ah
L0E8A:
        mov al, bl
L0E8C:
        mov word ptr cs:[36dh], ax
L0E90:
        push es
L0E91:
        push di
L0E92:
        mov ax, 28h
L0E95:
        mul dx
L0E97:
        mov dl, cl
L0E99:
        and dx, 7
L0E9C:
        sar cx, 1
L0E9E:
        sar cx, 1
L0EA0:
        sar cx, 1
L0EA2:
        add ax, cx
L0EA4:
        mov di, ax
L0EA6:
        mov cl, dl
L0EA8:
        mov word ptr cs:[344h], di
L0EAD:
        mov es, word ptr cs:[334h]
L0EB2:
        mov dx, 3ceh
L0EB5:
        mov ax, 805h
L0EB8:
        out dx, ax
L0EB9:
        mov ah, 0
L0EBB:
        mov al, 3
L0EBD:
        out dx, ax
L0EBE:
        mov ax, 7
L0EC1:
        out dx, ax
L0EC2:
        mov ax, 0ff08h
L0EC5:
        out dx, ax
L0EC6:
        and cl, cl
L0EC8:
        je L0ECD
L0ECA:
        jmp L0F59
L0ECD:
        push di
L0ECE:
        mov ax, word ptr [si]
L0ED0:
        or ax, word ptr [si + 2]
L0ED3:
        or ax, word ptr [si + 4]
L0ED6:
        or ax, word ptr [si + 6]
L0ED9:
        mov byte ptr [bp - 3], ah
L0EDC:
        test byte ptr [bp - 11h], 10h
L0EE0:
        je L0F0F
L0EE2:
        mov dl, 0ceh
L0EE4:
        mov ah, al
L0EE6:
        mov al, 8
L0EE8:
        out dx, ax
L0EE9:
        mov dl, 0c4h
L0EEB:
        mov ax, 802h
L0EEE:
        out dx, ax
L0EEF:
        mov ch, byte ptr [si + 6]
L0EF2:
        and byte ptr es:[di], ch
L0EF5:
        shr ah, 1
L0EF7:
        out dx, ax
L0EF8:
        mov ch, byte ptr [si + 4]
L0EFB:
        and byte ptr es:[di], ch
L0EFE:
        shr ah, 1
L0F00:
        out dx, ax
L0F01:
        mov ch, byte ptr [si + 2]
L0F04:
        and byte ptr es:[di], ch
L0F07:
        shr ah, 1
L0F09:
        out dx, ax
L0F0A:
        mov ch, byte ptr [si]
L0F0C:
        and byte ptr es:[di], ch
L0F0F:
        inc di
L0F10:
        test byte ptr [bp - 11h], 8
L0F14:
        je L0F45
L0F16:
        mov dl, 0ceh
L0F18:
        mov ah, byte ptr [bp - 3]
L0F1B:
        mov al, 8
L0F1D:
        out dx, ax
L0F1E:
        mov dl, 0c4h
L0F20:
        mov ax, 802h
L0F23:
        out dx, ax
L0F24:
        mov ch, byte ptr [si + 7]
L0F27:
        and byte ptr es:[di], ch
L0F2A:
        shr ah, 1
L0F2C:
        out dx, ax
L0F2D:
        mov ch, byte ptr [si + 5]
L0F30:
        and byte ptr es:[di], ch
L0F33:
        shr ah, 1
L0F35:
        out dx, ax
L0F36:
        mov ch, byte ptr [si + 3]
L0F39:
        and byte ptr es:[di], ch
L0F3C:
        shr ah, 1
L0F3E:
        out dx, ax
L0F3F:
        mov ch, byte ptr [si + 1]
L0F42:
        and byte ptr es:[di], ch
L0F45:
        add si, 8
L0F48:
        pop di
L0F49:
        add di, 28h
L0F4C:
        dec word ptr cs:[36dh]
L0F51:
        je L0F56
L0F53:
        jmp L0ECD
L0F56:
        jmp L1077
L0F59:
        push di
L0F5A:
        mov ax, word ptr [si]
L0F5C:
        or ax, word ptr [si + 2]
L0F5F:
        or ax, word ptr [si + 4]
L0F62:
        or ax, word ptr [si + 6]
L0F65:
        xchg ah, al
L0F67:
        mov bh, al
L0F69:
        xor bl, bl
L0F6B:
        shr ax, cl
L0F6D:
        shr bx, cl
L0F6F:
        mov byte ptr [bp - 2], ah
L0F72:
        mov byte ptr [bp - 3], al
L0F75:
        mov byte ptr [bp - 4], bl
L0F78:
        lodsw
L0F79:
        xchg ah, al
L0F7B:
        mov bh, al
L0F7D:
        xor bl, bl
L0F7F:
        shr ax, cl
L0F81:
        shr bx, cl
L0F83:
        mov byte ptr [bp - 5], ah
L0F86:
        mov byte ptr [bp - 6], al
L0F89:
        mov byte ptr [bp - 7], bl
L0F8C:
        lodsw
L0F8D:
        xchg ah, al
L0F8F:
        mov bh, al
L0F91:
        xor bl, bl
L0F93:
        shr ax, cl
L0F95:
        shr bx, cl
L0F97:
        mov byte ptr [bp - 8], ah
L0F9A:
        mov byte ptr [bp - 9], al
L0F9D:
        mov byte ptr [bp - 0ah], bl
L0FA0:
        lodsw
L0FA1:
        xchg ah, al
L0FA3:
        mov bh, al
L0FA5:
        xor bl, bl
L0FA7:
        shr ax, cl
L0FA9:
        shr bx, cl
L0FAB:
        mov byte ptr [bp - 0bh], ah
L0FAE:
        mov byte ptr [bp - 0ch], al
L0FB1:
        mov byte ptr [bp - 0dh], bl
L0FB4:
        lodsw
L0FB5:
        xchg ah, al
L0FB7:
        mov bh, al
L0FB9:
        xor bl, bl
L0FBB:
        shr ax, cl
L0FBD:
        shr bx, cl
L0FBF:
        mov byte ptr [bp - 0eh], ah
L0FC2:
        mov byte ptr [bp - 0fh], al
L0FC5:
        mov byte ptr [bp - 10h], bl
L0FC8:
        test byte ptr [bp - 11h], 10h
L0FCC:
        je L0FFD
L0FCE:
        mov dl, 0ceh
L0FD0:
        mov ah, byte ptr [bp - 2]
L0FD3:
        mov al, 8
L0FD5:
        out dx, ax
L0FD6:
        mov dl, 0c4h
L0FD8:
        mov ax, 802h
L0FDB:
        out dx, ax
L0FDC:
        mov ch, byte ptr [bp - 0eh]
L0FDF:
        and byte ptr es:[di], ch
L0FE2:
        shr ah, 1
L0FE4:
        out dx, ax
L0FE5:
        mov ch, byte ptr [bp - 0bh]
L0FE8:
        and byte ptr es:[di], ch
L0FEB:
        shr ah, 1
L0FED:
        out dx, ax
L0FEE:
        mov ch, byte ptr [bp - 8]
L0FF1:
        and byte ptr es:[di], ch
L0FF4:
        shr ah, 1
L0FF6:
        out dx, ax
L0FF7:
        mov ch, byte ptr [bp - 5]
L0FFA:
        and byte ptr es:[di], ch
L0FFD:
        inc di
L0FFE:
        test byte ptr [bp - 11h], 8
L1002:
        je L1033
L1004:
        mov dl, 0ceh
L1006:
        mov ah, byte ptr [bp - 3]
L1009:
        mov al, 8
L100B:
        out dx, ax
L100C:
        mov dl, 0c4h
L100E:
        mov ax, 802h
L1011:
        out dx, ax
L1012:
        mov ch, byte ptr [bp - 0fh]
L1015:
        and byte ptr es:[di], ch
L1018:
        shr ah, 1
L101A:
        out dx, ax
L101B:
        mov ch, byte ptr [bp - 0ch]
L101E:
        and byte ptr es:[di], ch
L1021:
        shr ah, 1
L1023:
        out dx, ax
L1024:
        mov ch, byte ptr [bp - 9]
L1027:
        and byte ptr es:[di], ch
L102A:
        shr ah, 1
L102C:
        out dx, ax
L102D:
        mov ch, byte ptr [bp - 6]
L1030:
        and byte ptr es:[di], ch
L1033:
        inc di
L1034:
        test byte ptr [bp - 11h], 4
L1038:
        je L1069
L103A:
        mov dl, 0ceh
L103C:
        mov ah, byte ptr [bp - 4]
L103F:
        mov al, 8
L1041:
        out dx, ax
L1042:
        mov dl, 0c4h
L1044:
        mov ax, 802h
L1047:
        out dx, ax
L1048:
        mov ch, byte ptr [bp - 10h]
L104B:
        and byte ptr es:[di], ch
L104E:
        shr ah, 1
L1050:
        out dx, ax
L1051:
        mov ch, byte ptr [bp - 0dh]
L1054:
        and byte ptr es:[di], ch
L1057:
        shr ah, 1
L1059:
        out dx, ax
L105A:
        mov ch, byte ptr [bp - 0ah]
L105D:
        and byte ptr es:[di], ch
L1060:
        shr ah, 1
L1062:
        out dx, ax
L1063:
        mov ch, byte ptr [bp - 7]
L1066:
        and byte ptr es:[di], ch
L1069:
        pop di
L106A:
        add di, 28h
L106D:
        dec word ptr cs:[36dh]
L1072:
        je L1077
L1074:
        jmp L0F59
L1077:
        call L1F3C
L107A:
        pop di
L107B:
        pop es
L107C:
        mov sp, bp
L107E:
        pop bp
L107F:
        ret
L1080:
        and cx, cx
L1082:
        jns L108F
L1084:
        neg cx
L1086:
        cmp cx, 20h
L1089:
        jg L10C5
L108B:
        neg cx
L108D:
        jmp short L1095
L108F:
        cmp cx, 140h
L1093:
        jge L10C5
L1095:
        and dx, dx
L1097:
        jns L10B0
L1099:
        neg dx
L109B:
        and dh, dh
L109D:
        jne L10C5
L109F:
        cmp dl, bl
L10A1:
        jge L10C5
L10A3:
        sub bl, dl
L10A5:
        mov ax, 10h
L10A8:
        mul dl
L10AA:
        add si, ax
L10AC:
        xor dx, dx
L10AE:
        jmp short L10C6
L10B0:
        mov ax, dx
L10B2:
        sub ax, 0c8h
L10B5:
        jns L10C5
L10B7:
        je L10C5
L10B9:
        neg ax
L10BB:
        xor bh, bh
L10BD:
        cmp ax, bx
L10BF:
        jg L10C6
L10C1:
        mov bl, al
L10C3:
        jmp short L10C6
L10C5:
        ret
L10C6:
        push bp
L10C7:
        mov bp, sp
L10C9:
        sub sp, 1eh
L10CC:
        push bx
L10CD:
        mov bx, cx
L10CF:
        sar bx, 1
L10D1:
        sar bx, 1
L10D3:
        sar bx, 1
L10D5:
        add bx, 3c7h
L10D9:
        mov bl, byte ptr cs:[bx]
L10DC:
        mov byte ptr [bp - 1bh], bl
L10DF:
        pop bx
L10E0:
        xor ah, ah
L10E2:
        mov al, bl
L10E4:
        mov word ptr cs:[36dh], ax
L10E8:
        push es
L10E9:
        push di
L10EA:
        mov es, word ptr cs:[354h]
L10EF:
        mov di, word ptr cs:[356h]
L10F4:
        add di, cx
L10F6:
        mov al, byte ptr es:[di]
L10F9:
        mov ax, 28h
L10FC:
        mul dx
L10FE:
        mov dl, cl
L1100:
        and dx, 7
L1103:
        sar cx, 1
L1105:
        sar cx, 1
L1107:
        sar cx, 1
L1109:
        add ax, cx
L110B:
        mov di, ax
L110D:
        mov cl, dl
L110F:
        mov word ptr cs:[344h], di
L1114:
        mov es, word ptr cs:[334h]
L1119:
        mov dx, 3ceh
L111C:
        mov ax, 805h
L111F:
        out dx, ax
L1120:
        mov ah, 0
L1122:
        mov al, 3
L1124:
        out dx, ax
L1125:
        mov ax, 7
L1128:
        out dx, ax
L1129:
        mov ax, 0ff08h
L112C:
        out dx, ax
L112D:
        and cl, cl
L112F:
        je L1134
L1131:
        jmp L123E
L1134:
        push di
L1135:
        mov ax, word ptr [si + 2]
L1138:
        or ax, word ptr [si + 6]
L113B:
        or ax, word ptr [si + 0ah]
L113E:
        or ax, word ptr [si + 0eh]
L1141:
        mov byte ptr [bp - 4], al
L1144:
        mov byte ptr [bp - 5], ah
L1147:
        mov ax, word ptr [si]
L1149:
        or ax, word ptr [si + 4]
L114C:
        or ax, word ptr [si + 8]
L114F:
        or ax, word ptr [si + 0ch]
L1152:
        mov byte ptr [bp - 3], ah
L1155:
        test byte ptr [bp - 1bh], 10h
L1159:
        je L1188
L115B:
        mov dl, 0ceh
L115D:
        mov ah, al
L115F:
        mov al, 8
L1161:
        out dx, ax
L1162:
        mov dl, 0c4h
L1164:
        mov ax, 802h
L1167:
        out dx, ax
L1168:
        mov ch, byte ptr [si + 0ch]
L116B:
        and byte ptr es:[di], ch
L116E:
        shr ah, 1
L1170:
        out dx, ax
L1171:
        mov ch, byte ptr [si + 8]
L1174:
        and byte ptr es:[di], ch
L1177:
        shr ah, 1
L1179:
        out dx, ax
L117A:
        mov ch, byte ptr [si + 4]
L117D:
        and byte ptr es:[di], ch
L1180:
        shr ah, 1
L1182:
        out dx, ax
L1183:
        mov ch, byte ptr [si]
L1185:
        and byte ptr es:[di], ch
L1188:
        inc di
L1189:
        test byte ptr [bp - 1bh], 8
L118D:
        je L11BE
L118F:
        mov dl, 0ceh
L1191:
        mov ah, byte ptr [bp - 3]
L1194:
        mov al, 8
L1196:
        out dx, ax
L1197:
        mov dl, 0c4h
L1199:
        mov ax, 802h
L119C:
        out dx, ax
L119D:
        mov ch, byte ptr [si + 0dh]
L11A0:
        and byte ptr es:[di], ch
L11A3:
        shr ah, 1
L11A5:
        out dx, ax
L11A6:
        mov ch, byte ptr [si + 9]
L11A9:
        and byte ptr es:[di], ch
L11AC:
        shr ah, 1
L11AE:
        out dx, ax
L11AF:
        mov ch, byte ptr [si + 5]
L11B2:
        and byte ptr es:[di], ch
L11B5:
        shr ah, 1
L11B7:
        out dx, ax
L11B8:
        mov ch, byte ptr [si + 1]
L11BB:
        and byte ptr es:[di], ch
L11BE:
        inc di
L11BF:
        test byte ptr [bp - 1bh], 4
L11C3:
        je L11F4
L11C5:
        mov dl, 0ceh
L11C7:
        mov ah, byte ptr [bp - 4]
L11CA:
        mov al, 8
L11CC:
        out dx, ax
L11CD:
        mov dl, 0c4h
L11CF:
        mov ax, 802h
L11D2:
        out dx, ax
L11D3:
        mov ch, byte ptr [si + 0eh]
L11D6:
        and byte ptr es:[di], ch
L11D9:
        shr ah, 1
L11DB:
        out dx, ax
L11DC:
        mov ch, byte ptr [si + 0ah]
L11DF:
        and byte ptr es:[di], ch
L11E2:
        shr ah, 1
L11E4:
        out dx, ax
L11E5:
        mov ch, byte ptr [si + 6]
L11E8:
        and byte ptr es:[di], ch
L11EB:
        shr ah, 1
L11ED:
        out dx, ax
L11EE:
        mov ch, byte ptr [si + 2]
L11F1:
        and byte ptr es:[di], ch
L11F4:
        inc di
L11F5:
        test byte ptr [bp - 1bh], 2
L11F9:
        je L122A
L11FB:
        mov dl, 0ceh
L11FD:
        mov ah, byte ptr [bp - 5]
L1200:
        mov al, 8
L1202:
        out dx, ax
L1203:
        mov dl, 0c4h
L1205:
        mov ax, 802h
L1208:
        out dx, ax
L1209:
        mov ch, byte ptr [si + 0fh]
L120C:
        and byte ptr es:[di], ch
L120F:
        shr ah, 1
L1211:
        out dx, ax
L1212:
        mov ch, byte ptr [si + 0bh]
L1215:
        and byte ptr es:[di], ch
L1218:
        shr ah, 1
L121A:
        out dx, ax
L121B:
        mov ch, byte ptr [si + 7]
L121E:
        and byte ptr es:[di], ch
L1221:
        shr ah, 1
L1223:
        out dx, ax
L1224:
        mov ch, byte ptr [si + 3]
L1227:
        and byte ptr es:[di], ch
L122A:
        add si, 10h
L122D:
        pop di
L122E:
        add di, 28h
L1231:
        dec word ptr cs:[36dh]
L1236:
        je L123B
L1238:
        jmp L1134
L123B:
        jmp L1432
L123E:
        push di
L123F:
        mov ax, word ptr [si]
L1241:
        or ax, word ptr [si + 4]
L1244:
        or ax, word ptr [si + 8]
L1247:
        or ax, word ptr [si + 0ch]
L124A:
        xchg ah, al
L124C:
        mov bh, al
L124E:
        shr ax, cl
L1250:
        mov byte ptr [bp - 2], ah
L1253:
        mov byte ptr [bp - 3], al
L1256:
        mov ax, word ptr [si + 2]
L1259:
        or ax, word ptr [si + 6]
L125C:
        or ax, word ptr [si + 0ah]
L125F:
        or ax, word ptr [si + 0eh]
L1262:
        xchg ah, al
L1264:
        mov bl, ah
L1266:
        shr bx, cl
L1268:
        mov byte ptr [bp - 4], bl
L126B:
        mov bh, al
L126D:
        xor bl, bl
L126F:
        shr ax, cl
L1271:
        shr bx, cl
L1273:
        mov byte ptr [bp - 5], al
L1276:
        mov byte ptr [bp - 6], bl
L1279:
        push dx
L127A:
        lodsw
L127B:
        mov dx, ax
L127D:
        xchg dh, dl
L127F:
        lodsw
L1280:
        xchg ah, al
L1282:
        mov bh, dl
L1284:
        mov bl, ah
L1286:
        shr dx, cl
L1288:
        shr bx, cl
L128A:
        mov byte ptr [bp - 7], dh
L128D:
        mov byte ptr [bp - 8], dl
L1290:
        mov byte ptr [bp - 9], bl
L1293:
        mov bh, al
L1295:
        xor bl, bl
L1297:
        shr ax, cl
L1299:
        shr bx, cl
L129B:
        mov byte ptr [bp - 0ah], al
L129E:
        mov byte ptr [bp - 0bh], bl
L12A1:
        lodsw
L12A2:
        mov dx, ax
L12A4:
        xchg dh, dl
L12A6:
        lodsw
L12A7:
        xchg ah, al
L12A9:
        mov bh, dl
L12AB:
        mov bl, ah
L12AD:
        shr dx, cl
L12AF:
        shr bx, cl
L12B1:
        mov byte ptr [bp - 0ch], dh
L12B4:
        mov byte ptr [bp - 0dh], dl
L12B7:
        mov byte ptr [bp - 0eh], bl
L12BA:
        mov bh, al
L12BC:
        xor bl, bl
L12BE:
        shr ax, cl
L12C0:
        shr bx, cl
L12C2:
        mov byte ptr [bp - 0fh], al
L12C5:
        mov byte ptr [bp - 10h], bl
L12C8:
        lodsw
L12C9:
        mov dx, ax
L12CB:
        xchg dh, dl
L12CD:
        lodsw
L12CE:
        xchg ah, al
L12D0:
        mov bh, dl
L12D2:
        mov bl, ah
L12D4:
        shr dx, cl
L12D6:
        shr bx, cl
L12D8:
        mov byte ptr [bp - 11h], dh
L12DB:
        mov byte ptr [bp - 12h], dl
L12DE:
        mov byte ptr [bp - 13h], bl
L12E1:
        mov bh, al
L12E3:
        xor bl, bl
L12E5:
        shr ax, cl
L12E7:
        shr bx, cl
L12E9:
        mov byte ptr [bp - 14h], al
L12EC:
        mov byte ptr [bp - 15h], bl
L12EF:
        lodsw
L12F0:
        mov dx, ax
L12F2:
        xchg dh, dl
L12F4:
        lodsw
L12F5:
        xchg ah, al
L12F7:
        mov bh, dl
L12F9:
        mov bl, ah
L12FB:
        shr dx, cl
L12FD:
        shr bx, cl
L12FF:
        mov byte ptr [bp - 16h], dh
L1302:
        mov byte ptr [bp - 17h], dl
L1305:
        mov byte ptr [bp - 18h], bl
L1308:
        mov bh, al
L130A:
        xor bl, bl
L130C:
        shr ax, cl
L130E:
        shr bx, cl
L1310:
        mov byte ptr [bp - 19h], al
L1313:
        mov byte ptr [bp - 1ah], bl
L1316:
        pop dx
L1317:
        test byte ptr [bp - 1bh], 10h
L131B:
        je L134C
L131D:
        mov dl, 0ceh
L131F:
        mov ah, byte ptr [bp - 2]
L1322:
        mov al, 8
L1324:
        out dx, ax
L1325:
        mov dl, 0c4h
L1327:
        mov ax, 802h
L132A:
        out dx, ax
L132B:
        mov ch, byte ptr [bp - 16h]
L132E:
        and byte ptr es:[di], ch
L1331:
        shr ah, 1
L1333:
        out dx, ax
L1334:
        mov ch, byte ptr [bp - 11h]
L1337:
        and byte ptr es:[di], ch
L133A:
        shr ah, 1
L133C:
        out dx, ax
L133D:
        mov ch, byte ptr [bp - 0ch]
L1340:
        and byte ptr es:[di], ch
L1343:
        shr ah, 1
L1345:
        out dx, ax
L1346:
        mov ch, byte ptr [bp - 7]
L1349:
        and byte ptr es:[di], ch
L134C:
        inc di
L134D:
        test byte ptr [bp - 1bh], 8
L1351:
        je L1382
L1353:
        mov dl, 0ceh
L1355:
        mov ah, byte ptr [bp - 3]
L1358:
        mov al, 8
L135A:
        out dx, ax
L135B:
        mov dl, 0c4h
L135D:
        mov ax, 802h
L1360:
        out dx, ax
L1361:
        mov ch, byte ptr [bp - 17h]
L1364:
        and byte ptr es:[di], ch
L1367:
        shr ah, 1
L1369:
        out dx, ax
L136A:
        mov ch, byte ptr [bp - 12h]
L136D:
        and byte ptr es:[di], ch
L1370:
        shr ah, 1
L1372:
        out dx, ax
L1373:
        mov ch, byte ptr [bp - 0dh]
L1376:
        and byte ptr es:[di], ch
L1379:
        shr ah, 1
L137B:
        out dx, ax
L137C:
        mov ch, byte ptr [bp - 8]
L137F:
        and byte ptr es:[di], ch
L1382:
        inc di
L1383:
        test byte ptr [bp - 1bh], 4
L1387:
        je L13B8
L1389:
        mov dl, 0ceh
L138B:
        mov ah, byte ptr [bp - 4]
L138E:
        mov al, 8
L1390:
        out dx, ax
L1391:
        mov dl, 0c4h
L1393:
        mov ax, 802h
L1396:
        out dx, ax
L1397:
        mov ch, byte ptr [bp - 18h]
L139A:
        and byte ptr es:[di], ch
L139D:
        shr ah, 1
L139F:
        out dx, ax
L13A0:
        mov ch, byte ptr [bp - 13h]
L13A3:
        and byte ptr es:[di], ch
L13A6:
        shr ah, 1
L13A8:
        out dx, ax
L13A9:
        mov ch, byte ptr [bp - 0eh]
L13AC:
        and byte ptr es:[di], ch
L13AF:
        shr ah, 1
L13B1:
        out dx, ax
L13B2:
        mov ch, byte ptr [bp - 9]
L13B5:
        and byte ptr es:[di], ch
L13B8:
        inc di
L13B9:
        test byte ptr [bp - 1bh], 2
L13BD:
        je L13EE
L13BF:
        mov dl, 0ceh
L13C1:
        mov ah, byte ptr [bp - 5]
L13C4:
        mov al, 8
L13C6:
        out dx, ax
L13C7:
        mov dl, 0c4h
L13C9:
        mov ax, 802h
L13CC:
        out dx, ax
L13CD:
        mov ch, byte ptr [bp - 19h]
L13D0:
        and byte ptr es:[di], ch
L13D3:
        shr ah, 1
L13D5:
        out dx, ax
L13D6:
        mov ch, byte ptr [bp - 14h]
L13D9:
        and byte ptr es:[di], ch
L13DC:
        shr ah, 1
L13DE:
        out dx, ax
L13DF:
        mov ch, byte ptr [bp - 0fh]
L13E2:
        and byte ptr es:[di], ch
L13E5:
        shr ah, 1
L13E7:
        out dx, ax
L13E8:
        mov ch, byte ptr [bp - 0ah]
L13EB:
        and byte ptr es:[di], ch
L13EE:
        inc di
L13EF:
        test byte ptr [bp - 1bh], 1
L13F3:
        je L1424
L13F5:
        mov dl, 0ceh
L13F7:
        mov ah, byte ptr [bp - 6]
L13FA:
        mov al, 8
L13FC:
        out dx, ax
L13FD:
        mov dl, 0c4h
L13FF:
        mov ax, 802h
L1402:
        out dx, ax
L1403:
        mov ch, byte ptr [bp - 1ah]
L1406:
        and byte ptr es:[di], ch
L1409:
        shr ah, 1
L140B:
        out dx, ax
L140C:
        mov ch, byte ptr [bp - 15h]
L140F:
        and byte ptr es:[di], ch
L1412:
        shr ah, 1
L1414:
        out dx, ax
L1415:
        mov ch, byte ptr [bp - 10h]
L1418:
        and byte ptr es:[di], ch
L141B:
        shr ah, 1
L141D:
        out dx, ax
L141E:
        mov ch, byte ptr [bp - 0bh]
L1421:
        and byte ptr es:[di], ch
L1424:
        pop di
L1425:
        add di, 28h
L1428:
        dec word ptr cs:[36dh]
L142D:
        je L1432
L142F:
        jmp L123E
L1432:
        call L1F3C
L1435:
        pop di
L1436:
        pop es
L1437:
        mov sp, bp
L1439:
        pop bp
L143A:
        ret
L143B:
        mov word ptr cs:[354h], ds
L1440:
        mov word ptr cs:[356h], bx
L1445:
        ret
L1446:
        ret
L1447:
        ret
L1448:
        push es
L1449:
        push di
L144A:
        mov byte ptr cs:[36fh], cl
L144F:
        mov ax, 28h
L1452:
        mul dl
L1454:
        mov dl, dh
L1456:
        xor dh, dh
L1458:
        add ax, dx
L145A:
        mov di, ax
L145C:
        mov word ptr cs:[344h], di
L1461:
        mov ax, word ptr cs:[334h]
L1465:
        mov es, ax
L1467:
        lodsw
L1468:
        mov word ptr cs:[36bh], ax
L146C:
        lodsw
L146D:
        mov word ptr cs:[36dh], ax
L1471:
        mov dx, 3ceh
L1474:
        mov ax, 805h
L1477:
        out dx, ax
L1478:
        mov ah, 0
L147A:
        mov al, 3
L147C:
        out dx, ax
L147D:
        mov ax, 7
L1480:
        out dx, ax
L1481:
        mov ax, 0ff08h
L1484:
        out dx, ax
L1485:
        mov cx, word ptr cs:[36bh]
L148A:
        test byte ptr cs:[36fh], 1
L1490:
        jne L14D1
L1492:
        push di
L1493:
        push cx
L1494:
        mov dl, 0c4h
L1496:
        mov ax, 102h
L1499:
        out dx, ax
L149A:
        lodsw
L149B:
        xchg al, ah
L149D:
        and word ptr es:[di], ax
L14A0:
        mov ax, 202h
L14A3:
        out dx, ax
L14A4:
        lodsw
L14A5:
        xchg al, ah
L14A7:
        and word ptr es:[di], ax
L14AA:
        mov ax, 402h
L14AD:
        out dx, ax
L14AE:
        lodsw
L14AF:
        xchg al, ah
L14B1:
        and word ptr es:[di], ax
L14B4:
        mov ax, 802h
L14B7:
        out dx, ax
L14B8:
        lodsw
L14B9:
        xchg al, ah
L14BB:
        and word ptr es:[di], ax
L14BE:
        inc di
L14BF:
        inc di
L14C0:
        loop L1496
L14C2:
        pop cx
L14C3:
        pop di
L14C4:
        add di, 28h
L14C7:
        dec word ptr cs:[36dh]
L14CC:
        jne L1492
L14CE:
        jmp L1559
L14D1:
        push di
L14D2:
        push cx
L14D3:
        mov dl, 0c4h
L14D5:
        mov ax, word ptr [si]
L14D7:
        or ax, word ptr [si + 2]
L14DA:
        or ax, word ptr [si + 4]
L14DD:
        or ax, word ptr [si + 6]
L14E0:
        mov bx, ax
L14E2:
        mov al, 8
L14E4:
        mov dl, 0ceh
L14E6:
        out dx, ax
L14E7:
        mov dl, 0c4h
L14E9:
        mov ax, 102h
L14EC:
        out dx, ax
L14ED:
        mov al, byte ptr [si + 1]
L14F0:
        and byte ptr es:[di], al
L14F3:
        mov ax, 202h
L14F6:
        out dx, ax
L14F7:
        mov al, byte ptr [si + 3]
L14FA:
        and byte ptr es:[di], al
L14FD:
        mov ax, 402h
L1500:
        out dx, ax
L1501:
        mov al, byte ptr [si + 5]
L1504:
        and byte ptr es:[di], al
L1507:
        mov ax, 802h
L150A:
        out dx, ax
L150B:
        mov al, byte ptr [si + 7]
L150E:
        and byte ptr es:[di], al
L1511:
        inc di
L1512:
        mov ah, bl
L1514:
        mov al, 8
L1516:
        mov dl, 0ceh
L1518:
        out dx, ax
L1519:
        mov dl, 0c4h
L151B:
        mov ax, 102h
L151E:
        out dx, ax
L151F:
        lodsb
L1520:
        and byte ptr es:[di], al
L1523:
        mov ax, 202h
L1526:
        out dx, ax
L1527:
        mov al, byte ptr [si + 1]
L152A:
        and byte ptr es:[di], al
L152D:
        mov ax, 402h
L1530:
        out dx, ax
L1531:
        mov al, byte ptr [si + 3]
L1534:
        and byte ptr es:[di], al
L1537:
        mov ax, 802h
L153A:
        out dx, ax
L153B:
        mov al, byte ptr [si + 5]
L153E:
        and byte ptr es:[di], al
L1541:
        inc di
L1542:
        add si, 7
L1545:
        dec cx
L1546:
        je L154A
L1548:
        jmp short L14D5
L154A:
        pop cx
L154B:
        pop di
L154C:
        add di, 28h
L154F:
        dec word ptr cs:[36dh]
L1554:
        je L1559
L1556:
        jmp L14D1
L1559:
        call L1F3C
L155C:
        pop di
L155D:
        pop es
L155E:
        ret
L155F:
        mov word ptr cs:[3ach], bx
L1564:
        mov al, 0
L1566:
        mov ah, 3dh
L1568:
        int 21h
L156A:
        jae L1571
L156C:
        mov ax, 0
L156F:
        jmp short L1593
L1571:
        push ds
L1572:
        mov word ptr cs:[3aah], ax
L1576:
        mov bx, ax
L1578:
        mov cx, 0ffffh
L157B:
        mov ax, word ptr cs:[3ach]
L157F:
        mov ds, ax
L1581:
        xor dx, dx
L1583:
        mov ah, 3fh
L1585:
        int 21h
L1587:
        pop ds
L1588:
        push ax
L1589:
        mov bx, word ptr cs:[3aah]
L158E:
        mov ah, 3eh
L1590:
        int 21h
L1592:
        pop ax
L1593:
        ret
L1594:
        push bp
L1595:
        mov ah, 1
L1597:
        int 16h
L1599:
        jne L159F
L159B:
        xor ax, ax
L159D:
        jmp short L15A3
L159F:
        mov ah, 0
L15A1:
        int 16h
L15A3:
        pop bp
L15A4:
        ret
L15A5:
        push es
L15A6:
        push di
L15A7:
        mov ax, 28h
L15AA:
        mul dl
L15AC:
        mov dl, dh
L15AE:
        xor dh, dh
L15B0:
        add ax, dx
L15B2:
        mov di, ax
L15B4:
        mov word ptr cs:[344h], di
L15B9:
        mov ax, word ptr cs:[334h]
L15BD:
        mov es, ax
L15BF:
        mov dx, 3ceh
L15C2:
        mov ax, 0a05h
L15C5:
        out dx, ax
L15C6:
        mov ah, 0
L15C8:
        mov al, 3
L15CA:
        out dx, ax
L15CB:
        mov ax, 7
L15CE:
        out dx, ax
L15CF:
        xor ax, ax
L15D1:
        lodsb
L15D2:
        and al, al
L15D4:
        je L1620
L15D6:
        cmp al, 0dh
L15D8:
        jne L15EA
L15DA:
        mov di, word ptr cs:[344h]
L15DF:
        add di, 140h
L15E3:
        mov word ptr cs:[344h], di
L15E8:
        jmp short L15CF
L15EA:
        push ds
L15EB:
        push si
L15EC:
        sub al, 20h
L15EE:
        shl ax, 1
L15F0:
        shl ax, 1
L15F2:
        shl ax, 1
L15F4:
        add ax, 3feh
L15F7:
        mov si, ax
L15F9:
        mov ax, cs
L15FB:
        mov ds, ax
L15FD:
        push di
L15FE:
        mov cx, 8
L1601:
        mov al, 8
L1603:
        mov ah, byte ptr [si]
L1605:
        inc si
L1606:
        out dx, ax
L1607:
        and byte ptr es:[di], bl
L160A:
        cmp bh, 0ffh
L160D:
        je L1615
L160F:
        not ah
L1611:
        out dx, ax
L1612:
        and byte ptr es:[di], bh
L1615:
        add di, 28h
L1618:
        loop L1603
L161A:
        pop di
L161B:
        inc di
L161C:
        pop si
L161D:
        pop ds
L161E:
        jmp short L15CF
L1620:
        call L1F3C
L1623:
        pop di
L1624:
        pop es
L1625:
        ret
L1626:
        push ds
L1627:
        push cs
L1628:
        pop ds
L1629:
        xor bx, bx
L162B:
        test word ptr ds:[38ah], 1
L1631:
        je L167C
L1633:
        mov al, byte ptr ds:[39ch]
L1636:
        not al
L1638:
        and al, 30h
L163A:
        mov bl, al
L163C:
        shl bl, 1
L163E:
        shl bl, 1
L1640:
        mov ax, word ptr ds:[38ch]
L1643:
        cmp ax, word ptr ds:[390h]
L1647:
        jl L164C
L1649:
        or bl, 4
L164C:
        shl ax, 1
L164E:
        add ax, word ptr ds:[38ch]
L1652:
        cmp ax, word ptr ds:[390h]
L1656:
        jg L165B
L1658:
        or bl, 8
L165B:
        mov ax, word ptr ds:[38eh]
L165E:
        cmp ax, word ptr ds:[392h]
L1662:
        jl L1667
L1664:
        or bl, 1
L1667:
        shl ax, 1
L1669:
        add ax, word ptr ds:[38eh]
L166D:
        cmp ax, word ptr ds:[392h]
L1671:
        jg L1676
L1673:
        or bl, 2
L1676:
        mov al, bl
L1678:
        and al, al
L167A:
        jne L16E4
L167C:
        xor ax, ax
L167E:
        test byte ptr ds:[3a0h], 1
L1683:
        je L1687
L1685:
        or al, 40h
L1687:
        test byte ptr ds:[3a4h], 1
L168C:
        je L1690
L168E:
        or al, 80h
L1690:
        test byte ptr ds:[39dh], 1
L1695:
        je L169B
L1697:
        or al, 5
L1699:
        jmp short L16E4
L169B:
        test byte ptr ds:[39fh], 1
L16A0:
        je L16A6
L16A2:
        or al, 9
L16A4:
        jmp short L16E4
L16A6:
        test byte ptr ds:[3a5h], 1
L16AB:
        je L16B1
L16AD:
        or al, 6
L16AF:
        jmp short L16E4
L16B1:
        test byte ptr ds:[3a7h], 1
L16B6:
        je L16BC
L16B8:
        or al, 0ah
L16BA:
        jmp short L16E4
L16BC:
        test byte ptr ds:[39eh], 1
L16C1:
        je L16C7
L16C3:
        or al, 1
L16C5:
        jmp short L16D0
L16C7:
        test byte ptr ds:[3a6h], 1
L16CC:
        je L16D0
L16CE:
        or al, 2
L16D0:
        test byte ptr ds:[3a1h], 1
L16D5:
        je L16DB
L16D7:
        or al, 4
L16D9:
        jmp short L16E4
L16DB:
        test byte ptr ds:[3a3h], 1
L16E0:
        je L16E4
L16E2:
        or al, 8
L16E4:
        pop ds
L16E5:
        ret
L16E6:
        cli
L16E7:
        mov word ptr cs:[358h], ds
L16EC:
        mov word ptr cs:[361h], si
L16F1:
        sti
L16F2:
        ret
L16F3:
        ret
L16F4:
        mov ax, word ptr cs:[334h]
L16F8:
        ret
L16F9:
        push ds
L16FA:
        push cs
L16FB:
        pop ds
L16FC:
        mov word ptr ds:[38ah], 1
L1702:
        xor ax, ax
L1704:
        add ax, word ptr ds:[390h]
L1708:
        je L1704
L170A:
        cmp ax, 0ffffh
L170D:
        jne L1715
L170F:
        mov word ptr ds:[38ah], 0
L1715:
        shr ax, 1
L1717:
        mov word ptr ds:[38ch], ax
L171A:
        mov ax, word ptr ds:[392h]
L171D:
        shr ax, 1
L171F:
        mov word ptr ds:[38eh], ax
L1722:
        mov ax, word ptr ds:[38ah]
L1725:
        pop ds
L1726:
        ret
L1727:
        mov word ptr cs:[3ach], bx
L172C:
        mov al, 0
L172E:
        mov ah, 3ch
L1730:
        push cx
L1731:
        xor cx, cx
L1733:
        int 21h
L1735:
        pop cx
L1736:
        jae L173B
L1738:
        jmp short L175F
L173A:
        nop
L173B:
        push ds
L173C:
        mov word ptr cs:[3aah], ax
L1740:
        mov bx, ax
L1742:
        mov ax, word ptr cs:[3ach]
L1746:
        mov ds, ax
L1748:
        xor dx, dx
L174A:
        mov ah, 40h
L174C:
        int 21h
L174E:
        pop ds
L174F:
        and ax, ax
L1751:
        je L175F
L1753:
        mov bx, word ptr cs:[3aah]
L1758:
        mov ah, 3eh
L175A:
        int 21h
L175C:
        xor ax, ax
L175E:
        ret
L175F:
        mov bx, word ptr cs:[3aah]
L1764:
        mov ah, 3eh
L1766:
        int 21h
L1768:
        mov ax, 0ffffh
L176B:
        ret
L176C:
        ret
L176D:
        ret
L176E:
        mov byte ptr cs:[370h], 0
L1774:
        push ds
L1775:
        push es
L1776:
        mov word ptr cs:[3b0h], bx
L177B:
        mov ax, dx
L177D:
        and ax, 0fh
L1780:
        mov word ptr cs:[3b2h], ax
L1784:
        mov word ptr cs:[3b4h], ax
L1788:
        shr dx, 1
L178A:
        shr dx, 1
L178C:
        shr dx, 1
L178E:
        shr dx, 1
L1790:
        lodsw
L1791:
        push ax
L1792:
        mov bx, ax
L1794:
        mov ax, cx
L1796:
        shr ax, 1
L1798:
        shr ax, 1
L179A:
        shr ax, 1
L179C:
        shr ax, 1
L179E:
        sub bx, ax
L17A0:
        mov ax, 28h
L17A3:
        sub ax, di
L17A5:
        shr ax, 1
L17A7:
        mov word ptr cs:[3aeh], ax
L17AB:
        cmp bx, ax
L17AD:
        jg L17B4
L17AF:
        mov word ptr cs:[3aeh], bx
L17B4:
        pop bx
L17B5:
        mov ax, bx
L17B7:
        shl bx, 1
L17B9:
        push dx
L17BA:
        mul dx
L17BC:
        pop dx
L17BD:
        shr cx, 1
L17BF:
        shr cx, 1
L17C1:
        shr cx, 1
L17C3:
        push cx
L17C4:
        test cx, 1
L17C8:
        je L17D0
L17CA:
        mov byte ptr cs:[370h], 1
L17D0:
        shr cx, 1
L17D2:
        add ax, cx
L17D4:
        mov cx, word ptr [si]
L17D6:
        sub cx, dx
L17D8:
        add si, 2
L17DB:
        add si, ax
L17DD:
        add si, ax
L17DF:
        test di, 1
L17E3:
        je L17FC
L17E5:
        mov ax, word ptr cs:[3aeh]
L17E9:
        shl ax, 1
L17EB:
        cmp ax, bx
L17ED:
        je L17FC
L17EF:
        add ax, di
L17F1:
        cmp ax, 27h
L17F4:
        jne L17FC
L17F6:
        mov byte ptr cs:[370h], 1
L17FC:
        mov ax, 28h
L17FF:
        mov dx, es
L1801:
        and dx, dx
L1803:
        je L182A
L1805:
        mul dx
L1807:
        add di, ax
L1809:
        mov dx, es
L180B:
        add dx, 0fh
L180E:
        shr dx, 1
L1810:
        shr dx, 1
L1812:
        shr dx, 1
L1814:
        shr dx, 1
L1816:
        sub cx, dx
L1818:
        mov dx, es
L181A:
        neg dx
L181C:
        and dx, 0fh
L181F:
        mov word ptr cs:[3b4h], dx
L1824:
        mov byte ptr cs:[371h], 1
L182A:
        mov byte ptr cs:[371h], 0
L1830:
        cmp cx, 0ah
L1833:
        jle L183E
L1835:
        mov cx, 0ah
L1838:
        mov byte ptr cs:[371h], 1
L183E:
        mov dx, 3ceh
L1841:
        mov ax, 105h
L1844:
        out dx, ax
L1845:
        mov es, word ptr cs:[334h]
L184A:
        pop ax
L184B:
        test ax, 1
L184E:
        je L1853
L1850:
        jmp L18DF
L1853:
        test word ptr cs:[3b2h], 0ffffh
L185A:
        je L188B
L185C:
        dec cx
L185D:
        push cx
L185E:
        push si
L185F:
        push di
L1860:
        mov cx, word ptr cs:[3aeh]
L1865:
        and cx, cx
L1867:
        je L1874
L1869:
        call L1974
L186C:
        test byte ptr cs:[370h], 1
L1872:
        je L1877
L1874:
        call L1A54
L1877:
        mov ax, 10h
L187A:
        sub ax, word ptr cs:[3b2h]
L187F:
        mov di, 28h
L1882:
        mul di
L1884:
        pop di
L1885:
        add di, ax
L1887:
        pop si
L1888:
        add si, bx
L188A:
        pop cx
L188B:
        and cx, cx
L188D:
        je L18B4
L188F:
        push cx
L1890:
        push si
L1891:
        push di
L1892:
        mov cx, word ptr cs:[3aeh]
L1897:
        and cx, cx
L1899:
        je L18A6
L189B:
        call L19B3
L189E:
        test byte ptr cs:[370h], 1
L18A4:
        je L18A9
L18A6:
        call L1ABE
L18A9:
        pop di
L18AA:
        add di, 280h
L18AE:
        pop si
L18AF:
        add si, bx
L18B1:
        pop cx
L18B2:
        loop L188B
L18B4:
        test byte ptr cs:[371h], 1
L18BA:
        je L18DC
L18BC:
        test word ptr cs:[3b4h], 0ffffh
L18C3:
        je L18DC
L18C5:
        mov cx, word ptr cs:[3aeh]
L18CA:
        and cx, cx
L18CC:
        je L18D9
L18CE:
        call L19E2
L18D1:
        test byte ptr cs:[370h], 1
L18D7:
        je L18DC
L18D9:
        call L1B20
L18DC:
        jmp L196E
L18DF:
        test word ptr cs:[3b2h], 0ffffh
L18E6:
        je L1915
L18E8:
        dec cx
L18E9:
        push cx
L18EA:
        push si
L18EB:
        push di
L18EC:
        call L1A19
L18EF:
        mov cx, word ptr cs:[3aeh]
L18F4:
        and cx, cx
L18F6:
        je L18FE
L18F8:
        dec cx
L18F9:
        je L1901
L18FB:
        call L1974
L18FE:
        call L1A54
L1901:
        mov ax, 10h
L1904:
        sub ax, word ptr cs:[3b2h]
L1909:
        mov di, 28h
L190C:
        mul di
L190E:
        pop di
L190F:
        add di, ax
L1911:
        pop si
L1912:
        add si, bx
L1914:
        pop cx
L1915:
        and cx, cx
L1917:
        je L194C
L1919:
        push cx
L191A:
        push si
L191B:
        push di
L191C:
        call L1A8F
L191F:
        mov cx, word ptr cs:[3aeh]
L1924:
        and cx, cx
L1926:
        je L1936
L1928:
        dec cx
L1929:
        je L1939
L192B:
        call L19B3
L192E:
        test byte ptr cs:[370h], 1
L1934:
        je L1939
L1936:
        call L1ABE
L1939:
        pop di
L193A:
        add di, 280h
L193E:
        pop si
L193F:
        add si, bx
L1941:
        pop cx
L1942:
        loop L1915
L1944:
        test byte ptr cs:[371h], 1
L194A:
        je L196E
L194C:
        test byte ptr cs:[371h], 1
L1952:
        je L196E
L1954:
        test word ptr cs:[3b4h], 0ffffh
L195B:
        je L196E
L195D:
        call L1AED
L1960:
        mov cx, word ptr cs:[3aeh]
L1965:
        dec cx
L1966:
        je L196B
L1968:
        call L19E2
L196B:
        call L1B20
L196E:
        call L1F3C
L1971:
        pop es
L1972:
        pop ds
L1973:
        ret
L1974:
        lodsw
L1975:
        and ax, 0fffh
L1978:
        je L19AD
L197A:
        dec ax
L197B:
        push ds
L197C:
        push si
L197D:
        shl ax, 1
L197F:
        shl ax, 1
L1981:
        shl ax, 1
L1983:
        shl ax, 1
L1985:
        add ax, word ptr cs:[3b2h]
L198A:
        shl ax, 1
L198C:
        mov si, ax
L198E:
        mov ds, word ptr cs:[33ch]
L1993:
        add si, word ptr cs:[3b0h]
L1998:
        push cx
L1999:
        push di
L199A:
        mov cx, 10h
L199D:
        sub cx, word ptr cs:[3b2h]
L19A2:
        movsb
L19A3:
        movsb
L19A4:
        add di, 26h
L19A7:
        loop L19A2
L19A9:
        pop di
L19AA:
        pop cx
L19AB:
        pop si
L19AC:
        pop ds
L19AD:
        add di, 2
L19B0:
        loop L1974
L19B2:
        ret
L19B3:
        lodsw
L19B4:
        and ax, 0fffh
L19B7:
        je L19DC
L19B9:
        dec ax
L19BA:
        push ds
L19BB:
        push si
L19BC:
        push cx
L19BD:
        push di
L19BE:
        mov cl, 5
L19C0:
        shl ax, cl
L19C2:
        mov si, ax
L19C4:
        mov ds, word ptr cs:[33ch]
L19C9:
        add si, word ptr cs:[3b0h]
L19CE:
        mov cx, 10h
L19D1:
        movsb
L19D2:
        movsb
L19D3:
        add di, 26h
L19D6:
        loop L19D1
L19D8:
        pop di
L19D9:
        pop cx
L19DA:
        pop si
L19DB:
        pop ds
L19DC:
        add di, 2
L19DF:
        loop L19B3
L19E1:
        ret
L19E2:
        lodsw
L19E3:
        and ax, 0fffh
L19E6:
        je L1A13
L19E8:
        dec ax
L19E9:
        push ds
L19EA:
        push si
L19EB:
        shl ax, 1
L19ED:
        shl ax, 1
L19EF:
        shl ax, 1
L19F1:
        shl ax, 1
L19F3:
        shl ax, 1
L19F5:
        mov si, ax
L19F7:
        mov ds, word ptr cs:[33ch]
L19FC:
        add si, word ptr cs:[3b0h]
L1A01:
        push cx
L1A02:
        push di
L1A03:
        mov cx, word ptr cs:[3b4h]
L1A08:
        movsb
L1A09:
        movsb
L1A0A:
        add di, 26h
L1A0D:
        loop L1A08
L1A0F:
        pop di
L1A10:
        pop cx
L1A11:
        pop si
L1A12:
        pop ds
L1A13:
        add di, 2
L1A16:
        loop L19E2
L1A18:
        ret
L1A19:
        lodsw
L1A1A:
        and ax, 0fffh
L1A1D:
        je L1A52
L1A1F:
        dec ax
L1A20:
        push ds
L1A21:
        push si
L1A22:
        shl ax, 1
L1A24:
        shl ax, 1
L1A26:
        shl ax, 1
L1A28:
        shl ax, 1
L1A2A:
        add ax, word ptr cs:[3b2h]
L1A2F:
        shl ax, 1
L1A31:
        mov si, ax
L1A33:
        mov ds, word ptr cs:[33ch]
L1A38:
        add si, word ptr cs:[3b0h]
L1A3D:
        push cx
L1A3E:
        push di
L1A3F:
        mov cx, 10h
L1A42:
        sub cx, word ptr cs:[3b2h]
L1A47:
        inc si
L1A48:
        movsb
L1A49:
        add di, 27h
L1A4C:
        loop L1A47
L1A4E:
        pop di
L1A4F:
        pop cx
L1A50:
        pop si
L1A51:
        pop ds
L1A52:
        inc di
L1A53:
        ret
L1A54:
        lodsw
L1A55:
        and ax, 0fffh
L1A58:
        je L1A8D
L1A5A:
        dec ax
L1A5B:
        push ds
L1A5C:
        push si
L1A5D:
        shl ax, 1
L1A5F:
        shl ax, 1
L1A61:
        shl ax, 1
L1A63:
        shl ax, 1
L1A65:
        add ax, word ptr cs:[3b2h]
L1A6A:
        shl ax, 1
L1A6C:
        mov si, ax
L1A6E:
        mov ds, word ptr cs:[33ch]
L1A73:
        add si, word ptr cs:[3b0h]
L1A78:
        push cx
L1A79:
        push di
L1A7A:
        mov cx, 10h
L1A7D:
        sub cx, word ptr cs:[3b2h]
L1A82:
        movsb
L1A83:
        inc si
L1A84:
        add di, 27h
L1A87:
        loop L1A82
L1A89:
        pop di
L1A8A:
        pop cx
L1A8B:
        pop si
L1A8C:
        pop ds
L1A8D:
        inc di
L1A8E:
        ret
L1A8F:
        lodsw
L1A90:
        and ax, 0fffh
L1A93:
        je L1ABC
L1A95:
        dec ax
L1A96:
        push ds
L1A97:
        push si
L1A98:
        shl ax, 1
L1A9A:
        shl ax, 1
L1A9C:
        shl ax, 1
L1A9E:
        shl ax, 1
L1AA0:
        shl ax, 1
L1AA2:
        mov si, ax
L1AA4:
        mov ds, word ptr cs:[33ch]
L1AA9:
        add si, word ptr cs:[3b0h]
L1AAE:
        push di
L1AAF:
        mov cx, 10h
L1AB2:
        inc si
L1AB3:
        movsb
L1AB4:
        add di, 27h
L1AB7:
        loop L1AB2
L1AB9:
        pop di
L1ABA:
        pop si
L1ABB:
        pop ds
L1ABC:
        inc di
L1ABD:
        ret
L1ABE:
        lodsw
L1ABF:
        and ax, 0fffh
L1AC2:
        je L1AEB
L1AC4:
        dec ax
L1AC5:
        push ds
L1AC6:
        push si
L1AC7:
        shl ax, 1
L1AC9:
        shl ax, 1
L1ACB:
        shl ax, 1
L1ACD:
        shl ax, 1
L1ACF:
        shl ax, 1
L1AD1:
        mov si, ax
L1AD3:
        mov ds, word ptr cs:[33ch]
L1AD8:
        add si, word ptr cs:[3b0h]
L1ADD:
        push di
L1ADE:
        mov cx, 10h
L1AE1:
        movsb
L1AE2:
        inc si
L1AE3:
        add di, 27h
L1AE6:
        loop L1AE1
L1AE8:
        pop di
L1AE9:
        pop si
L1AEA:
        pop ds
L1AEB:
        inc di
L1AEC:
        ret
L1AED:
        lodsw
L1AEE:
        and ax, 0fffh
L1AF1:
        je L1B1E
L1AF3:
        dec ax
L1AF4:
        push ds
L1AF5:
        push si
L1AF6:
        shl ax, 1
L1AF8:
        shl ax, 1
L1AFA:
        shl ax, 1
L1AFC:
        shl ax, 1
L1AFE:
        shl ax, 1
L1B00:
        mov si, ax
L1B02:
        mov ds, word ptr cs:[33ch]
L1B07:
        add si, word ptr cs:[3b0h]
L1B0C:
        push cx
L1B0D:
        push di
L1B0E:
        mov cx, word ptr cs:[3b4h]
L1B13:
        inc si
L1B14:
        movsb
L1B15:
        add di, 27h
L1B18:
        loop L1B13
L1B1A:
        pop di
L1B1B:
        pop cx
L1B1C:
        pop si
L1B1D:
        pop ds
L1B1E:
        inc di
L1B1F:
        ret
L1B20:
        lodsw
L1B21:
        and ax, 0fffh
L1B24:
        je L1B51
L1B26:
        dec ax
L1B27:
        push ds
L1B28:
        push si
L1B29:
        shl ax, 1
L1B2B:
        shl ax, 1
L1B2D:
        shl ax, 1
L1B2F:
        shl ax, 1
L1B31:
        shl ax, 1
L1B33:
        mov si, ax
L1B35:
        mov ds, word ptr cs:[33ch]
L1B3A:
        add si, word ptr cs:[3b0h]
L1B3F:
        push cx
L1B40:
        push di
L1B41:
        mov cx, word ptr cs:[3b4h]
L1B46:
        movsb
L1B47:
        inc si
L1B48:
        add di, 27h
L1B4B:
        loop L1B46
L1B4D:
        pop di
L1B4E:
        pop cx
L1B4F:
        pop si
L1B50:
        pop ds
L1B51:
        ret
L1B52:
        mov ah, 7
L1B54:
        int 21h
L1B56:
        and ax, 7fh
L1B59:
        ret
L1B5A:
        ret
L1B5B:
        mov ax, 21h
L1B5E:
        ret
L1B5F:
        push cx
L1B60:
        call L155F
L1B63:
        pop di
L1B64:
        shr ax, 1
L1B66:
        shr ax, 1
L1B68:
        push ax
L1B69:
        shr ax, 1
L1B6B:
        mov cx, ax
L1B6D:
        mov ax, word ptr cs:[3ach]
L1B71:
        mov ds, ax
L1B73:
        xor si, si
L1B75:
        mov ax, 0a000h
L1B78:
        mov es, ax
L1B7A:
        mov dx, 3ceh
L1B7D:
        mov ax, 805h
L1B80:
        out dx, ax
L1B81:
        mov ah, 0
L1B83:
        mov al, 3
L1B85:
        out dx, ax
L1B86:
        mov ax, 7
L1B89:
        out dx, ax
L1B8A:
        mov ax, 0ff08h
L1B8D:
        out dx, ax
L1B8E:
        mov dl, 0c4h
L1B90:
        mov ax, 102h
L1B93:
        out dx, ax
L1B94:
        lodsw
L1B95:
        and word ptr es:[di], ax
L1B98:
        mov ax, 202h
L1B9B:
        out dx, ax
L1B9C:
        lodsw
L1B9D:
        and word ptr es:[di], ax
L1BA0:
        mov ax, 402h
L1BA3:
        out dx, ax
L1BA4:
        lodsw
L1BA5:
        and word ptr es:[di], ax
L1BA8:
        mov ax, 802h
L1BAB:
        out dx, ax
L1BAC:
        lodsw
L1BAD:
        and word ptr es:[di], ax
L1BB0:
        inc di
L1BB1:
        inc di
L1BB2:
        loop L1B8E
L1BB4:
        call L1F3C
L1BB7:
        pop ax
L1BB8:
        ret
L1BB9:
        mov ax, 28h
L1BBC:
        mul dl
L1BBE:
        mov dl, dh
L1BC0:
        xor dh, dh
L1BC2:
        add ax, dx
L1BC4:
        mov di, ax
L1BC6:
        mov word ptr cs:[344h], di
L1BCB:
        mov ax, word ptr cs:[334h]
L1BCF:
        mov es, ax
L1BD1:
        mov dx, 3ceh
L1BD4:
        mov ax, 0a05h
L1BD7:
        out dx, ax
L1BD8:
        mov ah, 0
L1BDA:
        mov al, 3
L1BDC:
        out dx, ax
L1BDD:
        mov ax, 7
L1BE0:
        out dx, ax
L1BE1:
        mov ax, 0ff08h
L1BE4:
        out dx, ax
L1BE5:
        mov al, cl
L1BE7:
        mov cl, ch
L1BE9:
        xor ch, ch
L1BEB:
        push cx
L1BEC:
        push di
L1BED:
        and byte ptr es:[di], bl
L1BF0:
        inc di
L1BF1:
        loop L1BED
L1BF3:
        pop di
L1BF4:
        pop cx
L1BF5:
        add di, 28h
L1BF8:
        dec al
L1BFA:
        jne L1BEB
L1BFC:
        call L1F3C
L1BFF:
        ret
L1C00:
        push dx
L1C01:
        call L1F3C
L1C04:
        pop dx
L1C05:
        mov bh, dh
L1C07:
        mov ax, 28h
L1C0A:
        mul dl
L1C0C:
        mov dx, cx
L1C0E:
        shr dx, 1
L1C10:
        shr dx, 1
L1C12:
        shr dx, 1
L1C14:
        add ax, dx
L1C16:
        mov di, ax
L1C18:
        mov word ptr cs:[344h], di
L1C1D:
        mov ax, word ptr cs:[334h]
L1C21:
        mov es, ax
L1C23:
        mov dx, 3ceh
L1C26:
        xor al, al
L1C28:
        mov ah, bh
L1C2A:
        out dx, ax
L1C2B:
        mov ax, 0f01h
L1C2E:
        out dx, ax
L1C2F:
        mov ah, 0
L1C31:
        mov al, 3
L1C33:
        out dx, ax
L1C34:
        and cl, 7
L1C37:
        mov ah, 80h
L1C39:
        shr ah, cl
L1C3B:
        mov al, 8
L1C3D:
        out dx, ax
L1C3E:
        mov cl, bl
L1C40:
        xor ch, ch
L1C42:
        or byte ptr es:[di], al
L1C45:
        add di, 28h
L1C48:
        loop L1C42
L1C4A:
        xor ax, ax
L1C4C:
        out dx, ax
L1C4D:
        inc ax
L1C4E:
        out dx, ax
L1C4F:
        call L1F3C
L1C52:
        ret
L1C53:
        add si, 2
L1C56:
        push es
L1C57:
        push di
L1C58:
        push si
L1C59:
        push cx
L1C5A:
        push dx
L1C5B:
        push ds
L1C5C:
        pop es
L1C5D:
        mov dx, si
L1C5F:
        mov di, si
L1C61:
        add di, 20h
L1C64:
        mov cx, 3e80h
L1C67:
        mov ax, word ptr es:[di]
L1C6A:
        xchg ah, al
L1C6C:
        stosw
L1C6D:
        loop L1C67
L1C6F:
        call L0E1E
L1C72:
        pop dx
L1C73:
        pop cx
L1C74:
        pop si
L1C75:
        pop di
L1C76:
        pop es
L1C77:
        add si, 1ch
L1C7A:
        mov word ptr [si], 14h
L1C7E:
        mov word ptr [si + 2], 0c8h
L1C83:
        jmp L1448
L1C86:
        ret
L1C87:
        xor cx, cx
L1C89:
        xor dx, dx
L1C8B:
        mov ax, 0bh
L1C8E:
        int 33h
L1C90:
        push dx
L1C91:
        push cx
L1C92:
        call L1626
L1C95:
        pop cx
L1C96:
        pop dx
L1C97:
        cmp byte ptr cs:[352h], 0
L1C9D:
        je L1CA1
L1C9F:
        jmp short L1CC7
L1CA1:
        mov byte ptr cs:[352h], 1
L1CA7:
        test al, 1
L1CA9:
        je L1CB0
L1CAB:
        sub dx, 4
L1CAE:
        jmp short L1CB7
L1CB0:
        test al, 2
L1CB2:
        je L1CB7
L1CB4:
        add dx, 4
L1CB7:
        test al, 4
L1CB9:
        je L1CC0
L1CBB:
        sub cx, 4
L1CBE:
        jmp short L1CC7
L1CC0:
        test al, 8
L1CC2:
        je L1CC7
L1CC4:
        add cx, 4
L1CC7:
        and al, 0c0h
L1CC9:
        rol al, 1
L1CCB:
        rol al, 1
L1CCD:
        add cx, word ptr cs:[37ah]
L1CD2:
        jns L1CD8
L1CD4:
        xor cx, cx
L1CD6:
        jmp short L1CE1
L1CD8:
        cmp cx, 13fh
L1CDC:
        jle L1CE1
L1CDE:
        mov cx, 13fh
L1CE1:
        add dx, word ptr cs:[37ch]
L1CE6:
        jns L1CEC
L1CE8:
        xor dx, dx
L1CEA:
        jmp short L1CF5
L1CEC:
        cmp dx, 0c7h
L1CF0:
        jle L1CF5
L1CF2:
        mov dx, 0c7h
L1CF5:
        mov word ptr cs:[382h], cx
L1CFA:
        mov word ptr cs:[384h], dx
L1CFF:
        mov word ptr cs:[37ah], cx
L1D04:
        mov word ptr cs:[37ch], dx
L1D09:
        push dx
L1D0A:
        push cx
L1D0B:
        push ax
L1D0C:
        mov ax, 3
L1D0F:
        int 33h
L1D11:
        pop ax
L1D12:
        pop cx
L1D13:
        pop dx
L1D14:
        or bl, al
L1D16:
        ret
L1D17:
        cmp word ptr cs:[386h], 0
L1D1D:
        jne L1D20
L1D1F:
        ret
L1D20:
        push bp
L1D21:
        push es
L1D22:
        push di
L1D23:
        push bx
L1D24:
        push cx
L1D25:
        push ds
L1D26:
        push cs
L1D27:
        pop ds
L1D28:
        push ds
L1D29:
        push dx
L1D2A:
        mov dx, 3ceh
L1D2D:
        mov ax, 105h
L1D30:
        out dx, ax
L1D31:
        pop dx
L1D32:
        mov byte ptr ds:[376h], 1
L1D37:
        xor bh, bh
L1D39:
        mov bl, byte ptr ds:[388h]
L1D3D:
        mov cx, word ptr ds:[382h]
L1D41:
        mov dx, word ptr ds:[384h]
L1D45:
        mov ax, 0c8h
L1D48:
        sub ax, dx
L1D4A:
        cmp ax, bx
L1D4C:
        jge L1D50
L1D4E:
        mov bx, ax
L1D50:
        mov byte ptr ds:[377h], bl
L1D54:
        mov ax, 28h
L1D57:
        mul dx
L1D59:
        sar cx, 1
L1D5B:
        sar cx, 1
L1D5D:
        sar cx, 1
L1D5F:
        add ax, cx
L1D61:
        mov word ptr ds:[374h], ax
L1D64:
        mov si, ax
L1D66:
        mov di, 1f40h
L1D69:
        mov ax, word ptr ds:[336h]
L1D6C:
        mov ds, ax
L1D6E:
        mov es, ax
L1D70:
        mov cx, bx
L1D72:
        movsb
L1D73:
        movsb
L1D74:
        movsb
L1D75:
        movsb
L1D76:
        movsb
L1D77:
        add si, 23h
L1D7A:
        loop L1D72
L1D7C:
        pop ds
L1D7D:
        push word ptr ds:[334h]
L1D81:
        mov ax, word ptr ds:[336h]
L1D84:
        mov word ptr ds:[334h], ax
L1D87:
        mov si, word ptr ds:[37eh]
L1D8B:
        mov bl, byte ptr ds:[377h]
L1D8F:
        mov cx, word ptr ds:[382h]
L1D93:
        mov dx, word ptr ds:[384h]
L1D97:
        mov ds, word ptr ds:[380h]
L1D9B:
        cmp bh, 2
L1D9E:
        je L1DA5
L1DA0:
        call L0E28
L1DA3:
        jmp short L1DA8
L1DA5:
        call L1080
L1DA8:
        pop word ptr cs:[334h]
L1DAD:
        pop ds
L1DAE:
        pop cx
L1DAF:
        pop bx
L1DB0:
        pop di
L1DB1:
        pop es
L1DB2:
        pop bp
L1DB3:
        ret
L1DB4:
        and bh, bh
L1DB6:
        jns L1DC6
L1DB8:
        mov byte ptr cs:[376h], 0
L1DBE:
        mov word ptr cs:[386h], 0
L1DC5:
        ret
L1DC6:
        push ds
L1DC7:
        push cs
L1DC8:
        pop ds
L1DC9:
        mov al, byte ptr ds:[376h]
L1DCC:
        and al, al
L1DCE:
        je L1E07
L1DD0:
        push dx
L1DD1:
        push ds
L1DD2:
        push es
L1DD3:
        push si
L1DD4:
        push bx
L1DD5:
        push cx
L1DD6:
        mov dx, 3ceh
L1DD9:
        mov ax, 105h
L1DDC:
        out dx, ax
L1DDD:
        mov byte ptr ds:[376h], 0
L1DE2:
        mov di, word ptr ds:[374h]
L1DE6:
        mov ax, word ptr ds:[336h]
L1DE9:
        mov ds, ax
L1DEB:
        mov es, ax
L1DED:
        mov si, 1f40h
L1DF0:
        mov cl, byte ptr cs:[377h]
L1DF5:
        xor ch, ch
L1DF7:
        movsb
L1DF8:
        movsb
L1DF9:
        movsb
L1DFA:
        movsb
L1DFB:
        movsb
L1DFC:
        add di, 23h
L1DFF:
        loop L1DF7
L1E01:
        pop cx
L1E02:
        pop bx
L1E03:
        pop si
L1E04:
        pop es
L1E05:
        pop ds
L1E06:
        pop dx
L1E07:
        and bh, 40h
L1E0A:
        jne L1E2C
L1E0C:
        mov word ptr ds:[386h], 1
L1E12:
        mov byte ptr ds:[388h], bl
L1E16:
        mov word ptr ds:[37eh], si
L1E1A:
        mov word ptr ds:[382h], cx
L1E1E:
        mov word ptr ds:[384h], dx
L1E22:
        pop ds
L1E23:
        push ds
L1E24:
        mov word ptr cs:[380h], ds
L1E29:
        call L1D17
L1E2C:
        pop ds
L1E2D:
        ret
L1E2E:
        and bx, bx
L1E30:
        js L1E74
L1E32:
        mov ax, word ptr cs:[334h]
L1E36:
        mov word ptr cs:[332h], ax
L1E3A:
        jne L1E5F
L1E3C:
        mov bx, word ptr cs:[338h]
L1E41:
        mov al, byte ptr cs:[330h]
L1E45:
        and al, 1
L1E47:
        je L1E4E
L1E49:
        mov bx, word ptr cs:[33ah]
L1E4E:
        mov word ptr cs:[334h], bx
L1E53:
        mov word ptr cs:[336h], bx
L1E58:
        mov byte ptr cs:[376h], 0
L1E5E:
        ret
L1E5F:
        mov ax, 200h
L1E62:
        imul bx
L1E64:
        add ax, 0a000h
L1E67:
        mov word ptr cs:[334h], ax
L1E6B:
        mov word ptr cs:[338h], ax
L1E6F:
        mov word ptr cs:[33ah], ax
L1E73:
        ret
L1E74:
        mov ax, word ptr cs:[332h]
L1E78:
        mov word ptr cs:[334h], ax
L1E7C:
        mov word ptr cs:[338h], 0a000h
L1E83:
        mov word ptr cs:[33ah], 0a200h
L1E8A:
        ret
L1E8B:
        mov dx, 3ceh
L1E8E:
        mov ax, 105h
L1E91:
        out dx, ax
L1E92:
        mov ax, 200h
L1E95:
        imul bx
L1E97:
        add ax, 0a000h
L1E9A:
        push ds
L1E9B:
        push es
L1E9C:
        push si
L1E9D:
        push di
L1E9E:
        xor di, di
L1EA0:
        xor si, si
L1EA2:
        mov ds, ax
L1EA4:
        mov es, word ptr cs:[334h]
L1EA9:
        mov cx, 1f40h
L1EAC:
        rep movsb
L1EAE:
        pop di
L1EAF:
        pop si
L1EB0:
        pop es
L1EB1:
        pop ds
L1EB2:
        ret
L1EB3:
        push cx
L1EB4:
        mov ax, dx
L1EB6:
        mov cl, bl
L1EB8:
        mov dx, 28h
L1EBB:
        mul dx
L1EBD:
        shr bx, 1
L1EBF:
        shr bx, 1
L1EC1:
        shr bx, 1
L1EC3:
        add bx, ax
L1EC5:
        mov ax, 0a000h
L1EC8:
        mov es, ax
L1ECA:
        and cl, 7
L1ECD:
        xor cl, 7
L1ED0:
        mov ah, 1
L1ED2:
        shl ah, cl
L1ED4:
        mov dx, 3ceh
L1ED7:
        mov al, 8
L1ED9:
        out dx, ax
L1EDA:
        mov ax, 205h
L1EDD:
        out dx, ax
L1EDE:
        mov ah, 0
L1EE0:
        mov al, 3
L1EE2:
        out dx, ax
L1EE3:
        mov al, byte ptr es:[bx]
L1EE6:
        pop ax
L1EE7:
        mov byte ptr es:[bx], al
L1EEA:
        mov ax, 0ff08h
L1EED:
        out dx, ax
L1EEE:
        mov ax, 5
L1EF1:
        out dx, ax
L1EF2:
        mov ax, 3
L1EF5:
        out dx, ax
L1EF6:
        ret
L1EF7:
        push ds
L1EF8:
        push cs
L1EF9:
        pop ds
L1EFA:
        mov bx, word ptr ds:[361h]
L1EFE:
        add bx, 4
L1F01:
        mov word ptr [bx], cx
L1F03:
        mov ax, word ptr [bx]
L1F05:
        and ax, ax
L1F07:
        jns L1F03
L1F09:
        pop ds
L1F0A:
        ret
L1F0B:
        mov word ptr cs:[37ah], cx
L1F10:
        mov word ptr cs:[37ch], dx
L1F15:
        ret
L1F16:
        ret
; CS:1F17
L1F17:
        push ds
L1F18:
        mov bx, 40h
L1F1B:
        mov ds, bx
L1F1D:
        mov bl, 20h
L1F1F:
        mov ah, al
L1F21:
        and ah, 7
L1F24:
        cmp ah, 7
L1F27:
        jne L1F2B
L1F29:
        mov bl, 30h
L1F2B:
        and byte ptr ds:[10h], 0cfh
L1F30:
        or byte ptr ds:[10h], bl
L1F34:
        xor ah, ah
L1F36:
        push bp
L1F37:
        int 10h
L1F39:
        pop bp
L1F3A:
        pop ds
L1F3B:
        ret
L1F3C:
        mov dx, 3c4h
L1F3F:
        mov ax, 0f02h
L1F42:
        out dx, ax
L1F43:
        mov dl, 0ceh
L1F45:
        mov ax, 0ff08h
L1F48:
        out dx, ax
L1F49:
        mov ax, 5
L1F4C:
        out dx, ax
L1F4D:
        mov ax, 3
L1F50:
        out dx, ax
L1F51:
        mov ax, 0f07h
L1F54:
        out dx, ax
L1F55:
        ret
L1F56:
        push dx
L1F57:
        mov dx, word ptr cs:[34ch]
L1F5C:
        add dl, 6
L1F5F:
        in al, dx
L1F60:
        test al, 8
L1F62:
        je L1F5F
L1F64:
        in al, dx
L1F65:
        test al, 8
L1F67:
        jne L1F64
L1F69:
        pop dx
L1F6A:
        ret
L1F6B:
        nop
; CS:1F6C
        DB      6h, 0a1h, 0eh, 3eh, 8bh, 16h, 10h, 3eh, 0a3h, 14h, 43h, 89h, 16h, 16h, 43h, 0b8h
        DB      0ffh, 0ffh, 50h, 2bh, 0c0h, 50h, 0b8h, 0c6h, 0bh, 50h, 0e8h, 0a3h, 11h, 83h, 0c4h, 6h
        DB      0a1h, 0eh, 3eh, 8bh, 16h, 10h, 3eh, 0a3h, 4ah, 40h, 89h, 16h, 4ch, 40h, 0b8h, 6h
        DB      0h, 50h, 0b8h, 0d0h, 0bh, 50h, 8dh, 46h, 80h, 50h, 0e8h, 0fbh, 1fh, 83h, 0c4h, 6h
        DB      0b8h, 0e6h, 39h, 50h, 8dh, 46h, 80h, 50h, 0e8h, 2fh, 0c1h, 83h, 0c4h, 4h, 0a1h, 0e0h
        DB      3eh, 8bh, 16h, 0e2h, 3eh, 5h, 0ch, 0h, 52h, 50h, 0e8h, 3fh, 0f9h, 83h, 0c4h, 4h
        DB      89h, 86h, 7eh, 0ffh, 8bh, 0d8h, 8ah, 7h, 2ah, 0e4h, 40h, 50h, 53h, 0b8h, 0e6h, 39h
        DB      50h, 0e8h, 0c4h, 1fh, 83h, 0c4h, 6h, 8dh, 46h, 80h, 50h, 0b8h, 0e6h, 39h, 50h, 0e8h
        DB      0f8h, 0c0h, 83h, 0c4h, 4h, 0b8h, 0ffh, 0ffh, 50h, 0ffh, 0b6h, 7eh, 0ffh, 0b8h, 0e6h, 39h
        DB      50h, 0e8h, 4eh, 0aeh, 83h, 0c4h, 6h, 0ah, 0c0h, 75h, 37h, 8ah, 1eh, 0e6h, 39h, 2ah
        DB      0ffh, 88h, 0bfh, 0e7h, 39h, 0b8h, 0e7h, 39h, 50h, 0b8h, 0cdh, 0fh, 50h, 0e8h, 62h, 0f7h
        DB      83h, 0c4h, 4h, 0b8h, 0ffh, 0ffh, 50h, 2bh, 0c0h, 50h, 0b8h, 0e6h, 39h, 50h, 0e8h, 0ffh
        DB      10h, 83h, 0c4h, 6h, 0a1h, 0eh, 3eh, 8bh, 16h, 10h, 3eh, 0a3h, 0e0h, 3eh, 89h, 16h
        DB      0e2h, 3eh, 8bh, 0e5h, 5dh, 0c3h, 56h, 0ffh, 36h, 34h, 43h, 8bh, 1eh, 2ch, 3fh, 0d1h
        DB      0e3h, 8bh, 36h, 0c4h, 32h, 0ffh, 30h, 0b8h, 0d6h, 0bh, 50h, 0ffh, 36h, 0f2h, 3dh, 0e8h
        DB      0aeh, 1ah, 83h, 0c4h, 8h, 5eh, 0c3h, 90h, 55h, 8bh, 0ech, 56h, 0c4h, 5eh, 4h, 26h

_TEXT   ENDS
        END     L0100
