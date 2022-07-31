#ifndef _CEDAR_UTILS_H_
#define _CEDAR_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cedar.h>

/*
 * Draw text wrapped to fit in a box
 *
 * str		The string to wrap into the box
 * x		The x coordinate on the screen of the box
 * y		The y coordinate on the screen of the box
 * width	The width of the box
 * height	The height of the box
 */
void cedar_wrapTextInBox(const char *str, int x, int y, unsigned int width, unsigned int height);

/*
 * Use a sprite to mask a colour drawn to the screen
 *
 * x			The X coordinate to start drawing at
 * y			The Y coordinate to start drawing at
 * color		The colour to use
 * mask			The sprite to use as the mask mask
 */
void cedar_SpriteColorMask(uint24_t x, uint8_t y, uint8_t color, gfx_sprite_t *mask);

/*
 * Replacement for gfx_SetTextTransparentColor that fixes bug in clipped text
 * drawing.
 */
#define cedar_SetTextTransparentColor(c) {\
	register uint8_t color = c;\
	gfx_SetTextTransparentColor(color);\
	gfx_SetTransparentColor(color);\
}

#define cedar_SetColors(fg, bg)\
	gfx_SetColor(bg);\
	gfx_SetTextBGColor(bg);\
	gfx_SetTextFGColor(fg);\
	gfx_SetTextTransparentColor(bg);\
	gfx_SetTransparentColor(bg);

#ifdef __cplusplus
}
#endif

#endif  // _CEDAR_UTILS_H_
