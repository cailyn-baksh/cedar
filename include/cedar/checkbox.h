/*
 * Cedar checkbox widget
 */
#ifndef _LIBCEDAR_CHECKBOX_H_
#define _LIBCEDAR_CHECKBOX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cedar/defs.h>

#define CEDAR_CHECKBOX_WIDTH 10
#define CEDAR_CHECKBOX_HEIGHT CEDAR_CHECKBOX_WIDTH

typedef struct {
	bool checked;
} CheckboxData;

/*
 * Create a new Checkbox widget
 *
 * id		The ID of the widget
 * x		The X coordinate of the widget
 * y		The Y coordinate of the widget
 */
CedarWidget *CedarCheckbox(ID id, int x, int y);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_LABEL_H_
