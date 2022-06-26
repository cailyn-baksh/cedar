#include <stdlib.h>

#include "cedar.h"

void cedar_destroyWidget(Widget *widget) {
	if (widget->prev != NULL) {
		widget->prev->next = widget->next;
	}

	free(widget->data);
	free(widget);
}
