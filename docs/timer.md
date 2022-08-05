# Timer

## `CedarTimer`
```c
typedef struct CedarTimer CedarTimer;
```
A timer


### `ID id`
The ID of the timer


### `CedarTimer *next`
A pointer to the next timer


### `CedarTimer *prev`
A pointer to the previous timer


### `bool wound`
Whether the timer has been wound up (is it enabled)


### `unsigned int period`
How many clock ticks per each tick of this timer. Clock ticks are expected to be
32768 Hz.


### `unsigned int lastTick`
The last clock tick that this timer fired on.


---

## `void cedar_AddTimer(CedarWindow *window, ID id, unsigned int period)`
Adds a timer to a window

**window** The window to add the timer to
**id** The ID of the timer
**period** The number of milliseconds between each timer tick
