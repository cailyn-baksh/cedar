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

CedarMenuItem *getNextSelectableMenuItem(CedarMenu *menu) {
	CedarMenuItem *item = menu->selected->next;

	while (item != NULL && isMenuItemSeparator(item)) {
		item = item->next;
	}

	return item;
}

CedarMenuItem *getPrevSelectableMenuItem(CedarMenu *menu) {
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
	uint24_t callbackReturnCode;

	switch (event) {
		case EVENT_KEYDOWN:
			switch (param) {
				case CEDAR_KB_QUIT:
					// quit
					return CALLBACK_EXIT;
				case CEDAR_KB_UP:
					// up arrow
					if (self->menu != NULL) {
						// this window has a menu; modify menu selection
						if (self->menu->selected == NULL) {
							// no menu is selected; select menu bar
							self->menu->selected = self->menu->first;
							//self->menu->selected = getNextSelectableMenuItem(self->menu);

							if (self->menu->selected != NULL) {
								if (self->widgets.selected != NULL) {
									// dispatch blur event to previously selected widget
									callbackReturnCode = cedar_dispatchEvent(EVENT_BLUR, self->widgets.selected, 0);

									switch (callbackReturnCode) {
										case CALLBACK_EXIT:
											return CALLBACK_EXIT;
									}
								}
							}
						} else if (self->menu->submenuActive
						 && self->menu->selected->child != NULL) {
							// a submenu is active
							CedarMenu *submenu = getActiveSubmenu(self->menu);
							CedarMenuItem *prevItem = getPrevSelectableMenuItem(submenu);

							if (prevItem != NULL) {
								// Select previous submenu item
								submenu->selected = prevItem;
							} else {
								// Exit submenu
								submenu->parent->submenuActive = false;
								submenu->selected = NULL;
							}
						}

						self->repaint = true;
					}

					// dont propagate this event to the widget
					return CALLBACK_DO_NOT_PROPAGATE;
				case CEDAR_KB_DOWN:
					// down arrow
					if (self->menu != NULL && self->menu->selected != NULL) {
						// this window has a menu and it is selected; modify menu selection
						if (self->menu->submenuActive
						 && self->menu->selected->child != NULL) {
							// submenu is active
							CedarMenu *submenu = getActiveSubmenu(self->menu);
							CedarMenuItem *nextItem = getNextSelectableMenuItem(submenu);

							if (nextItem != NULL) {
								// Select next submenu item
								submenu->selected = nextItem;
							}
						} else {
							// no submenu active; deselect menu bar
							self->menu->selected = NULL;

							// Send focus event to selected widget
							if (self->widgets.selected != NULL) {
								// self->widget.selected should never be NULL
								callbackReturnCode = cedar_dispatchEvent(EVENT_FOCUS, self->widgets.selected, 0);

								switch (callbackReturnCode) {
									case CALLBACK_EXIT:
										return CALLBACK_EXIT;
								}
							}
						}

						self->repaint = true;
					}

					// dont propagate this event to the widget
					return CALLBACK_DO_NOT_PROPAGATE;
				case CEDAR_KB_RIGHT:
					// right arrow
					if (self->menu != NULL && self->menu->selected != NULL) {
						// Select next menu item that is not a separator
						CedarMenuItem *nextItem = getNextSelectableMenuItem(self->menu);

						if (nextItem != NULL) {
							// There is a next item that we can select
							deselectAllSubmenus(self->menu);
							self->menu->selected = nextItem;
							self->repaint = true;
						}
					} else if (self->widgets.selected->next != NULL) {
						// Select next widgets
						CedarWidget *nextWidget = self->widgets.selected->next;
						while (nextWidget != NULL
						 && !(nextWidget->attrs & ATTR_FOCUSABLE)) {
							nextWidget = nextWidget->next;
						}

						if (nextWidget != NULL) {
							callbackReturnCode = cedar_dispatchEvent(EVENT_BLUR, self->widgets.selected, 0);
							switch (callbackReturnCode) {
								case CALLBACK_EXIT:
									return CALLBACK_EXIT;
							}

							self->widgets.selected = nextWidget;

							callbackReturnCode = cedar_dispatchEvent(EVENT_FOCUS, self->widgets.selected, 0);
							switch (callbackReturnCode) {
								case CALLBACK_EXIT:
									return CALLBACK_EXIT;
							}
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

							self->repaint = true;
						}
					} else if (self->widgets.selected->prev != NULL) {
						// Select prev widgets
						CedarWidget *prevWidget = self->widgets.selected->prev;
						while (prevWidget != NULL
						 && !(prevWidget->attrs & ATTR_FOCUSABLE)) {
							prevWidget = prevWidget->prev;
						}

						if (prevWidget != NULL) {
							callbackReturnCode = cedar_dispatchEvent(EVENT_BLUR, self->widgets.selected, 0);
							switch (callbackReturnCode) {
								case CALLBACK_EXIT:
									return CALLBACK_EXIT;
							}

							self->widgets.selected = prevWidget;

							callbackReturnCode = cedar_dispatchEvent(EVENT_FOCUS, self->widgets.selected, 0);
							switch (callbackReturnCode) {
								case CALLBACK_EXIT:
									return CALLBACK_EXIT;
							}
						}
					}

					return CALLBACK_DO_NOT_PROPAGATE;
			}
			break;
		case EVENT_KEYUP:
			switch (param) {
				case CEDAR_KB_ENTER:
					if (self->menu != NULL && self->menu->selected != NULL) {
						// menu is selected
						CedarMenuItem *selected = cedar_GetLastSelectedMenuItem(self->menu);

						if (selected == NULL) break;

						if (isMenuItemButton(selected)) {
							// button menu item
							callbackReturnCode = cedar_dispatchEvent(EVENT_MENUSELECT, self, selected->id);
							switch (callbackReturnCode) {
								case CALLBACK_EXIT:
									return CALLBACK_EXIT;
							}
						} else if (isMenuItemSubmenu(selected)) {
							// submenu
							selected->parent->submenuActive = true;
							selected->child->selected = selected->child->first;

							if (selected->child->selected != NULL && isMenuItemSeparator(selected->child->selected)) {
								CedarMenuItem *nextSelectable = getNextSelectableMenuItem(selected->child);
								if (nextSelectable != NULL) {
									selected->child->selected = nextSelectable;
								}
							}

							self->repaint = true;
						}

						// event was for the menu so don't propagate to widgets
						return CALLBACK_DO_NOT_PROPAGATE;
					}
			}
			break;
		case EVENT_HSCROLL:
		case EVENT_VSCROLL:
			self->repaint = true;
	}

	return CALLBACK_NORMAL;
}

