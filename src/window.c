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

uint16_t prevKbState[8] = { 0 };

bool key_2nd = false;
bool key_alpha = false;
bool alphaLock = false;

void cedar_initWindow(Window *window, WindowEventHandler *handler) {
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

	window->handler = handler;
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

int dispatchEvents(Window *window) {
	uint24_t handlerReturnCode;

	kb_Scan();

	// Check for key events
	bool keydown = false, keyup = false;
	for (int i=1; i < 8; ++i) {
		if ((prevKbState[i] & kb_Data[i]) != prevKbState[i]) {
			// Not all keys previously pressed are still pressed (keyup)
			keyup = true;
		} else if ((prevKbState[i] & kb_Data[i]) != kb_Data[i]) {
			// Not all keys currently pressed were previously pressed (keydown)
			keydown = true;
		}
	}

	if (wasKeyPressed(1, kb_2nd)) {
		key_2nd = !key_2nd;
	} else if (wasKeyPressed(2, kb_Alpha)) {
		key_alpha = !key_alpha;

		if (key_2nd) {
			key_2nd = false;
			alphaLock = !alphaLock;
		}
	} else {
		if (keyup) {
			handlerReturnCode = window->handler(window, EVENT_KEYUP);
			switch (handlerReturnCode) {
				case HANDLER_EXIT:
					return HANDLER_EXIT;
				case HANDLER_DO_NOT_PROPAGATE:
					goto dontPropagateKeyup;
			}

			if (window->menu && window->menu->selected) {
				// Menu is selected
			} else {
				// Menu is not selected
				handlerReturnCode = window->widgets.selected->handler(window->widgets.selected, EVENT_KEYUP);
				switch (handlerReturnCode) {
					case HANDLER_EXIT:
						return HANDLER_EXIT;
					case HANDLER_DO_NOT_PROPAGATE:
						goto dontPropagateKeyup;
				}
			}

			dontPropagateKeyup:
			if (!wasKeyReleased(1, kb_2nd)) key_2nd = false;
			if (!wasKeyReleased(1, kb_Alpha) && !alphaLock) key_alpha = false;
		}

		if (keydown) {
			handlerReturnCode = window->handler(window, EVENT_KEYDOWN);
			switch (handlerReturnCode) {
				case HANDLER_EXIT:
					return HANDLER_EXIT;
				case HANDLER_DO_NOT_PROPAGATE:
					goto dontPropagateKeydown;
			}

			if (window->menu && window->menu->selected) {
				// Menu is selected
				if (wasKeyPressed(6, kb_Enter)) {
					MenuItem *selected = getLastSelectedMenuItem(window->menu);

					if (selected->type == MENUITEM_BUTTON) {
						selected->handler(window->menu);
					} else if (selected->type == MENUITEM_PARENT && selected->submenu != NULL) {
						selected->submenu->submenuActive = true;
					}
				}
			} else {
				// Menu is not selected
				handlerReturnCode = window->widgets.selected->handler(window->widgets.selected, EVENT_KEYDOWN);
				switch (handlerReturnCode) {
					case HANDLER_EXIT:
						return HANDLER_EXIT;
					case HANDLER_DO_NOT_PROPAGATE:
						goto dontPropagateKeydown;
				}
			}

			dontPropagateKeydown:
			;
		}
	}

	// store current kb_Data for next check
	// (kb_Data definition is a little weird so we use the address from keypadc.h)
	memcpy(prevKbState, (void *)0xF50010, sizeof(uint16_t) * 8);

	return 0;
}

int cedar_display(Window *window) {
	uint24_t handlerReturnCode;

	handlerReturnCode = window->handler(window, EVENT_CREATE);
	if (handlerReturnCode == HANDLER_EXIT) {
		return HANDLER_EXIT;
	}

	window->widgets.selected = window->widgets.first;

	DBGPRINT("Initialized\n");

	for (;;) {
		/* Dispatch events */
		handlerReturnCode = dispatchEvents(window);
		if (handlerReturnCode != HANDLER_NORMAL) return handlerReturnCode;

		/* Paint */
		handlerReturnCode = window->handler(window, EVENT_PAINT);
		switch (handlerReturnCode) {
			case HANDLER_EXIT:
				return HANDLER_EXIT;
			case HANDLER_DO_NOT_PROPAGATE:
				goto dontPropagatePaint;
		}

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
				handlerReturnCode = widget->handler(widget, EVENT_PAINT);
				switch (handlerReturnCode) {
					case HANDLER_EXIT:
						return HANDLER_EXIT;
					case HANDLER_DO_NOT_PROPAGATE:
						goto dontPropagatePaint;
				}
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

		dontPropagatePaint:
		gfx_SwapDraw();
	}
}


uint24_t defaultWindowEventHandler(Window *window, int event) {
	uint24_t handlerReturnCode;

	switch (event) {
		case EVENT_KEYDOWN:
			DBGPRINT("Keydown\n");
			if (key_2nd && wasKeyPressed(1, kb_Mode)) {
				// Quit
				return HANDLER_EXIT;
			} else if (wasKeyPressed(7, kb_Up)) {
				// Up arrow key pressed
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

				return HANDLER_DO_NOT_PROPAGATE;
			} else if (wasKeyPressed(7, kb_Down)) {
				// Down arrow key pressed
				if (window->menu) {
					window->menu->selected = NULL;
				}

				return HANDLER_DO_NOT_PROPAGATE;
			} else if (wasKeyPressed(7, kb_Right)) {
				// Right arrow key pressed
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
					handlerReturnCode = window->widgets.selected->handler(window->widgets.selected, EVENT_BLUR);
					switch (handlerReturnCode) {
						case HANDLER_EXIT:
							return HANDLER_EXIT;
					}

					window->widgets.selected = window->widgets.selected->next;
					handlerReturnCode = window->widgets.selected->handler(window->widgets.selected, EVENT_FOCUS);
					switch (handlerReturnCode) {
						case HANDLER_EXIT:
							return HANDLER_EXIT;
					}
				}

				return HANDLER_DO_NOT_PROPAGATE;
			} else if (wasKeyPressed(7, kb_Left)) {
				// Left arrow key pressed
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
					handlerReturnCode = window->widgets.selected->handler(window->widgets.selected, EVENT_BLUR);
					switch (handlerReturnCode) {
						case HANDLER_EXIT:
							return HANDLER_EXIT;
					}

					window->widgets.selected = window->widgets.selected->prev;
					handlerReturnCode = window->widgets.selected->handler(window->widgets.selected, EVENT_FOCUS);
					switch (handlerReturnCode) {
						case HANDLER_EXIT:
							return HANDLER_EXIT;
					}
				}

				return HANDLER_DO_NOT_PROPAGATE;
			}
			break;
	}

	return HANDLER_NORMAL;
}
