#include <stdlib.h>

#include "cedar.h"
#include "cedar/button.h"

Widget *Button(int x, int y, int width, int height, const char *text) {
	Widget *widget = malloc(sizeof(Widget));

	widget->type = WIDGET_BUTTON;

	widget->next = NULL;
	widget->prev = NULL;

	widget->x = x;
	widget->y = y;
	widget->width = width;
	widget->height = height;

	widget->data = NULL;

	widget->handler = defaultButtonHandler;

	return widget;
}

int defaultButtonHandler(Widget *widget, int event) {
	switch (event) {
		case EVENT_PAINT:
			// Paint widget
			break;
		case EVENT_KEYDOWN:
			// Handle keypress
	}
}
