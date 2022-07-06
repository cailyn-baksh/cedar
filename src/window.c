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

bool key_2nd = false;
bool key_alpha = false;
bool alphaLock = false;

/*
 * Blanks an portion of the drawing buffer
 */
void clearRect(int x, int y, int width, int height) {
	uint8_t prevColor = gfx_SetColor(255);
	gfx_FillRectangle(x, y, width, height);
	gfx_SetColor(prevColor);
}

/*
 * Paint active submenus.
 */
void paintActiveSubmenus(Menu *menu) {
	if (menu->submenuActive && menu->selected && menu->selected->type == MENUITEM_PARENT) {
		if (menu->selected->submenu->submenuActive) {
			// Paint a submenu of this submenu
			paintActiveSubmenus(menu->selected->submenu);
		} else {
			// Paint this submenu
			gfx_Rectangle(5, MENUBAR_HEIGHT, 50, GFX_LCD_HEIGHT - MENUBAR_HEIGHT);

			for (MenuItem *current=menu->first; current != NULL; current = current->next) {
				if (current->type == MENUITEM_SEPARATOR) {

				} else {

				}
			}
		}
	}
}

/*
 * Returns the last selected menu item in a menu
 */
MenuItem *getLastSelectedMenuItem(Menu *menu) {
	if (menu->submenuActive) {
		return getLastSelectedMenuItem(menu->selected->submenu);
	} else {
		return menu->selected;
	}
}

