#include <stdlib.h>
#include <string.h>

#include "cedar.h"
#include "cedar/button.h"
#include "cedar/utils.h"

Widget *Button(int x, int y, int width, int height, const char *text) {
	Widget *widget = malloc(sizeof(Widget));

	widget->type = WIDGET_BUTTON;

	widget->next = NULL;
	widget->prev = NULL;

	widget->x = x;
	widget->y = y;
	widget->width = width;
	widget->height = height;

	widget->data = malloc(sizeof(ButtonData));
	ButtonData *data = widget->data;

	data->text = malloc(strlen(text) * sizeof(char));
	strcpy(data->text, text);

	widget->handler = defaultButtonHandler;

	return widget;
}

uint24_t defaultButtonHandler(Widget *widget, int event) {
	switch (event) {
		case EVENT_DESTROY:
			// Clean up widget
			free(getData(ButtonData, widget)->text);
			break;
		case EVENT_PAINT:
			// Paint widget
			gfx_Rectangle(widget->realX, widget->realY, widget->width, widget->height);
			wrapTextInBox(getData(ButtonData, widget)->text, widget->realX+2,
				widget->realY+2, widget->width-2, widget->height-2);
			break;
		case EVENT_KEYDOWN:
			// Handle keypress
			break;
		case EVENT_KEYUP:
			// Handle keyrelease
			break;
	}

	return HANDLER_NORMAL;
}
