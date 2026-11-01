	page	,132
	title	stdalloc - OS/2 routine to get memory from heap or stack
;***
;stdallo.asm - OS/2 routine to get memory from heap or stack
;
;	Copyright (c) 1986-1988, Microsoft Corporation.  All rights reserved.
;
;Purpose:
;	This routine returns memory from the heap or stack.
;
;*******************************************************************************


include	version.inc
.xlist
include	cmacros.inc
include	msdos.inc
include	brkctl.inc
.list


sBegin	data
	assumes	ds,data

extrn	_end:word 		; stack bottom

externW	_asizds			; limit of data segment
externW	_abrktb			; heap structure

	extrn	STKHQQ:word	; limit for stack growth

sEnd	data

	extrn	DOSREALLOCSEG:far ; DOS Function Call

sBegin	code
assumes	ds,data
assumes	cs,code

page
;***
;_stdalloc - try to steal memory from heap or stack
;
;Purpose:
;	This routine returns memory from the heap or stack.
;
;	First the heap is examined to see if it is big enough.
;	If it is not, this routine attempts to lengthen the
;	Global Data Segment in order to enlarge the heap.
;	If this is not possible or is unsuccessful, this
;	code will examine the amount of unused stack space
;	to see if it can accommodate the allocation request.
;
;	If carry is clear on return, then the allocation was
;	successful and DS:AX points to the allocated memory.
;	Carry set on return indicates allocation failure.
;
;	NOTE:	This routine DEPENDS on the fact that DS=SS=DGROUP
;
;	This is always a near routine since it is only called by the C run-time
;
;Entry:
;	AX	= number of bytes requested
;
;Exit:
;	Carry Clear (no error):
;	    DS:AX   = address of allocated memory
;	Carry Set:
;	    Error - unable to allocate
;
;Uses:	BX, CX, DX are destroyed
;	SP will be altered if the memory came from the stack
;
;Exceptions:
;
;*******************************************************************************

labelNP	<PUBLIC,_stdalloc>

	pop	dx		; get return offset
;
; See if there is space in the heap
; If not, see if the heap can be increased by enlarging the Data Segment
;
	mov	bx,[_abrktb].sz	; get current start of heap
	add	bx,ax
	jc	try_stack	; not enough room in DS after heap
	cmp	bx,[_asizds]
	jc	heap_success	; Yes! Enough Room in Heap

	mov	cx,bx
	dec	cx
	or	cl,0FH
	inc	cx		; Round up to a multiple of 16 bytes

	push	ax		; ****** save a copy of AX around call

	push	cx
	push	ds
	call	DOSREALLOCSEG	; Try to enlarge the Data Segment

	test	ax,ax
	pop	ax		; ****** restore copy of AX after call
	jnz	try_stack	; Cannot enlarge Data Segment

	dec	cx
	mov	[_asizds],cx	; set size of enlarged Data Segment
heap_success:
	xchg	bx,[_abrktb].sz	; set new top of heap and
	clc			; get start of allocated memory
	jmp	short return
;
; See if there is enough space on the stack
;
try_stack:
	mov	bx,sp
	sub	bx,ax		; new position
	jb	return		; error - out of memory
	cmp	bx,[STKHQQ]	; SP - AX : STKHQQ (for heap/stack)
	jb	return		;   error - out of memory
				; carry is clear!
	mov	sp,bx		; set new stack pointer
return:
;
;	Return the address in BX if Carry Clear
;	Return value is garbage if Carry Set
;
	mov	ax,bx
	jmp	dx		; return to cx

sEnd	code

	end
