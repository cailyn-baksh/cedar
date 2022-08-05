#include <graphx.h>

#include <cedar.h>
#include <cedar/widget.h>
#include <cedar/checkbox.h>
#include <cedar/utils.h>

#include "assets.h"
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

			//gfx_SetPalette(cedar_sprite_mask_palette, sizeof_cedar_sprite_mask_palette, cedar_sprite_masks_palette_offset);
			//gfx_Sprite(checkbox_unchecked, pos->xmin, pos->ymin);
			//gfx_SetDefaultPalette(gfx_8bpp);
			//gfx_Rectangle(pos->xmin, pos->ymin, CEDAR_CHECKBOX_WIDTH, CEDAR_CHECKBOX_HEIGHT);

			if (((CheckboxData *)self->data)->checked) {
				cedar_SpriteColorMask(pos->xmin, pos->ymin, self->parent->colors, checkbox_checked);
			} else {
				cedar_SpriteColorMask(pos->xmin, pos->ymin, self->parent->colors, checkbox_unchecked);
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
