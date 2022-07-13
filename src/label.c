#include <stdlib.h>
#include <string.h>

#include "cedar.h"
#include "cedar/label.h"
#include "cedar/utils.h"

#include "cedardbg.h"

Widget *Label(int x, int y, int width, int height, const char *text) {
	Widget *widget = malloc(sizeof(Widget));

	widget->attrs = 0;

	widget->next = NULL;
	widget->prev = NULL;

	widget->x = x;
	widget->y = y;
	widget->width = width;
	widget->height = height;

	widget->data = malloc(sizeof(LabelData));
	LabelData *data = widget->data;

	data->text = malloc(strlen(text) * sizeof(char));
	strcpy(data->text, text);

	widget->handler = defaultLabelHandler;

	return widget;
}

CALLBACKRESULT defaultLabelHandler(Widget *widget, EVENT event, ID id, uint24_t param) {
	switch (event) {
		case EVENT_DESTROY:
			// Clean up widget
			free(getData(LabelData, widget)->text);
			break;
		case EVENT_PAINT:
			// Paint widget
			wrapTextInBox(getData(LabelData, widget)->text, widget->realX,
				widget->realY, widget->width, widget->height);
			break;
	}

	return CALLBACK_DEFAULT;
}
