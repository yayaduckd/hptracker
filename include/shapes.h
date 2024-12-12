#ifndef NOKKIA5110_SHAPES_HPP
#define NOKKIA5110_SHAPES_HPP

#include "nokia5110.h"

const uint16_t duck[] = {0b0000000001110000, 0b0000000011001100, 0b0000000011001110, 0b0001111110001000,
						 0b0011000000001000, 0b0011111111110000, 0b0000100001000000, 0b0000110001100000};


// Function Declarations
void fillBlack(Nokia5110 *lcd);
void drawPixel(Nokia5110 *lcd, int x, int y, bool black);
void drawDuck(Nokia5110 *lcd, int x, int y);
void drawRectangle(Nokia5110 *lcd, int x, int y, int width, int height, bool fill, bool black);

#endif // NOKKIA5110_SHAPES_HPP