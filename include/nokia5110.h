#ifndef NOKIA5110_HPP
#define NOKIA5110_HPP

#include <stdint.h>
#include <stdlib.h>

#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/flash.h" // for the flash erasing and writing
#include "pico/flash.h"
#include "hardware/sync.h" // for the interrupts


const uint BOARD_LED_PIN = CYW43_WL_GPIO_LED_PIN;

typedef struct {
    spi_inst_t *port;
    uint8_t d_c_pin;
    uint8_t reset_pin;
    uint8_t cs_pin;
    uint8_t backlight_pin;
    uint8_t dn_mosi_pin;
    uint8_t sck_pin;
} Nokia5110;

// Function Declarations
void initLCD(Nokia5110 *lcd);
void sendCommand(Nokia5110 *lcd, uint8_t command);
void sendData(Nokia5110 *lcd, uint8_t data);

#endif // NOKIA5110_HPP