///////////// SCREEN SETTINGS ////////////
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1 // Not used
#define SCREEN_ADDRESS 0x3C

///////////// CONTROL SETTINGS ///////////
#define MIN_TEMP 60
#define MAX_TEMP 80

// Increase and decrease temperature step
#define SET_TEMPERATURE_STEP 0.1

////////////////// PINS //////////////////
// Rotary encoder pins
#define CLK_PIN  8
#define DATA_PIN 9
#define BTN_PIN  2

// Temperature sensor pin
#define ONE_WIRE_BUS 4

// Relay pin
#define RELAY_PIN 5

// Pin for error LED
#define ERROR_LED_PIN 13

/////////////// INTERVALS ////////////////
// Refresh screen interval in milliseconds
#define REFRESH_SCREEN_INTERVAL 500

// Duration for splashscreen
#define SPLASH_SCREEN_DURATION 2000
