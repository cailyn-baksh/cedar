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
#define LIBCEDAR_MINOR_VERSION 4
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

/* The result of a callback function */
typedef uint24_t CALLBACKRESULT;

/* An event code */
typedef uint24_t EVENT;

/* An ID */
typedef uint24_t ID;

/*
 * A function to handle an event
 *
 * self		A pointer to the object handling the event
 * event	The event to be handled
 * id		The ID of the thing that triggered the event. This can be NULL
 * param	A versatile parameter
 */
typedef CALLBACKRESULT EventHandler(void *self, EVENT event, ID id, uint24_t param);

/*
 * Event codes. These are used to indicate to event handlers which event was
 * fired. More event codes can be defined outside the library; these are just
 * the core events. The event code range 0x00000-0x007FF is reserved.
 *
 * EVENT_CREATE			Fired when a component is created
 * EVENT_DESTROY		Fired when a component is destroyed
 * EVENT_PAINT			Fired when painting
 * EVENT_FOCUS			Fired when a component recieves focus by the user
 * EVENT_BLUR			Fired when a component loses focus from the user
 * EVENT_KEYDOWN		Fired when a key is pressed
 * EVENT_KEYUP			Fired when a key is released
 * EVENT_MENUSELECT		Fired when a menu button is pressed
 * EVENT_HOTKEY			Fired when a hotkey is pressed
 * EVENT_TICK			Fired when a timer ticks
 * EVENT_VSCROLL		Fired when a component's container scrolls vertically
 * EVENT_HSCROLL		Fired when a component's container scrolls horizontally
 */
#define EVENT_CREATE				((EVENT)0x000001)
#define EVENT_DESTROY				((EVENT)0x000002)
#define EVENT_PAINT					((EVENT)0x000003)
#define EVENT_FOCUS					((EVENT)0x000004)
#define EVENT_BLUR					((EVENT)0x000005)
#define EVENT_KEYDOWN				((EVENT)0x000006)
#define EVENT_KEYUP					((EVENT)0x000007)
#define EVENT_MENUSELECT			((EVENT)0x000008)
#define EVENT_HOTKEY				((EVENT)0x000009)
#define EVENT_TICK					((EVENT)0x00000A)
#define EVENT_VSCROLL				((EVENT)0x00000B)
#define EVENT_HSCROLL				((EVENT)0x00000C)


/*
 * Callback return codes
 *
 * CALLBACK_DEFAULT				Returned by handlers normally.
 * CALLBACK_EXIT				Returned by handlers when the mainloop should exit
 * CALLBACK_DO_NOT_PROPAGATE	Returned by handlers when the event that triggered
 *								them shouldn't be passed to subsequent event
 *								handlers.
 */
#define CALLBACK_DEFAULT				((CALLBACKRESULT)0x000000)
#define CALLBACK_EXIT					((CALLBACKRESULT)0x000001)
#define CALLBACK_DO_NOT_PROPAGATE		((CALLBACKRESULT)0x000002)


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

	EventHandler *handler;
};

/*
 * A widget.
 *
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
	ID id;

	uint24_t attrs;

	Widget *prev;
	Widget *next;

	int x;
	int y;
	unsigned int width;
	unsigned int height;

	int realX;
	int realY;

	bool enabled;

	void *data;

	EventHandler *handler;
};

#define ATTR_FOCUSABLE			0x1

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

	ID id;

	MenuItem *next;
	MenuItem *prev;

	char label[12];

	Menu *parent;
	Menu *child;

	bool invertColours;
};

void cedar_init();

void cedar_cleanup();

/*
 * Initialize a window
 *
 * window		The window to initialize.
 * handler		A handler for the window's events
 */
void cedar_initWindow(Window *window, EventHandler *handler);

/*
 * Clean up after a window
 *
 * window		The window to clean up.
 */
void cedar_destroyWindow(Window *window);

/*
 * The default window event handler
 */
CALLBACKRESULT defaultWindowEventHandler(Window *self, EVENT event, ID id, uint24_t param);

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
 * id			The id of the menu button
 */
void cedar_addMenuItem(Menu *menu, const char *label, ID id);

/*
 * Add a submenu to the menu.
 *
 * parent		The parent menu to add this child menu to.
 * label		The label of the submenu. This must be less than 12 characters.
 * submenu		The child menu to add to the parent menu.
 */
void cedar_addSubmenu(Menu *parent, const char *label, Menu *child);

MenuItem *getLastSelectedMenuItem(Menu *menu);

#ifndef _NOEXTERN
extern uint16_t prevKbState[8];

extern bool key_2nd;
extern bool key_alpha;
extern bool alphaLock;
#endif  // _NOEXTERN

#define wasKeyPressed(group, key)	(!(prevKbState[group] & key) && (kb_Data[group] & key))
#define wasKeyReleased(group, key)	((prevKbState[group] & key) && !(kb_Data[group] & key))

#endif  // _LIBCEDAR_H_
