; Shadow Sorcerer
; License: Public Domain (CC0)
; Decompiled from 1991 DOS release by Nancy Sadkov (https://aermia.com/)

_TEXT   SEGMENT BYTE PUBLIC 'CODE'
        ASSUME  CS:_TEXT, DS:_TEXT, ES:_TEXT, SS:_TEXT

        PUBLIC  __acrtused
__acrtused:
        ret

        PUBLIC  _outp, _outpw, _inp
_outp:
        push    bp
        mov     bp,sp
        mov     dx,[bp+4]
        mov     ax,[bp+6]
        out     dx,al
        pop     bp
        ret

_outpw:
        push    bp
        mov     bp,sp
        mov     dx,[bp+4]
        mov     ax,[bp+6]
        out     dx,ax
        pop     bp
        ret

_inp:
        push    bp
        mov     bp,sp
        mov     dx,[bp+4]
        in      al,dx
        xor     ah,ah
        pop     bp
        ret

        PUBLIC  _sd_far_movsw
_sd_far_movsw:
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        mov     es,[bp+4]
        mov     di,[bp+6]
        mov     ds,[bp+8]
        mov     si,[bp+10]
        mov     cx,[bp+12]
        rep     movsw
        pop     di
        pop     si
        pop     es
        pop     ds
        pop     bp
        ret

        PUBLIC  _sd_far_movsb
_sd_far_movsb:
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        mov     es,[bp+4]
        mov     di,[bp+6]
        mov     ds,[bp+8]
        mov     si,[bp+10]
        mov     cx,[bp+12]
        rep     movsb
        pop     di
        pop     si
        pop     es
        pop     ds
        pop     bp
        ret

        PUBLIC  _sd_set_page
_sd_set_page:
        push    bp
        mov     bp,sp
        mov     al,[bp+4]
        mov     ah,5
        int     10h
        pop     bp
        ret

        PUBLIC  _sd_latch
_sd_latch:
        push    bp
        mov     bp,sp
        push    es
        push    bx
        mov     es,[bp+4]
        mov     bx,[bp+6]
        mov     al,es:[bx]
        xor     ah,ah
        pop     bx
        pop     es
        pop     bp
        ret

        PUBLIC  _sd_cli, _sd_sti
_sd_cli:
        cli
        ret
_sd_sti:
        sti
        ret

        PUBLIC  _sd_wait_retrace_asm
_sd_wait_retrace_asm:
        mov     dx,cs:[34Ch]
        add     dl,6
vret1:
        in      al,dx
        test    al,8
        jz      vret1
vret2:
        in      al,dx
        test    al,8
        jnz     vret2
        ret

        PUBLIC  _bios16_poll
_bios16_poll:
        mov     ah,1
        int     16h
        jz      b16_nokey
        mov     ah,0
        int     16h
        ret
b16_nokey:
        xor     ax,ax
        ret

INTSTUB MACRO name, vec
        LOCAL   nos
        PUBLIC  name
name:
        push    bp
        mov     bp,sp
        push    ds
        push    es
        push    si
        push    di
        mov     si,[bp+8]
        or      si,si
        jz      nos
        mov     ax,ss:[si+0]
        mov     es,ax
        mov     ax,ss:[si+6]
        mov     ds,ax
nos:
        mov     bx,[bp+4]
        mov     si,ss:[bx+8]
        mov     di,ss:[bx+10]
        mov     ax,ss:[bx+0]
        mov     cx,ss:[bx+4]
        mov     dx,ss:[bx+6]
        push    word ptr ss:[bx+2]
        pop     bx
        int     vec
        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        pushf
        pop     ax
        and     ax,1
        mov     bx,[bp+6]
        mov     ss:[bx+12],ax
        pop     ax
        mov     ss:[bx+10],ax
        pop     ax
        mov     ss:[bx+8],ax
        pop     ax
        mov     ss:[bx+6],ax
        pop     ax
        mov     ss:[bx+4],ax
        pop     ax
        mov     ss:[bx+2],ax
        pop     ax
        mov     ss:[bx+0],ax
        pop     di
        pop     si
        pop     es
        pop     ds
        pop     bp
        ret
ENDM

INTSTUB _bios10, 10h
INTSTUB _bios16, 16h
INTSTUB _bios21, 21h
INTSTUB _bios33, 33h

        PUBLIC  _segread
_segread:
        push    bp
        mov     bp,sp
        mov     bx,[bp+4]
        mov     [bx+0],es
        mov     [bx+2],cs
        mov     [bx+4],ss
        mov     [bx+6],ds
        pop     bp
        ret

SVC     MACRO name, thunk
        PUBLIC  name
name:
        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        push    bp
        push    ds
        push    es
        mov     bp,sp
        push    cs
        pop     ds
        mov     [c_old_ss],ss
        mov     [c_old_sp],sp
        cli
        mov     ax,cs
        mov     ss,ax
        mov     sp,OFFSET c_stack_top
        sti
        push    word ptr [c_old_ss]
        push    bp
        call    thunk
        add     sp,4
        cli
        mov     ax,[c_old_ss]
        mov     ss,ax
        mov     sp,[c_old_sp]
        sti
        pop     es
        pop     ds
        pop     bp
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        ret
ENDM

SVC_STAY MACRO name, thunk
        PUBLIC  name
name:
        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        push    bp
        push    ds
        push    es
        mov     bp,sp
        push    cs
        pop     ds
        push    ss
        push    bp
        call    thunk
        add     sp,4
        pop     es
        pop     ds
        pop     bp
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        ret
ENDM

        EXTRN   _c_svc00:NEAR
        EXTRN   _c_svc02:NEAR
        EXTRN   _c_svc05:NEAR
        EXTRN   _c_svc06:NEAR
        EXTRN   _c_svc07:NEAR
        EXTRN   _c_svc09:NEAR
        EXTRN   _c_svc0D:NEAR
        EXTRN   _c_svc0E:NEAR
        EXTRN   _c_svc0F:NEAR
        EXTRN   _c_svc10:NEAR
        EXTRN   _c_svc11:NEAR
        EXTRN   _c_svc12:NEAR
        EXTRN   _c_svc13:NEAR
        EXTRN   _c_svc15:NEAR
        EXTRN   _c_svc16:NEAR
        EXTRN   _c_svc17:NEAR
        EXTRN   _c_svc18:NEAR
        EXTRN   _c_svc19:NEAR
        EXTRN   _c_svc1A:NEAR
        EXTRN   _c_svc1E:NEAR
        EXTRN   _c_svc1F:NEAR
        EXTRN   _c_svc20:NEAR
        EXTRN   _c_svc21:NEAR
        EXTRN   _c_svc22:NEAR

        EXTRN   _c_svc01:NEAR
        EXTRN   _c_svc03:NEAR
        EXTRN   _c_svc04:NEAR
        EXTRN   _c_svc08:NEAR
        EXTRN   _c_svc0A:NEAR
        EXTRN   _c_svc0B:NEAR
        EXTRN   _c_svc0C:NEAR
        EXTRN   _c_svc14:NEAR
        EXTRN   _c_svc1B:NEAR
        EXTRN   _c_svc1C:NEAR
        EXTRN   _c_svc1D:NEAR

SVC_STAY c_svc01, _c_svc01
SVC     c_svc03, _c_svc03
SVC     c_svc04, _c_svc04
SVC     c_svc08, _c_svc08
SVC_STAY c_svc0A, _c_svc0A
SVC     c_svc0B, _c_svc0B
SVC_STAY c_svc0C, _c_svc0C
SVC     c_svc14, _c_svc14
SVC     c_svc1B, _c_svc1B
SVC_STAY c_svc1C, _c_svc1C
SVC     c_svc1D, _c_svc1D

SVC     c_svc00, _c_svc00
SVC     c_svc02, _c_svc02
SVC     c_svc05, _c_svc05
SVC     c_svc06, _c_svc06
SVC     c_svc07, _c_svc07
SVC     c_svc09, _c_svc09
SVC     c_svc0D, _c_svc0D
SVC     c_svc0E, _c_svc0E
SVC     c_svc0F, _c_svc0F
SVC     c_svc10, _c_svc10
SVC     c_svc11, _c_svc11
SVC     c_svc12, _c_svc12
SVC     c_svc13, _c_svc13
SVC     c_svc15, _c_svc15
SVC     c_svc16, _c_svc16
SVC     c_svc17, _c_svc17
SVC     c_svc18, _c_svc18
SVC     c_svc19, _c_svc19
SVC     c_svc1A, _c_svc1A
SVC     c_svc1E, _c_svc1E
SVC     c_svc1F, _c_svc1F
SVC     c_svc20, _c_svc20
SVC     c_svc21, _c_svc21
SVC     c_svc22, _c_svc22

c_stack         DB      2048 DUP (0)
c_stack_top     LABEL   BYTE
c_old_ss        DW      0
c_old_sp        DW      0

_TEXT   ENDS
        END
