#include <string.h>

#include "cedar.h"

void cedar_addMenuSeparator(Menu *menu) {
	MenuItem *item = malloc(sizeof(MenuItem));

	item->type = MENUITEM_SEPARATOR;
	item->prev = menu->last;
	menu->last = item;
}

void cedar_addMenuItem(Menu *menu, const char *label, MenuSelectHandler *handler) {
	MenuItem *item = malloc(sizeof(MenuItem));

	item->type = MENUITEM_BUTTON;

	strncpy(item->label, label, 11);
	item->label[11] = '\0';

	item->handler = handler;

	item->prev = menu->last;
	menu->last = item;
}

void cedar_addSubmenu(Menu *parent, const char *label, Menu *submenu) {
	MenuItem *item = malloc(sizeof(MenuItem));

	item->type = MENUITEM_PARENT;

	strncpy(item->label, label, 11);
	item->label[11] = '\0';

	item->submenu = submenu;

	item->prev = parent->last;
	parent->last = item;
}
