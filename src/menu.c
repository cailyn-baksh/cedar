#include <string.h>

#include "cedar.h"

void cedar_initMenu(Menu *menu) {
	menu->first = NULL;
	menu->last = NULL;
}

void cedar_addMenuSeparator(Menu *menu) {
	MenuItem *item = malloc(sizeof(MenuItem));

	item->type = MENUITEM_SEPARATOR;
	item->next = NULL;
	item->prev = menu->last;
	menu->last = item;

	if (menu->first == NULL) menu->first = item;
}

void cedar_addMenuItem(Menu *menu, const char *label, MenuSelectHandler *handler) {
	MenuItem *item = malloc(sizeof(MenuItem));

	item->type = MENUITEM_BUTTON;

	strncpy(item->label, label, 11);
	item->label[11] = '\0';

	item->handler = handler;

	item->next = NULL;
	item->prev = menu->last;
	menu->last = item;

	if (menu->first == NULL) menu->first = item;
}

void cedar_addSubmenu(Menu *parent, const char *label, Menu *submenu) {
	MenuItem *item = malloc(sizeof(MenuItem));

	item->type = MENUITEM_PARENT;

	strncpy(item->label, label, 11);
	item->label[11] = '\0';

	item->submenu = submenu;

	item->next = NULL;
	item->prev = parent->last;
	parent->last = item;

	if (parent->first == NULL) parent->first = item;
}
