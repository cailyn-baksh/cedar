# Menu


## `CedarMenu`
```c
typedef struct Menu Menu;
```
A menu.


### `CedarMenuItem *first`
A pointer to the first item in the menu. If `NULL` there are no items in this menu.


### `CedarMenuItem *last`
A pointer to the last item in the menu. If `NULL` there are no items in this menu.


### `CedarMenuItem *selected`
A pointer to the currently selected item in the menu. If `NULL` then the menu is
not selected.


### `CedarMenu *parent`
A pointer to the menu that contains this menu. If `NULL` this is a top level menu.


### `bool submenuActive`
Whether or not this menu is active. If `true`, then `selected` should have a non-null `child`, which is the active submenu.

---

## `CedarMenuItem`
```c
typedef struct MenuItem MenuItem;
```
An item in a menu.


## `ID id`
The ID of this menu item.


### `CedarMenuItem *next`
A pointer to the next item in the menu.


### `CedarMenuItem *prev`
A pointer to the previous item in the menu.


### `char label[12]`
A string label for the menu item. The label cannot be longer than 12 characters.
The last character must be a null terminator. If the first character is a null character, then this menu item is a menu separator.


### `CedarMenu *parent`
A pointer to the parent menu of this menu item. `NULL` if this is in a top level
menu.


### `CedarMenu *child`
A pointer to the child menu of this menu item. If `NULL` then this menu item is treated as a button, and will trigger `EVENT_MENUSELECT` when clicked.


---

## `void cedar_InitMenu(CedarMenu *menu)`
Initializes a menu. This must be called before passing a `Menu` to any other
functions.

**menu** The menu to initialize


## `void cedar_AddMenuSeparator(CedarMenu *menu)`
Adds a separator to a menu

**menu** The menu to add the separator to


## `void cedar_AddMenuItem(CedarMenu *menu, ID id, const char *label)`
Adds an item to a menu.

**menu** The menu to add the item to

**id** The ID of the menu item

**label** A label for the menu item. This is truncated to 12 characters.


## `void cedar_AddSubmenu(CedarMenu *parent, ID id, const char *label, CedarMenu *child)`
Adds a submenu to a menu.

**parent** The parent menu to add this child menu to

**id** The ID of the menu item

**label** A label for the menu item. This is truncated to 12 characters.

**child** The child menu to add to the parent menu.


## `CedarMenuItem *GetLastSelectedMenuItem(CedarMenu *menu)`
Traverses submenus to find the lowest menu item that is currently selected.

**menu** The menu to find the currently selected menu item in.
Returns a pointer to the currently selected menu item.

**Returns** a pointer to the lowest submenu item currently selected.
