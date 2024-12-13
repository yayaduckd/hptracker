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

struct {
	uint8_t width = 84;
	uint8_t height = 48;
} Window;

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

// mode enum
enum Mode {
	Normal,
	Menu,
	SetMaxHealth,
};

typedef struct {
	uint16_t health;
	uint16_t maxHealth;
	uint16_t lastChangeTime;

	uint8_t menuIndex = 0;

	Mode mode = Normal;
} GameState;

GameState state;
ButtonHeldTimes heldTimes = {0, 0, 0};
ButtonState currentButtons = {false, false, false};

const int FLASH_TARGET_OFFSET = PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE;
const uint8_t *flash_target_contents = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);

void print_buf(const uint8_t *buf, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		printf("%02x", buf[i]);
		if (i % 16 == 15) printf("\n");
		else printf(" ");
	}
}

static void call_flash_range_erase(void *param) {
	uint32_t offset = (uint32_t)param;
	flash_range_erase(offset, FLASH_SECTOR_SIZE);
}

static void call_flash_range_program(void *param) {
	uint32_t offset = ((uintptr_t *)param)[0];
	const uint8_t *data = (const uint8_t *)((uintptr_t *)param)[1];
	flash_range_program(offset, data, FLASH_PAGE_SIZE);
}

typedef struct {
	uint16_t health;
	uint16_t maxHealth;
} SaveData;

void saveStateToFlash(GameState state) {

	LCD_clrScr();
	char line1[16];
	char line2[16];
	snprintf(line1, sizeof(line1), "Saving state");
	snprintf(line2, sizeof(line2), "to flash...");
	LCD_print(line1, 0, 0);
	LCD_print(line2, 0, 1);
	cyw43_arch_gpio_put(BOARD_LED_PIN, 0);

	SaveData saveData = {state.health, state.maxHealth};

	// buffer with page size
	uint8_t buffer[FLASH_PAGE_SIZE];

	uint8_t *myDataAsBytes = (uint8_t *)&saveData;
	int myDataSize = sizeof(saveData);

	memcpy(buffer, myDataAsBytes, myDataSize);

	// fill buffer with 0xFF
	memset(buffer + myDataSize, 0xFF, FLASH_PAGE_SIZE - myDataSize);

	int rc = flash_safe_execute(call_flash_range_erase, (void *)FLASH_TARGET_OFFSET, UINT32_MAX);
	hard_assert(rc == PICO_OK);

	uintptr_t params[] = {FLASH_TARGET_OFFSET, (uintptr_t)buffer};

	rc = flash_safe_execute(call_flash_range_program, params, UINT32_MAX);
	hard_assert(rc == PICO_OK);

	cyw43_arch_gpio_put(BOARD_LED_PIN, 1);
	sleep_ms(1000);
	LCD_clrScr();
}

static void readStateFromFlash(SaveData *saveData) {
	const uint8_t *flash_target_contents = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);
	memcpy(saveData, flash_target_contents, sizeof(SaveData));
}

//--------------------------------------
// Input Handling
//--------------------------------------

static ButtonState readButtonState(void) {
	// Invert gpio_get because we assume: Pressed = LOW, Not Pressed = HIGH
	ButtonState state;
	state.left = !gpio_get(INPUT_LEFT_PIN);
	state.right = !gpio_get(INPUT_RIGHT_PIN);
	state.ok = !gpio_get(INPUT_OK_PIN);
	return state;
}

static void resetHeldTimes(ButtonHeldTimes *heldTimes) {
	heldTimes->left = 0;
	heldTimes->right = 0;
	heldTimes->ok = 0;
}

static void updateHeldTimes(ButtonHeldTimes *heldTimes, const ButtonState *state) {
	heldTimes->left = state->left ? (heldTimes->left + 1) : 0;
	heldTimes->right = state->right ? (heldTimes->right + 1) : 0;
	heldTimes->ok = state->ok ? (heldTimes->ok + 1) : 0;
}

//--------------------------------------
// State Update Helpers
//--------------------------------------

static int getIncrement(int heldTime) {
	return (heldTime > CHANGE_ACCELERATE_THRESHOLD) ? ACCELERATED_INCREMENT : DEFAULT_INCREMENT;
}

static void updateGameStateFromInput(GameState *state, ButtonHeldTimes *heldTimes, const ButtonState *currentButtons,
									 bool setMax = false) {
	// Determine increments
	int leftIncrement = getIncrement(heldTimes->left);
	int rightIncrement = getIncrement(heldTimes->right);

	if (setMax) {
		if (currentButtons->left)
			state->maxHealth -= state->maxHealth < leftIncrement ? state->maxHealth : leftIncrement;
		if (currentButtons->right)
			state->maxHealth +=
				state->maxHealth + rightIncrement > UINT16_MAX ? UINT16_MAX - state->maxHealth : rightIncrement;
	} else {
		if (currentButtons->left) state->health -= state->health < leftIncrement ? state->health : leftIncrement;
		if (currentButtons->right)
			state->health +=
				state->health + rightIncrement > state->maxHealth ? state->maxHealth - state->health : rightIncrement;
	}
}

