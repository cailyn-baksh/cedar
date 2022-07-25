#ifndef _LIBCEDAR_CANVAS_H_
#define _LIBCEDAR_CANVAS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cedar.h>

/*
 * A canvas is a widget which can be freely drawn in.
 *
 * x		The X coordinate of the left side of the canvas
 * y		The Y coordinate of the top of the canvas
 * width	The width of the canvas
 * height	The height of the canvas
 */
CedarWidget *Canvas(int x, int y, int width, int height);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_CANVAS_H_
