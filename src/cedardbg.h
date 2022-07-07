/*
 * Wrapper arount CE debug.h to only include when DEBUG is defined
 */
#ifndef _CEDARDBG_H_
#define _CEDARDBG_H_

#ifdef DEBUG
#include <debug.h>

#define DBGPRINT(...) dbg_printf(__VA_ARGS__)
#else
#define DBGPRINT
#define NDEBUG
#endif  // DEBUG

#endif  // _CEDARDBG_H_
