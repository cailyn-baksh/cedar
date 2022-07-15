// FIXME: not all callback return codes are properly handled

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

uint16_t cedar_prevKbState[8] = { 0 };

bool cedar_2nd = false;
bool cedar_alpha = false;
bool cedar_alphaLock = false;

/*
 * Get the currently active submenu of a parent menu
 *
 * parent		The root level menu of the submenu tree
 * Returns the menu that is currently active, or NULL
 */
CedarMenu *getActiveSubmenu(CedarMenu *parent) {
	CedarMenu *menu = parent;

	while (menu != NULL && menu->submenuActive) {
		if (menu->selected != NULL) {
			menu = menu->selected->child;
		} else {
			menu = NULL;
		}
	}

	return menu;
}

CedarMenuItem *getNextSelectableMenuitem(CedarMenu *menu) {
	CedarMenuItem *item = menu->selected->next;

	while (item != NULL && isMenuItemSeparator(item)) {
		item = item->next;
	}

	return item;
}

CedarMenuItem *getPrevSelectableMenuitem(CedarMenu *menu) {
	CedarMenuItem *item = menu->selected->prev;

	while (item != NULL && isMenuItemSeparator(item)) {
		item = item->prev;
	}

	return item;
}

void deselectAllSubmenus(CedarMenu *menu) {
	if (menu != NULL && menu->submenuActive && menu->selected->child != NULL) {
		deselectAllSubmenus(menu->selected->child);
	}

	menu->selected = NULL;
	menu->submenuActive = false;
}

