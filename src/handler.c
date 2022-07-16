#include "cedar.h"

// should these be turned into a macro?
void _cedar_RegisterWindowEventHandler(CedarWindow *window, CedarEventHandlerCallback callback) {
	CedarEventHandler *oldHandler = window->handlers;

	window->handlers = malloc(sizeof(CedarEventHandler));
	window->handlers->callback = callback;
	window->handlers->next = oldHandler;
}

void _cedar_RegisterWidgetEventHandler(CedarWidget *widget, CedarEventHandlerCallback callback) {
	CedarEventHandler *oldHandler = widget->handlers;

	widget->handlers = malloc(sizeof(CedarEventHandler));
	widget->handlers->callback = callback;
	widget->handlers->next = oldHandler;
}

CALLBACKRESULT _cedar_dispatchEvent(CedarEventHandler *firstHandler, void *self, EVENT event, uint24_t param) {
	CALLBACKRESULT result = CALLBACK_NORMAL;
	CedarEventHandler *handler = firstHandler;

	while (result == CALLBACK_NORMAL && handler != NULL) {
		result = handler->callback(self, event, param);
		handler = handler->next;
	}

	return result;
}