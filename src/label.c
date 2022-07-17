#include <stdlib.h>
#include <string.h>

#include "cedar.h"
#include "cedar/label.h"
#include "cedar/utils.h"

#include "cedardbg.h"

CALLBACKRESULT defaultLabelHandler(CedarWidget *self, EVENT event, uint24_t param) {
	switch (event) {
		case EVENT_DESTROY:
			// clean up label-specific data;
			free(((LabelData *)self->data)->text);
			break;
		case EVENT_PAINT:
			// Paint widget
		{
			CedarPoint *pos = (CedarPoint *)param;
			cedar_wrapTextInBox(((LabelData *)self->data)->text, pos->x, pos->y, self->bounds.width, self->bounds.height);
		}
	}

	return CALLBACK_NORMAL;
}

CedarWidget *CedarLabel(ID id, int x, int y, int width, int height, const char *text) {
	CedarWidget *widget = malloc(sizeof(CedarWidget) + sizeof(LabelData));

	widget->id = id;
	widget->attrs = 0;

	widget->parent = NULL;
	widget->next = NULL;
	widget->prev = NULL;

	widget->bounds.x = x;
	widget->bounds.y = y;
	widget->bounds.width = width;
	widget->bounds.height = height;

	widget->repaint = true;

	widget->handlers = malloc(sizeof(CedarEventHandler));
	widget->handlers->callback = defaultLabelHandler;
	widget->handlers->next = NULL;

	LabelData *data = (LabelData *)widget->data;

	data->text = malloc(strlen(text) * sizeof(char));
	strcpy(data->text, text);

	return widget;
}
