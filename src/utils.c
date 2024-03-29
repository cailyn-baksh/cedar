#include "cedar.h"
#include "cedar/utils.h"

#include <debug.h>

#define isWhitespace(c) (c == '\0'\
					  || c == ' '\
					  || c == '\t')

void cedar_wrapTextInBox(const char *str, int x, int y, unsigned int width, unsigned int height) {
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

					if (xOffset > width) {
						// At end of label, break to next line
						xOffset = 0;
						yOffset += 10;

						if (yOffset > height) return;  // too tall exit
					}

					gfx_SetTextXY(x + xOffset, y + yOffset);
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

		if (xOffset + wordLength > width && wordLength < width) {
			// The word is too long to fit on this line, but it will fit on the
			// next line. Break to next line
			// If the word will not fit on the next line then the print loop
			// will break it at the end of the line
			xOffset = 0;
			yOffset += 10;

			if (yOffset > height) return;  // too tall exit
		}
	} while (*(str++));
}
