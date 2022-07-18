#include <string.h>

#include "cedar.h"
#include "cedardbg.h"

void cedar_SetMenu(CedarWindow *window, CedarMenu *menu) {
	DBGPRINT("Setting menu of window at %p to menu at %p\n", window, menu);

	window->menu = menu;
	window->frame.height -= MENUBAR_HEIGHT;
	window->origin.y -= MENUBAR_HEIGHT;
}

void cedar_InitMenu(CedarMenu *menu) {
	DBGPRINT("Initializing menu at %p\n", menu);

	menu->first = NULL;
	menu->last = NULL;
	menu->selected = NULL;
	menu->parent = NULL;
	menu->submenuActive = false;
}

void cedar_AddMenuSeparator(CedarMenu *menu) {
	DBGPRINT("Adding separator to menu at %p\n", menu);

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
	DBGPRINT("Adding item '%s' with id %x to menu at %p\n", label, id, parent);

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
	DBGPRINT("Adding submenu '%s' at %p with id %x to menu at %p\n", label, child, id, parent);

	CedarMenuItem *item = malloc(sizeof(CedarMenuItem));

	item->id = id;

	strncpy(item->label, label, 11);
	item->label[11] = '\0';

	item->parent = parent;
	item->child = child;

	child->parent = parent;

	item->next = NULL;
	item->prev = parent->last;
	parent->last = item;

	if (item->prev != NULL) item->prev->next = item;
	if (parent->first == NULL) parent->first = item;
}
