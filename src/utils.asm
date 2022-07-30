	section	.text,"ax",@progbits
	assume	adl = 1
	section	.text,"ax",@progbits

; void cedar_SpriteColorMask(uint24_t x, uint8_t y, uint8_t color, gfx_sprite_t *mask)
	public _cedar_SpriteColorMask
_cedar_SpriteColorMask:
	;db 0x18
	;db 0xFE
	push ix
	ld ix,0
	add ix,sp  ; params <- ix -> locals

	ld hl,-8								; sp -= 8
	add hl,sp
	ld sp,hl
	; ix+15		gfx_sprite_t *mask
	; ix+12		uint8_t color
	; ix+9		uint8_t y
	; ix+6		uint24_t x
	; ix+3		return addr
	; ix		prev ix
	; ix-3		uint24_t spriteSize
	; ix-6		uint24_t currentX
	; ix-7		uint8_t currentY
	; ix-8		uint8_t prevColor

	ld iy,(ix+15)							; iy = mask

	; calculate size of sprite
	ld h,(iy)								; h = mask->width
	ld l,(iy+1)								; l = mask->height
	mlt hl									; hl = mask->width * mask->height
	ld (ix-3),hl							; spriteSize = hl

	; Initialize pen coordinates
	ld hl,(ix+6)							; hl = x
	ld (ix-6),hl							; currentX = hl

	ld a,(ix+9)								; a = y
	ld (ix-7),a								; currentY = a

	; Save current global color
	push iy									; prevColor = gfx_SetColor(0)
	ld c,0
	push bc
	call _gfx_SetColor
	pop bc
	pop iy
	ld (ix-8),a

	; for (de=0; de < spriteSize; ++de)
	ld de,0									; de = 0 (loop counter)
beginLoop._cedar_SpriteColorMask:
	push de									; hl = de
	pop hl

	ld bc,(ix-3)							; bc = spriteSize

	; if hl < bc
	or a,a
	sbc hl,bc
	jp p,endLoop._cedar_SpriteColorMask
		ld bc,0								; bc = mask->width
		ld c,(iy)

		ld hl,(ix+6)						; hl = x

		or a,a								; hl += bc
		adc hl,bc
		push hl								; bc = hl
		pop bc

		ld hl,(ix-6)						; hl = currentX

		; reset X coordinate to left boundary if x coordinate exceeds maximum
		; if hl >= bc
		or a,a
		sbc hl,bc
		jp m,drawPx._cedar_SpriteColorMask
			; Move to next line
			ld hl,(ix+6)					; hl = x
			ld (ix-6),hl					; currentX = hl

			inc (ix-7)						; currentY += 1

drawPx._cedar_SpriteColorMask:
		; Access current sprite pixel
		push iy
		inc iy								; iy = &mask->data
		inc iy
		add iy,de							; iy = &mask->data[i]

		; Calculate offset in jump table
		ld bc,0								; bcu, b = 0
		ld c,(iy)							; c = mask->data[i]
		sla c								; c *= 2
		; c cant overflow, so we dont need to worry about carrying into bc

		pop iy								; restore iy to mask

		; jump into jump table
		ld hl,jmpTbl.drawPx._cedar_SpriteColorMask
		add hl,bc
		jp (hl)


jmpTbl.drawPx._cedar_SpriteColorMask:
	jr black.drawPx._cedar_SpriteColorMask
	jr white.drawPx._cedar_SpriteColorMask
	jr trans.drawPx._cedar_SpriteColorMask
	jr color.drawPx._cedar_SpriteColorMask

black.drawPx._cedar_SpriteColorMask:
		ld bc,0x00
		jr setPx.drawPx._cedar_SpriteColorMask
white.drawPx._cedar_SpriteColorMask:
		ld bc,0xFF
		jr setPx.drawPx._cedar_SpriteColorMask
trans.drawPx._cedar_SpriteColorMask:
		jp continueLoop._cedar_SpriteColorMask
color.drawPx._cedar_SpriteColorMask:
		ld bc,(ix+12)


setPx.drawPx._cedar_SpriteColorMask:
		; Set the pixel color
		push iy								; save registers for function calls
		push de

		; set the color based on mask's pixel value
		push bc
		call _gfx_SetColor
		pop bc

		; gfx_SetPixel(currentX, currentY)
		ld e,(ix-7)							; push currentY
		push de
		ld de,(ix-6)						; push currentX
		push de
		call _gfx_SetPixel
		pop de
		pop de

		pop de								; restore registers
		pop iy

continueLoop._cedar_SpriteColorMask:
	inc de									; ++i
	inc (ix-6)								; ++currentX
	jp beginLoop._cedar_SpriteColorMask

endLoop._cedar_SpriteColorMask:
	; Restore previous color
	ld bc,(ix-8)
	push bc
	call _gfx_SetColor						; dont worry about saving regs
	pop bc

	; dealloc frame and return
	ld hl,8
	add hl,sp
	ld sp,hl

	pop ix
	;db 0x18
	;db 0xFE
	ret


	extern _gfx_SetPixel
	extern _gfx_SetColor
