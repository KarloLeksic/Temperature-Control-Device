// Include libraries for display
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// Button library
#include "OneButton.h"

// Temperature sensor liibrary
#include "OneWire.h"
#include "DallasTemperature.h"

// Include defines
#include "defines.h"

// Include fonts
#include "Fonts/FreeSans7pt7b.h"
#include "Fonts/FreeSans16pt7b.h"

// Include icons
#include "icons.h"

// Object for the screen
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Object for the button
OneButton button(BTN_PIN, true);

// Objects for temperature sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);
DeviceAddress addr;

// Variables for reading rotary encoder
int counter;
int state;
int lastState;

// For refreshing screen interval
uint64_t lastRefresh;

// Menu variables
bool menu;

// Parameters for regulation in degrees celsious
float currentTemp, tempOffset = 0.1, wantedTemp = 69;

void setup()
{
  // Init display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    // If there is an error, blink the LED
    pinMode(ERROR_LED_PIN, OUTPUT);
    while (1)
    {
      digitalWrite(ERROR_LED_PIN, HIGH);
      delay(200);
      digitalWrite(ERROR_LED_PIN, LOW);
      delay(200);
    }
  }

  // Set relay pin as output
  pinMode(RELAY_PIN, OUTPUT);

  // Set rotary encoder pins
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DATA_PIN, INPUT_PULLUP);

  // Button settings
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), checkTicks, CHANGE);
  button.attachClick(singleClick);

  // Init temp sensor
  tempSensor.begin();
  tempSensor.getAddress(addr, 0);

  // Read initial rotary encoder state
  lastState = digitalRead(CLK_PIN);

  // Initial display settings
  display.clearDisplay();
  display.setTextColor(WHITE);

  // Initial screen
  splashCsreen();
}

void loop()
{
  // Measure temperature and refresh screen every REFRESH_SCREEN_INTERVAL ms
  if ((unsigned long)(millis() - lastRefresh) > REFRESH_SCREEN_INTERVAL)
  {
    // Measure current temperature
    currentTemp = readTemperature();

    // Do regulation
    tempRegulation();

    // Draw screen with current temperature
    mainScreen();
    lastRefresh = millis();
  }

  // Read button state
  button.tick();

  // Enter the menu on menu flag
  while (menu)
  {
    readRotaryState();

    // Refresh screen every 200 ms
    if ((unsigned long)(millis() - lastRefresh) > 200)
    {
      setTemperatureScreen();
      lastRefresh = millis();
    }

    button.tick();
  }
}

// Turning on and off the relay for regulation
void tempRegulation()
{
  if (currentTemp > wantedTemp + tempOffset)
  {
    // Turn off the relay
    digitalWrite(RELAY_PIN, LOW);
  }
  else if (currentTemp < wantedTemp - tempOffset)
  {
    // Turn on the relay
    digitalWrite(RELAY_PIN, HIGH);
  }
}

// Button tick on interrupt
void checkTicks()
{
  button.tick();
}

// Function for single click
void singleClick()
{
  // Chagne manu flag on click
  menu = !menu;
}

// Check if the rotary is rotated
void readRotaryState()
{
  // Read clock state
  state = digitalRead(CLK_PIN);

  // If change occured
  if (state != lastState)
  {
    // Check direction
    if (digitalRead(DATA_PIN) != state)
    {
      // Increase temperature
      wantedTemp = wantedTemp + SET_TEMPERATURE_STEP;

      if (wantedTemp > MAX_TEMP)
      {
        wantedTemp = MAX_TEMP;
      }
    }
    else
    {
      // Decrease temoerature
      wantedTemp = wantedTemp - SET_TEMPERATURE_STEP;

      if (wantedTemp < MIN_TEMP)
      {
        wantedTemp = MIN_TEMP;
      }
    }

  }
  lastState = state;
}

// Main screen shows current temperature
void mainScreen()
{
  // Clear all content on the screen
  display.clearDisplay();

  // Draw temperature logo and current temperature
  display.drawBitmap(0, 9, thermometer_icon, 32, 32, 1);
  display.setFont(&FreeSans16pt7b);
  display.setCursor(35, 35);
  display.print(currentTemp, 1);
  display.print("C");

  // Draw set temperature
  display.setFont(&FreeSans7pt7b);
  display.setCursor(10, 60);
  display.print("Set temp.: ");
  display.print(wantedTemp, 1);
  display.print("C");

  // Display all on the screen
  display.display();
}

void setTemperatureScreen()
{
  // Clear all content on the screen
  display.clearDisplay();

  // Draw menu for selecting temperature
  display.setFont(&FreeSans7pt7b);
  display.setCursor(10, 20);
  display.print("Set temperature:");

  display.setFont(&FreeSans16pt7b);
  display.setCursor(20, 55);
  display.print(wantedTemp, 1);
  display.print(" C");

  // Display all on the screen
  display.display();
}

// Read temperature from sensor
float readTemperature()
{
  tempSensor.requestTemperatures();
  return tempSensor.getTempC(addr);
}

// Initial screen image
void splashCsreen()
{
  // Clear all content on the screen
  display.clearDisplay();

  // Draw beer icon
  display.drawBitmap(32, 0, beerIcon, 64, 64, 1);
  display.display();

  // A little delay before start
  delay(SPLASH_SCREEN_DURATION);
}
