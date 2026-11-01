	page	,132
	title	crt0 - OS/2 C start up routine
;***
;crt0.asm - OS/2 C start up routine
;
;	Copyright (c) 1986-1988, Microsoft Corporation, All Rights Reserved
;
;Purpose:
;	How startup works in a few words -
;
;	The startup and termination is performed by a few modules
;
;		crt0.asm	OS/2 specific init/term
;		crt0msg.asm	OS/2 error messages
;		crt0dat.asm	remainder of shared OS/2 init/term
;
;	*************  IMPORTANT  *****************************************
;
;	The "DOSSEG" directive in this module must not be removed or else
;	the user will have to link with the "/DOSSEG" linker switch in
;	order to get proper segment ordering.
;
;	See the C documentation for more information about the /DOSSEG switch.
;
;	All assembler modules must be assembled with the /Mx switch, i.e.
;
;		masm -Mx crt0,;
;
;*******************************************************************************

;*******************************;*
	DOSSEG			;* specifies DOS SEGment ordering *
;*******************************;*

;*******************************;*
	INCLUDELIB DOSCALLS	;* Library Search Record for "DOSCALLS.LIB"
;*******************************;*

?DF=	1			; this is special for c startup

.xlist
include	version.inc
include	cmacros.inc
include	msdos.inc
include	brkctl.inc
.list

	page
;===========================================================================
;
;	Segment definitions
;
;	The segment order is essentially the same as in XENIX.
;	This module is edited after assembly to contain a dosseg comment
;	record for the linker.
;
;===========================================================================

createSeg _TEXT, code,	word,	public, CODE,	<>
createSeg C_ETEXT,etext, word,	public, ENDCODE,<>

createSeg _DATA, data,	word,	public, DATA,	DGROUP
createSeg STACK, stack,	para,	stack,	STACK,	DGROUP

defGrp	DGROUP			; define DGROUP

codeOFFSET equ	offset _TEXT:
dataOFFSET equ	offset DGROUP:

page

public	__acrtused 		; trick to force in startup
	__acrtused = 9876h	; funny value not easily matched in SYMDEB

extrn	__acrtmsg:abs 		; trick to pull in startup messages


sBegin	stack
assumes	ds,data
	db	2560 dup (?)	; default stack size (2.5 KB)
sEnd

page

sBegin	data

extrn	_edata:byte 		; end of data (start of bss)
extrn	_end:byte 		; end of bss (start of stack)

externW	__argc
externDP __argv
externDP environ
externB _osfile
externB _osmajor		; Major and Minor versions of OS/2
externB _osmode 		; real/protected mode flag

;	these are used by DOS C memory management (not used in Windows)

;*
;*	The following (_acmdln and _aenvseg) must be in this order!
;*
globalW	_acmdln,0 		; Offset of command line string in env
globalW	_aenvseg,0 		; Selector of Environment segment

globalW	_asizds,0 		; DS size (in bytes)
globalW	_atopsp,0 		; top of stack (heap bottom)
globalW	_aexit_rtn,<codeoffset __exit> ; NEAR pointer

labelW	<PUBLIC,_abrktb> 	; segment table for brkctl
	dw	?
	dw	DGROUP
	db	(MAXSEG_PM-1) * (size segrec) dup (?)

labelW	<PUBLIC,_abrktbe>
globalW	_abrkp,<dataoffset _abrktb>


;	special C environment string

labelB	<PUBLIC,_acfinfo>
cfile	db	'_C_FILE_INFO'
cfilex	db	'=',0
cfileln	=	cfilex-cfile+1

sEnd

	page


externP	_cinit			; run-time initializers

externP	_NMSG_WRITE 		; pascal - write error message to stdout
externP	_FF_MSGBANNER 		; pascal - C/FORTRAN error message banner

externP	_setargv 		; process command line arguments
externP	_setenvp 		; process environment
externP	_nullcheck 		; check for null assignment

