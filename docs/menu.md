# Menu


## `Menu`
```c
typedef struct Menu Menu;
```
A menu.


### `MenuItem *first`
A pointer to the first item in the menu


### `MenuItem *last`
A pointer to the last item in the menu


### `MenuItem *selected`
A pointer to the currently selected item in the menu. If `NULL` then the menu is
not selected.


### `bool active`
Whether or not this menu is active. If the menu is selected (i.e. `selected` is
`NULL`) and `active` is `false`, then a submenu of this menu is selected.

---

## `MenuItem`
```c
typedef struct MenuItem MenuItem;
```
An item in a menu.


### `enum type`
An anonymous enum representing the type of this menu item.

#### `MENUITEM_PARENT`
This menu item contains a child menu

#### `MENUITEM_BUTTON`
This menu item is a button. It will trigger `EVENT_MENUSELECT` when it is
activated.

#### `MENUITEM_SEPARATOR`
Separates menu items with a line. This menu item cannot be interacted with.


### `MenuItem *next`
A pointer to the next item in the menu.


### `MenuItem *prev`
A pointer to the previous item in the menu.


### `char label[12]`
A string label for the menu item. The label cannot be longer than 12 characters.
The last character is a null terminator.


### `Menu *parent`
A pointer to the parent menu of this menu item. `NULL` if this is in a top level
menu.


### `unsigned int buttonID`
The ID of the menu button. This is only valid if `type` is `MENUITEM_BUTTON`.


### `Menu *child`
A pointer to the child menu of this menu item. This is only valid if `type` is
`MENUITEM_PARENT`.


### `bool invertColours`
If `true`, the menu item should be painted with its colours inverted.

---

## `void cedar_initMenu(Menu *menu)`
Initializes a menu. This must be called before passing a `Menu` to any other
functions.

**menu** The menu to initialize


## `void cedar_addMenuSeparator(Menu *menu)`
Adds a separator to a menu

**menu** The menu to add the separator to


## `void cedar_addMenuItem(Menu *menu, const char *label, unsigned int buttonID)`
Adds an item to a menu.

**menu** The menu to add the item to
**label** A label for the menu item. Must be less than 12 characters.
**buttonID** The ID of the button.


## `void cedar_addSubmenu(Menu *parent, const char *label, Menu *child)`
Adds a submenu to a menu.

**parent** The parent menu to add this child menu to
**label** A label for the menu item. Must be less than 12 characters.
**child** The child menu to add to the parent menu.


## `MenuItem *getLastSelectedMenuItem(Menu *menu)`
Traverses submenus to find the lowest menu item that is currently selected.

**Returns** a pointer to the lowest submenu item currently selected.
