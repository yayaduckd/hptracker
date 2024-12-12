#include "nokia5110.h"

void initLCD(Nokia5110 *lcd) {
	sendCommand(lcd, 0x21); // Extended Commands mode
	sendCommand(lcd, 0xB8); // Set Vop (Contrast)
	sendCommand(lcd, 0x04); // Set Temp coefficent
	sendCommand(lcd, 0x14); // LCD bias mode 1:48
	sendCommand(lcd, 0x0C); // LCD in normal mode
	sendCommand(lcd, 0x20); // Standard Commands mode, Horizontal addressing mode
	sendCommand(lcd, 0x0C); // LCD in normal mode
}


void sendCommand(Nokia5110 *lcd, uint8_t command) {
	gpio_put(lcd->d_c_pin, 0);
	gpio_put(lcd->cs_pin, 0);
	spi_write_blocking(lcd->port, &command, 1);
	gpio_put(lcd->cs_pin, 1);
}

void sendData(Nokia5110 *lcd, uint8_t data) {
	gpio_put(lcd->d_c_pin, 1);
	gpio_put(lcd->cs_pin, 0);
	spi_write_blocking(lcd->port, &data, 1);
	gpio_put(lcd->cs_pin, 1);
}