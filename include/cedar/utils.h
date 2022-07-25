#ifndef _CEDAR_UTILS_H_
#define _CEDAR_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cedar.h>

/*
 * Draw text wrapped to fit in a box
 *
 * str		The string to wrap into the box
 * x		The x coordinate on the screen of the box
 * y		The y coordinate on the screen of the box
 * width	The width of the box
 * height	The height of the box
 */
void cedar_wrapTextInBox(const char *str, int x, int y, unsigned int width, unsigned int height);

#ifdef __cplusplus
}
#endif

#endif  // _CEDAR_UTILS_H_
