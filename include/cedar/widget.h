#ifndef _LIBCEDAR_WIDGET_H_
#define _LIBCEDAR_WIDGET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <graphx.h>

#include <cedar/defs.h>

/*
 * A widget.
 *
 * id			The ID of this widget.
 * attrs		Widget attributes. These generally are constant
 * prev			A pointer to the previous widget
 * next			A pointer to the next widget
 * bounds		The bounds of this widget
 * repaint		Force repaint
 * enabled		Whether or not this widget is enabled.
 * handlers		The event handlers for this widget
 * data			Data for this widget, specific to each widget subclass.
 */
struct CedarWidget {
	ID id;

	uint24_t attrs;  // IDEA: make attrs quasi-constant by creating 3 bytes of
					 // padding that can only be accessed through assembly

	CedarWindow *parent;
	CedarWidget *prev;
	CedarWidget *next;

	gfx_region_t bounds;

	bool repaint;
	bool enabled;

	CedarEventHandler *handlers;

	uint8_t data[];
};
/*
 * Initializes a new empty widget
 */
void cedar_initWidget(CedarWidget *widget);

/*
 * Adds a widget to a window
 *
 * window		The window to add the widget to
 * widget		The widget to add to the window
 */
void cedar_AddWidget(CedarWindow *window, CedarWidget *widget);

/*
 * Destroys a widget and cleans up its memory
 *
 * widget		The widget to clean up
 */
// should this be exposed? could it be a part of cedar_DestroyWindow?
void cedar_DestroyWidget(CedarWidget *widget);


#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_WIDGET_H_
