#ifndef _LIBCEDAR_DEFS_H_
#define _LIBCEDAR_DEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

struct CdrWindow;
typedef struct CdrWindow CdrWindow;

struct CdrWidget;
typedef struct CdrWidget CdrWidget;

struct Cdr

typedef uint24_t CALLBACKRESULT;
typedef uint24_t EVENT;
typedef uint24_t ID;

typedef CALLBACKRESULT CdrEventHandlerCallback(void *self, EVENT event, uint24_t paramA, uint24_t paramB);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_DEFS_H_
