#ifndef _LIBCEDAR_BUTTON_H_
#define _LIBCEDAR_BUTTON_H_

#include <cedar.h>

// TODO: either prefix with Cedar or dont expose to user
typedef struct {
	char *text;
	bool focused;
} ButtonData;

/*
 * A button is a widget which contains text in a box, and which can be clicked
 *
 * x		The X coordinate of the left side of the button
 * y		The Y coordinate of the top of the button
 * width	The width of the button
 * height	The height of the button
 */
CedarWidget *_CedarButton(int x, int y, int width, int height, const char *text);

CedarWidget *CedarButton(ID id, int x, int y, int width, int height, const char *text);

#endif  // _LIBCEDAR_BUTTON_H_
