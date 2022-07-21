# Widgets

## `Widget`
```c
typedef struct Widget Widget;
```
A Widget.


### `ID id`
The ID of this widget


### `uint24_t attrs`
Attributes of the widget.


### `CedarWindow *parent`
A pointer to the window that contains this widget.


### `CedarWidget *prev`
A pointer to the previous widget. If `NULL` then this is the first widget.


### `CedarWidget *next`
A pointer to the next widget. If `NULL` then this is the last widget.


### `gfx_region_t bounds`
The rectangle that contains this widget in virtual coordinate space.


### `bool repaint`
If `true`, this widget will be repainted if it is visible.


### `bool enabled`
If `true`, this widget is enabled.


### `CedarEventHandler *handlers`
The chain of event handlers to handle events dispatched to this window.


### `uint8_t data[]`
Flexible array member used by widget implementations to store widget-specific data.

---

### `getData(type, widget)`
Helper macro to access a widget's data member and cast it to a particular type.


### `cedar_DestroyWidget(Widget *widget)`
Frees a widget's memory. The previous and next widgets will be linked to each
other.

**widget** The widget to clean up.


---

## Labels
Labels are a widget that contain text


### `LabelData`
Label data. In label widgets, `data` is an instance of this struct.


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

---

## Buttons
Buttons are widgets which can be selected to trigger events.


### `ButtonData`
Button data. In button widgets, `data` is an instance of this structure.


#### `char *text`
The text of the button.


#### `bool focused`
Whether or not the button is focused


### `Widget *Button(int x, int y, int width, int height, const char *text)`
Creates a button widget.

**x** The X coordinate of the left side of the button

**y** The Y coordinate of the top of the button

**width** The width of the button in pixels

**height** The height of the button in pixels

**text** The content of the label

Returns a new Button widget
