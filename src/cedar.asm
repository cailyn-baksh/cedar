	section	.text,"ax",@progbits
	assume	adl = 1
	section	.text,"ax",@progbits

	public _cedar_Init
_cedar_Init:
	push hl

	call _gfx_Begin

	; gfx_SetDrawBuffer -> gfx_SetDraw(gfx_buffer) -> gfx_SetDraw(1)
	ld l,1  ; h = junk data, l = 1
	push hl ; push gfx_buffer (xxx 001)
			; the convention for representing 24 bit registers in this project
			; will be in pairs of three nybbles (one trybble)
	call _gfx_SetDraw
	pop hl

	; gfx_SetTextConfig(gfx_text_clip) -> gfx_SetTextConfig(1)
	ld l,1
	push hl
	call _gfx_SetTextConfig
	pop hl

	pop hl
	ret

	public _cedar_Cleanup
_cedar_Cleanup:
	call _gfx_End
	ret

	extern _gfx_Begin
	extern _gfx_SetDraw
	extern _gfx_SetTextConfig
	extern _gfx_End
