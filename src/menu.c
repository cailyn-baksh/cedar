#include <string.h>

#include "cedar.h"

void cedar_setMenu(Window *window, Menu *menu) {
	window->menu = menu;
	window->realTop = MENUBAR_HEIGHT;
}

void cedar_initMenu(Menu *menu) {
	menu->first = NULL;
	menu->last = NULL;
	menu->selected = NULL;
	menu->active = false;
}

void cedar_addMenuSeparator(Menu *menu) {
	MenuItem *item = malloc(sizeof(MenuItem));

	item->type = MENUITEM_SEPARATOR;
	item->parent = menu;

	item->next = NULL;
	item->prev = menu->last;
	menu->last = item;

	if (item->prev != NULL) item->prev->next = item;
	if (menu->first == NULL) menu->first = item;
}

void cedar_addMenuItem(Menu *parent, const char *label, unsigned int buttonID) {
	MenuItem *item = malloc(sizeof(MenuItem));

	item->type = MENUITEM_BUTTON;

	strncpy(item->label, label, 11);
	item->label[11] = '\0';
	item->buttonID = buttonID;

	item->parent = parent;

	item->next = NULL;
	item->prev = parent->last;
	parent->last = item;

	if (item->prev != NULL) item->prev->next = item;
	if (parent->first == NULL) parent->first = item;
}

void cedar_addSubmenu(Menu *parent, const char *label, Menu *child) {
	MenuItem *item = malloc(sizeof(MenuItem));

	item->type = MENUITEM_PARENT;

	strncpy(item->label, label, 11);
	item->label[11] = '\0';

	item->child = child;
	item->parent = parent;

	item->next = NULL;
	item->prev = parent->last;
	parent->last = item;

	if (item->prev != NULL) item->prev->next = item;
	if (parent->first == NULL) parent->first = item;
}
