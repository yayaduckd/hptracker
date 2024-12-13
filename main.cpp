extern "C" {
	#include "dwm_pico_5110_LCD.h"
}
#include "nokia5110.h"
#include "shapes.h"
#include "sprites.h"

// Function Declarations
void initLCD(Nokia5110 *lcd);
void fillBlack(Nokia5110 *lcd);
void drawDuck(Nokia5110 *lcd, int x, int y);
void sendCommand(Nokia5110 *lcd, uint8_t command);
void sendData(Nokia5110 *lcd, uint8_t data);

void setupLCD() {


	Nokia5110 lcd = {
		.port = spi0, .d_c_pin = 4, .reset_pin = 1, .cs_pin = 0, .backlight_pin = 5, .dn_mosi_pin = 3, .sck_pin = 2};

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
}


#define INPUT_LEFT_PIN 19
#define INPUT_RIGHT_PIN 16
#define INPUT_OK_PIN 17
void setupInputs() {
	gpio_init(INPUT_LEFT_PIN);
	gpio_set_dir(INPUT_LEFT_PIN, GPIO_IN);
	gpio_pull_up(INPUT_LEFT_PIN);

	gpio_init(INPUT_RIGHT_PIN);
	gpio_set_dir(INPUT_RIGHT_PIN, GPIO_IN);
	gpio_pull_up(INPUT_RIGHT_PIN);

	gpio_init(INPUT_OK_PIN);
	gpio_set_dir(INPUT_OK_PIN, GPIO_IN);
	gpio_pull_up(INPUT_OK_PIN);
}


int main() {


	bi_decl(bi_program_description("This is a test binary."));
	bi_decl(bi_1pin_with_name(BOARD_LED_PIN, "On-board LED"));

	stdio_init_all();

	if (cyw43_arch_init()) {
		printf("Wi-Fi init failed");
		return -1;
	}
	cyw43_arch_gpio_put(BOARD_LED_PIN, 1);

	setupLCD();
	setupInputs();


	// drawDuck(&lcd, 0, 0);
	// draw each pixel 1 by 1, display is 84x48

	

	// drawSprite(0, 0, 84, 48, mlemarray, false, true);


	int health = 0;
	int maxHealth = 100;
	bool reverse = false;
	while (true) {
		if (maxHealth > 0) {
			drawHealthBar(health, maxHealth);
		}
		// health += reverse ? -1 : 1;
		// if (health == 100 || health == 0) {
		// 	reverse = !reverse;
		// }

		bool left = !gpio_get(INPUT_LEFT_PIN);
		bool right = !gpio_get(INPUT_RIGHT_PIN);
		bool ok = !gpio_get(INPUT_OK_PIN);

		if (ok) {
			if (left) {
				maxHealth -= 1;
			} 
			if (right) {
				maxHealth += 1;
			}


		} else {
			if (left) {
				health -= 1;
			} 
			if (right) {
				health += 1;
			}
		}

		// clamp max health
		if (maxHealth < 0) {
			maxHealth = 0;
		}

		// clamp health
		if (health < 0) {
			health = 0;
		}
		if (health > maxHealth) {
			health = maxHealth;
		}



		LCD_refreshScr();

		// health string
		char healthStr[10];
		sprintf(healthStr, "%d/%d", health, maxHealth);
		LCD_print(healthStr, 35, 1);

		sleep_ms(10);
	}

	// drawSprite(5, 20, SPRITE_HP_COUNTOUR_WIDTH, SPRITE_HP_COUNTOUR_HEIGHT, SPRITE_HP_COUNTOUR_DATA, true, true);
	// LCD_refreshScr();

}