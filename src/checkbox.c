#include <graphx.h>

#include <cedar.h>
#include <cedar/widget.h>
#include <cedar/checkbox.h>

#include "cedardbg.h"

CALLBACKRESULT defaultCheckboxHandler(CedarWidget *self, EVENT event, uint24_t param) {
	switch (event) {
		case EVENT_KEYDOWN:
			// Toggle selection
			if (param == CEDAR_KB_ENTER && self->enabled) {
				((CheckboxData *)self->data)->checked = !(((CheckboxData *)self->data)->checked);
				self->repaint = true;
			}
			break;
		case EVENT_PAINT:
		{
			gfx_region_t *pos = (gfx_region_t *)param;

			// TODO: indicate when selected

			gfx_Rectangle(pos->xmin, pos->ymin, CEDAR_CHECKBOX_WIDTH, CEDAR_CHECKBOX_HEIGHT);

			if (((CheckboxData *)self->data)->checked) {
				gfx_FillRectangle(pos->xmin+1, pos->ymin+1, CEDAR_CHECKBOX_WIDTH-2, CEDAR_CHECKBOX_HEIGHT-2);

				gfx_SetColor(255);
				gfx_Line_NoClip(pos->xmin+2, pos->ymax-4, pos->xmin+4, pos->ymax-2);
				gfx_Line_NoClip(pos->xmin+4, pos->ymax-2, pos->xmax-2, pos->ymin+2);
				gfx_SetColor(0);
			}
			break;
		}
	}

	return CALLBACK_NORMAL;
}

CedarWidget *CedarCheckbox(ID id, int x, int y) {
	CedarWidget *widget = malloc(sizeof(CedarWidget) + sizeof(CheckboxData));

	widget->id = id;
	widget->attrs = ATTR_FOCUSABLE;

	widget->parent = NULL;
	widget->next = NULL;
	widget->prev = NULL;

	widget->bounds.xmin = x;
	widget->bounds.ymin = y;
	widget->bounds.xmax = x + CEDAR_CHECKBOX_WIDTH;
	widget->bounds.ymax = y + CEDAR_CHECKBOX_HEIGHT;

	widget->repaint = true;
	widget->enabled = true;

	widget->handlers = malloc(sizeof(CedarEventHandler));
	widget->handlers->callback = defaultCheckboxHandler;
	widget->handlers->next = NULL;

	CheckboxData *data = (CheckboxData *)widget->data;
	data->checked = false;

	return widget;
}
