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
			gfx_region_t *pos = (gfx_region_t *)param;

			if (((ButtonData *)self->data)->focused) {
				// Draw focused
				cedar_SetColors(self->parent->colors.bg, self->parent->colors.fg);

				gfx_FillRectangle(pos->xmin, pos->ymin, GFX_REGION_WIDTH(*pos), GFX_REGION_HEIGHT(*pos));
				cedar_wrapTextInBox(((ButtonData *)self->data)->text, pos->xmin+2, pos->ymin+2, GFX_REGION_WIDTH(*pos)-2, GFX_REGION_HEIGHT(*pos)-2);

				cedar_SetColors(self->parent->colors.fg, self->parent->colors.bg);
			} else {
				// Draw unfocused
				gfx_Rectangle(pos->xmin, pos->ymin, GFX_REGION_WIDTH(*pos), GFX_REGION_HEIGHT(*pos));
				cedar_wrapTextInBox(((ButtonData *)self->data)->text, pos->xmin+2, pos->ymin+2, GFX_REGION_WIDTH(*pos)-2, GFX_REGION_HEIGHT(*pos)-2);
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
			if (param == CEDAR_KB_ENTER) {
				cedar_dispatchEvent(EVENT_BUTTONPRESS, self->parent, (uint24_t)self->id);
			}
			break;
	}

	return CALLBACK_NORMAL;
}

CedarWidget *CedarButton(ID id, int x, int y, int width, int height, const char *text) {
	CedarWidget *widget = malloc(sizeof(CedarWidget) + sizeof(ButtonData));

	widget->id = id;
	widget->attrs = ATTR_FOCUSABLE;

	widget->parent = NULL;
	widget->prev = NULL;
	widget->next = NULL;

	widget->bounds.xmin = x;
	widget->bounds.ymin = y;
	widget->bounds.xmax = x + width;
	widget->bounds.ymax = y + height;

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