static void clampValues(GameState *state) {
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

static void renderNormalModeUI(const GameState *state) {
	// if (state->maxHealth > 0) {
	drawSprite(0, 0, DNDLOGO_WIDTH, DNDLOGO_HEIGHT, DNDLOGO_DATA, false, true);
	drawHealthBar(state->health, state->maxHealth);
	LCD_refreshScr();
	// }

	char healthStr[16];
	snprintf(healthStr, sizeof(healthStr), "%d/%d", state->health, state->maxHealth);
	LCD_print(healthStr, UI_HEALTH_TEXT_X, UI_HEALTH_TEXT_Y);
}

static const char menuOptions[3][16] = {
	"Set Max Health",
	"Save to Flash",
	"Return",
};
static void renderMenuUI(const GameState *state) {

	drawSprite(0, 0, DNDLOGO_WIDTH, DNDLOGO_HEIGHT, DNDLOGO_DATA, false, true);
	// three squares to represent the menu options
	drawRectangle(10, 15, 15, 15, true, state->menuIndex == 0);
	// white inside if not selected
	if (state->menuIndex != 0) {
		drawRectangle(11, 16, 13, 13, false, true);
	}
	drawRectangle(30, 15, 15, 15, true, state->menuIndex == 1);
	if (state->menuIndex != 1) {
		drawRectangle(31, 16, 13, 13, false, true);
	}
	drawRectangle(50, 15, 15, 15, true, state->menuIndex == 2);
	if (state->menuIndex != 2) {
		drawRectangle(51, 16, 13, 13, false, true);
	}
	LCD_refreshScr();

	char line1[16];
	snprintf(line1, sizeof(line1), "Menu");
	LCD_printCenter(line1, 4, 5);

	char line2[16];
	snprintf(line2, sizeof(line2), menuOptions[state->menuIndex]);
	int length = strlen(menuOptions[state->menuIndex]);
	LCD_printCenter(line2, length, 4);
}

static void showIntroSequence() {
	LCD_clrScr();

	// center sprite
	int x = (LCD_WIDTH - DNDLOGO_WIDTH) / 2;
	int y = (LCD_HEIGHT - DNDLOGO_HEIGHT) / 2;

	drawSprite(x, y, DNDLOGO_WIDTH, DNDLOGO_HEIGHT, DNDLOGO_DATA, false, true);
	LCD_refreshScr();
	sleep_ms(1000);
	char line1[16];
	snprintf(line1, sizeof(line1), "D&D HP Tracker");
	LCD_printCenter(line1, 14, 5);
	sleep_ms(1000);
	LCD_clrScr();
	LCD_clrBuff();

	// move logo into corner
	for (int i = x + y; i >= 0; i--) {
		bool which = i % 2 == 0;
		drawSprite(i, i, DNDLOGO_WIDTH, DNDLOGO_HEIGHT, DNDLOGO_DATA, false, true);
		LCD_refreshScr();
		sleep_ms(40);
		LCD_clrScr();
	}

	LCD_clrScr();
}

void normalMode(GameState &state) {
	// Render current state
	renderNormalModeUI(&state);

	// Read buttons
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
		updateGameStateFromInput(&state, &heldTimes, &currentButtons, false);
		state.lastChangeTime = currentTime;
	}
	// Clamp values to valid ranges
	clampValues(&state);
}

void maxHealthMode(GameState &state) {
	// Render current state
	renderNormalModeUI(&state);
	char line1[16];
	snprintf(line1, sizeof(line1), "Set Max...");
	LCD_printCenter(line1, 10, 5);

	// Read buttons

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
		updateGameStateFromInput(&state, &heldTimes, &currentButtons, true);
		state.lastChangeTime = currentTime;
	}
	// Clamp values to valid ranges
	clampValues(&state);
}

void menuMode(GameState &state) {
	// Render current state
	renderMenuUI(&state);

	// Read buttons
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

	if (currentButtons.right) {
		state.menuIndex++;
		if (state.menuIndex > 2) {
			state.menuIndex = 0;
		}
	}
	if (currentButtons.left) {
		if (state.menuIndex == 0) {
			state.menuIndex = 2;
		} else {
			state.menuIndex--;
		}
	}
	if (currentButtons.ok) {
		if (state.menuIndex == 0) {
			state.mode = SetMaxHealth;
		} else if (state.menuIndex == 1) {
			saveStateToFlash(state);
			state.mode = Normal;
		} else {
			state.mode = Normal;
		}
		LCD_clrBuff();
		LCD_clrScr();
		resetHeldTimes(&heldTimes);
		currentButtons.ok = false;
		currentButtons.left = false;
		currentButtons.right = false;
		sleep_ms(500);
	}
	// Update game state based on current inputs
	if (currentButtons.left || currentButtons.right || currentButtons.ok) {
		state.lastChangeTime = currentTime;
	}
	resetHeldTimes(&heldTimes);
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

	// we are ready
	showIntroSequence();

	SaveData saveData;
	readStateFromFlash(&saveData);
	if (saveData.maxHealth == 0 && saveData.health == 0) {
		state.health = INITIAL_HEALTH;
		state.maxHealth = INITIAL_MAX_HEALTH;
	} else {
		state.health = saveData.health;
		state.maxHealth = saveData.maxHealth;
	}
	state.lastChangeTime = to_ms_since_boot(get_absolute_time());
	state.mode = Normal;

	while (true) {
		currentButtons = readButtonState();

		switch (state.mode) {
		case Normal:
			normalMode(state);
			break;
		case Menu:
			menuMode(state);
			break;
		case SetMaxHealth:
			maxHealthMode(state);
			break;
		}

		// Handle mode changes
		if (currentButtons.ok && state.mode != Menu) {
			// if (state.mode == Normal) {
			// 	state.mode = SetMaxHealth;
			// } else if (state.mode == SetMaxHealth) {
			// 	state.mode = Normal;
			// }
			state.mode = Menu;
			LCD_clrBuff();
			LCD_clrScr();
			sleep_ms(200);
			resetHeldTimes(&heldTimes);
		}

		// Delay to prevent busy loop
		sleep_ms(LOOP_DELAY_MS);
	}
} // memset(buffer, 0xFF, FLASH_PAGE_SIZE);
