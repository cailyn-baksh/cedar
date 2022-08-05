#include <stdlib.h>

#include <sys/timers.h>

#include <cedar/window.h>
#include <cedar/timer.h>

/*
 * Called by cedar_Init.
 * Configures timer 1 to tick at 32768Hz, and count up indefinitely
 */
void _cedar_ConfigTimer() {
	timer_Disable(1);
	timer_Set(1, 0);
	timer_Enable(1, TIMER_32K, TIMER_NOINT, TIMER_UP);
}

void cedar_AddTimer(CedarWindow *window, ID id, unsigned int period) {
	CedarTimer *timer = malloc(sizeof(CedarTimer));

	timer->id = id;

	timer->prev = window->timers.last;
	window->timers.last = timer;

	timer->next = NULL;
	if (window->timers.first == NULL) window->timers.first = timer;

	timer->wound = true;
	timer->period = period * 32.768;  // convert ms to 32768Hz ticks
	timer->lastTick = 0;
}
