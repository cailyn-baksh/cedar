#ifndef _LIBCEDAR_MENU_H_
#define _LIBCEDAR_MENU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cedar/defs.h>

/*
 * A menu.
 *
 * first			A pointer to the first menu item
 * last				A pointer to the last menu item
 * selected			A pointer to the currently selected menu item
 * submenuActive	Whether or not a submenu of this menu is active
 */
struct CedarMenu {
	CedarMenuItem *first;
	CedarMenuItem *last;
	CedarMenuItem *selected;
	CedarMenu *parent;
	bool submenuActive;
};

/*
 * A menu item.
 *
 * id		The ID of this menu item.
 * next		A pointer to the next menu item
 * prev		A pointer to the previous menu item
 * label	A label for this menu item. If label[0] == '\0', then this menu item
 *			is a separator. This label must be null terminated.
 * parent	A pointer to the menu which contains this menu item
 * child	A pointer to the submenu this menu item represents. If NULL then
 *			this menu item is treated as a button and triggers EVENT_MENUSELECT
 *			when clicked.
 */
struct CedarMenuItem {
	ID id;

	CedarMenuItem *next;
	CedarMenuItem *prev;

	char label[12];

	CedarMenu *parent;
	CedarMenu *child;
};

#define isMenuItemSeparator(item) (item->label[0] == '\0')
#define isMenuItemButton(item) (!isMenuItemSeparator(item) && item->child == NULL)
#define isMenuItemSubmenu(item) (item->child != NULL)

/*
 * Sets the menu for the window's menubar
 *
 * window		The window to set the menubar of
 * menu			The menu to make into the menubar
 */
void cedar_SetMenu(CedarWindow *window, CedarMenu *menu);

/*
 * Initializes a menu. This should be called before calling any other functions
 * on a menu.
 *
 * menu		The menu to initialize
 */
void cedar_InitMenu(CedarMenu *menu);
/*
 * Adds a separator to the menu
 * 
 * menu		The menu to add the separator to
 */
void cedar_AddMenuSeparator(CedarMenu *menu);
/*
 * Adds a button menu item to the menu
 *
 * menu		The menu to add the item to
 * id		The id of the menu item
 * label	The label of the menu item
 */
void cedar_AddMenuItem(CedarMenu *menu, ID id, const char *label);
/*
 * Adds a submenu to the menu
 *
 * parent	A pointer to the parent menu to add the child menu to
 * id		The ID of the menu item
 * label	The label for the submenu
 * child	A pointer to the child menu to add to the parent menu
 */
void cedar_AddSubmenu(CedarMenu *parent, ID id, const char *label, CedarMenu *child);

/*
 * Traverses submenus to get the menu item the user is currently selecting
 *
 * menu		The menu to find the currently selected menu item in
 * Returns a pointer to the currently selected menu item.
 */
CedarMenuItem *cedar_GetLastSelectedMenuItem(CedarMenu *menu);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_MENU_H_
