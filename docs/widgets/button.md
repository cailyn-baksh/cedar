# Buttons
Buttons are widgets which can be selected to trigger events.


## `ButtonData`
Button data. In button widgets, `data` is an instance of this structure.


### `char *text`
The text of the button.


### `bool focused`
Whether or not the button is focused


## `CedarWidget *CedarButton(int x, int y, int width, int height, const char *text)`
Creates a button widget.

**x** The X coordinate of the left side of the button

**y** The Y coordinate of the top of the button

**width** The width of the button in pixels

**height** The height of the button in pixels

**text** The content of the label

Returns a new Button widget
