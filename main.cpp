extern "C" {
#include "dwm_pico_5110_LCD.h"
}
#include "nokia5110.h"
#include "shapes.h"
#include "sprites.h"

// Function Declarations
// void initLCD(Nokia5110* lcd);
// void fillBlack(Nokia5110* lcd);
// void drawDuck(Nokia5110* lcd, int x, int y);
// void sendCommand(Nokia5110* lcd, uint8_t command);
// void sendData(Nokia5110* lcd, uint8_t data);

// Pins
#define INPUT_LEFT_PIN 19
#define INPUT_RIGHT_PIN 16
#define INPUT_OK_PIN 17

void setupLCD()
{

    Nokia5110 lcd = {
        .port = spi0, .d_c_pin = 4, .reset_pin = 1, .cs_pin = 0, .backlight_pin = 5, .dn_mosi_pin = 3, .sck_pin = 2
    };

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

void setupInputs()
{
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

// Configuration parameters (can be easily tweaked)
static const int INITIAL_HEALTH = 0;
static const int INITIAL_MAX_HEALTH = 100;
static const int DEFAULT_INCREMENT = 1;
static const int ACCELERATED_INCREMENT = 10;
static const int UI_HEALTH_TEXT_X = 35;
static const int UI_HEALTH_TEXT_Y = 1;
static const int LOOP_DELAY_MS = 10;
static const int HOLD_INCREASE_DELAY = 150;
static const int CHANGE_ACCELERATE_THRESHOLD = 10;

// Structures to keep code organized
typedef struct {
    bool left;
    bool right;
    bool ok;
} ButtonState;

typedef struct {
    unsigned int left;
    unsigned int right;
    unsigned int ok;
} ButtonHeldTimes;

typedef struct {
    uint16_t health;
    uint16_t maxHealth;
    uint16_t lastChangeTime;
} GameState;

const int FLASH_TARGET_OFFSET = PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE;
const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);

void print_buf(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
        if (i % 16 == 15)
            printf("\n");
        else
            printf(" ");
    }
}

static void call_flash_range_erase(void *param) {
    uint32_t offset = (uint32_t)param;
    flash_range_erase(offset, FLASH_SECTOR_SIZE);
}

static void call_flash_range_program(void *param) {
    uint32_t offset = ((uintptr_t*)param)[0];
    const uint8_t *data = (const uint8_t *)((uintptr_t*)param)[1];
    flash_range_program(offset, data, FLASH_PAGE_SIZE);
}

void saveStateToFlash(GameState gameState) {

	// buffer with page size
	uint8_t buffer[FLASH_PAGE_SIZE];

    uint8_t* myDataAsBytes = (uint8_t*) &gameState;
    int myDataSize = sizeof(gameState);

	memcpy(buffer, myDataAsBytes, myDataSize);

	// fill buffer with 0xFF
	memset(buffer + myDataSize, 0xFF, FLASH_PAGE_SIZE - myDataSize);

    
	int rc = flash_safe_execute(call_flash_range_erase, (void*)FLASH_TARGET_OFFSET, UINT32_MAX);
	hard_assert(rc == PICO_OK);

	uintptr_t params[] = { FLASH_TARGET_OFFSET, (uintptr_t)buffer};

    rc = flash_safe_execute(call_flash_range_program, params, UINT32_MAX);
    hard_assert(rc == PICO_OK);
}

static void readStateFromFlash(GameState* gameState) {
	const uint8_t* flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
    memcpy(gameState, flash_target_contents, sizeof(GameState));
}

//--------------------------------------
// Input Handling
//--------------------------------------

static ButtonState readButtonState(void)
{
    // Invert gpio_get because we assume: Pressed = LOW, Not Pressed = HIGH
    ButtonState state;
    state.left = !gpio_get(INPUT_LEFT_PIN);
    state.right = !gpio_get(INPUT_RIGHT_PIN);
    state.ok = !gpio_get(INPUT_OK_PIN);
    return state;
}

static void resetHeldTimes(ButtonHeldTimes* heldTimes)
{
    heldTimes->left = 0;
    heldTimes->right = 0;
    heldTimes->ok = 0;

}

static void updateHeldTimes(ButtonHeldTimes* heldTimes, const ButtonState* state)
{
    heldTimes->left = state->left ? (heldTimes->left + 1) : 0;
    heldTimes->right = state->right ? (heldTimes->right + 1) : 0;
    heldTimes->ok = state->ok ? (heldTimes->ok + 1) : 0;
}

