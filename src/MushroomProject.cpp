#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AHTX0.h>

// --- PIN DEFINITIONS ---
#define SOIL_PIN   7  // Analog pin for the soil sensor
#define I2C_SDA    8  // I2C SDA pin
#define I2C_SCL    9  // I2C SCL pin

// --- SENSOR CALIBRATION (IMPORTANT!) ---
// You must find these values yourself for accurate readings
// 1. Upload this code and check the Serial Monitor for the "Raw Value".
// 2. Put the sensor in bone-dry soil and note the "Raw Value". This is your DRY_VALUE.
// 3. Submerge the sensor in a glass of water. Note the "Raw Value". This is your WET_VALUE.
// 4. Update the values below.
const int DRY_VALUE = 3300; // Placeholder: ESP32 raw value in dry soil (approx.)
const int WET_VALUE = 900; // Placeholder: ESP32 raw value in water (approx.)

// --- OLED SCREEN CONFIGURATION ---
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

// --- OBJECT CREATION ---
// Create an OLED display object
// The I2C address 0x3C is the most common for 128x64 displays
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // 1. Start the Serial Monitor for debugging
  Serial.begin(115200);
  while (!Serial); // Wait for native USB serial to connect
  Serial.println("System booting up... Soil Moisture Test.");

  // 2. Initialize the I2C bus with your custom pins
  // This MUST be called before display.begin() on an ESP32
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("I2C Bus Initialized.");

  // 3. Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed. Check wiring or address."));
    while (true); // If it fails, loop forever and do nothing
  }
  Serial.println("OLED Initialized.");

  // 4. Show a startup message on the OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Soil Sensor Test");
  display.println("Reading value...");
  display.display();
  delay(2000);
}

void loop() {
  // 1. Read the raw analog value from the soil sensor
  int soilRaw = analogRead(SOIL_PIN);

  // 2. Map the raw value to a 0-100% scale
  // We use constrain() to make sure the value doesn't go below 0% or above 100%
  int soilPercent = map(soilRaw, DRY_VALUE, WET_VALUE, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  // 3. Print the values to the Serial Monitor
  Serial.print("Raw Value: ");
  Serial.print(soilRaw);
  Serial.print("  |  Percentage: ");
  Serial.print(soilPercent);
  Serial.println("%");

  // 4. Format and Print to the OLED
  display.clearDisplay();
  display.setTextColor(WHITE);

  // Print a title
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Soil:");

  // Print the percentage in a large font
  display.setTextSize(3);
  display.setCursor(20, 25);
  display.print(soilPercent);
  display.print(" %");

  // 5. Show the new data on the screen
  display.display();

  // Wait 1 second before the next reading
  delay(1000);
}