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
#define LIBCEDAR_MAJOR_VERSION 0
#define LIBCEDAR_MINOR_VERSION 1
#define LIBCEDAR_VERSION_STR _LIBCEDAR_QUOTE(LIBCEDAR_MAJOR_VERSION) "." _LIBCEDAR_QUOTE(LIBCEDAR_MINOR_VERSION)

#include <stdbool.h>
#include <stdint.h>

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
typedef uint24_t MenuSelectHandler();

/*
 * Handle events for a widget.
 */
typedef uint24_t WidgetEventHandler(Widget *widget, int event);

/*
 * Handle events for a window.
 *
 * 
 */
typedef uint24_t WindowEventHandler(Window *window, int event);

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
 * baseY			The base Y coordinate of the main drawing area.
 *
 * handler				An event handler to handle window events.
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

	unsigned int width;
	unsigned int height;

	WindowEventHandler *handler;
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
#define EVENT_CREATE				0x000001
#define EVENT_DESTROY				0x000002
#define EVENT_PAINT					0x000003
#define EVENT_FOCUS					0x000004
#define EVENT_BLUR					0x000005
#define EVENT_KEYDOWN				0x000006
#define EVENT_KEYUP					0x000007
#define EVENT_ENABLE				0x000008
#define EVENT_DISABLE				0x000009
#define EVENT_MENUSELECT			0x00000A
#define EVENT_HOTKEY				0x00000B
#define EVENT_TICK					0x00000C
#define EVENT_VSCROLL				0x00000D
#define EVENT_HSCROLL				0x00000E


/*
 * Handler return codes
 *
 * HANDLER_NORMAL			Returned by handlers normally
 * HANDLER_EXIT				Returned by handlers when the mainloop should exit
 * HANDLER_DO_NOT_PROPAGATE	Returned by handlers when the event that triggered
 *							them shouldn't be passed to subsequent event
 *							handlers.
 */
#define HANDLER_NORMAL				0x000000
#define HANDLER_EXIT				0x000001
#define HANDLER_DO_NOT_PROPAGATE	0x000002

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
	unsigned int width;
	unsigned int height;

	int realX;
	int realY;

	void *data;

	WidgetEventHandler *handler;
};

#define getData(type, widget) ((type *)widget->data)

#define MENUBAR_HEIGHT 20

#define MENU_DROPDOWN_WIDTH 100
#define MENU_DROPDOWN_HEIGHT (GFX_LCD_HEIGHT - MENUBAR_HEIGHT - 5)

/*
 * A menu
 *
 * first			The first MenuItem in the menu
 * last				The last MenuItem in the menu
 * selected			A pointer to the currently selected MenuItem in the menu
 * submenuActive	Whether the selected submenu is active. N/A if selected
 *					item is not MENUITEM_PARENT.
 */
struct Menu {
	MenuItem *first;
	MenuItem *last;
	MenuItem *selected;
	bool active;
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

	Menu *parent;
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
 * handler		A handler for the window's events
 */
void cedar_initWindow(Window *window, WindowEventHandler *handler);

/*
 * Clean up after a window
 *
 * window		The window to clean up.
 */
void cedar_destroyWindow(Window *window);

/*
 * The default window event handler
 */
uint24_t defaultWindowEventHandler(Window *window, int event);

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

/*
 * Initialize a menu
 *
 * menu			The menu to initialize
 */
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

#ifndef _NOEXTERN
extern uint16_t prevKbState[8];

extern bool key_2nd;
extern bool key_alpha;
extern bool alphaLock;
#endif  // _NOEXTERN

#define wasKeyPressed(group, key)	(!(prevKbState[group] & key) && (kb_Data[group] & key))
#define wasKeyReleased(group, key)	((prevKbState[group] & key) && !(kb_Data[group] & key))

#endif  // _LIBCEDAR_H_
