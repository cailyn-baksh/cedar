#ifndef _LIBCEDAR_RADIO_H_
#define _LIBCEDAR_RADIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <cedar/defs.h>

typedef struct {
	ID *selection;
} RadioButtonData;

CedarWidget *CedarRadioButton(ID id, int x, int y, ID *selection);

#ifdef __cplusplus
}
#endif

#endif  // _LIBCEDAR_RADIO_H_