externP	main			; C main program
externP	exit			; exit ( code )

if	sizeC
extrn	__exit:far 		; _exit ( code) (cmacros name conflict)
else
extrn	__exit:near
endif

	extrn	DOSGETVERSION:far
	extrn	DOSGETMACHINEMODE:far

sBegin	code
assumes	cs,code

page
;***
;_astart - start of all C programs
;
;Purpose:
;	Startup routine to initialize C run-time environment.
;
;Entry:
;	OS/2 Start-Up Conditions:
;
;	DS	= Automatic Data Segment
;	SS:SP	= Stack Segment and Initial Stack Pointer
;	ES	= 0000
;	AX	= Selector of Environment Segment
;	BX	= Offset of Command Line in Environment Segment
;	CX	= Size of Automatic Data Segment (CX=0 means 65536 bytes)
;	BP	= 0000
;
;Exit:
;	Exits to OS/2 via exit().
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

labelNP	<PUBLIC,_astart> 	; start address of all "C" programs
;
;
	cld			; set direction flag (up)
	mov	[_atopsp],sp	; Top of Stack Region
	mov	[_abrktb].sz,sp	; top DS free location
	mov	[_abrktb].sg,ds	; DGROUP segment
	dec	cx
	mov	[_asizds],cx	; Size of Global Data Segment
	mov	[_aenvseg],ax	; Handle of Environment Segment
	mov	[_acmdln],bx	; Offset of Command Line String
;
;	get OS/2 version
;
	push	ax
	mov	ax,sp

	push	ss
	push	ax		; address for version
	call	DOSGETVERSION
	pop	ax
	xchg	ah,al		; swap bytes
	mov	word ptr [_osmajor],ax
;
; Get real/protected mode flag
;
	mov	ax,dataOFFSET _osmode
	push	ds
	push	ax
	call	DOSGETMACHINEMODE

;****
;*	C_FILE_INFO must be processed before _cinit() is called
;*	because _cinit() checks handles 0-2 for device/pipe.
;****

;	fix up files inherited from child using _C_FILE_INFO

	call	inherit

;	do necessary initialization BEFORE command line processing!

	call	_cinit		; shared by OS/2 and Windows

;	process command line and environment

	call	_setargv	; crack command line
	call	_setenvp	; crack environment

;	call main and exit

if	sizeD
	push	word ptr [environ+2] ; the environment is not always in DS
endif
	push	word ptr [environ]

if	sizeD
	push	word ptr [__argv+2] ; the arguments are not always in DS
endif
	push	word ptr [__argv]

	push	[__argc]	; argument count

	call	main		; main ( argc , argv , envp )

; use whatever is in ax after returning here from the main program

	push	ax
	call	exit		; exit (AX)
				;   _exit will call terminators

page
;***
;_amsg_exit, _cintDIV - Fast exit fatal errors
;
;Purpose:
;	Exit the program with error code of 255 and appropriate error
;	message.  cintDIV is used for integer divide by zero, amsg_exit
;	is for other run time errors.
;
;Entry:
;	AX	= error message number (amsg_exit only).
;
;Exit:
;	calls exit() [cintDIV] or indirect through _aexit_rtn [amsg_exit].
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

labelNP	<PUBLIC,_cintDIV>

	assumes ds,nothing
	assumes ss,data

;	_NMSG_WRITE will reestablish ds = DGROUP

	mov	ax,3		; Integer divide by zero interrupt
	mov	[_aexit_rtn],codeoffset _exit ; call high-level exit()
				; to cause file buffer flushing

labelNP	<PUBLIC,_amsg_exit>
	push	ax		; message number for _NMSG_WRITE
	call	_FF_MSGBANNER	; run-time error message banner
	call	_NMSG_WRITE	; write error message to stdout

	assumes ds,data

	mov	ax,255
	push	ax
	call	word ptr [_aexit_rtn] ; _exit(255) ordinarily
				; (or exit(255) for div by 0)
				; NEAR routine pointer

