# Widgets

## `Widget`
```c
typedef struct Widget Widget;
```
A Widget.


### `WidgetType type`
The type of the widget


### `uint24_t id`
The ID of the widget.


### `uint24_t attrs`
Attributes of the widget.


### `Widget *prev`
A pointer to the previous widget. `NULL` if this is the first widget.


### `Widget *next`
A pointer to the next widget. `NULL if this is the last widget.


### `int x`
The virtual x coordinate of this widget.


### `int y`
The virtual y coordinate of this widget.


### `unsigned int width`
The width of this widget in pixels.


### `unsigned int height`
The height of this widget in pixels.


### `int realX`
The "real" x coordinate on the screen of this widget. This value is undefined
except during a paint event.


### `int realY`
The "real" y coordinate on the screen of this widget. This value is undefined
except during a paint event.


### `bool enabled`
If true, this widget can be interacted with. 


### `void *data`
A pointer to data for this widget. This is used to store widget-specific data
for derived widgets.


### `WidgetEventHandler *handler`
A handler called when the widget recieves an event.

---

### `getData(type, widget)`
Helper macro to access a widget's data member and cast it to a particular type.


### `cedar_destroyWidget(Widget *widget)`
Frees a widget's memory. The previous and next widgets will be linked to each
other.

**widget** The widget to clean up.


### `WidgetEventHandler`
```c
typedef uint24_t WidgetEventHandler(Widget *widget, uint24_t event);
```
An event handler for a widget

**widget** A pointer to the widget that triggered the event

**event** The event that was triggered

Returns an event return code indicating what the event dispatcher should do
after the handler finishes handling the event.

---

## Labels
Labels are a widget that contain text


### `LabelData`
Label data. In label widgets, `data` points to an instance of this structure.


#### `char *text`
The text of the label


### `Widget *Label(int x, int y, int width, int height, const char *text)`
Creates a label widget. A label is a widget which contains text.

**x** The X coordinate of the left side of the label

**y** The Y coordinate of the top of the label

**width** The width of the label in pixels

**height** The height of the label in pixels

**text** The content of the label

Returns a new Label widget


### `uint24_t defaultLabelHandler(Widget *widget, uint24_t event)`
The default event handler for label widgets.

---

## Buttons
Buttons are widgets which can be selected to trigger events


### `ButtonData`
Button data. In button widgets, `data` points to an instance of this structure.


#### `char *text`
The text of the button.


#### `bool focused`
Whether or not the button is focused


### `Widget *Button(int x, int y, int width, int height, const char *text)
Creates a button widget.

**x** The X coordinate of the left side of the button

**y** The Y coordinate of the top of the button

**width** The width of the button in pixels

**height** The height of the button in pixels

**text** The content of the label

Returns a new Button widget


### `uint24_t defaultButtonhandler(Widget *widget, uint24_t event)`
The default event handler for button widgets