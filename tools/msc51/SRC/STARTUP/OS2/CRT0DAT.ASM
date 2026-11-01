	page
	title	crt0dat - OS/2 C run-time initialization/termination
;***
;crt0dat.asm - OS/2 C run-time initialization/termination routines
;
;	Copyright (c) 1986-1988, Microsoft Corporation, All Rights Reserved
;
;Purpose:
;	This module contains the routines _cinit, exit, and _exit
;	for C run-time startup and termination.  _cinit and exit
;	are called from the _astart code in crt0.asm.
;
;*******************************************************************************

_NFILE_	=	20		; Maximum number of file handles

?DF	=	1		;; tell cmacros.inc we want to define our own segments

include	version.inc
.xlist
include	cmacros.inc
include	msdos.inc
.list

createSeg _TEXT, code,	word,	public, CODE,	<>
createSeg CDATA, cdata,	word,	common, DATA,	DGROUP
createSeg _DATA, data,	word,	public, DATA,	DGROUP

createSeg XIB,	xibseg, word,	public, DATA,	DGROUP
createSeg XI,	xiseg,	word,	public, DATA,	DGROUP ; init's
createSeg XIE,	xieseg, word,	public, DATA,	DGROUP

createSeg XOB,	xobseg, word,	public, BSS,	DGROUP
createSeg XO,	xoseg,	word,	public, BSS,	DGROUP ; onexit table
createSeg XOE,	xoeseg, word,	public, BSS,	DGROUP

createSeg XPB,	xpbseg, word,	public, DATA,	DGROUP
createSeg XP,	xpseg,	word,	public, DATA,	DGROUP ; preterm's
createSeg XPE,	xpeseg, word,	public, DATA,	DGROUP

createSeg XCB,	xcbseg, word,	public, DATA,	DGROUP
createSeg XC,	xcseg,	word,	public, DATA,	DGROUP ; term's
createSeg XCE,	xceseg, word,	public, DATA,	DGROUP

defGrp	DGROUP			;; define DGROUP

codeOFFSET equ	offset _TEXT:
dataOFFSET equ	offset DGROUP:

page

sBegin	xibseg
xibegin	label	byte
sEnd	xibseg

sBegin	xieseg
xiend	label	byte
sEnd	xieseg

sBegin	xobseg
xontab	label	byte		; start of onexit table
sEnd	xobseg

sBegin	xoeseg
xonend	label	byte
sEnd	xoeseg

sBegin	xpbseg
xpbegin	label	byte		; end of onexit table
sEnd	xpbseg

sBegin	xpeseg
xpend	label	byte
sEnd	xpeseg


sBegin	xcbseg
xcbegin	label	byte
sEnd	xcbseg

sBegin	xceseg
xcend	label	byte
sEnd	xceseg



sBegin	cdata			; floating point setup segment
assumes	ds,data

	dw	0		; force segment to be at least 0's
labelD	<PUBLIC,_fpinit> 	; public for signal
fpmath	dd	1 dup (?)	; linking trick for fp
fpdata	dd	1 dup (?)
fpsignal dd	1 dup (?)	; fp signal message

externW	_aenvseg 		; Selector of Environment segment
sEnd

sBegin	data
assumes	ds,data

globalQ	_fac,0			; floating accumulator
globalW	errno,0			; initial error code
globalW	_umaskval,0 		; initial umask value

;================= following must be in this order

labelW	<PUBLIC,_osversion>
labelB	<PUBLIC,_dosvermajor>
globalB	_osmajor,0
labelB	<PUBLIC,_dosverminor>
globalB	_osminor,0

;================= above must be in this order

labelB	<PUBLIC,_dosmode>
globalB	_osmode,0

labelW	<PUBLIC,_oserr>
globalW	_doserrno,0 		; initial DOS error code

globalW	_nfile,_NFILE_ 		; maximum number of file handle

labelB	<PUBLIC,_osfile>
	db	3 dup (FOPEN+FTEXT) ; stdin, stdout, stderr
	db	_NFILE_-3 dup (0) ; the other 17 handles

labelB	<PUBLIC,_pipe>
	db	_NFILE_ dup (0AH) ; peek-ahead buffers for pipes