//--------------------------------------
// State Update Helpers
//--------------------------------------

static int getIncrement(int heldTime)
{
    return (heldTime > CHANGE_ACCELERATE_THRESHOLD) ? ACCELERATED_INCREMENT : DEFAULT_INCREMENT;
}

static void updateGameStateFromInput(GameState* state, ButtonHeldTimes* heldTimes, const ButtonState* currentButtons)
{
    // Determine increments
    int leftIncrement = getIncrement(heldTimes->left);
    int rightIncrement = getIncrement(heldTimes->right);

    // If we are holding OK, we adjust maxHealth; otherwise, we adjust health
    if (currentButtons->ok) {
        if (currentButtons->left)
            state->maxHealth -= state->maxHealth - leftIncrement < 0 ? state->maxHealth : leftIncrement;
        if (currentButtons->right)
            state->maxHealth += state->maxHealth + rightIncrement > UINT16_MAX ? UINT16_MAX - state->maxHealth : rightIncrement;
    } else {
        if (currentButtons->left)
            state->health -= state->health - leftIncrement < 0 ? state->health : leftIncrement;
        if (currentButtons->right)
			state->health += state->health + rightIncrement > state->maxHealth ? state->maxHealth - state->health : rightIncrement;
    }
}

static void clampValues(GameState* state)
{
    if (state->maxHealth < 0) {
        state->maxHealth = 0;
    }

    if (state->health < 0) {
        state->health = 0;
    }

    if (state->health > state->maxHealth) {
        state->health = state->maxHealth;
    }
}

//--------------------------------------
// Rendering
//--------------------------------------

static void renderUI(const GameState* state)
{
    if (state->maxHealth > 0) {
        drawHealthBar(state->health, state->maxHealth);
        LCD_refreshScr();
    }

    char healthStr[16];
    snprintf(healthStr, sizeof(healthStr), "%d/%d", state->health, state->maxHealth);
    LCD_print(healthStr, UI_HEALTH_TEXT_X, UI_HEALTH_TEXT_Y);
}

int main()
{

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

    GameState state;
	readStateFromFlash(&state);
	if (state.maxHealth == 0 && state.health == 0) {
		state.health = INITIAL_HEALTH;
		state.maxHealth = INITIAL_MAX_HEALTH;
	}
    state.lastChangeTime = to_ms_since_boot(get_absolute_time());

    ButtonHeldTimes heldTimes = { 0, 0, 0 };

    while (true) {
        // Render current state
        renderUI(&state);

        // Read buttons
        ButtonState currentButtons = readButtonState();

        int currentTime = to_ms_since_boot(get_absolute_time());

        // If within the hold-increase delay, ignore button inputs
        bool withinDelay = (currentTime - state.lastChangeTime < HOLD_INCREASE_DELAY);
        if (withinDelay) {
            // Ignore inputs
            currentButtons.left = false;
            currentButtons.right = false;
            currentButtons.ok = false;
        } else {
            // Update held times if not within delay period
            updateHeldTimes(&heldTimes, &currentButtons);
        }

        // Update game state based on current inputs
        if (currentButtons.left || currentButtons.right || currentButtons.ok) {
            updateGameStateFromInput(&state, &heldTimes, &currentButtons);
            state.lastChangeTime = currentTime;
        }

        // Save state to flash
        if (currentButtons.ok && !currentButtons.left && !currentButtons.right && heldTimes.ok > 10) {
            LCD_clrScr();
			char line1[16];
			char line2[16];
			snprintf(line1, sizeof(line1), "Saving state");
			snprintf(line2, sizeof(line2), "to flash...");
			LCD_print(line1, 0, 0);
			LCD_print(line2, 0, 1);
			cyw43_arch_gpio_put(BOARD_LED_PIN, 0);
            saveStateToFlash(state);
			cyw43_arch_gpio_put(BOARD_LED_PIN, 1);
            sleep_ms(1000);
            LCD_clrScr();
        }

        // Clamp values to valid ranges
        clampValues(&state);

        // Delay to prevent busy loop
        sleep_ms(LOOP_DELAY_MS);
    }
}	// memset(buffer, 0xFF, FLASH_PAGE_SIZE);
