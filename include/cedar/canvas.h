#ifndef _LIBCEDAR_CANVAS_H_
#define _LIBCEDAR_CANVAS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cedar/defs.h>

typedef struct {
	uint8_t *data;
} CanvasData;

/*
 * A canvas is a widget which can be freely drawn in.
 *
 * x		The X coordinate of the left side of the canvas
 * y		The Y coordinate of the top of the canvas
 * width	The width of the canvas
 * height	The height of the canvas
 */
CedarWidget *Canvas(ID id, int x, int y, int width, int height);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_CANVAS_H_
