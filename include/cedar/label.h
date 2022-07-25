/*
 * Cedar Label Widget
 */
#ifndef _LIBCEDAR_LABEL_H_
#define _LIBCEDAR_LABEL_H_

#ifdef __cplusplus
extern "C" {
#endif

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
CedarWidget *CedarLabel(ID id, int x, int y, int width, int height, const char *text);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_LABEL_H_
