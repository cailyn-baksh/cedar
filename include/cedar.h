#ifndef _LIBCEDAR_H_
#define _LIBCEDAR_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LIBCEDAR_MAJOR		2
#define LIBCEDAR_MINOR		0
#define LIBCEDAR_REVISION	0

#include <stddef.h>
#include <stdint.h>

#include <cedar/defs.h>

/*
 * Initializes the library. This must be called first.
 */
void cdr_Init();
/*
 * Cleans up after the library. This must be called last.
 */
void cdr_Cleanup();

/*
 * A Window.
 *
 * widgets		A doubly linked list of widgets in the window.
 * 	.first		The first widget
 * 	.last		The last widget
 * 	.selected	The currently selected widget
 *
 * handlers		The event handlers associated with the window
 * 	.callbacks	A VLA of handler callbacks
 * 	.count		The number of callbacks in the array
 * 	.alloc		The number of callbacks allocated in the array.
 */
struct CdrWindow {
	struct {
		CdrWidget *first;
		CdrWidget *last;
		CdrWidget *selected;
	} widgets;

	struct {
		size_t count;
		size_t alloc;
		CdrEventHandlerCallback **callbacks;
	} handlers;
};

void cdr_InitWindow(CdrWindow *window);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_H_

