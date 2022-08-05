# Radio Buttons
Radio buttons are widgets which can be used in groups to select one of several
options. Radio buttons are linked by a pointer to a shared `ID` variable, which
indicated which of them is currently selected.


## `RadioButtonData`


### `ID *selected`
A pointer to the currently selected item.


## `CedarWidget *CedarRadioButton(ID id, int x, int y, ID *selection)`
Creates a new radio button

**id** The ID of the widget

**x** The X coordinate of the widget

**y** The Y coordinate of the widget

**selection** A pointer to a variable which indicates whether this checkbox is
selected. This variable is shared between all radio buttons in a group