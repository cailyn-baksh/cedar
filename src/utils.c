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

#define MASK_BLACK	0x0
#define MASK_WHITE	0x1
#define MASK_TRANS	0x2
#define MASK_COLOR	0x3

// this routine can probably be rewritted in asm
void cedar_SpriteColorMask(uint24_t x, uint8_t y, uint8_t color, gfx_sprite_t *mask) {
	size_t spriteSize = mask->width * mask->height;

	uint24_t currentX = x;
	uint8_t currentY = y;
	uint8_t prevColor = gfx_SetColor(0);  // get the old color

	for (size_t i=0; i < spriteSize; ++i, ++currentX) {
		if (currentX >= mask->width + x) {
			currentX = x;
			currentY += 1;
		}

		switch (mask->data[i]) {
			case MASK_BLACK:
				gfx_SetColor(CEDAR_COLOR_BLACK);
				break;
			case MASK_WHITE:
				gfx_SetColor(CEDAR_COLOR_WHITE);
				break;
			case MASK_TRANS:
				continue;
			case MASK_COLOR:
				gfx_SetColor(color);
				break;
		}
		gfx_SetPixel(currentX, currentY);
	}

	gfx_SetColor(prevColor);
}
