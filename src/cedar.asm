include '../inc/library.inc'
include '../inc/include_library.inc'

library CEDAR, 1

include_library "E:\CEdev\CEdev\lib\libload\libload.lib"
;include_library "E:\CEdev\CEdev\lib\libload\graphx.lib"


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                   Exports                                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	export cedar_init
	export cedar_cleanup

;	export cedar_initWindow
;	export cedar_destroyWindow
;	export cedar_display

;	export cedar_addWidget
;	export cedar_destroyWidget

;	export cedar_setMenu
;	export cedar_addMenuSeparator
;	export cedar_addMenuItem
;	export cedar_addSubmenu

;	export Label
;	export defaultLabelHandler

;	export Button
;	export defaultButtonHandler

;	export Canvas


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
