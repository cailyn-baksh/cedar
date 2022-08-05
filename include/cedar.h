/*
 * A widget library for the TI 84+ CE
 */
#ifndef _LIBCEDAR_H_
#define _LIBCEDAR_H_

#define _LIBCEDAR_Q(s) #s
#define _LIBCEDAR_QUOTE(s) _LIBCEDAR_Q(s)

// Version info
// minor version is incremented each release
// major version is incremented after each breaking change (after major version 1)
#define LIBCEDAR_MAJOR		1
#define LIBCEDAR_MINOR		1
#define LIBCEDAR_REVISION	0
#define LIBCEDAR_VERSION_STR _LIBCEDAR_QUOTE(LIBCEDAR_MAJOR) "." _LIBCEDAR_QUOTE(LIBCEDAR_MINOR) "." _LIBCEDAR_QUOTE(LIBCEDAR_REVISION)

#ifdef LIBCEDAR_CHECK_VERSION

#if LIBCEDAR_MAJOR != LIBCEDAR_TARGET_MAJOR
#error Desired libcedar version is incompatible with installed version (major versions do not match)
#endif  // major version

#if LIBCEDAR_MINOR < LIBCEDAR_TARGET_MINOR
#error Desired libcedar version is newer than installed version (minor version out of date)
#endif  // minor version

#if defined(LIBCEDAR_VERSION_REQUIRE_SAME)

#if (LIBCEDAR_MINOR != LIBCEDAR_TARGET_MINOR) || (LIBCEDAR_REVISION != LIBCEDAR_TARGET_REVISION)
#error Desired libcedar version is incompatible with installed version (installed version must be same)
#endif

#elif defined(LIBCEDAR_VERSION_REQUIRE_SAME_OR_BETTER)

#if LIBCEDAR_REVISION < LIBCEDAR_TARGET_REVISION
#error Desired libcedar version is newer than installed version (installed version must be same or better)
#endif

#endif

#endif  // LIBCEDAR_CHECK_VERSION

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include <graphx.h>
#include <keypadc.h>

#include <cedar/defs.h>
#include <cedar/menu.h>
#include <cedar/timer.h>
#include <cedar/widget.h>
#include <cedar/window.h>

#define GFX_REGION_WIDTH(r)		((r).xmax - (r).xmin)
#define GFX_REGION_HEIGHT(r)	((r).ymax - (r).ymin)

/*
 * A singly linked list of event handlers
 *
 * callback		A pointer to this handler's callback function
 * next			A pointer to the next handler
 */
struct CedarEventHandler {
	CedarEventHandlerCallback *callback;
	CedarEventHandler *next;
};


#define getData(type, widget) ((type *)widget->data)

/*
 * Initializes the library. This must be called before any other cedar functions
 */
void cedar_Init();

/*
 * Cleans up after the library. This should be called after you are done using
 * the library.
 */
void cedar_Cleanup();

/*
 * Register a new event handler.
 *
 * handlerStack	The handler stack to push this handler onto
 * callback		The callback to push onto the handler stack
 */
void cedar_RegisterEventHandler(CedarEventHandler *handlerStack, CedarEventHandlerCallback *callback);

CALLBACKRESULT _cedar_dispatchEvent(CedarEventHandler *firstHandler, void *self, EVENT event, uint24_t param);

/*
 * Dispatch an event to a component. Component can be a pointer to any type as
 * long as it has a handlers member.
 */
#define cedar_dispatchEvent(event, component, param) _cedar_dispatchEvent((component)->handlers, component, event, param)

struct CedarColors {
	uint8_t fg;
	uint8_t bg;
	uint8_t alt;
	uint8_t disabled;
} __attribute__((packed));

#ifndef _NOEXTERN
extern struct CedarColors cedar_colors;
#endif  // _NOEXTERN

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_H_