void cedar_InitWindow(CedarWindow *window) {
	window->widgets.first = NULL;
	window->widgets.last = NULL;
	window->widgets.selected = NULL;

	window->menu = NULL;

	window->frame.x = 0;
	window->frame.y = 0;
	window->frame.width = GFX_LCD_WIDTH;
	window->frame.height = GFX_LCD_HEIGHT;

	window->origin.x = 0;
	window->origin.y = 0;

	window->handlers = malloc(sizeof(CedarEventHandler));
	window->handlers->callback = defaultWindowEventHandler;
	window->handlers->next = NULL;
}

void cedar_DestroyWindow(CedarWindow *window) {
	// Clean up menu bar

	// Clean up widgets
	for (CedarWidget *current=window->widgets.first; current != NULL; current = current->next) {
		if (current->prev != NULL) {
			// everything always gets destroyed
			cedar_dispatchEvent(EVENT_DESTROY, current->prev, 0);
			cedar_DestroyWidget(current->prev);
		}
	}

	free(window);
}

void cedar_AddWidget(CedarWindow *window, CedarWidget *widget) {
	if (window->widgets.last != NULL) {
		// link this widget into the list
		widget->prev = window->widgets.last;
		widget->prev->next = widget;
	} else {
		// This is the first widget
		window->widgets.first = widget;
	}
	window->widgets.last = widget;
	widget->parent = window;
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
			clearRect(5, MENUBAR_HEIGHT, MENU_DROPDOWN_WIDTH, MENU_DROPDOWN_HEIGHT);
			gfx_Rectangle(5, MENUBAR_HEIGHT, MENU_DROPDOWN_WIDTH, MENU_DROPDOWN_HEIGHT);

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
CedarMenuItem *cedar_GetLastSelectedMenuItem(CedarMenu *menu) {
	if (menu->selected != NULL
	 && menu->selected->child != NULL
	 && menu->submenuActive) {
		// find selected item in submenu
		return cedar_GetLastSelectedMenuItem(menu->selected->child);
	 } else {
		return menu->selected;
	 }
}

void cedar_Display(CedarWindow *window) {
	uint24_t callbackReturnCode;

	callbackReturnCode = cedar_dispatchEvent(EVENT_CREATE, window, 0);

	switch (callbackReturnCode) {
		case CALLBACK_EXIT:
			return;
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

		callbackReturnCode = cedar_dispatchEvent(EVENT_FOCUS, window->widgets.selected, 0);
		switch (callbackReturnCode) {
			case CALLBACK_EXIT:
				return;
		}
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

						if (keycode == kb_Key2nd && keycode == kb_KeyAlpha) {
							// ignore meta key release
							continue;
						}

						// dispatch event
						if (is2nd) keycode |= CEDAR_2NDFLAG;
						if (isAlpha) keycode |= CEDAR_ALPHAFLAG;

						// dispatch keyup to window
						callbackReturnCode =
							cedar_dispatchEvent(EVENT_KEYUP, window, keycode);
						switch (callbackReturnCode) {
							case CALLBACK_EXIT:
								return;
							case CALLBACK_DO_NOT_PROPAGATE:
								goto dontPropagateKeyup;
						}

						// dispatch keyup to widget
						callbackReturnCode = cedar_dispatchEvent(EVENT_KEYUP, window->widgets.selected, keycode);
						switch (callbackReturnCode) {
							case CALLBACK_EXIT:
								return;
						}

						dontPropagateKeyup:
						;
					}
				}
			} else if ((prevKbState[i] & kb_Data[i]) != kb_Data[i]) {
				// Not all keys currently pressed were previously pressed (keydown)
				for (uint8_t mask=0x80; mask != 0; mask >>= 1) {
					// try each key mask on this row until we find one
					if (!(prevKbState[i] & mask) && (kb_Data[i] & mask)) {
						// This key was released
						uint24_t keycode = mask | (i << 8);

						if (keycode == kb_Key2nd && keycode == kb_KeyAlpha) {
							// ignore meta key release
							continue;
						}

						if (is2nd) keycode |= CEDAR_2NDFLAG;
						if (isAlpha) keycode |= CEDAR_ALPHAFLAG;

						// dispatch keydown to window
						callbackReturnCode =
							cedar_dispatchEvent(EVENT_KEYDOWN, window, keycode);
						switch (callbackReturnCode) {
							case CALLBACK_EXIT:
								return;
							case CALLBACK_DO_NOT_PROPAGATE:
								goto dontPropagateKeydown;
						}

						callbackReturnCode = cedar_dispatchEvent(EVENT_KEYDOWN, window->widgets.selected, keycode);
						switch (callbackReturnCode) {
							case CALLBACK_EXIT:
								return;
						}

						dontPropagateKeydown:
						;
					}
				}
			}
		}

		// Dispatch scroll events
		{
			CedarWidget *selected = window->widgets.selected;
			int delta = 0;

			// FIXME: window coords get stuck equal to widget coords when scrolling left/up

			if (selected->bounds.x < window->frame.x) {
				// scroll left
				switch (window->scrollMode) {
					case WINDOW_SCROLL_WIDGET:
						delta = selected->bounds.x - window->frame.x;
						break;
				}

				window->frame.x += delta;
				cedar_dispatchEvent(EVENT_HSCROLL, window, delta);
			} else if (selected->bounds.x + selected->bounds.width > window->frame.x + window->frame.width) {
				// scroll right
				switch (window->scrollMode) {
					case WINDOW_SCROLL_WIDGET:
						delta = (selected->bounds.x + selected->bounds.width - window->frame.width) - window->frame.x;
						break;
				}

				window->frame.x += delta;
				cedar_dispatchEvent(EVENT_HSCROLL, window, delta);
			}

			if (selected->bounds.y < window->frame.y) {
				// scroll up
				switch (window->scrollMode) {
					case WINDOW_SCROLL_WIDGET:
						delta = selected->bounds.y - window->frame.y;
						break;
				}

				window->frame.y += delta;
				cedar_dispatchEvent(EVENT_VSCROLL, window, delta);
			} else if (selected->bounds.y + selected->bounds.height > window->frame.y + window->frame.height) {
				// scroll down
				switch (window->scrollMode) {
					case WINDOW_SCROLL_WIDGET:
						delta = (selected->bounds.y + selected->bounds.height - window->frame.height) - window->frame.y;
						break;
				}

				window->frame.y += delta;
				cedar_dispatchEvent(EVENT_VSCROLL, window, delta);
			}
		}

		/* Paint */
		if (window->repaint) {
			gfx_FillScreen(255);
		}

		callbackReturnCode = cedar_dispatchEvent(EVENT_PAINT, window, 0);
		switch (callbackReturnCode) {
			case CALLBACK_EXIT:
				return;
			case CALLBACK_DO_NOT_PROPAGATE:
				goto dontPropagatePaint;
		}

		for (CedarWidget *widget=window->widgets.first; widget != NULL; widget = widget->next) {
			if (widget->repaint || window->repaint) {
				// translate widget's coordinates to a position in the drawing buffer
				CedarRect bufferRegion = {
					.x = (widget->bounds.x + window->origin.x) - window->frame.x,
					.y = (widget->bounds.y + window->origin.y) - window->frame.y,
					.width = widget->bounds.width,
					.height = widget->bounds.height
				};
				CedarPoint realPos = {
					.x = bufferRegion.x,
					.y = bufferRegion.y
				};
				bool onscreen = true;

				if (bufferRegion.x < window->frame.x) {
					// left side is off screen to the left
					bufferRegion.x = 0;
				} else if (bufferRegion.x > window->frame.x + window->frame.width) {
					// left side is off screen to the right
					onscreen = false;
				}

				if (bufferRegion.x + bufferRegion.width > window->frame.x + window->frame.width) {
					// right side is off screen to the right
					bufferRegion.width = window->frame.width - bufferRegion.x;
				} else if (bufferRegion.x + bufferRegion.width < window->frame.x) {
					// right side is off screen to the left
					onscreen = false;
				}

				if (bufferRegion.y < window->frame.y) {
					// top side is above top of the screen
					bufferRegion.y = 0;
				} else if (bufferRegion.y > window->frame.y + window->frame.height) {
					// top side is below bottom of the screen
					onscreen = false;
				}

				if (bufferRegion.y + bufferRegion.height > window->frame.y + window->frame.height) {
					// bottom side is below the bottom of the screen
					bufferRegion.height = window->frame.height - bufferRegion.y;
				} else if (bufferRegion.y + bufferRegion.height < window->frame.y) {
					// bottom side is above the top of the screen
					onscreen = false;
				}

				if (onscreen) {
					// Widget is fully visible
					clearRect(bufferRegion.x, bufferRegion.y, bufferRegion.width, bufferRegion.height);

					callbackReturnCode = cedar_dispatchEvent(EVENT_PAINT, widget, (uint24_t)&realPos);
					// return code is checked after blitting

					if (!window->repaint) {
						// only blit if we're not going to blit everything later
						gfx_BlitRectangle(gfx_buffer, bufferRegion.x, bufferRegion.y, bufferRegion.width, bufferRegion.height);
					}
					widget->repaint = false;  // repaint finished

					switch (callbackReturnCode) {
						case CALLBACK_EXIT:
							return;
						case CALLBACK_DO_NOT_PROPAGATE:
							goto dontPropagatePaint;
					}
				}
			}
		}

		// Paint menu bar
		if (window->menu != NULL) {
			unsigned int menuBarPaintOffset = 5;

			clearRect(0, 0, window->frame.width, MENUBAR_HEIGHT);

			for (CedarMenuItem *current=window->menu->first; current != NULL; current = current->next) {
				if (isMenuItemSeparator(current)) {
					// Draw separator
					gfx_VertLine_NoClip(menuBarPaintOffset, 2, MENUBAR_HEIGHT-4);
					menuBarPaintOffset += 6;
				} else {
					unsigned int labelWidth = gfx_GetStringWidth(current->label);

					if (menuBarPaintOffset + labelWidth > window->frame.width) {
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

			gfx_HorizLine_NoClip(0, MENUBAR_HEIGHT, window->frame.width);

			paintActiveSubmenus(window->menu);
		}

		dontPropagatePaint:

		if (window->repaint) {
			gfx_BlitBuffer();
			window->repaint = false;
		}
	}
}
