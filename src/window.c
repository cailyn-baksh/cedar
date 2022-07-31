#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <graphx.h>
#include <keypadc.h>
#include <sys/timers.h>

#include <cedar/utils.h>

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
				default:
					if (self->menu != NULL && self->menu->selected != NULL) {
						// menu is selected, dont propagate keydown to widgets
						return CALLBACK_DO_NOT_PROPAGATE;
					}
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

	window->timers.first = NULL;
	window->timers.last = NULL;

	window->menu = NULL;

	window->frame.xmin = 0;
	window->frame.ymin = 0;
	window->frame.xmax = GFX_LCD_WIDTH;
	window->frame.ymax = GFX_LCD_HEIGHT;

	window->origin.x = 0;
	window->origin.y = 0;

	window->colors.bg = CEDAR_COLOR_WHITE;
	window->colors.fg = CEDAR_COLOR_BLACK;
	window->colors.alt = CEDAR_COLOR_BLACK;

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
static void paintActiveSubmenus(CedarMenu *menu, CedarWindow *parent) {
	if (menu->selected != NULL
	 && menu->selected->child != NULL) {
		// This menu has a submenu
		CedarMenu *submenu = menu->selected->child;
			

		if (menu->submenuActive) {
			// This is the active submenu; paint it

			gfx_SetColor(parent->colors.fg);
			gfx_FillRectangle(7, MENUBAR_HEIGHT+2, MENU_DROPDOWN_WIDTH, MENU_DROPDOWN_HEIGHT);
			
			gfx_SetColor(parent->colors.bg);
			gfx_FillRectangle(5, MENUBAR_HEIGHT, MENU_DROPDOWN_WIDTH, MENU_DROPDOWN_HEIGHT);

			gfx_SetColor(parent->colors.fg);
			gfx_Rectangle(5, MENUBAR_HEIGHT, MENU_DROPDOWN_WIDTH, MENU_DROPDOWN_HEIGHT);

			cedar_SetColors(parent->colors.fg, parent->colors.bg);

			int submenuItemY = MENUBAR_HEIGHT + 5;
			for (CedarMenuItem *current=submenu->first; current != NULL; current = current->next) {
				if (isMenuItemSeparator(current)) {
					cedar_SetColors(parent->colors.bg, parent->colors.fg);
					gfx_HorizLine(5, submenuItemY+4, MENU_DROPDOWN_WIDTH);
					cedar_SetColors(parent->colors.fg, parent->colors.bg);
					submenuItemY += 10;
				} else {
					// TODO: add visual feedback for keypress before keyup
					if (current == submenu->selected) {
						// Indicate item is selected
						cedar_SetColors(parent->colors.bg, parent->colors.fg);

						gfx_FillRectangle(6, submenuItemY-2, MENU_DROPDOWN_WIDTH-2, 12);
						gfx_PrintStringXY(current->label, 10, submenuItemY);

						cedar_SetColors(parent->colors.fg, parent->colors.bg);
					} else {
						gfx_PrintStringXY(current->label, 10, submenuItemY);
					}

					submenuItemY += 10;
				}
			}
		} else {
			// Try painting a child submenu
			paintActiveSubmenus(submenu, parent);
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
		/* Timers */
		{
			unsigned int now = timer_Get(1);

			for (CedarTimer *current=window->timers.first; current != NULL; current = current->next) {
				if ((unsigned int)(now - current->lastTick) >= current->period) {
					callbackReturnCode = cedar_dispatchEvent(EVENT_TICK, window, current->id);
					current->lastTick = now;
				}
			}
		}

		/* Key Events */
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

			if (selected->bounds.xmin < window->frame.xmin) {
				// scroll left
				switch (window->scrollMode) {
					case WINDOW_SCROLL_WIDGET:
						delta = selected->bounds.xmin - window->frame.xmin;
						break;
				}

				window->frame.xmin += delta;
				window->frame.xmax += delta;
				cedar_dispatchEvent(EVENT_HSCROLL, window, delta);
			} else if (selected->bounds.xmax > window->frame.xmax) {
				// scroll right
				switch (window->scrollMode) {
					case WINDOW_SCROLL_WIDGET:
						delta = selected->bounds.xmax - window->frame.xmax;
						break;
				}

				window->frame.xmin += delta;
				window->frame.xmax += delta;
				cedar_dispatchEvent(EVENT_HSCROLL, window, delta);
			}

			if (selected->bounds.ymin < window->frame.ymin) {
				// scroll up
				switch (window->scrollMode) {
					case WINDOW_SCROLL_WIDGET:
						delta = selected->bounds.ymin - window->frame.ymin;
						break;
				}

				window->frame.ymin += delta;
				window->frame.ymax += delta;
				cedar_dispatchEvent(EVENT_VSCROLL, window, delta);
			} else if (selected->bounds.ymax > window->frame.ymax) {
				// scroll down
				switch (window->scrollMode) {
					case WINDOW_SCROLL_WIDGET:
						delta = selected->bounds.ymax - window->frame.ymax;
						break;
				}

				window->frame.ymin += delta;
				window->frame.ymax += delta;
				cedar_dispatchEvent(EVENT_VSCROLL, window, delta);
			}
		}

		/* Paint */
		if (window->repaint) {
			gfx_FillScreen(window->colors.bg);
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
				bool onscreen = true;

				if (widget->bounds.xmin > window->frame.xmax
				 || widget->bounds.xmax < window->frame.xmin
				 || widget->bounds.ymin > window->frame.ymax
				 || widget->bounds.ymax < window->frame.ymin) {
					onscreen = false;
				}

				if (onscreen) {
					gfx_region_t realWidgetPos = {
						.xmin = widget->bounds.xmin - window->frame.xmin + window->origin.x,
						.ymin = widget->bounds.ymin - window->frame.ymin + window->origin.y,
						.xmax = widget->bounds.xmax - window->frame.xmin + window->origin.x,
						.ymax = widget->bounds.ymax - window->frame.ymin + window->origin.y
					};

					// Clear region on screen
					gfx_SetColor(window->colors.bg);
					gfx_FillRectangle(realWidgetPos.xmin, realWidgetPos.ymin, GFX_REGION_WIDTH(realWidgetPos), GFX_REGION_HEIGHT(realWidgetPos));

					// Load colors
					gfx_SetColor(window->colors.fg);
					gfx_SetTransparentColor(window->colors.bg);
					gfx_SetTextFGColor(window->colors.fg);
					gfx_SetTextBGColor(window->colors.bg);
					cedar_SetTextTransparentColor(window->colors.bg);

					callbackReturnCode = cedar_dispatchEvent(EVENT_PAINT, widget, (uint24_t)&realWidgetPos);
					// return code is checked after blitting

					if (!window->repaint) {
						// only blit if we're not going to blit everything later
						gfx_BlitRectangle(gfx_buffer, realWidgetPos.xmin, realWidgetPos.ymin, GFX_REGION_WIDTH(realWidgetPos), GFX_REGION_HEIGHT(realWidgetPos));
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

			// Clear region on screen
			gfx_SetColor(window->colors.bg);
			gfx_FillRectangle(0, 0, GFX_REGION_WIDTH(window->frame), MENUBAR_HEIGHT);
			
			// Load colors
			cedar_SetColors(window->colors.fg, window->colors.bg);

			for (CedarMenuItem *current=window->menu->first; current != NULL; current = current->next) {
				if (isMenuItemSeparator(current)) {
					// Draw separator
					cedar_SetColors(window->colors.bg, window->colors.fg);
					gfx_VertLine_NoClip(menuBarPaintOffset, 2, MENUBAR_HEIGHT-4);
					menuBarPaintOffset += 6;
				} else {
					unsigned int labelWidth = gfx_GetStringWidth(current->label);

					if (menuBarPaintOffset + labelWidth > GFX_REGION_WIDTH(window->frame)) {
						// no more room to paint
						// TODO: allow menu to scroll
						break;
					}

					if (current == window->menu->selected) {
						// indicate item is selected
						cedar_SetColors(window->colors.bg, window->colors.fg);

						gfx_FillRectangle(menuBarPaintOffset-2, 3, labelWidth+4, 14);
						gfx_PrintStringXY(current->label, menuBarPaintOffset, 5);

						cedar_SetColors(window->colors.fg, window->colors.bg);
					} else {
						gfx_PrintStringXY(current->label, menuBarPaintOffset, 5);
					}

					menuBarPaintOffset += labelWidth + 5;
				}
			}

			paintActiveSubmenus(window->menu, window);

			cedar_SetColors(window->colors.bg, window->colors.fg);
			gfx_HorizLine_NoClip(0, MENUBAR_HEIGHT, GFX_REGION_WIDTH(window->frame));
		}

		dontPropagatePaint:

		if (window->repaint) {
			gfx_BlitBuffer();
			window->repaint = false;
		}
	}
}