globalW	__argc,0
globalDP __argv,0
globalDP environ,0 		; environment pointer
globalD	_pgmptr,0 		; far pointer to program name
globalW	_child,0 		; flag that child program is executing
				; needed by signal code in real mode

sEnd	data

page

extrn	DOSCLOSE:far 		
extrn	DOSQHANDTYPE:far 
extrn	DOSSETVEC:far 	
extrn	DOSEXIT:far 

;	*** Increase File Handle Count ***
;	Enable this extrn reference if you are increasing the file count.
;
;extrn	DOSSETMAXFH:far
;
;	*** End Increase File Handle Count ***


sBegin	code
assumes	cs,code

if	sizeC
global	proc	far
endif

externNP _fptrap
externP	_cintDIV
externP	_nullcheck

page
;***
;_cinit - C initialization
;
;Purpose:
;	This routine performs the shared DOS and Windows initialization.
;	The following order of initialization must be preserved -
;
;	1.	Check for devices for file handles 0 - 2
;	2.	Integer divide interrupt vector setup
;	3.	Floating point initialization
;	4.	General C initializer routines
;
;Entry:
;
;Exit:
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

cProc	_cinit,<PUBLIC>,<>

cBegin	nogen			; no local frame to set up
assumes	ds,data
assumes	ss,data


;	*** Increase File Handle Count ***
;
;	This code is intentially commented out; the user must enable
;	this code to access more than 20 files.
;
;	mov	ax,_NFILE_	; new file handle count
;	push	ax		; onto stack
;	call	DOSSETMAXFH	; set net file handle limit
;	;check for error here, if desired (AX = 0 means no error)
;	
;	*** End Increase File Handle Count ***


;	1.	Check for devices for file handles 0 - 2

	mov	bx,2

	push	dx		; space for device driver attribute
	mov	dx,sp
devloop:
	and	_osfile[bx],not (FDEV or FPIPE)
				; Assume this handle is not a device or pipe
	push	ax		; space for isdevice flag
	mov	ax,sp
	push	bx		; handle
	push	ss
	push	ax		; address for isdevice flag
	push	ss
	push	dx		; address for device driver attribute
	call	DOSQHANDTYPE

	or	ax,ax		; test for error
	pop	ax		; get isdevice flag
	jnz	notdevpipe

	mov	cl,FDEV 	; assume device
	cmp	ax,IS_DEV	; is the handle a device ?
	je	devpipe 	; yes - set FDEV bit
	cmp	ax,IS_PIPE	; is the handle a pipe ?
	jne	notdevpipe	; no - it is a file
	mov	cl,FPIPE	; PIPE instead of DEVICE
devpipe:
	or	_osfile[bx],cl	; set FDEV or FPIPE bit as appropriate
notdevpipe:
	dec	bx
	jns	devloop

	pop	dx		; clean up stack

;	2.	Integer divide interrupt vector setup

	xor	ax,ax

	push	ax
	push	ax		; space for previous handler (returned)
	mov	bx,sp

	push	ax		; zero means divide by 0 exception

	mov	dx,offset _TEXT:__cintDIV
	push	cs
	push	dx		; address of divide exception handler

	push	ss
	push	bx		; address of address for prev. handler

	call	DOSSETVEC
	pop	ax		; clean-up stack
	pop	ax		; (ignore return value)

;	3.	Floating point initialization

	mov	cx,word ptr [fpmath+2]
	jcxz	nofloat_i

	mov	si,[_aenvseg]	; environment segment
	lds	ax,[fpdata]	; get task data area
	assumes	ds,nothing
	mov	dx,ds		;   into dx:ax
	xor	bx,bx		; (si) = environment segment
	call	[fpmath]	; fpmath(0) - init
	jnc	fpok

	push	ss		; restore ds from ss
	pop	ds
	jmp	_fptrap		; issue "Floating point not loaded"
				; error and abort

fpok:
	lds	ax,[fpsignal]	; get signal address
	assumes	ds,nothing
	mov	dx,ds
	mov	bx,3
	call	[fpmath]	; fpmath(3) - set signal address
	push	ss
	pop	ds
	assumes	ds,data

nofloat_i:

