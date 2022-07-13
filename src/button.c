#include <stdlib.h>
#include <string.h>

#include "cedar.h"
#include "cedar/button.h"
#include "cedar/utils.h"

#include "cedardbg.h"

Widget *Button(int x, int y, int width, int height, const char *text) {
	Widget *widget = malloc(sizeof(Widget));

	widget->attrs = ATTR_FOCUSABLE;

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

	data->focused = false;

	widget->handler = defaultButtonHandler;

	return widget;
}

CALLBACKRESULT defaultButtonHandler(Widget *widget, EVENT event, ID id, uint24_t param) {
	switch (event) {
		case EVENT_DESTROY:
			// Clean up widget
			free(getData(ButtonData, widget)->text);
			break;
		case EVENT_PAINT:
			// Paint widget
			if (getData(ButtonData, widget)->focused) {
				// Draw focused
				gfx_FillRectangle(widget->realX, widget->realY, widget->width, widget->height);

				
				gfx_SetTextFGColor(255);
				gfx_SetTextBGColor(0);
				gfx_SetTextTransparentColor(0);

				wrapTextInBox(getData(ButtonData, widget)->text, widget->realX+2,
					widget->realY+2, widget->width-2, widget->height-2);

				gfx_SetTextFGColor(0);
				gfx_SetTextBGColor(255);
				gfx_SetTextTransparentColor(255);
			} else {
				// Draw unfocused
				gfx_Rectangle(widget->realX, widget->realY, widget->width, widget->height);
				wrapTextInBox(getData(ButtonData, widget)->text, widget->realX+2,
					widget->realY+2, widget->width-2, widget->height-2);
			}

			break;
		case EVENT_FOCUS:
			getData(ButtonData, widget)->focused = true;
			break;
		case EVENT_BLUR:
			getData(ButtonData, widget)->focused = false;
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
