#include <stdlib.h>
#include <string.h>

#include "cedar.h"
#include "cedar/label.h"

Widget *Label(int x, int y, int width, int height, const char *text) {
	Widget *widget = malloc(sizeof(Widget));

	widget->type = WIDGET_LABEL;

	widget->next = NULL;
	widget->prev = NULL;

	widget->x = x;
	widget->y = y;
	widget->width = gfx_GetCharWidth('m') * width;
	widget->height = height;

	widget->data = malloc(sizeof(LabelData));
	LabelData *data = widget->data;

	size_t len = strlen(text);
	data->nLines = (len + (width-1)) / width;  // ceiling division

	// allocate enough room for all the lines
	data->lines = malloc(data->nLines * sizeof(char *));

	for (size_t index=0, line=0; line < data->nLines; ++line, index += width) {
		data->lines[line] = malloc(width * sizeof(char) + 1);
		strncpy(data->lines[line], &text[index], width);
		data->lines[line][width] = '\0';
	}

	widget->handler = defaultLabelHandler;

	return widget;
}

void paintLabel(Widget *widget) {
	for (int i=0, lineOffset=0; i < getData(LabelData, widget)->nLines; ++i, lineOffset += 10) {
		gfx_PrintStringXY(getData(LabelData, widget)->lines[i], widget->realX, widget->realY + lineOffset);
	}
}

int defaultLabelHandler(Widget *widget, int event) {
	switch (event) {
		case EVENT_PAINT:
			// Paint widget
			paintLabel(widget);
			break;
	}
}
