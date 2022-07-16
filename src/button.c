#include <stdlib.h>
#include <string.h>

#include "cedar.h"
#include "cedar/button.h"
#include "cedar/utils.h"

#include "cedardbg.h"

CALLBACKRESULT defaultButtonHandler(CedarWidget *self, EVENT event, uint24_t param) {
	switch (event) {
		case EVENT_DESTROY:
			// Clean up button-specific data
			free(((ButtonData *)self->data)->text);
			break;
		case EVENT_PAINT:
			// Paint widget
		{
			CedarPoint *pos = (CedarPoint *)param;

			if (((ButtonData *)self->data)->focused) {
				// Draw focused
				gfx_FillRectangle(pos->x, pos->y, self->bounds.width, self->bounds.height);

				gfx_SetTextFGColor(255);
				gfx_SetTextBGColor(0);
				gfx_SetTextTransparentColor(0);

				cedar_wrapTextInBox(((ButtonData *)self->data)->text, pos->x+2, pos->y+2, self->bounds.width-2, self->bounds.height-2);

				gfx_SetTextFGColor(0);
				gfx_SetTextBGColor(255);
				gfx_SetTextTransparentColor(255);
			} else {
				// Draw unfocused
				gfx_Rectangle(pos->x, pos->y, self->bounds.width, self->bounds.height);
				cedar_wrapTextInBox(((ButtonData *)self->data)->text, pos->x+2, pos->y+2, self->bounds.width-2, self->bounds.height-2);
			}
		}
			break;
		case EVENT_FOCUS:
			((ButtonData *)self->data)->focused = true;
			break;
		case EVENT_BLUR:
			((ButtonData *)self->data)->focused = false;
			break;
		case EVENT_KEYUP:
			// Send button press event to parent window
			// TODO: provide way for widget handlers to access parent window
			break;
	}

	return CALLBACK_NORMAL;
}

CedarWidget *CedarButton(ID id, int x, int y, int width, int height, const char *text) {
	CedarWidget *widget = malloc(sizeof(CedarWidget) + sizeof(ButtonData));

	widget->id = id;
	widget->attrs = ATTR_FOCUSABLE;

	widget->prev = NULL;
	widget->next = NULL;

	widget->bounds.x = x;
	widget->bounds.y = y;
	widget->bounds.width = width;
	widget->bounds.height = height;

	widget->repaint = true;
	widget->enabled = true;

	widget->handlers = malloc(sizeof(CedarEventHandler));
	widget->handlers->callback = defaultButtonHandler;
	widget->handlers->next = NULL;

	ButtonData *data = (ButtonData *)widget->data;

	data->text = malloc(strlen(text) * sizeof(char));
	strcpy(data->text, text);

	data->focused = false;

	return widget;
}
