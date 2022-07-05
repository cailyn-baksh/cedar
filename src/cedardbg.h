/*
 * Wrapper arount CE debug.h to only include when DEBUG is defined
 */
#ifndef _CEDARDBG_H_
#define _CEDARDBG_H_

#ifdef DEBUG
#include <debug.h>

#define DBGPRINT(fmt, ...) dbg_printf(fmt, __VA_ARGS__)
#else
#define DBGPRINT
#endif  // DEBUG

#endif  // _CEDARDBG_H_