static uint24_t defaultWindowEventHandler(CedarWindow *self, EVENT event, uint24_t param) {
	uint24_t handlerReturnCode;

	switch (event) {
		case EVENT_PAINT:
			// TODO: change drawing so that we dont need to do this
			// consider blanking draw buffer before each paint
			// blitting vs buffer swapping:
			//   - blitting would require two large memory operations (blank
			//     screen and blit draw buffer)
			//   - buffer swapping would require one large memory operation
			//     (just blanking the screen)
			// is it possible to avoid blanking the screen?
			//   with blitting it could be possible (is it worth it?)
			// probable best solution:
			//   - use blitting, and only blank and reblit regions as necessary
			//   - give each component a repaint flag that event handlers can
			//     use to force a repaint
			//   - provide a global repaint flag that forces all to redraw
			break;
		case EVENT_KEYDOWN:
			switch (param) {
				case CEDAR_KB_QUIT:
					// quit
					return CALLBACK_EXIT;
				case CEDAR_KB_UP:
					// up arrow
					if (self->menu != NULL) {
						// this window has a menu; modify menu selection
						if (self->menu->submenuActive
						 && self->menu->selected->child != NULL) {
							// a submenu is active
							CedarMenu *submenu = getActiveSubmenu(self->menu);
							CedarMenuItem *prevItem = getPrevSelectableMenuItem(submenu);

							if (prevItem != NULL) {
								// Select previous submenu item
								submenu->selected = prevItem;
							} else {
								// Exit submenu
								submenu->submenuActive = false;
								submenu->selected = NULL;
							}
						} else if (self->menu->selected == NULL) {
							// no menu is selected; select menu bar
							CedarMenuItem *firstItem = self->menu->first;

							while (firstItem != NULL && isMenuItemSeparator(firstItem)) {
								// try next one
								firstItem = firstItem->next;
							}

							if (firstItem != NULL) {
								// select first selectable item in the menubar
								self->menu->selected = firstItem;

								if (self->widgets.selected != NULL) {
									// dispatch blur event to previously selected widget
									cedar_dispatchEvent(EVENT_BLUR, self->widgets.selected, 0);
								}
							}
						}
					}

					// dont propagate this event to the widget
					return CALLBACK_DO_NOT_PROPAGATE;
				case CEDAR_KB_DOWN:
					// down arrow
					if (self->menu != NULL) {
						// this window has a menu; modify menu selection
						if (self->menu->submenuActive
						 && self->menu->selected->child != NULL) {
							// submenu is active
							CedarMenu *submenu = getActiveSubmenu(self->menu);
							CedarMenuItem *nextItem = getNextSelectableMenuitem(submenu);

							if (nextItem != NULL) {
								// Select next submenu item
								submenu->selected = nextItem;
							}
						} else if (self->menu->selected != NULL) {
							// no submenu active; deselect menu bar
							self->menu->selected = NULL;

							// Send focus event to selected widget
							if (self->widgets.selected != NULL) {
								// self->widget.selected should never be NULL
								cedar_dispatchEvent(EVENT_FOCUS, self->widgets.selected, 0);
							}
						}
					}

					// dont propagate this event to the widget
					return CALLBACK_DO_NOT_PROPAGATE;
				case CEDAR_KB_RIGHT:
					// right arrow
					if (self->menu != NULL && self->menu->selected != NULL) {
						// Select next menu item that is not a separator
						CedarMenuItem *nextItem = getNextSelectableMenuitem(self->menu);

						if (nextItem != NULL) {
							// There is a next item that we can select
							deselectAllSubmenus(self->menu);
							self->menu->selected = nextItem;
						}
					} else if (self->widgets.selected->next != NULL) {
						// Select next widgets
						CedarWidget *nextWidget = self->widgets.selected->next;
						while (nextWidget != NULL
						 && !(nextWidget->attrs & ATTR_FOCUSABLE)) {
							nextWidget = nextWidget->next;
						}

						if (nextWidget != NULL) {
							cedar_dispatchEvent(EVENT_BLUR, self->widgets.selected, 0);
							self->widgets.selected = nextWidget;
							cedar_dispatchEvent(EVENT_FOCUS, self->widgets.selected, 0);
						}
					}

					return CALLBACK_DO_NOT_PROPAGATE;
				case CEDAR_KB_LEFT:
					// left arrow
					if (self->menu != NULL && self->menu->selected != NULL) {
						// select the previous menu item that is not a separator
						CedarMenuItem *prevItem = getPrevSelectableMenuItem(self->menu);

						if (prevItem != NULL) {
							// There is a previous item that we can select
							deselectAllSubmenus(self->menu);
							self->menu->selected = prevItem;
						}
					} else if (self->widgets.selected->next != NULL) {
						// Select prev widgets
						CedarWidget *prevWidget = self->widgets.selected->prev;
						while (prevWidget != NULL
						 && !(prevWidget->attrs & ATTR_FOCUSABLE)) {
							prevWidget = prevWidget->prev;
						}

						if (prevWidget != NULL) {
							cedar_dispatchEvent(EVENT_BLUR, self->widgets.selected, 0);
							self->widgets.selected = prevWidget;
							cedar_dispatchEvent(EVENT_FOCUS, self->widgets.selected, 0);
						}
					}

					return CALLBACK_DO_NOT_PROPAGATE;
			}
		case EVENT_KEYUP:
			switch (param) {
				case CEDAR_KB_ENTER:
					if (self->menu != NULL && self->menu->selected != NULL) {
						// menu is selected
						CedarMenuItem *selected = getLastSelectedMenuItem(self->menu);

						if (isMenuItemButton(selected)) {
							// button menu item
							cedar_dispatchEvent(EVENT_MENUSELECT, self, selected->id);
						} else if (isMenuItemSubmenu(selected)) {
							// submenu
							selected->parent->submenuActive = true;
							selected->child->selected = selected->child->first;
							selected->child->selected = getNextSelectableMenuitem(selected->child);  // FIXME: does not check for NULL
						}

						// event was for the menu so don't propagate to widgets
						return CALLBACK_DO_NOT_PROPAGATE;
					}
			}
			break;
	}

	return CALLBACK_DEFAULT;
}

void cedar_InitWindow(CedarWindow *window) {
	window->widgets.first = NULL;
	window->widgets.last = NULL;
	window->widgets.selected = NULL;

	window->menu = NULL;

	window->bounds.x = 0;
	window->bounds.y = 0;
	window->bounds.width = GFX_LCD_WIDTH;
	window->bounds.height = GFX_LCD_HEIGHT;

	window->origin.x = 0;
	window->origin.y = 0;

	window->handlers = malloc(sizeof(CedarEventHandler));
	window->handlers->callback = defaultWindowEventHandler;
	window->handlers->next = NULL;
}

void cedar_destroyWindow(CedarWindow *window) {
	// Clean up menu bar

	// Clean up widgets
	for (CedarWidget *current=window->widgets.first; current != NULL; current = current->next) {
		if (current->prev != NULL) {
			cedar_dispatchEvent(EVENT_DESTROY, current->prev, 0);
			cedar_destroyWidget(current->prev);
		}
	}

	free(window);
}

