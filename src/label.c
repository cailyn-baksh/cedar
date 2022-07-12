#include <stdlib.h>
#include <string.h>

#include "cedar.h"
#include "cedar/label.h"

#include "cedardbg.h"

Widget *Label(int x, int y, int width, int height, const char *text) {
	Widget *widget = malloc(sizeof(Widget));

	widget->type = WIDGET_LABEL;

	widget->next = NULL;
	widget->prev = NULL;

	widget->x = x;
	widget->y = y;
	widget->width = width;
	widget->height = height;

	widget->data = malloc(sizeof(LabelData));
	LabelData *data = widget->data;

	data->text = malloc(strlen(text) * sizeof(char));
	strcpy(data->text, text);

	widget->handler = defaultLabelHandler;

	return widget;
}

#define isWhitespace(c) (c == '\0'\
					  || c == ' '\
					  || c == '\t')

void drawLabelText(Widget *label) {
	const char *str = getData(LabelData, label)->text;
	const char *startOfWord = str;

	unsigned int xOffset = 0;
	unsigned int yOffset = 0;
	unsigned int wordLength = 0;

	bool hasFoundWord = false;

	do {
		if (hasFoundWord && isWhitespace(*str)) {
			// print entire word
			while (startOfWord != str) {  // pointer comparison
				if (!(xOffset == 0 && isWhitespace(*startOfWord))) {
					// Print chars that are not leading whitespace

					if (xOffset > label->width) {
						// At end of label, break to next line
						xOffset = 0;
						yOffset += 10;

						if (yOffset > label->height) return;  // too tall exit
					}

					gfx_SetTextXY(label->realX + xOffset, label->realY + yOffset);
					gfx_PrintChar(*startOfWord);
					xOffset += gfx_GetCharWidth(*startOfWord);
				}
				++startOfWord;
			}
			// after loop startOfWord = str+1
			wordLength = 0;
			hasFoundWord = false;
		} else {
			// Increase length of word
			hasFoundWord = true;
			wordLength += gfx_GetCharWidth(*str);
		}

		if (xOffset + wordLength > label->width && wordLength < label->width) {
			// The word is too long to fit on this line, but it will fit on the
			// next line. Break to next line
			// If the word will not fit on the next line then the print loop
			// will break it at the end of the line
			xOffset = 0;
			yOffset += 10;

			if (yOffset > label->height) return;  // too tall exit
		}
	} while (*(str++));
}

uint24_t defaultLabelHandler(Widget *widget, int event) {
	switch (event) {
		case EVENT_DESTROY:
			// Clean up widget
			free(getData(LabelData, widget)->text);
			break;
		case EVENT_PAINT:
			// Paint widget
			drawLabelText(widget);
			break;
	}

	return HANDLER_NORMAL;
}
