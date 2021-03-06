/*
 * A widget library for the TI 84+ CE
 */
#ifndef _LIBCEDAR_H_
#define _LIBCEDAR_H_

#define _LIBCEDAR_Q(s) #s
#define _LIBCEDAR_QUOTE(s) _LIBCEDAR_Q(s)

// Version info
// minor version is incremented each release
// major version is incremented after each breaking change (after major version 1)
#define LIBCEDAR_MAJOR		1
#define LIBCEDAR_MINOR		0
#define LIBCEDAR_REVISION	0
#define LIBCEDAR_VERSION_STR _LIBCEDAR_QUOTE(LIBCEDAR_MAJOR) "." _LIBCEDAR_QUOTE(LIBCEDAR_MINOR) "." _LIBCEDAR_QUOTE(LIBCEDAR_REVISION)

#ifdef LIBCEDAR_CHECK_VERSION

#if LIBCEDAR_MAJOR != LIBCEDAR_TARGET_MAJOR
#error Desired libcedar version is incompatible with installed version (major versions do not match)
#endif  // major version

#if LIBCEDAR_MINOR < LIBCEDAR_TARGET_MINOR
#error Desired libcedar version is newer than installed version (minor version out of date)
#endif  // minor version

#if defined(LIBCEDAR_VERSION_REQUIRE_SAME)

#if (LIBCEDAR_MINOR != LIBCEDAR_TARGET_MINOR) || (LIBCEDAR_REVISION != LIBCEDAR_TARGET_REVISION)
#error Desired libcedar version is incompatible with installed version (installed version must be same)
#endif

#elif defined(LIBCEDAR_VERSION_REQUIRE_SAME_OR_BETTER)

#if LIBCEDAR_REVISION < LIBCEDAR_TARGET_REVISION
#error Desired libcedar version is newer than installed version (installed version must be same or better)
#endif

#endif

#endif  // LIBCEDAR_CHECK_VERSION

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include <graphx.h>
#include <keypadc.h>

#include "cedar/defs.h"

// TODO: make it easier to traverse parent/child relationships

#define GFX_REGION_WIDTH(r)		((r).xmax - (r).xmin)
#define GFX_REGION_HEIGHT(r)	((r).ymax - (r).ymin)

/*
 * A singly linked list of event handlers
 *
 * callback		A pointer to this handler's callback function
 * next			A pointer to the next handler
 */
struct CedarEventHandler {
	CedarEventHandlerCallback *callback;
	CedarEventHandler *next;
};

/*
 * A window.
 *
 * widgets.first		A pointer to the first widget in the window
 * widgets.last			A pointer to the last widget in the window
 * widgets.selected		A pointer to the currently selected widget
 * menu					A pointer to the Menu struct for this window's menu bar.
 *						If NULL, then this window does not have a menu bar.
 * drawableArea			A rectangle within the drawing buffer representing the
 *						main drawable area (i.e. where widgets are drawn).
 * scrolledTo			Where the window has been scrolled to. This point
 *						corresponds to the top left corner of the window.
 * handlers				The event handlers registered to this window.
 */
struct CedarWindow {
	struct {
		CedarWidget *first;
		CedarWidget *last;
		CedarWidget *selected;
	} widgets;

	CedarMenu *menu;

	gfx_region_t frame;
	gfx_point_t origin;

	enum {
		WINDOW_SCROLL_PAGE,
		WINDOW_SCROLL_WIDGET
	} scrollMode;

	bool repaint;

	CedarEventHandler *handlers;
};

/*
 * A widget.
 *
 * id			The ID of this widget.
 * attrs		Widget attributes. These generally are constant
 * prev			A pointer to the previous widget
 * next			A pointer to the next widget
 * bounds		The bounds of this widget
 * repaint		Force repaint
 * enabled		Whether or not this widget is enabled.
 * handlers		The event handlers for this widget
 * data			Data for this widget, specific to each widget subclass.
 */
struct CedarWidget {
	ID id;

	uint24_t attrs;  // IDEA: make attrs quasi-constant by creating 3 bytes of
					 // padding that can only be accessed through assembly

	CedarWindow *parent;
	CedarWidget *prev;
	CedarWidget *next;

	gfx_region_t bounds;

	bool repaint;
	bool enabled;

	CedarEventHandler *handlers;

	uint8_t data[];
};
// TODO: consider merging widgets and windows

#define ATTR_FOCUSABLE			0x1

#define getData(type, widget) ((type *)widget->data)

#define MENUBAR_HEIGHT 20

#define MENU_DROPDOWN_WIDTH 100
#define MENU_DROPDOWN_HEIGHT (GFX_LCD_HEIGHT - MENUBAR_HEIGHT - 5)

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
 * Initializes the library. This must be called before any other cedar functions
 */
void cedar_Init();

/*
 * Cleans up after the library. This should be called after you are done using
 * the library.
 */
void cedar_Cleanup();

/*
 * Initializes a window
 *
 * window		A pointer to the window to initialize.
 */
void cedar_InitWindow(CedarWindow *window);
/*
 * Destroys a window and cleans up its memory.
 *
 * window		The window to destroy.
 */
void cedar_DestroyWindow(CedarWindow *window);
/*
 * Displays a window
 *
 * window		The window to display
 * Returns the code the window mainloop exited on.
 */
void cedar_Display(CedarWindow *window);

/*
 * Register a new event handler.
 *
 * handlerStack	The handler stack to push this handler onto
 * callback		The callback to push onto the handler stack
 */
void cedar_RegisterEventHandler(CedarEventHandler *handlerStack, CedarEventHandlerCallback *callback);

CALLBACKRESULT _cedar_dispatchEvent(CedarEventHandler *firstHandler, void *self, EVENT event, uint24_t param);

/*
 * Dispatch an event to a component. Component can be a pointer to any type as
 * long as it has a handlers member.
 */
#define cedar_dispatchEvent(event, component, param) _cedar_dispatchEvent((component)->handlers, component, event, param)

/*
 * Adds a widget to a window
 *
 * window		The window to add the widget to
 * widget		The widget to add to the window
 */
void cedar_AddWidget(CedarWindow *window, CedarWidget *widget);

/*
 * Destroys a widget and cleans up its memory
 *
 * widget		The widget to clean up
 */
// should this be exposed? could it be a part of cedar_DestroyWindow?
void cedar_DestroyWidget(CedarWidget *widget);

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

#endif  // _LIBCEDAR_H_
