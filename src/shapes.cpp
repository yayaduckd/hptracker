#include "shapes.h"

void fillBlack(Nokia5110 *lcd) {
	for (int i = 0; i < 504; i++) {
		sendData(lcd, 0xFF);
	}
}

void drawPixel(Nokia5110 *lcd, int x, int y, bool black) {
	if (black) {
		sendCommand(lcd, 0x80 | x);
		sendCommand(lcd, 0x40 | y);
		sendData(lcd, 0b10000000);
	} else {
		sendCommand(lcd, 0x80 | x);
		sendCommand(lcd, 0x40 | y);
		sendData(lcd, 0x00);
	}
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

void drawRectangle(Nokia5110 *lcd, int x, int y, int width, int height, bool fill, bool black) {

}