page
;***
;inherit - process C_FILE_INFO variable from the environment
;
;Purpose:
;	locates and interprets the "C_FILE_INFO" environment variable.
;	The value of this variable is written into the "_osfile" array.
;	This routine recognizes both DOS and OS/2 formats:
;
;	DOS:	";C_FILE_INFO" + count byte "N" + "N" data bytes + "\0"
;
;		where each data byte is written directly into _osfile
;		except that 0xFF represents 0
;
;	OS/2:	"_C_FILE_INFO=<AA><BB><CC><DD>" + "\0"
;
;		In this case the variable is a null-terminated string
;		(a well-formed environment variable) where each pair
;		of successive letters form one byte in _osfile.
;		The letters are in the range "A" through "P", representing
;		0 through 15.  The first letter of each pair is the more
;		significant 4 bits of the result.
;
;Entry:
;
;Exit:
;
;Uses:
;	AX, BX, CX, DX, SI, DI, ES
;
;Exceptions:
;
;*******************************************************************************

inherit proc	near
	mov	bx,cfileln
	cmp	[_osmode],0
	jne	not_fapi
;
;	Set up real-mode version of ;C_FILE_INFO
;
	mov	[cfile],';'	; change _C_FILE_INFO= to ;C_FILE_INFO
	mov	[cfilex],0
	dec	bx		; length is one less
not_fapi:
	xor	di,di
	mov	es,[_aenvseg]	; ES:DI points to environment strings
	mov	cx,07FFFh	; environment max = 32K
	cmp	byte ptr es:[di],0
	jne	cfilp
	inc	di		; first environment string is null
cfilp:
	cmp	byte ptr es:[di],0 ; check for end of environment
	je	nocfi		;   yes - not found
	mov	si,dataOFFSET cfile
	mov	dx,cx		; DX has count of bytes left in environment
	mov	cx,bx		; BX=cfileln
	repe	cmpsb		; compare for '_C_FILE_INFO='/';C_FILE_INFO'
	mov	cx,dx		; environment max = 32K
	je	gotcfi		;   yes - now do something with it
	xor	ax,ax
	repne	scasb		; search for end of current string
	je	cfilp		; keep searching
;
	jmp	short nocfi	;   no 00 !!! - assume end of env.
;
;	found _C_FILE_INFO, so transfer handle info into _osfile
;
gotcfi:
	push	es
	push	ds

	pop	es		; es = DGROUP
	mov	si,di		; si = startup of _osfile info
	pop	ds		; ds = env. segment
	assumes	es,data
	assumes	ds,nothing

	mov	di,dataOFFSET _osfile ; di = _osfile block

	cmp	bx,cfileln
	jne	real_cfi
;
; Prot-mode _C_FILE_INFO:
;	Read in pairs of characters, expected to be ['A'..'P'].
;	Each pair represents one byte in the _osfile array.
;	A null is the normal terminator for the string.
;
	mov	cl,4

osfile_lp:
	lodsb			; get next byte (more significant 4 bits)
	sub	al,'A'
	jb	nocfi		; string should terminate with a null
	shl	al,cl
	xchg	dx,ax		; save in DL

	lodsb			; get next byte (less significant 4 bits)
	sub	al,'A'
	jb	nocfi
	or	al,dl		; this assumes that AL is in range
	stosb
	jmp	short osfile_lp
;
; Real-mode C_FILE_INFO
;
real_cfi:
	lodsb			; must be less than 20
	cbw
	xchg	cx,ax		; cx = number of entries

osflp:
	lodsb			; get next byte
	inc	al
	jz	saveit		; was FF - save as 00
	dec	ax		; restore al
saveit:
	stosb
	loop	osflp		; transfer next character

;------

nocfi:
	push	ss
	pop	ds		; ds = DGROUP
	assumes	ds,data

	ret
inherit endp

sEnd
	end	_astart		; start address
