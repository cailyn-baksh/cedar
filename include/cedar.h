/*
 * A widget library for the TI 84+ CE
 */
#ifndef _LIBCEDAR_H_
#define _LIBCEDAR_H_

#include <stdbool.h>

#include <graphx.h>
#include <keypadc.h>

struct Window;
typedef struct Window Window;

enum WidgetType;
typedef enum WidgetType WidgetType;

struct Widget;
typedef struct Widget Widget;

struct Menu;
typedef struct Menu Menu;

struct MenuItem;
typedef struct MenuItem MenuItem;

/*
 * Handles a menu button being selected
 */
typedef int MenuSelectHandler(Menu *menu);

/*
 * Handle events for a widget.
 */
typedef int WidgetEventHandler(Widget *widget, int event);

/*
 * A window
 *
 * widgets.first		Pointer to the first widget in this window. If NULL
 *						there are no widgets in this window.
 * widgets.last			Pointer to the last widget in this window. If NULL
 *						there are no widgets in this window.
 * widgets.selected		Pointer to the currently selected widget.
 *
 * menu					The menu
 *
 * realTop				The screen coordinate of the top of the main drawable
 *						area (where widgets are drawn).
 * realLeft				The screen coordinate of the left of the main drawable
 *						area (where widgets are drawn).
 *
 * projTop				The virtual Y coordinate at the top of the screen.
 * projLeft				The virtual X coordinate at the left of the screen.
 * 
 * width				The width of the window.
 * height				The height of the window
 *
 * baseX			The base X coordinate of the main drawing area.
 * baseY			The base Y coordinate of the main drawing area
 */
struct Window {
	struct {
		Widget *first;
		Widget *last;
		Widget *selected;
	} widgets;

	Menu *menu;

	int realTop;
	int realLeft;

	int projTop;
	int projLeft;

	int width;
	int height;
};


// event code range 0x0000-0x007F reserved for base events

/*
 * EVENT_PAINT		Fired when painting
 * EVENT_FOCUS		Fired when a component is selected by the user
 * EVENT_BLUR		Fired when a component loses focus from the user
 * EVENT_KEYDOWN	Fired when a button is pressed
 * EVENT_KEYUP		Fired when a button is released
 * EVENT_SCROLL		Fired when a component's container scrolls
 */
#define EVENT_PAINT			0x0001
#define EVENT_FOCUS			0x0002
#define EVENT_BLUR			0x0003
#define EVENT_KEYDOWN		0x0004
#define EVENT_KEYUP			0x0005
#define EVENT_SCROLL		0x0006

enum WidgetType {
	WIDGET_LABEL,
	WIDGET_BUTTON,
	WIDGET_CANVAS,
	WIDGET_CHECKBOX,
	WIDGET_RADIO
};

/*
 * A widget.
 *
 * type			The type of widget
 * prev			Pointer to the previous widget in this window. May be NULL.
 * next			Pointer to the next widget in this window. May be NULL.
 *
 * x			The X coordinate of the widget in pixels
 * y			The Y coordinate of the widget in pixels
 * width		The width of the widget in pixels
 * height		The height of the widget in pixels
 * realX		The X coordinate in the drawing buffer
 * realY		The Y coordinate in the drawing buffer
 * data			Pointer to widget-specific data.
 *
 * handler		Pointer to the event handler function
 */
struct Widget {
	WidgetType type;

	Widget *prev;
	Widget *next;

	int x;
	int y;
	int width;
	int height;

	int realX;
	int realY;

	void *data;

	WidgetEventHandler *handler;
};

#define getData(type, widget) ((type *)widget->data)

#define MENUBAR_HEIGHT 20

/*
 * A menu
 *
 * first		The first MenuItem in the menu
 * last			The last MenuItem in the menu
 */
struct Menu {
	MenuItem *first;
	MenuItem *last;
};

/*
 * An item in a menu
 *
 * type		The type of menu.
 * next		The next item in the menu
 * prev		The previous item in the menu
 *
 * label	A string label for the menu item
 *
 * submenu	A submenu to display when this menu is selected. Only used when
 *			type == MENUITEM_PARENT
 * handler	A handler to invoke when the menu is selected.
 */
struct MenuItem {
	enum {
		MENUITEM_PARENT,
		MENUITEM_BUTTON,
		MENUITEM_SEPARATOR
	} type;

	MenuItem *next;
	MenuItem *prev;

	char label[12];

	union {
		Menu *submenu;
		MenuSelectHandler *handler;
	};
};

void cedar_init();

void cedar_cleanup();

/*
 * Initialize a window
 *
 * window		The window to initialize.
 */
void cedar_initWindow(Window *window);

/*
 * Clean up after a window
 *
 * window		The window to clean up.
 */
void cedar_destroyWindow(Window *window);

/*
 * Display a window.
 *
 * window		The window to display.
 */
int cedar_display(Window *window);

/*
 * Add a widget to a window. Widgets can only be added to one window.
 *
 * window		The window to add the widget to
 * widget		The widget to add to the window
 */
void cedar_addWidget(Window *window, Widget *widget);

/*
 * Free a widget's memory. The previous and next widgets will be linked to each
 * other.
 *
 * widget		The widget to clean up.
 */
void cedar_destroyWidget(Widget *widget);

/*
 * Set the menu for a window
 *
 * window		The window to add the menu to
 * menu			The menu to add to the window
 */
void cedar_setMenu(Window *window, Menu *menu);

void cedar_initMenu(Menu *menu);

/*
 * Add a separator to a menu
 *
 * menu			The menu to add the separator to
 */
void cedar_addMenuSeparator(Menu *menu);

/*
 * Add an item to the menu. See also: cedar_addSubmenu
 *
 * menu			The menu to add the item to
 * label		The label of the menu. This must be less than 12 characters.
 * handler		The handler to call when the menu item is selected
 */
void cedar_addMenuItem(Menu *menu, const char *label, MenuSelectHandler *handler);

/*
 * Add a submenu to the menu.
 *
 * parent		The parent menu to add this child menu to.
 * label		The label of the submenu. This must be less than 12 characters.
 * submenu		The child menu to add to the parent menu.
 */
void cedar_addSubmenu(Menu *parent, const char *label, Menu *submenu);

/*
 * A checkbox is a widget which can be checked on or off.
 */
Widget *Checkbox(int x, int y, bool state);

/*
 */
Widget *Radio(int x, int y, int group);

#endif  // _LIBCEDAR_H_
