extern "C" {
	#include "dwm_pico_5110_LCD.h"
}
#include "nokia5110.h"
#include "shapes.h"

// Function Declarations
void initLCD(Nokia5110 *lcd);
void fillBlack(Nokia5110 *lcd);
void drawDuck(Nokia5110 *lcd, int x, int y);
void sendCommand(Nokia5110 *lcd, uint8_t command);
void sendData(Nokia5110 *lcd, uint8_t data);


const uint8_t array [] = {
// 'Sprite-0001', 84x48px
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xeb, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfe, 0x9f, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xed, 0xf9, 0x7f, 0xbf, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xfe, 0xcf, 0x37, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xee, 0xf7, 0x7f, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xfc, 0xe6, 0x73, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xdf, 0x6f, 0xbf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xfd, 0xf0, 0xfb, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xdf, 0x9f, 0xbf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 
  0xf9, 0xfb, 0xfd, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 
  0xff, 0x9f, 0xdf, 0xe2, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 
  0xfd, 0xfd, 0xfe, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 
  0xdf, 0xdf, 0xcf, 0x7f, 0xfb, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xfd, 
  0xfd, 0xfd, 0xc7, 0xff, 0xbe, 0x0f, 0xc0, 0xff, 0xdf, 0xff, 0xdf, 
  0xde, 0x91, 0xff, 0xf9, 0xce, 0xe1, 0xef, 0xfd, 0xff, 0xfd, 0xfd, 
  0xe0, 0x7f, 0xff, 0xd3, 0xe8, 0xff, 0xff, 0xdf, 0xff, 0xdf, 0xdf, 
  0xbf, 0xff, 0xfd, 0x7e, 0x3f, 0xff, 0xfd, 0xff, 0xfc, 0xfd, 0xf9, 
  0xff, 0xef, 0xcf, 0xef, 0xff, 0xff, 0xdf, 0xff, 0xef, 0xdf, 0xdf, 
  0xfc, 0xfc, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xfe, 0xfd, 0xfc, 0x7e, 
  0x1f, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xef, 0xff, 0xf0, 0x0f, 
  0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

int main() {
	Nokia5110 lcd = {
		.port = spi0, .d_c_pin = 4, .reset_pin = 1, .cs_pin = 0, .backlight_pin = 5, .dn_mosi_pin = 3, .sck_pin = 2};

	bi_decl(bi_program_description("This is a test binary."));
	bi_decl(bi_1pin_with_name(BOARD_LED_PIN, "On-board LED"));

	stdio_init_all();

	if (cyw43_arch_init()) {
		printf("Wi-Fi init failed");
		return -1;
	}
	cyw43_arch_gpio_put(BOARD_LED_PIN, 1);


	// Set up pins
	gpio_init(lcd.d_c_pin);
	gpio_set_dir(lcd.d_c_pin, GPIO_OUT);
	gpio_init(lcd.reset_pin);
	gpio_set_dir(lcd.reset_pin, GPIO_OUT);
	gpio_init(lcd.cs_pin);
	gpio_set_dir(lcd.cs_pin, GPIO_OUT);
	gpio_init(lcd.backlight_pin);
	gpio_set_dir(lcd.backlight_pin, GPIO_OUT);


	// Set up SPI
	spi_init(lcd.port, 1000000);
	gpio_set_function(lcd.dn_mosi_pin, GPIO_FUNC_SPI);
	gpio_set_function(lcd.sck_pin, GPIO_FUNC_SPI);

	// Reset Display
	gpio_put(lcd.reset_pin, 0);
	sleep_ms(100);
	gpio_put(lcd.reset_pin, 1);

	// Initialize Display
	initLCD(&lcd);
	gpio_put(lcd.backlight_pin, 1);

	// Fill Display
	
	LCD_setSPIInstance(lcd.port);
    LCD_setSCE(lcd.cs_pin);
    LCD_setRST(lcd.reset_pin);
    LCD_setDC(lcd.d_c_pin);
    LCD_setDIN(lcd.dn_mosi_pin);
    LCD_setSCLK(lcd.sck_pin);

	LCD_init();
	LCD_clrScr();
	LCD_clrBuff();
	cyw43_arch_gpio_put(BOARD_LED_PIN, 0);

	// drawDuck(&lcd, 0, 0);
	// draw each pixel 1 by 1, display is 84x48
	for (int i = 0; i < 84 * 48; i++) {

		int x = i % 84;
		int y = i / 84;

		int byte = i / 8;
		int bit = 7 - (i % 8);
		int value = (array[byte] >> bit) & 1;
		LCD_setPixel(x, y, !value);
	}
	LCD_refreshScr();

}