#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <graphx.h>
#include <keypadc.h>

#define _NOEXTERN
#include "cedar.h"
#undef _NOEXTERN

#include "cedardbg.h"

void cedar_initWindow(Window *window) {
	window->widgets.first = NULL;
	window->widgets.last = NULL;
	window->widgets.selected = NULL;

	window->menu = NULL;
	window->menuSelected = false;

	window->realTop = 0;
	window->realLeft = 0;
	window->projTop = 0;
	window->projLeft = 0;
	window->width = GFX_LCD_WIDTH;
	window->height = GFX_LCD_HEIGHT;
}

void cedar_destroyWindow(Window *window) {
	// Clean up menu bar

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

bool key_2nd = false;
bool key_alpha = false;
bool alphaLock = false;

int cedar_display(Window *window) {
	// Previous state of the keyboard
	uint8_t prevKbState[8] = { 0 };
	window->widgets.selected = window->widgets.first;

	for (;;) {
		/* Dispatch events */
		kb_Scan();

		if (kb_Data[1] & kb_2nd) {
			key_2nd = !key_2nd;
		} else if (kb_Data[2] & kb_Alpha) {
			key_alpha = !key_alpha;

			if (key_2nd) {
				key_2nd = false;
				alphaLock = !alphaLock;
			}
		} else {
			if (key_2nd && (kb_Data[1] & kb_Mode)) {
				// Quit
				return 0;
			} else if ((kb_Data[7] & kb_Up) && (window->menu != NULL)) {
				// select the menu
				window->menu->selected = window->menu->first;
				window->menuSelected = true;
			} else if ((kb_Data[7] & kb_Down) && (window->menu != NULL) && (window->menuSelected)) {
				window->menu->selected = NULL;
				window->menuSelected = false;
			} else if ((kb_Data[7] & kb_Right) && (window->widgets.selected->next != NULL)) {
				if (!window->menuSelected) {
					// Select next widget
					window->widgets.selected->handler(window->widgets.selected, EVENT_BLUR);
					window->widgets.selected = window->widgets.selected->next;
					window->widgets.selected->handler(window->widgets.selected, EVENT_FOCUS);
				} else {
					// Select next menu item
				}
			} else if ((kb_Data[7] & kb_Left) && (window->widgets.selected->prev != NULL)) {
				if (!window->menuSelected) {
					// Select previous widget
					window->widgets.selected->handler(window->widgets.selected, EVENT_BLUR);
					window->widgets.selected = window->widgets.selected->prev;
					window->widgets.selected->handler(window->widgets.selected, EVENT_FOCUS);
				} else {
					// Select previous menu item
				}
			} else {
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
					if (!window->menuSelected) {
						// Menu is not selected
						window->widgets.selected->handler(window->widgets.selected, EVENT_KEYUP);

						// check special keys
						if (!(prevKbState[1] & kb_2nd)) {
							key_2nd = false;
						}
						if (!(prevKbState[2] & kb_Alpha)) {
							if (!alphaLock) {
								key_alpha = false;
							}
						}
					}
				}

				if (keydown) {
					if (!window->menuSelected) {
						window->widgets.selected->handler(window->widgets.selected, EVENT_KEYDOWN);
					} else {
						// Menu is selected
						if (window->menu->selected->type == MENUITEM_BUTTON) {
							window->menu->selected->handler(window->menu);
						} else if (window->menu->selected->type == MENUITEM_PARENT) {

						}
					}
				}

			}

		}

		// store current kb_Data for next check
		memcpy(prevKbState, kb_Data, 8);

		/* Paint */
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

		// Paint menu bar
		if (window->menu != NULL) {
			unsigned int menubarPaintOffset = 5;

			for (MenuItem *current=window->menu->first; current != NULL; current = current->next) {
				if (current->type == MENUITEM_SEPARATOR) {
					// Draw separator
					gfx_HorizLine_NoClip(menubarPaintOffset, 5, MENUBAR_HEIGHT);
					menubarPaintOffset += 6;
				} else {
					unsigned int labelWidth = gfx_GetStringWidth(current->label);

					if (menubarPaintOffset + labelWidth > window->width) {
						// stop painting menu items; no more room
						break;
					}

					gfx_PrintStringXY(current->label, menubarPaintOffset, 5);

					menubarPaintOffset += labelWidth + 5;
				}
			}

			gfx_HorizLine_NoClip(0, MENUBAR_HEIGHT, window->width);

			gfx_BlitRectangle(gfx_buffer, 0, 0, window->width, MENUBAR_HEIGHT+1);
		}
	}
}
