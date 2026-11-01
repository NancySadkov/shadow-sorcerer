	page	,132
	title	stdenvp - OS/2 standard _setenvp routine
;***
;stdenvp.asm - OS/2 standard _setenvp routine
;
;	Copyright (c) 1985-1988, Microsoft Corporation.  All rights reserved.
;
;Purpose:
;	This module is called by the C start-up routine to set up "environ".
;	It copies the environment strings into the heap/stack, preceded by a
;	null-terminated array of pointers to those strings.
;	The global symbol "_environ" is set to point to this array.
;
;*******************************************************************************

include	version.inc
	.xlist
include	cmacros.inc
include	msdos.inc
	.list

sBegin	data
assumes	ds,data

externDP environ 		; environment pointer
externW	_aenvseg 		; Environment segment
externB	_acfinfo 		; C_FILE_INFO string

staticCP retadr,0 		; return address

sEnd	data


sBegin	code

externNP _stdalloc 		; routine to allocate heap/stack space
externNP _amsg_exit 		; Routine for error messages.

assumes	ds,data
assumes	cs,code

page
;***
;_stdenvp - set up "envp" for C programs
;
;Purpose:
;	Reads the environment and build the envp array for C programs.
;
;Entry:
;	The environment strings occur at the beginning of the segment.
;	The list of environment strings is terminated by an extra null
;	byte.  Thus two null bytes in a row indicate the end of the
;	last environment string and the end of the environment, resp.
;
;Exit:
;	"environ" points to a null-terminated list of pointers to ASCIZ
;	strings, each of which is of the form "VAR=VALUE".  The strings
;	are copied from the environment segment into space allocated on
;	the heap/stack.  The list of pointers is also located on there.
;
;Uses:
;	Locks the environment segment before use, and unlocks afterward
;	Allocates space on the heap/stack for the environment strings
;	and a list of pointers to them.
;
;	All registers except DS, SS, and BP are modified
;	Note especially that SI and DI are NOT preserved!
;
;Exceptions:
;
;*******************************************************************************

labelP	<PUBLIC,_setenvp>

	pop	word ptr [retadr] ; get return address (offset)

if	sizeC
	pop	word ptr [retadr+2] ; get return address (segment)
endif

assumes	ds,data

	mov	es,_aenvseg	; point to environment segment
	assumes	es,nothing

;	setup envp vector and move environment to heap/stack
;
;	ES = environment segment, DS = SS = DGROUP

	xor	ax,ax		; ax = 0 (search byte)
	xor	si,si		; si = 0 (envp count)
	xor	di,di		; di = 0 (initial offset)
	mov	cx,-1		; cx = ffff (infinite count)

;*
;	Scan past the list of environment strings
;	Stop on a null string
;	SI will count the number of environment strings
;	DI will count the number of bytes the strings occupy
;
	cmp	al,ES:[di]	; may not be a double null on OS/2 v1.0
	je	noenv
scanenv:
	repnz	scasb
	inc	si		; si = envp count
	scasb
	jnz	scanenv
				; di = count of string bytes
noenv:
	inc	si		; si = envp count + 1
	mov	ax,di		; ax = envlen
	inc	ax
	and	al,not 1	; round up to even
	mov	di,si		; di = argument count
	shl	si,1		; argument count*2
if	sizeD
	shl	si,1		; argument count*2
endif
;
;	Need to allocate space for environment strings on the heap/stack
;	Number bytes needed = 2[4] * (Number of Strings + 1)
;				+ Number of Bytes in Strings
;
	add	ax,si		; ax = space to allocate
;
; _stdalloc allocates (AX) bytes from the heap or the stack
;
	call	_stdalloc	; preserves bp, si, di
	jnc	env_ok

	mov	ax,9		;Memory error.
	jmp	_amsg_exit

env_ok:
	mov	word ptr [environ],ax ; save envp[]

if	sizeD
	mov	word ptr [environ+2],ss ; segment address for large model
endif

envnext:
	mov	cx,di		; cx = envcnt
	mov	di,ax
	add	di,si		; di = env strings area
	mov	bx,ax		; bx = env pointers area
;
; BX points to where envp[0], envp[1], ... will be stored
; DI points to where *envp[0], *envp[1], ... will be stored
;
	push	es
	pop	ds
	xor	si,si		; ds:si = environment table

	push	ss
	pop	es		; es:di = env dest

assumes	es,data
assumes	ds,nothing

	dec	cx		; adjust for the last entry of 0000
	jcxz	envdone		;   done - no environment

;
; Copy each null-terminated environment string from DS:SI to ES:DI
; Add the address of each string to the list at ES:BX
; Skip the "_C_FILE_INFO" entry by not putting it in the pointer list
;
envloop:
	mov	ax,[si]
	cmp	ax,word ptr [_acfinfo]
	jne	not_cfi		; don't save pointer if '_C'...
;
may_be_cfi:
	push	cx
	push	si
	push	di		; preserve context

	mov	di,dataOFFSET _acfinfo ; ES:DI points to C_FILE_INFO string
	mov	cx,6		; length of that string in words
	repe	cmpsw		; cmpsw is faster than cmpsb

	pop	di
	pop	si
	pop	cx		; restore context
	je	envstr		; if match, skip this pointer
;
not_cfi:
	mov	es:[bx],di	; save pointer to start of string
if	sizeD
	mov	es:[bx+2],ss	; segment address for large model
	add	bx,4
else
	inc	bx
	inc	bx
endif

envstr:
	lodsb
	stosb
	or	al,al
	jnz	envstr		; keep copying string

	loop	envloop		; more strings to copy

;
;	DS:SI	points after the environment strings
;
envdone:
	mov	es:[bx],cx	; zero terminate envp table
if	sizeD
	mov	es:[bx+2],cx	; extra zero for large model
endif

	push	ss
	pop	ds
	push	ss
	pop	es		; ES = DS = DGROUP

assumes	ds,data

	jmp	[retadr]

sEnd	code

	end
