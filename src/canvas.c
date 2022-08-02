#include <cedar.h>
#include <cedar/canvas.h>

CALLBACKRESULT defaultCanvasHandler(CedarWidget *self, EVENT event, uint24_t param) {
	switch (event) {
		case EVENT_PAINT:
		{
			gfx_region_t *pos = (gfx_region_t *)param;
			int width = pos->xmax - pos->xmin;
			int height = pos->ymax - pos->ymin;

			// TODO: make canvas implementation better
			for (int i=0, y=0; y < height; ++y) {
				for (int x=0; x < width; ++x, ++i) {
					gfx_SetColor(self->data[i]);
					gfx_SetPixel(x, y);
				}
			}
		}
	}

	return CALLBACK_NORMAL;
}

CedarWidget *Canvas(ID id, int x, int y, int width, int height) {
	CedarWidget *widget = malloc(sizeof(CedarWidget) + (width * height));

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
	widget->handlers->callback = defaultCanvasHandler;
	widget->handlers->next = NULL;

	return widget;
}