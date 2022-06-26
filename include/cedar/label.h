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
 * A label is a widget which contains text.
 *
 * x		The X coordinate of the left side of the label
 * y		The Y coordinate of the top of the label
 * width	The width of the label's drawing area
 * height	The height of the label's drawing area
 * text		The content of the label
 */
Widget *Label(int x, int y, int width, int height, const char *text);

/*
 * The default event handler for label widgets.
 */
int defaultLabelHandler(Widget *widget, int event);

#endif  // _LIBCEDAR_LABEL_H_
