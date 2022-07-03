;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                   Exports                                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	export cedar_init
	export cedar_cleanup


cedar_init:
;	call gfx_Begin
;	call gfx_SetDrawBuffer
	ret

cedar_cleanup:
;	call gfx_End
	ret

;extern gfx_Begin
;extern gfx_SetDrawBuffer
;extern gfx_End
