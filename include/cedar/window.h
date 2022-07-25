#ifndef _LIBCEDAR_WINDOW_H_
#define _LIBCEDAR_WINDOW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <graphx.h>

#include <cedar/defs.h>

/*
 * A window.
 *
 * widgets.first		A pointer to the first widget in the window
 * widgets.last			A pointer to the last widget in the window
 * widgets.selected		A pointer to the currently selected widget
 * menu					A pointer to the Menu struct for this window's menu bar.
 *						If NULL, then this window does not have a menu bar.
 * drawableArea			A rectangle within the drawing buffer representing the
 *						main drawable area (i.e. where widgets are drawn).
 * scrolledTo			Where the window has been scrolled to. This point
 *						corresponds to the top left corner of the window.
 * handlers				The event handlers registered to this window.
 */
struct CedarWindow {
	struct {
		CedarWidget *first;
		CedarWidget *last;
		CedarWidget *selected;
	} widgets;

	CedarMenu *menu;

	gfx_region_t frame;
	gfx_point_t origin;

	enum {
		WINDOW_SCROLL_PAGE,
		WINDOW_SCROLL_WIDGET
	} scrollMode;

	bool repaint;

	CedarEventHandler *handlers;
};

/*
 * Initializes a window
 *
 * window		A pointer to the window to initialize.
 */
void cedar_InitWindow(CedarWindow *window);

/*
 * Destroys a window and cleans up its memory.
 *
 * window		The window to destroy.
 */
void cedar_DestroyWindow(CedarWindow *window);

/*
 * Displays a window
 *
 * window		The window to display
 * Returns the code the window mainloop exited on.
 */
void cedar_Display(CedarWindow *window);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_WINDOW_H_
