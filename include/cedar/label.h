/*
 * Cedar Label Widget
 */
#ifndef _LIBCEDAR_LABEL_H_
#define _LIBCEDAR_LABEL_H_

#include <cedar.h>

/*
 * Label data. In label widgets, data points to an instance of this structure.
 */
typedef struct {
	char *text;
} LabelData;

/*
 * Create a label widget. A label is a widget which contains text.
 *
 * x		The X coordinate of the left side of the label
 * y		The Y coordinate of the top of the label
 * width	The width of the label in pixels
 * height	The height of the label in pixels
 * text		The content of the label
 * Returns a new Label widget
 */
Widget *Label(int x, int y, int width, int height, const char *text);

/*
 * The default event handler for label widgets.
 */
uint24_t defaultLabelHandler(Widget *widget, uint24_t event);

#endif  // _LIBCEDAR_LABEL_H_
