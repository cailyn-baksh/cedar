# Window

## `Window`
```c
typedef struct Window Window;
```
A window

### `Widget *widgets.first`
A pointer to the first widget in this window. If `NULL` there are no widgets in
this window.


### `Widget *widgets.last`
A pointer to the last widget in this window. If `NULL` there are no widgets in
this window.


### `Widget *widgets.selected`
A pointer to the currently selected widget. If `NULL` no widget is selected.


### `Menu *menu`
A pointer to the menu for this window. If `NULL` then this window does not have
a menu.


### `int realTop`
The coordinate in the drawing buffer of the top of the main drawable area.


### `int realLeft`
The coordinate in the drawing buffer of the top of the main drawable area.


### `int projTop`
The virtual Y coordinate at the top of the screen.


### `int projLeft`
The virtual X coordinate at the left of the screen.


### `unsigned int width`
The width of the window.


### `unsigned int height`
The width of the window,


### `WindowEventHandler *handler`
A pointer to a function to handle window events

---

### `WindowEventHandler`
```c
typedef uint24_t WindowEventHandler(Window *window, uint24_t event)
```
An event handler for a window

**window** A pointer to the widget that triggered the event

**event** The event that was triggered

Returns an event return code indicating what the event dispatcher should do
after the handler finishes handling the event.

---

## `void cedar_initWindow(Window *window, WindowEventHandler *handler)`
Initializes a window.

**window** The window to initialize

**handler** A pointer to the handler for the window's events


## `void cedar_destroyWindow(Window *window)`
Cleans up after a window

**window** The window to clean up.


## `uint24_t defaultWindowEventHandler(Window *window, int event)`
The default window event handler


## `int cedar_display(Window *window)`
Displays a window. Starts a mainloop which dispatches events and paints the
window.

**window** The window to display

Returns the event code that caused the mainloop to exit


## `void cedar_addWidget(Window *window, Widget *widget)`
Adds a widget to a window.

**window** The window to add the widget to

**widget** The widget to add to the window


## `void cedar_setMenu(Window *window, Menu *menu)`
Set the menu for a window

**window** The window to add the menu to

**menu** The menu to add to the window
