#include <string.h>

#include "cedar.h"

void cedar_SetMenu(CedarWindow *window, CedarMenu *menu) {
	window->menu = menu;
	window->bounds.y = MENUBAR_HEIGHT;
}

void cedar_InitMenu(CedarMenu *menu) {
	menu->first = NULL;
	menu->last = NULL;
	menu->selected = NULL;
	menu->submenuActive = false;
}

void cedar_AddMenuSeparator(CedarMenu *menu) {
	CedarMenuItem *item = malloc(sizeof(CedarMenuItem));

	item->id = ID_ANONYMOUS;

	item->label[0] = '\0';

	item->parent = menu;
	item->child = NULL;

	item->next = NULL;
	item->prev = menu->last;
	menu->last = item;

	if (item->prev != NULL) item->prev->next = item;
	if (menu->first == NULL) menu->first = item;
}

void cedar_AddMenuItem(CedarMenu *parent, ID id, const char *label) {
	CedarMenuItem *item = malloc(sizeof(CedarMenuItem));

	item->id = id;

	strncpy(item->label, label, 11);
	item->label[11] = '\0';

	item->parent = parent;
	item->child = NULL;

	item->next = NULL;
	item->prev = parent->last;
	parent->last = item;

	if (item->prev != NULL) item->prev->next = item;
	if (parent->first == NULL) parent->first = item;
}

void cedar_AddSubmenu(CedarMenu *parent, ID id, const char *label, CedarMenu *child) {
	CedarMenuItem *item = malloc(sizeof(CedarMenuItem));

	item->id = id;

	strncpy(item->label, label, 11);
	item->label[11] = '\0';

	item->parent = parent;
	item->child = child;

	item->next = NULL;
	item->prev = parent->last;
	parent->last = item;

	if (item->prev != NULL) item->prev->next = item;
	if (parent->first == NULL) parent->first = item;
}
