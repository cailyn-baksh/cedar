# Window

## `Window`
```c
typedef struct Window Window;
```
A window


### `CedarWidget *widgets.first`
A pointer to the first widget in the window. If `NULL` then there are no widgets in this window.


## `CedarWidget *widgets.last`
A pointer to the last widget in the window. If `NULL` then there are no widgets in this window.


### `CedarWidget *widgets.selected`
A pointer to the currently selected widget. If `NULL` then no widget is selected.


### `CedarMenu *menu`
A pointer to the menu for this window's menu bar. If `NULL` then this window does not have a menu bar.


### `gfx_region_t frame`
The rectangle within virtual coordinate space that is currently visible in the window.


### `gfx_point_t origin`
A point in the drawing buffer that is mapped to (0,0) in virtual coordinate space.


### `enum scrollMode`
Which method to use to scroll the window.


#### `WINDOW_SCROLL_PAGE`
Scroll in chunks the size of the window frame.


#### `WINDOW_SCROLL_WIDGET`
Scroll just enough to fit the currently selected widget.


### `bool repaint`
When true, the menubar and all widgets are repainted.


### `CedarEventHandler *handlers`
The chain of event handlers to handle events dispatched to this window.


---

## `void cedar_InitWindow(CedarWindow *window)`
Initializes a window.

**window** The window to initialize


## `void cedar_DestroyWindow(CedarWindow *window)`
Cleans up after a window

**window** The window to clean up.


## `void cedar_Display(CedarWindow *window)`
Displays a window. Starts a mainloop which dispatches events and paints the
window.

**window** The window to display


## `void cedar_RegisterEventHandler(CedarEventHandler *handlerStack, CedarEventHandlerCallback *callback)`
Register a new event handler.

**handlerStack** The handler stack to push this handler onto

**callback** The callback to push onto the handler stack


## `cedar_dispatchEvent(event, component, param)`
Dispatch `event` to `component`. `component` must be a pointer type with a `handlers` member.

**event** The event to dispatch

**component** The component to dispatch the event to

**param** The parameter associated with the event

Returns a `CALLBACKRESULT` indicating the result of the event handlers


## `void cedar_AddWidget(CedarWindow *window, CedarWidget *widget)`
Adds a widget to a window.

**window** The window to add the widget to

**widget** The widget to add to the window


## `void cedar_SetMenu(CedarWindow *window, CedarMenu *menu)`
Set the menu for a window

**window** The window to add the menu to

**menu** The menu to add to the window
