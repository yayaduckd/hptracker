#include "shapes.h"

void drawSprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *data, bool transparent,
				bool black) {
	for (int i = 0; i < (width * height); i++) {
		int x1 = i % width;
		int y1 = i / width;

		int byte = i / 8;
		int bit = 7 - (i % 8);
		int value = (data[byte] >> bit) & 1;
		if (!value) {
			LCD_setPixel(x + x1, y + y1, black);
		} else if (!transparent) {
			LCD_setPixel(x + x1, y + y1, !black);
		}
	}
}

void drawRectangle(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, bool black, bool fill) {
	if (fill) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				LCD_setPixel(x0 + i, y0 + j, black);
			}
		}
	} else {
		for (int i = 0; i < width; i++) {
			LCD_setPixel(x0 + i, y0, true);
			LCD_setPixel(x0 + i, y0 + height, black);
		}

		for (int i = 0; i < height; i++) {
			LCD_setPixel(x0, y0 + i, true);
			LCD_setPixel(x0 + width, y0 + i, black);
		}
	}
}

void drawHealthBar(int health, int maxHealth) {

	const uint8_t x = 5;
	const uint8_t y = 20;

	const uint8_t rectx = x + 21;
	const uint8_t recty = y + 2;
	const uint8_t rectWidth = 48;
	const uint8_t rectHeight = 7;

	const uint8_t healthWidth = ((float)health / (float)maxHealth) * rectWidth;
	const int remainingWidth = rectWidth - healthWidth;
	const uint8_t healthHeight = rectHeight;

	drawSprite(x, y, SPRITE_HP_COUNTOUR_WIDTH, SPRITE_HP_COUNTOUR_HEIGHT, SPRITE_HP_COUNTOUR_DATA, false, true);
	if (maxHealth == 0) {
		drawRectangle(rectx, recty, rectWidth, healthHeight, true, true);
		return;
	} else if (health == 0) {
		drawRectangle(rectx, recty, rectWidth, healthHeight, false, true);
		return;
	}
	drawRectangle(rectx, recty, healthWidth, healthHeight, true, true);
	drawRectangle(rectx + healthWidth, recty, remainingWidth, healthHeight, false, true);
}

void drawDuck(Nokia5110 *lcd, int x, int y) {
	for (int i = 0; i < 8; i++) {
		sendCommand(lcd, 0x80 | (x + i));
		sendCommand(lcd, 0x40 | y);
		sendData(lcd, duck[i] & 0xFF);
	}

	for (int i = 0; i < 8; i++) {
		sendCommand(lcd, 0x80 | (x + i));
		sendCommand(lcd, 0x40 | (y + 1));
		sendData(lcd, (duck[i] >> 8) & 0xFF);
	}
}