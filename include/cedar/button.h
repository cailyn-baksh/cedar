#ifndef _LIBCEDAR_BUTTON_H_
#define _LIBCEDAR_BUTTON_H_

#include <cedar.h>

/*
 * A button is a widget which contains text in a box, and which can be clicked
 *
 * x		The X coordinate of the left side of the button
 * y		The Y coordinate of the top of the button
 * width	The width of the button
 * height	The height of the button
 */
Widget *Button(int x, int y, int width, int height, const char *text);

/*
 * The default event handler for label widgets.
 */
int defaultButtonHandler(Widget *widget, int event);

#endif  // _LIBCEDAR_BUTTON_H_