;	4.	General C initializer routines

	mov	si,dataOFFSET xibegin
	mov	di,dataOFFSET xiend
	call	initterm	; call the initializers

	ret
cEnd	nogen

page
;***
;exit(status), _exit(status) - C termination
;
;Purpose:
;	The termination sequence is more complicated due to the multiple
;	entry points - exit(code) and _exit(code).  The _exit() routine
;	is a quick exit routine that does not do certain C exit functions
;	like stdio buffer flushing and onexit processing.
;
;	exit (status):
;
;	1.	call runtime preterminators
;
;	_exit (status):
;
;	2.	perform C terminators
;	3.	close all open files
;	4.	perform _nullcheck() for null pointer assignment
;	5.	terminate floating point
;	6.	terminate with return code to DOS
;
;Entry:
;	int status - exit status (0-255)
;
;Exit:
;	Routine exits to DOS.
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

public	_exit
_exit:
cProc	dummy1,<>,<>

parmw	status

cBegin
assumes	ds,data
assumes	ss,data

;	1.	call runtime preterminators
;		- onexit processing
;		- flushall
;		- rmtmp

	mov	si,dataOFFSET xontab ; beginning of onexit table
	mov	di,dataOFFSET xonend ; end of onexit table
	call	initterm

	mov	si,dataOFFSET xpbegin ; beginning of pre-terminators
	mov	di,dataOFFSET xpend ; end of pre-terminators
	call	initterm

	jmp	short exiting	; jump into _exit

cend	nogen

public	__exit
__exit:
cProc	dummy2,<>,<>

parmw	status

cBegin
assumes	ds,data
assumes	ss,data

exiting:

;	2.	perform C terminators

	mov	si,dataOFFSET xcbegin
	mov	di,dataOFFSET xcend
	call	initterm


;	3.	close all files (except pre-opened handles 0-2)

	mov	cx,_NFILE_-3
	mov	bx,3		; close handles that are not pre-opened

closeloop:
	test	_osfile[bx],FOPEN
	jz	notopen

	push	bx
	call	DOSCLOSE

notopen:
	inc	bx
	loop	closeloop

;	4.	perform _nullcheck() for null pointer assignment

	call	_nullcheck	; perform null check (ignore return)
	or	ax,ax		; zero if no null pointer asmt. detected
	jz	startclose
	cmp	status,0	; zero if no other error has occurred
	jnz	startclose
	mov	status,255	; nonzero status to indicate an
				; null-pointer-assignment error
startclose:

;	5.	terminate floating point

	call	_ctermsub	; fast cleanup

;	6.	return to the DOS

	mov	ax,1		; terminate all threads of this process
	push	ax
	mov	al,byte ptr [status] ; get return value (low 8 bits)
	push	ax
	call	DOSEXIT		; exit with return code

cEnd	nogen

if	sizeC
global	endp
endif

page
;***
;_ctermsub - What does this do?
;
;Purpose:
;	UNDONE: What does this entry point do?
;
;Entry:
;
;Exit:
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

labelNP	<PUBLIC,_ctermsub>

;	5.	terminate floating point

	mov	cx,word ptr [fpmath+2] ; test for floating point
	jcxz	nofloat_t	;   no

	mov	bx,2		;   yes - cleanup
	call	[fpmath]

nofloat_t:

	ret

page
;***
;initterm - do all the initializers and terminators
;
;Purpose:
;	The initializers and terminators may be written in C
;	so we are assuming C conventions (DS=SS, CLD, SI and DI preserved)
;	We go through them in reverse order for onexit.
;
;Entry:
;	SI	= start of procedure list
;	DI	= end of procedure list
;
;Exit:
;
;Uses:
;
;Exceptions:
;
;*******************************************************************************

initterm:
	cmp	si,di		; are we done?
	jae	itdone		;   yes - no more

if	sizeC
	sub	di,4
	mov	ax,[di]
	or	ax,[di+2]
	jz	initterm	; skip null procedures
	call	dword ptr [di]
else
	dec	di
	dec	di
	mov	cx,[di]
	jcxz	initterm	; skip null procedures
	call	cx
endif
	jmp	initterm	; keep looping

itdone:
	ret

sEnd

	end
