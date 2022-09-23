#include <stddef.h>
#include <stdlib.h>

#include <cedar.h>

void cdr_InitWindow(CdrWindow *window) {
	window->widgets.first = NULL;
	window->widgets.last = NULL;
	window->widgets.selected = NULL;

	// Init handler array
	window->handlers.count = 0;
	window->handlers.alloc = 1;
	window->handlers.callbacks = malloc(sizeof() * window->handlers.alloc);
}
