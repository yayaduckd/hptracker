#ifndef NOKKIA5110_SHAPES_HPP
#define NOKKIA5110_SHAPES_HPP

#include "nokia5110.h"
extern "C" {
	#include "dwm_pico_5110_LCD.h"
}
#include "sprites.h"

const uint16_t duck[] = {0b0000000001110000, 0b0000000011001100, 0b0000000011001110, 0b0001111110001000,
						 0b0011000000001000, 0b0011111111110000, 0b0000100001000000, 0b0000110001100000};


// Function Declarations
void drawSprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *data, bool transparent, bool black);
void drawRectangle(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, bool black, bool fill);
void drawHealthBar(int health, int maxHealth);
void drawDuck(Nokia5110 *lcd, int x, int y);

#endif // NOKKIA5110_SHAPES_HPP