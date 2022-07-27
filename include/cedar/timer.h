#ifndef _LIBCEDAR_TIMER_H_
#define _LIBCEDAR_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cedar/defs.h>

/*
 * A timer
 *
 * id			The ID of the timer
 * next			A pointer to the next timer
 * prev			A pointer to the previous timer
 * wound		Whether the timer has been wound up (is it enabled)
 * period		How many clock ticks per each tick of this timer
 * lastTick		The last clock tick that this timer fired
 */
struct CedarTimer {
	ID id;

	CedarTimer *next;
	CedarTimer *prev;

	bool wound;
	unsigned int period;
	unsigned int lastTick;
};

/*
 * Add a timer to a window
 *
 * window		The window to add the timer to
 * id			The ID of the timer
 * period		The number of milliseconds between each tick.
 */
void cedar_AddTimer(CedarWindow *window, ID id, unsigned int period);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_TIMER_H_
