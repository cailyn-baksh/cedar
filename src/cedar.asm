	section	.text,"ax",@progbits
	assume	adl = 1
	section	.text,"ax",@progbits

	public _cedar_init
_cedar_init:
	push hl

	call _gfx_Begin

	; gfx_SetDrawBuffer -> gfx_SetDraw(gfx_buffer) -> gfx_SetDraw(1)
	dec sp  ; high byte (junk data)
	ld l,1  ; h = junk data, l = 1
	push hl ; push gfx_buffer (xx 01)
	call _gfx_SetDraw
	pop hl
	inc sp

	pop hl
	ret

	public _cedar_cleanup
_cedar_cleanup:
	call _gfx_End
	ret

	extern _gfx_Begin
	extern _gfx_SetDraw
	extern _gfx_End
