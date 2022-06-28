#include <stdlib.h>

#include "cedar.h"
#include "cedar/label.h"

Widget *Label(int x, int y, int width, int height, const char *text) {
	Widget *widget = malloc(sizeof(Widget));

	widget->type = WIDGET_LABEL;

	widget->next = NULL;
	widget->prev = NULL;

	widget->x = x;
	widget->y = y;
	widget->width = width;
	widget->height = height;

	widget->data = malloc(sizeof(LabelData));
	getData(LabelData, widget)->text = text;

	widget->handler = defaultLabelHandler;

	return widget;
}

int defaultLabelHandler(Widget *widget, int event) {
	switch (event) {
		case EVENT_PAINT:
			// Paint widget
			break;
	}
}
