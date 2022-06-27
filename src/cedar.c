#include <graphx.h>

#include "cedar.h"

void cedar_init() {
	gfx_Begin();
	gfx_SetDrawBuffer();
}

void cedar_cleanup() {
	gfx_End();
}