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
 * widgets.first		A pointer to the first widget in the window. If NULL
 *						then there are no widgets in this window.
 * widget.last			A pointer to the last widget in the window. If NULL then
 *						there are no widgets in this window.
 * widget.selected		A pointer to the currently selected widget. If NULL then
 *						no widget is selected.
 *
 * timers.first			A pointer to the first timer associated with this window
 * timers.last			A pointer to the last timer associated with this window
 *
 * menu					A pointer to the menu for this window’s menu bar. If
 *						NULL then this window does not have a menu bar.
 *
 * frame				The rectangle within virtual coordinate space that is
 *						currently visible in the window.
 * origin				A point in the drawing buffer that is mapped to (0,0) in
 *						virtual coordinate space.
 * scrollMode			Which method to use to scroll the window.
 *
 * colors.fg			The foreground color for components in this window.
 * colors.bg			The background color for components in this window.
 * colors.alt			The alternate color for components in this window.
 *
 * repaint				When true, the menubar and all widgets are repainted.
 *
 * handlers				The chain of event handlers to handle events dispatched
 *						to this window.
 */
struct CedarWindow {
	struct {
		CedarWidget *first;
		CedarWidget *last;
		CedarWidget *selected;
	} widgets;

	struct {
		CedarTimer *first;
		CedarTimer *last;
	} timers;

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
