#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <graphx.h>
#include <keypadc.h>

#define _NOEXTERN
#include "cedar.h"
#undef _NOEXTERN

#include <assert.h>
#include "cedardbg.h"

uint16_t prevKbState[8] = { 0 };

bool key_2nd = false;
bool key_alpha = false;
bool alphaLock = false;

void cedar_initWindow(Window *window, EventHandler *handler) {
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
	for (Widget *current=window->widgets.first; current != NULL; current = current->next) {
		if (current->prev != NULL) {
			cedar_destroyWidget(current->prev);
		}
		window->widgets.first->next->handler(window->widgets.first->next, EVENT_DESTROY, NULL, 0);
	}

	free(window);
}

void cedar_addWidget(Window *window, Widget *widget) {
	if (window->widgets.first == NULL) {
		// This is the first widget
		window->widgets.first = widget;
	} else {
		// Not the first widget, link this widget into the list
		window->widgets.last->next = widget;
		widget->prev = window->widgets.last;
	}
	window->widgets.last = widget;
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
	if (menu->selected
	 && menu->selected->type == MENUITEM_PARENT
	  && menu->selected->child->active) {
		// Submenu is active
		Menu *activeMenu = menu->selected->child;

		if (activeMenu->selected->type == MENUITEM_PARENT
		 && activeMenu->selected->child->active) {
			// Paint a submenu of this submenu
			paintActiveSubmenus(activeMenu);
		} else {
			// Paint this submenu
			gfx_FillRectangle(7, MENUBAR_HEIGHT+2, MENU_DROPDOWN_WIDTH, MENU_DROPDOWN_HEIGHT);  // shadow :)
			clearRect(5, MENUBAR_HEIGHT, MENU_DROPDOWN_WIDTH, MENU_DROPDOWN_HEIGHT);
			gfx_Rectangle(5, MENUBAR_HEIGHT, MENU_DROPDOWN_WIDTH, MENU_DROPDOWN_HEIGHT);

			int submenuItemY = MENUBAR_HEIGHT + 5;

			for (MenuItem *current=activeMenu->first; current != NULL; current = current->next) {
				if (current->type == MENUITEM_SEPARATOR) {
					gfx_HorizLine(5, submenuItemY+4, MENU_DROPDOWN_WIDTH);
					submenuItemY += 10;
				} else {
					if (current->invertColours
					 ^ ((activeMenu->selected != NULL)
					   && (current == activeMenu->selected))) {
						// indicate item is selected
						gfx_FillRectangle(5, submenuItemY-2, MENU_DROPDOWN_WIDTH, 12);

						gfx_SetTextFGColor(255);
						gfx_SetTextBGColor(0);
						gfx_SetTextTransparentColor(0);
						gfx_PrintStringXY(current->label, 10, submenuItemY);
						gfx_SetTextFGColor(0);
						gfx_SetTextBGColor(255);
						gfx_SetTextTransparentColor(255);
					} else {
						if (activeMenu->selected != NULL && current == activeMenu->selected) {
							// Item is selected but colours are inverted
							gfx_Rectangle(5, submenuItemY-2, MENU_DROPDOWN_WIDTH, 12);
						}
						gfx_PrintStringXY(current->label, 10, submenuItemY);
					}

					submenuItemY += 10;
				}
			}
		}
	}
}

/*
 * Returns the last selected menu item in a menu
 */
MenuItem *getLastSelectedMenuItem(Menu *menu) {
	if (menu->selected
	 && menu->selected->type == MENUITEM_PARENT
	  && menu->selected->child->active) {
		// Return selected item in submenu
		return getLastSelectedMenuItem(menu->selected->child);
	} else {
		return menu->selected;
	}
}

