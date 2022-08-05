# Widgets

## `CedarWidget`
```c
typedef struct CedarWidget CedarWidget;
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

