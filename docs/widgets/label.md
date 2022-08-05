# Labels
Labels are a widget that contain text


## `LabelData`
Label data. In label widgets, `data` is an instance of this struct.


### `char *text`
The text of the label


## `CedarWidget *CedarLabel(int x, int y, int width, int height, const char *text)`
Creates a label widget. A label is a widget which contains text.

**x** The X coordinate of the left side of the label

**y** The Y coordinate of the top of the label

**width** The width of the label in pixels

**height** The height of the label in pixels

**text** The content of the label

Returns a new Label widget
