#include <Adafruit_NeoPixel.h>

#define LED_PIN 6  // Pin to which the LEDs are connected
#define LED_COUNT 60  // Number of LEDs

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int currentR = 0, currentG = 255, currentB = 0, currentBright = 127;
bool RAINBOW_ON = false;

// Initialize serial communication and LED strip
void setup()
{
  Serial.begin(9600);
  strip.begin();
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(currentR, currentG, currentB));
    strip.setBrightness(currentBright);
  }
  strip.show();
}

// Main loop to read serial commands and update LED modes
void loop()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    if (command == "A")
    {
      Serial.println("Z");
    }
    else
    {
      handleCommand(command);
    }
  }

  if (RAINBOW_ON == true) rainbow(50);
}

// Handle incoming serial commands to control LEDs
void handleCommand(String command)
{
  if (command.startsWith("SET_COLOR"))
  {
    int r, g, b;
    sscanf(command.c_str(), "SET_COLOR %d,%d,%d", &r, &g, &b);
    currentR = r;
    currentG = g;
    currentB = b;
    setColor(currentR, currentG, currentB);
    RAINBOW_ON = false;
  }
  else if (command.startsWith("SET_BRIGHTNESS"))
  {
    int b;
    sscanf(command.c_str(), "SET_BRIGHTNESS %d", &b);
    currentBright = b;
    setBrightness(currentBright);
  }
  else if (command == "TURN_OFF")
  {
    setColor(0, 0, 0);
    RAINBOW_ON = false;
  }
  else if (command == "GET_COLOR")
  {
    getColor(command);
  }
  else if (command == "RAINBOW")
  {
    RAINBOW_ON = true;
  }
}

// Set brightness of the LED strip
void setBrightness(int b)
{
  strip.setBrightness(b);
  strip.show();
}

// Set color of the LEDs
void setColor(int r, int g, int b)
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

// Get the current color and brightness of the LEDs
void getColor(String command)
{
  String response = String(currentR) + "," + String(currentG) + "," + String(currentB) + "," + String(currentBright);
  Serial.println(response);
}

// Display a rainbow animation on the LED strip
void rainbow(uint8_t wait) {
  static uint16_t j = 0;
  static uint16_t i = 0;
  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= wait)
  {
    previousMillis = millis();
    for (i = 0; i < strip.numPixels(); i++)
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    j++;
    if (j >= 256 * 5)
    {
      j = 0;
    }
  }
}

// Helper function to generate rainbow colors across 0-255 positions
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}