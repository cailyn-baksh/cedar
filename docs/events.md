# Events

## `EVENT_CREATE`
Triggered when a window or widget is created.

**Substrates**: [Widget](#), [Window](window.md)

## `EVENT_DESTROY`
Triggered when a window or widget is destroyed.

**Substrates**: [Widget](#), [Window](window.md)

## `EVENT_PAINT`
Triggered when an window or widget should be drawn.

**Substrates**: [Widget](#), [Window](window.md)

## `EVENT_FOCUS`
Triggered when a widget gains focus.

**Substrates**: [Widget](#)

## `EVENT_BLUR`
Triggered when a widget loses focus.

**Substrates**: [Widget](#)

## `EVENT_KEYDOWN`
Triggered when a key is pressed while a widget is selected.

**Substrates**: [Widget](#), [Window](window.md)

## `EVENT_KEYUP`
Triggered when a key is released while a widget is selected.

**Substrates**: [Widget](#), [Window](window.md)

## `EVENT_ENABLE`
Triggered when a widget is enabled. **Not Implemented**

**Substrates**: [Widget](#)

## `EVENT_DISABLE`
Triggered when a widget is disabled. **Not Implemented**

**Substrates**: [Widget](#)

## `EVENT_MENUSELECT`
Triggered when a menu item is selected.

**Substrates**: [Window](window.md)

## `EVENT_HOTKEY`
Triggered when a hotkey is pressed. **Not Implemented**

**Substrates**: [Window](window.md)

## `EVENT_TICK`
Triggered when a timer ticks. **Not Implemented**

**Substrates**: [Window](window.md)

## `EVENT_VSCROLL`
Triggered when a window scrolls vertically

**Substrates**: [Window](window.md)

## `EVENT_HSCROLL`
Triggered when a window scrolls horizontally

**Substrates**: [Window](window.md)

## `EVENT_BUTTONPRESS`
Triggered when a Button widget is pressed

**Substrates**: [Window](window.md)


---


# Event Callback Return Codes

## `CALLBACK_NORMAL`
Returns normal status. The event will propagate to the next callback.

## `CALLBACK_EXIT`
Immediately exits the mainloop.

## `CALLBACK_DO_NOT_PROPAGATE`
Stops the event from propagate to the next callback.
