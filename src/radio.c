#include <graphx.h>

#include <cedar.h>
#include <cedar/widget.h>
#include <cedar/radio.h>
#include <cedar/utils.h>

#include "assets.h"

CALLBACKRESULT defaultRadioButtonHandler(CedarWidget *self, EVENT event, uint24_t param) {
	switch (event) {
		case EVENT_KEYDOWN:
			if (param == CEDAR_KB_ENTER && self->enabled) {
				*(((RadioButtonData *)self->data)->selection) = self->id;
				self->parent->repaint = true;
			}
			break;
		case EVENT_PAINT:
		{
			gfx_region_t *pos = (gfx_region_t *)param;

			if (*(((RadioButtonData *)self->data)->selection) == self->id) {
				cedar_SpriteColorMask(pos->xmin, pos->ymin, radio_selected);
			} else {
				cedar_SpriteColorMask(pos->xmin, pos->ymin, radio_deselected);
			}
			break;
		}
	}

	return CALLBACK_NORMAL;
}

CedarWidget *CedarRadioButton(ID id, int x, int y, ID *selection) {
	CedarWidget *widget = malloc(sizeof(CedarWidget) + sizeof(RadioButtonData));

	widget->id = id;
	widget->attrs = ATTR_FOCUSABLE;

	widget->parent = NULL;
	widget->next = NULL;
	widget->prev = NULL;

	widget->bounds.xmin = x;
	widget->bounds.ymin = y;
	widget->bounds.xmax = x + radio_selected->width;
	widget->bounds.ymax = y + radio_selected->height;

	widget->repaint = true;
	widget->enabled = true;

	widget->handlers = malloc(sizeof(CedarEventHandler));
	widget->handlers->callback = defaultRadioButtonHandler;
	widget->handlers->next = NULL;

	RadioButtonData *data = (RadioButtonData *)widget->data;
	data->selection = selection;

	return widget;
}
