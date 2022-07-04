#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <graphx.h>
#include <keypadc.h>

#include "cedar.h"

void cedar_initWindow(Window *window) {
	window->widgets.first = NULL;
	window->widgets.last = NULL;
	window->widgets.selected = NULL;

	window->menu = NULL;

	window->realTop = 0;
	window->realLeft = 0;
	window->projTop = 0;
	window->projLeft = 0;
	window->width = GFX_LCD_WIDTH;
	window->height = GFX_LCD_HEIGHT;
}

void cedar_destroyWindow(Window *window) {
	// Clean up widgets
	if (window->widgets.first != NULL) {
		while (window->widgets.first->next != NULL) {
			cedar_destroyWidget(window->widgets.first->next);
		}

		cedar_destroyWidget(window->widgets.first);
	}

	free(window);
}

void cedar_addWidget(Window *window, Widget *widget) {
	if (window->widgets.last == NULL) {
		// This is the first widget
		window->widgets.first = widget;
		window->widgets.last = widget;
	} else {
		// Not the first widget, link this widget into the list
		window->widgets.last->next = widget;
		widget->prev = window->widgets.last;
		window->widgets.last = widget;
	}
}

void cedar_setMenu(Window *window, Menu *menu) {
	window->menu = menu;
	window->realTop = MENUBAR_HEIGHT;
}

// Previous state of the keyboard
uint8_t prevKbState[8] = { 0 };

int cedar_display(Window *window) {
	window->widgets.selected = window->widgets.first;

	for (;;) {
		/* Dispatch events */
		kb_Scan();

		// Check for arrow keys & dispatch focus/blur events
		if ((kb_Data[7] & kb_Right) && (window->widgets.selected->next != NULL)) {
			// Select next widget
			window->widgets.selected->handler(window->widgets.selected, EVENT_BLUR);
			window->widgets.selected = window->widgets.selected->next;
			window->widgets.selected->handler(window->widgets.selected, EVENT_FOCUS);
		} else if ((kb_Data[7] & kb_Left) && (window->widgets.selected->prev != NULL)) {
			// Select previous widget
			window->widgets.selected->handler(window->widgets.selected, EVENT_BLUR);
			window->widgets.selected = window->widgets.selected->prev;
			window->widgets.selected->handler(window->widgets.selected, EVENT_FOCUS);
		}

		// Check for key events
		bool keydown, keyup;
		for (int i=1; i < 8; ++i) {
			if (!keyup && (prevKbState[i] & kb_Data[i]) != prevKbState[i]) {
				// Not all keys previously pressed are still pressed (keyup)
				keyup = true;
			} else if (!keydown && (prevKbState[i] & kb_Data[i]) != kb_Data[i]) {
				// Not all keys currently pressed were previously pressed (keydown)
				keydown = true;
			}
		}

		if (keyup) {
			window->widgets.selected->handler(window->widgets.selected, EVENT_KEYUP);
		}

		if (keydown) {
			window->widgets.selected->handler(window->widgets.selected, EVENT_KEYDOWN);
		}

		// store current kb_Data for next check
		memcpy(prevKbState, kb_Data, 8);

		/* Paint */
		if (window->menu != NULL) {
			// Paint menu bar
			gfx_HorizLine(0, MENUBAR_HEIGHT, GFX_LCD_WIDTH);

			gfx_BlitRectangle(gfx_buffer, 0, 0, GFX_LCD_WIDTH, MENUBAR_HEIGHT+1);
		}

		// Paint widgets
		for (Widget *widget = window->widgets.first; widget != NULL; widget = widget->next) {
			if (widget->x >= window->projLeft
				&& (widget->x + widget->width) <= (window->projLeft + window->width)
				&& widget->y >= window->projTop
				&& (widget->y + widget->height) <= (window->projTop + window->height)
			) {
				// Widget is visible

				// Set the real coordinates
				widget->realX = window->realLeft + (widget->x - window->projLeft);
				widget->realY = window->realTop + (widget->y - window->projTop);

				// Draw it
				widget->handler(widget, EVENT_PAINT);
				gfx_BlitRectangle(gfx_buffer, widget->realX, widget->realY, widget->width, widget->height);
			}
		}
	}
}