void cedar_addWidget(CedarWindow *window, CedarWidget *widget) {
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
void paintActiveSubmenus(CedarMenu *menu) {
	if (menu->selected != NULL
	 && menu->selected->child != NULL) {
		// This menu has a submenu
		CedarMenu *submenu = menu->selected->child;

		if (menu->submenuActive) {
			// This is the active submenu; paint it
			gfx_FillRectangle(7, MENUBAR_HEIGHT+2, MENU_DROPDOWN_WIDTH, MENU_DROPDOWN_HEIGHT);
			clearRect(6, MENUBAR_HEIGHT-1, MENU_DROPDOWN_WIDTH-1, MENU_DROPDOWN_HEIGHT-1);

			int submenuItemY = MENUBAR_HEIGHT + 5;
			for (CedarMenuItem *current=submenu->first; current != NULL; current = current->next) {
				if (isMenuItemSeparator(current)) {
					gfx_HorizLine(5, submenuItemY+4, MENU_DROPDOWN_WIDTH);
					submenuItemY += 10;
				} else {
					// TODO: add visual feedback for keypress before keyup
					if (current == submenu->selected) {
						// Indicate item is selected
						gfx_FillRectangle(5, submenuItemY-2, MENU_DROPDOWN_WIDTH, 12);

						// TODO: use customizable colour schemes
						gfx_SetTextFGColor(255);
						gfx_SetTextBGColor(0);
						gfx_SetTextTransparentColor(0);
						gfx_PrintStringXY(current->label, 10, submenuItemY);
						gfx_SetTextFGColor(0);
						gfx_SetTextBGColor(255);
						gfx_SetTextTransparentColor(255);
					} else {
						gfx_PrintStringXY(current->label, 10, submenuItemY);
					}

					submenuItemY += 10;
				}
			}
		} else {
			// Try painting a child submenu
			paintActiveSubmenus(submenu);
		}
	}
}

/*
 * Returns the last selected menu item in a menu
 */
CedarMenuItem *getLastSelectedMenuItem(CedarMenu *menu) {
	if (menu->selected != NULL
	 && isMenuItemSubmenu(menu->selected)
	 && menu->submenuActive) {
		// find selected item in submenu
		return getLastSelectedMenuItem(menu->selected->child);
	 } else {
		 return menu->selected;
	 }
}

CALLBACKRESULT _cedar_dispatchEvent(CedarEventHandler *firstHandler, void *self, EVENT event, uint24_t param) {
	CALLBACKRESULT result = CALLBACK_DEFAULT;
	CedarEventHandler *handler = firstHandler;

	while (result == CALLBACK_DEFAULT && handler != NULL) {
		result = handler->callback(self, event, param);
		handler = handler->next;
	}

	return result;
}

int cedar_display(CedarWindow *window) {
	uint24_t handlerReturnCode;

	handlerReturnCode = cedar_dispatchEvent(EVENT_CREATE, window, 0);
	if (handlerReturnCode == CALLBACK_EXIT) {
		return 0;  // TODO: decide how to return (should it return void?)
	}

	{
		// Set selected widget to first widget that can be selected
		CedarWidget *firstSelectable = window->widgets.first;
		while (firstSelectable != NULL && !(firstSelectable->attrs & ATTR_FOCUSABLE)) {
			firstSelectable = firstSelectable->next;
		}

		if (firstSelectable != NULL) {
			window->widgets.selected = firstSelectable;
		} else {
			window->widgets.selected = window->widgets.first;
		}

		cedar_dispatchEvent(EVENT_FOCUS, window->widgets.selected, 0);
	}

	uint16_t prevKbState[8] = { 0 };
	bool is2nd = false;
	bool isAlpha = false;
	bool alphalock = false;

	for (;;) {
		/* Dispatch events */

		// store current keyboard state and then update keyboard state
		memcpy(prevKbState, (void *)0xF50010, sizeof(uint16_t) * 8);
		kb_Scan();

		// Check meta keys
		if (!(prevKbState[1] & kb_2nd) && (kb_Data[1] & kb_2nd)) {
			// 2nd key was pressed
			is2nd = !is2nd;
		}

		if (!(prevKbState[2] & kb_Alpha) && (kb_Data[2] & kb_Alpha)) {
			// alpha key was pressed
			isAlpha = !isAlpha;

			if (is2nd) {
				// Apply alpha lock
				is2nd = false;
				alphalock = !alphalock;
			}
		}

		// Dispatch key events
		for (int i=1; i < 8; ++i) {
			if ((prevKbState[i] & kb_Data[i]) != prevKbState[i]) {
				// Not all keys previously pressed are still pressed (keyup)
				for (uint8_t mask=0x80; mask != 0; mask >>= 1) {
					// try each key mask on this row until we find one
					if ((prevKbState[i] & mask) && !(kb_Data[i] & mask)) {
						// This key was pressed
						uint24_t keycode = mask | (i << 8);

						if (keycode != kb_Key2nd && keycode != kb_KeyAlpha) {
							// ignore meta key release
							continue;
						}

						// dispatch event
						if (is2nd) keycode |= CEDAR_2NDFLAG;
						if (isAlpha) keycode |= CEDAR_ALPHAFLAG;

						// dispatch keyup to window
						handlerReturnCode =
							cedar_dispatchEvent(EVENT_KEYUP, window, keycode);

						// dispatch keyup to widget
						if (handlerReturnCode == CALLBACK_DEFAULT) {
							cedar_dispatchEvent(EVENT_KEYUP, window->widgets.selected, keycode);
						}
					}
				}
			} else if ((prevKbState[i] & kb_Data[i]) != kb_Data[i]) {
				// Not all keys currently pressed were previously pressed (keydown)
				for (uint8_t mask=0x80; mask != 0; mask >>= 1) {
					// try each key mask on this row until we find one
					if (!(prevKbState[i] & mask) && (kb_Data[i] & mask)) {
						// This key was released
						uint24_t keycode = mask | (i << 8);

						if (keycode != kb_Key2nd && keycode != kb_KeyAlpha) {
							// ignore meta key release
							continue;
						}

						if (is2nd) keycode |= CEDAR_2NDFLAG;
						if (isAlpha) keycode |= CEDAR_ALPHAFLAG;

						// dispatch keydown to window
						handlerReturnCode =
							cedar_dispatchEvent(EVENT_KEYDOWN, window, keycode);

						// dispatch keydown to widget
						if (handlerReturnCode == CALLBACK_DEFAULT) {
							cedar_dispatchEvent(EVENT_KEYDOWN, window->widgets.selected, keycode);
						}
					}
				}
			}
		}

		handlerReturnCode = dispatchEvents(window);
		if (handlerReturnCode != CALLBACK_DEFAULT) {
			return handlerReturnCode;
		}

		/* Paint */
		cedar_dispatchEvent(EVENT_PAINT, window, 0);

		for (CedarWidget *widget=window->widgets.first; widget != NULL; widget = widget->next) {
			if (widget->repaint || window->repaint) {
				// the position of the widget in the drawing buffer
				CedarPoint realPos = {
					.x = widget->bounds.x - window->origin.x,
					.y = widget->bounds.y - window->origin.y
				};

				if (realPos.x >= window->bounds.x
				 && realPos.x + widget->bounds.x <= window->bounds.x + window->bounds.width
				 && realPos.y >= window->bounds.y
				 && realPos.y + widget->bounds.y <= window->bounds.y + window->bounds.height) {
					// Widget is fully visible
					clearRect(realPos.x, realPos.y, widget->bounds.width, widget->bounds.height);

					cedar_dispatchEvent(EVENT_PAINT, widget, &realPos);

					if (!window->repaint) {
						// only blit if we're not going to blit everything later
						gfx_BlitRectangle(gfx_buffer, realPos.x, realPos.y, widget->bounds.width, widget->bounds.height);
					}
				}
			}
		}

		// Paint menu bar
		if (window->menu != NULL) {
			unsigned int menuBarPaintOffset = 5;

			clearRect(0, 0, window->bounds.width, MENUBAR_HEIGHT);

			for (CedarMenuItem *current=window->menu->first; current != NULL; current = current->next) {
				if (isMenuItemSeparator(current)) {
					// Draw separator
					gfx_VertLine_NoClip(menuBarPaintOffset, 2, MENUBAR_HEIGHT-4);
					menuBarPaintOffset += 6;
				} else {
					unsigned int labelWidth = gfx_GetStringWidth(current->label);

					if (menuBarPaintOffset + labelWidth > window->bounds.width) {
						// no more room to paint
						// TODO: allow menu to scroll
						break;
					}

					if (current == window->menu->selected) {
						// indicate item is selected
						gfx_FillRectangle(menuBarPaintOffset-2, 3, labelWidth+4, 14);

						gfx_SetTextFGColor(255);
						gfx_SetTextBGColor(0);
						gfx_SetTextTransparentColor(0);
						gfx_PrintStringXY(current->label, menuBarPaintOffset, 5);
						gfx_SetTextFGColor(0);
						gfx_SetTextBGColor(255);
						gfx_SetTextTransparentColor(255);
					} else {
						gfx_PrintStringXY(current->label, menuBarPaintOffset, 5);
					}

					menuBarPaintOffset += labelWidth + 5;
				}
			}

			gfx_HorizLine_NoClip(0, MENUBAR_HEIGHT, window->bounds.width);

			paintActiveSubmenus(window->menu);
		}

		dontPropagatePaint:

		if (window->repaint) {
			gfx_BlitBuffer();
		}
	}
}
