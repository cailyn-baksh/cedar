#include "cedar.h"

void cedar_RegisterEventHandler(CedarEventHandler *handlerStack, CedarEventHandlerCallback *callback) {
	CedarEventHandler *nextHandler = malloc(sizeof(CedarEventHandler));
	nextHandler->callback = handlerStack->callback;
	nextHandler->next = handlerStack->next;

	handlerStack->callback = callback;
	handlerStack->next = nextHandler;
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