uint24_t deselectSubmenuTree(Menu *root) {
	uint24_t submenusClosed = 1;

	if (root->selected != NULL
	 && root->selected->type == MENUITEM_PARENT
	  && root->selected->child->active) {
		root->selected->child->selected = NULL;
		submenusClosed += deselectSubmenuTree(root->selected->child);
	}
	root->active = false;

	return submenusClosed;
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
			handlerReturnCode = window->handler(window, EVENT_KEYUP, NULL, 0);
			switch (handlerReturnCode) {
				case CALLBACK_EXIT:
					return CALLBACK_EXIT;
				case CALLBACK_DO_NOT_PROPAGATE:
					goto dontPropagateKeyup;
			}

			if (window->menu && window->menu->selected) {
				// Menu is selected
				if (wasKeyReleased(6, kb_Enter)) {
					MenuItem *selected = getLastSelectedMenuItem(window->menu);

					selected->invertColours = false;

					switch (selected->type) {
						case MENUITEM_BUTTON:
							handlerReturnCode = window->handler(window, EVENT_MENUSELECT, window->menu->selected->id, 0);
							switch (handlerReturnCode) {
								case CALLBACK_EXIT:
									return CALLBACK_EXIT;
							}
							break;
						case MENUITEM_PARENT:
							if (selected->child != NULL) {
								selected->child->active = true;
								selected->child->selected = selected->child->first;
							}
							break;
					}
				}
			} else {
				// Menu is not selected
				handlerReturnCode = window->widgets.selected->handler(window->widgets.selected, EVENT_KEYUP, NULL, 0);
				switch (handlerReturnCode) {
					case CALLBACK_EXIT:
						return CALLBACK_EXIT;
					case CALLBACK_DO_NOT_PROPAGATE:
						goto dontPropagateKeyup;
				}
			}

			dontPropagateKeyup:
			if (!wasKeyReleased(1, kb_2nd)) key_2nd = false;
			if (!wasKeyReleased(1, kb_Alpha) && !alphaLock) key_alpha = false;
		}

		if (keydown) {
			handlerReturnCode = window->handler(window, EVENT_KEYDOWN, NULL, 0);
			switch (handlerReturnCode) {
				case CALLBACK_EXIT:
					return CALLBACK_EXIT;
				case CALLBACK_DO_NOT_PROPAGATE:
					goto dontPropagateKeydown;
			}

			if (window->menu && window->menu->selected) {
				// Menu is selected
				if (wasKeyPressed(6, kb_Enter)) {
					MenuItem *selected = getLastSelectedMenuItem(window->menu);

					if (selected->type != MENUITEM_SEPARATOR) {
						selected->invertColours = true;
					}
				}
			} else {
				// Menu is not selected
				handlerReturnCode = window->widgets.selected->handler(window->widgets.selected, EVENT_KEYDOWN, NULL, 0);
				switch (handlerReturnCode) {
					case CALLBACK_EXIT:
						return CALLBACK_EXIT;
					case CALLBACK_DO_NOT_PROPAGATE:
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

	handlerReturnCode = window->handler(window, EVENT_CREATE, NULL, 0);
	if (handlerReturnCode == CALLBACK_EXIT) {
		return CALLBACK_EXIT;
	}

	{
		// Set selected widget to first widget that can be selected
		Widget *firstSelectable = window->widgets.first;
		while (firstSelectable != NULL && !(firstSelectable->attrs & ATTR_FOCUSABLE)) {
			firstSelectable = firstSelectable->next;
		}

		if (firstSelectable != NULL) {
			window->widgets.selected = firstSelectable;
		} else {
			window->widgets.selected = window->widgets.first;
		}

		window->widgets.selected->handler(window->widgets.selected, EVENT_FOCUS, NULL, 0);
	}

	for (;;) {
		/* Dispatch events */
		handlerReturnCode = dispatchEvents(window);
		if (handlerReturnCode != CALLBACK_DEFAULT) {
			return handlerReturnCode;
		}

		/* Paint */
		handlerReturnCode = window->handler(window, EVENT_PAINT, NULL, 0);
		switch (handlerReturnCode) {
			case CALLBACK_EXIT:
				return CALLBACK_EXIT;
			case CALLBACK_DO_NOT_PROPAGATE:
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
				handlerReturnCode = widget->handler(widget, EVENT_PAINT, NULL, 0);
				switch (handlerReturnCode) {
					case CALLBACK_EXIT:
						return CALLBACK_EXIT;
					case CALLBACK_DO_NOT_PROPAGATE:
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

					if (current->invertColours
					 ^ ((window->menu->selected != NULL)
					   && (current == window->menu->selected))) {
						// indicate item is selected
						gfx_FillRectangle(menubarPaintOffset-2, 3, labelWidth+4, 14);

						gfx_SetTextFGColor(255);
						gfx_SetTextBGColor(0);
						gfx_SetTextTransparentColor(0);
						gfx_PrintStringXY(current->label, menubarPaintOffset, 5);
						gfx_SetTextFGColor(0);
						gfx_SetTextBGColor(255);
						gfx_SetTextTransparentColor(255);
					} else {
						if (window->menu->selected != NULL && current == window->menu->selected) {
							// Item is selected but colours are inverted
							gfx_Rectangle(menubarPaintOffset-2, 3, labelWidth+4, 14);
						}
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

MenuItem *getNextSelectableMenuitem(Menu *menu) {
	MenuItem *item = menu->selected->next;

	while (item != NULL && item->type == MENUITEM_SEPARATOR) {
		item = item->next;
	}

	return item;
}

MenuItem *getPrevSelectableMenuitem(Menu *menu) {
	MenuItem *item = menu->selected->prev;

	while (item != NULL && item->type == MENUITEM_SEPARATOR) {
		item = item->prev;
	}

	return item;
}

uint24_t defaultWindowEventHandler(Window *self, EVENT event, ID id, uint24_t param) {
	static uint8_t blankScreenThisFrame = 0;
	uint24_t handlerReturnCode;

	switch (event) {
		case EVENT_PAINT:
			if (blankScreenThisFrame > 0) {
				gfx_FillScreen(255);
				--blankScreenThisFrame;
			}
			break;
		case EVENT_KEYDOWN:
			if (key_2nd && wasKeyPressed(1, kb_Mode)) {
				// Quit
				return CALLBACK_EXIT;
			} else if (wasKeyPressed(7, kb_Up)) {
				// Up arrow key pressed
				if (self->menu) {
					if (self->menu->selected->type == MENUITEM_PARENT
					 && self->menu->selected->child->active) {
						// A submenu is active
						Menu *submenu = getLastSelectedMenuItem(self->menu)->parent;
						MenuItem *prevItem = getPrevSelectableMenuitem(submenu);

						if (prevItem != NULL) {
							// Select previous submenu item
							submenu->selected = prevItem;
						} else {
							self->menu->selected->child->active = false;
							deselectSubmenuTree(self->menu);
							blankScreenThisFrame = 2;  // Clear buffer and next buffer
						}
					} else {
						// Select the first menu item that is not a separator
						MenuItem *firstItem = self->menu->first;

						while (firstItem != NULL && firstItem->type == MENUITEM_SEPARATOR) {
							firstItem = firstItem->next;
						}

						if (firstItem != NULL) {
							self->menu->selected = firstItem;
						}

						// Send blur event to selected widget
						if (self->widgets.selected != NULL) {
							self->widgets.selected->handler(self->widgets.selected, EVENT_BLUR, NULL, 0);
						}
					}
				}

				return CALLBACK_DO_NOT_PROPAGATE;
			} else if (wasKeyPressed(7, kb_Down)) {
				// Down arrow key pressed
				if (self->menu) {
					if (self->menu->selected->type == MENUITEM_PARENT
					 && self->menu->selected->child->active) {
						// A submenu is active
						Menu *submenu = getLastSelectedMenuItem(self->menu)->parent;
						MenuItem *nextItem = getNextSelectableMenuitem(submenu);

						if (nextItem != NULL) {
							submenu->selected = nextItem;
						}
					} else {
						self->menu->selected = NULL;

						// Send focus event to selected widget
						if (self->widgets.selected != NULL) {
							self->widgets.selected->handler(self->widgets.selected, EVENT_FOCUS, NULL, 0);
						}
					}
				}

				return CALLBACK_DO_NOT_PROPAGATE;
			} else if (wasKeyPressed(7, kb_Right)) {
				// Right arrow key pressed
				if (self->menu && self->menu->selected) {
					// Select next menu item that is not a separator
					if (deselectSubmenuTree(self->menu) > 1) {
						blankScreenThisFrame = 2;
					}

					MenuItem *nextItem = getNextSelectableMenuitem(self->menu);

					if (nextItem != NULL) {
						self->menu->selected = nextItem;
					}
				} else if (self->widgets.selected->next != NULL) {
					// Select next widget
					Widget *nextSelectableWidget = self->widgets.selected->next;
					while (!(nextSelectableWidget->attrs & ATTR_FOCUSABLE)){
						nextSelectableWidget = nextSelectableWidget->next;
					}

					if (nextSelectableWidget != NULL) {
						// send blur event to previously selected widget
						handlerReturnCode = self->widgets.selected->handler(self->widgets.selected, EVENT_BLUR, NULL, 0);
						switch (handlerReturnCode) {
							case CALLBACK_EXIT:
								return CALLBACK_EXIT;
						}

						self->widgets.selected = nextSelectableWidget;

						// send focus event
						handlerReturnCode = self->widgets.selected->handler(self->widgets.selected, EVENT_FOCUS, NULL, 0);
						switch (handlerReturnCode) {
							case CALLBACK_EXIT:
								return CALLBACK_EXIT;
						}
					}
				}

				return CALLBACK_DO_NOT_PROPAGATE;
			} else if (wasKeyPressed(7, kb_Left)) {
				// Left arrow key pressed
				if (self->menu && self->menu->selected) {
					// Select previous menu item that is not a separator
					if (deselectSubmenuTree(self->menu) > 1) {
						blankScreenThisFrame = 2;
					}

					MenuItem *prevItem = getPrevSelectableMenuitem(self->menu);

					if (prevItem != NULL) {
						self->menu->selected = prevItem;
					}
				} else if (self->widgets.selected->prev != NULL) {
					// Select previous widget
					Widget *prevSelectableWidget = self->widgets.selected->prev;
					while (prevSelectableWidget != NULL && !(prevSelectableWidget->attrs & ATTR_FOCUSABLE)){
						prevSelectableWidget = prevSelectableWidget->prev;
					}

					if (prevSelectableWidget != NULL) {
						// send blur event to previously selected widget
						handlerReturnCode = self->widgets.selected->handler(self->widgets.selected, EVENT_BLUR, NULL, 0);
						switch (handlerReturnCode) {
							case CALLBACK_EXIT:
								return CALLBACK_EXIT;
						}

						self->widgets.selected = prevSelectableWidget;

						// select focus event
						handlerReturnCode = self->widgets.selected->handler(self->widgets.selected, EVENT_FOCUS, NULL, 0);
						switch (handlerReturnCode) {
							case CALLBACK_EXIT:
								return CALLBACK_EXIT;
						}
					}
				}

				return CALLBACK_DO_NOT_PROPAGATE;
			}
			break;
	}

	return CALLBACK_DEFAULT;
}
