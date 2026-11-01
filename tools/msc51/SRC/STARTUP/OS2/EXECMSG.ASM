	page	,132
	title	execmsg - exec messages
;***
;execmsg.asm - OS/2 exec error messages
;
;	Copyright (c) 1987-1988, Microsoft Corporation.  All rights reserved.
;
;Purpose:
;	error messages used with exec*() library functions
;	(and spawn*() functions with P_OVERLAY).
;
;*******************************************************************************
	page	,132

include	version.inc
.xlist
include cmacros.inc


.list

createSeg HDR,	nhdr,	byte,	public, MSG,	DGROUP
createSeg MSG,	nmsg,	byte,	public, MSG,	DGROUP
createSeg PAD,	npad,	byte,	common, MSG,	DGROUP
createSeg EPAD,	nepad,	byte,	common, MSG,	DGROUP

	page

ERROR_BAD_ENVIRONMENT = 10
ERROR_NOT_ENOUGH_MEMORY = 8

externP	_exit
externP	_NMSG_WRITE
externP	_FF_MSGBANNER


sBegin	NMSG
assumes	ds,data

	dw	5
	db	'R6005',13,10,'- not enough memory on exec',13,10,0
	dw	6
	db	'R6006',13,10,'- bad format on exec',13,10,0
	dw	7
	db	'R6007',13,10,'- bad environment on exec',13,10,0

sEnd	NMSG

sBegin	NPAD
assumes	ds,data
	dw	-1
; no padding for now;
; MAX padding would be
;	db	67 dup(0)
sEnd

	page

sBegin	CODE
assumes	cs,CODE

page
;***
;_execmsg - handles fatal exec*() error messages
;
;Purpose:
;	prints out fatal error message for exec failures
;	using __FF_MSGBANNER and __NMSG_WRITE,
;	and then terminates hard by calling __exit.
;
;Entry:
;	AX == DosExecPgm error return code
;
;Exit:
;	none.
;
;Uses:
;	AX, BX, CX, DX, ES.
;
;Exceptions:
;	Warning:  This routine does not return to the caller, it will always
;		  exit through __exit().
;*******************************************************************************

cProc	_execmsg,<PUBLIC, NEAR>

cBegin	nogen

	or	ax,ax		;See if any DOS error at all
	jz	Exec_Exit

	mov	dx,7		;See if "Bad Environment"
	cmp	ax,ERROR_BAD_ENVIRONMENT
	je	Exec_Msg

	mov	dx,5		;See if "Out of Memory"
	cmp	ax,ERROR_NOT_ENOUGH_MEMORY
	je	Exec_Msg

	mov	dx,6		;See if not "Bad Exec"

Exec_Msg:


	push	dx		;Parmeter is the message index number.
	call	__FF_MSGBANNER	;write out 13,10,'run-time error '
	call	__NMSG_WRITE


	mov	ax,-1

Exec_Exit:
	push	ax
	call	__exit		;Terminate job.

cEnd	nogen

sEnd	CODE
	end
