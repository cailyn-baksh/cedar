	section	.text,"ax",@progbits
	assume	adl = 1
	public _cedar_init
	public _cedar_cleanup

_cedar_init:
	call _gfx_Begin
	; gfx_SetDrawBuffer -> gfx_SetDraw(gfx_buffer)
	ld hl,1  ; gfx_buffer = 1
	push hl
	call _gfx_SetDraw
	ret

_cedar_cleanup:
	call _gfx_End
	ret

	extern _gfx_Begin
	extern _gfx_SetDraw
	extern _gfx_End