int cedar_display(Window *window) {
	// Previous state of the keyboard
	uint16_t prevKbState[8] = { 0 };

	window->widgets.selected = window->widgets.first;

	for (;;) {
		/* Dispatch events */
		kb_Scan();

		// Check for key events
		bool keydown = false, keyup = false;
		for (int i=1; i < 8; ++i) {
			if (!keyup && (prevKbState[i] & kb_Data[i]) != prevKbState[i]) {
				// Not all keys previously pressed are still pressed (keyup)
				keyup = true;
			} else if (!keydown && (prevKbState[i] & kb_Data[i]) != kb_Data[i]) {
				// Not all keys currently pressed were previously pressed (keydown)
				keydown = true;
			}
		}

		if (keydown && (kb_Data[1] & kb_2nd)) {
			key_2nd = !key_2nd;
		} else if (keydown && (kb_Data[2] & kb_Alpha)) {
			key_alpha = !key_alpha;

			if (key_2nd) {
				key_2nd = false;
				alphaLock = !alphaLock;
			}
		} else {
			if (keydown && key_2nd && (kb_Data[1] & kb_Mode)) {
				// Quit
				return 0;
			} else if (keydown && (kb_Data[7] & kb_Up)) {
				// Up key pressed
				if (window->menu) {
					// Select the first menu item that is not a separator
					MenuItem *firstItem = window->menu->first;

					while (firstItem != NULL && firstItem->type == MENUITEM_SEPARATOR) {
						firstItem = firstItem->next;
					}

					if (firstItem != NULL) {
						window->menu->selected = firstItem;
					}
				}
			} else if (keydown && (kb_Data[7] & kb_Down)) {
				// Down key pressed
				if (window->menu) {
					window->menu->selected = NULL;
				}
			} else if (keydown && (kb_Data[7] & kb_Right)) {
				// Right key pressed
				if (window->menu && window->menu->selected) {
					// Select next menu item that is not a separator
					MenuItem *nextItem = window->menu->selected->next;

					while (nextItem != NULL && nextItem->type == MENUITEM_SEPARATOR) {
						nextItem = nextItem->next;
					}

					if (nextItem != NULL) {
						window->menu->selected = nextItem;
					}
				} else if (window->widgets.selected->next) {
					// Select next widget
					window->widgets.selected->handler(window->widgets.selected, EVENT_BLUR);
					window->widgets.selected = window->widgets.selected->next;
					window->widgets.selected->handler(window->widgets.selected, EVENT_FOCUS);
				}
			} else if (keydown && (kb_Data[7] & kb_Left)) {
				// Left key pressed
				if (window->menu && window->menu->selected) {
					// Select previous menu item that is not a separator
					MenuItem *prevItem = window->menu->selected->prev;

					while (prevItem != NULL && prevItem->type == MENUITEM_SEPARATOR) {
						prevItem = prevItem->prev;
					}

					if (prevItem != NULL) {
						window->menu->selected = prevItem;
					}
				} else if (window->widgets.selected->prev) {
					// Select previous widget
					window->widgets.selected->handler(window->widgets.selected, EVENT_BLUR);
					window->widgets.selected = window->widgets.selected->prev;
					window->widgets.selected->handler(window->widgets.selected, EVENT_FOCUS);
				}
			} else {
				if (keyup) {
					DBGPRINT("Keyup\n");
					if (!window->menu || !window->menu->selected) {
						// Menu is not selected
						window->widgets.selected->handler(window->widgets.selected, EVENT_KEYUP);

						// check special keys
						if (!(prevKbState[1] & kb_2nd)) key_2nd = false;
						if (!(prevKbState[2] & kb_Alpha) && !alphaLock) key_alpha = false;
					}
				}

				if (keydown) {
					DBGPRINT("Keydown\n");
					if (!window->menu || !window->menu->selected) {
						window->widgets.selected->handler(window->widgets.selected, EVENT_KEYDOWN);
					} else {
						// Menu is selected
						if (kb_Data[6] & kb_Enter) {
							DBGPRINT("Menu item selected");

							MenuItem *selected = getLastSelectedMenuItem(window->menu);

							if (selected->type == MENUITEM_BUTTON) {
								selected->handler(window->menu);
							} else if (selected->type == MENUITEM_PARENT && selected->submenu != NULL) {
								selected->submenu->submenuActive = true;
							}
						}
					}
				}
			}
		}

		// store current kb_Data for next check
		// (kb_Data definition is a little weird so we use the address from keypadc.h)
		memcpy(prevKbState, (void *)0xF50010, sizeof(uint16_t) * 8);

		/* Paint */
		// Paint widgets
		for (Widget *widget = window->widgets.first; widget != NULL; widget = widget->next) {
			if (widget->x >= window->projLeft
				&& (widget->x + widget->width) <= (window->projLeft + window->width)
				&& widget->y >= window->projTop
				&& (widget->y + widget->height) <= (window->projTop + window->height)
			) {
				// Widget is visible

				// Clear its region of the buffer
				clearRect(widget->realX, widget->realY, widget->width, widget->height);

				// Set the real coordinates
				widget->realX = window->realLeft + (widget->x - window->projLeft);
				widget->realY = window->realTop + (widget->y - window->projTop);

				// Draw it
				widget->handler(widget, EVENT_PAINT);
			}
		}

		// Paint menu bar
		if (window->menu != NULL) {
			unsigned int menubarPaintOffset = 5;

			clearRect(0, 0, window->width, MENUBAR_HEIGHT);

			for (MenuItem *current=window->menu->first; current != NULL; current = current->next) {
				if (current->type == MENUITEM_SEPARATOR) {
					// Draw separator
					gfx_VertLine_NoClip(menubarPaintOffset, 2, MENUBAR_HEIGHT-4);
					menubarPaintOffset += 6;
				} else {
					unsigned int labelWidth = gfx_GetStringWidth(current->label);

					if (menubarPaintOffset + labelWidth > window->width) {
						// stop painting menu items; no more room
						break;
					}

					if (current == window->menu->selected) {  // pointer comparison
						gfx_FillRectangle(menubarPaintOffset-2, 3, labelWidth+4, 14);

						gfx_SetTextFGColor(255);
						gfx_SetTextBGColor(0);
						gfx_SetTextTransparentColor(0);
						gfx_PrintStringXY(current->label, menubarPaintOffset, 5);
						gfx_SetTextFGColor(0);
						gfx_SetTextBGColor(255);
						gfx_SetTextTransparentColor(255);
					} else {
						gfx_PrintStringXY(current->label, menubarPaintOffset, 5);
					}

					menubarPaintOffset += labelWidth + 5;
				}
			}

			gfx_HorizLine_NoClip(0, MENUBAR_HEIGHT, window->width);

			paintActiveSubmenus(window->menu);
		}

		gfx_SwapDraw();
	}
}
