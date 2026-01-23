/**
 * =====================================================================
 * MINIMAL SOIL SENSOR TEST (for ESP32-S3)
 * =====================================================================
 * This sketch does only one thing: It reads the analog value from
 * your custom soil sensor and prints the raw 12-bit value (0-4095)
 * to the Serial Monitor.
 *
 * This proves if your chip, wiring, and config are working.
 */

// 1. We must always include Arduino.h in a .cpp file
#include <Arduino.h>

// 2. Define the pin your sensor is connected to.
// Based on your diagram.json, chip1:AOUT connects to esp:7 (GPIO 7).
#define SOIL_PIN A0 

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);

  // You can optionally configure the ADC for 12-bit resolution,
  // though it's often the default on S3. This is good practice.
  // We're reading from ADC1.
  // analogSetAttenuation(ADC_11db); // Sets the range to ~3.3V
}

void loop() {
  // 1. Read the raw analog value from the sensor pin
  // This will be a number between 0 and 4095.
  int rawValue = analogRead(SOIL_PIN);

  // 2. Print the value to the Serial Monitor
  Serial.print("Raw Analog Value: ");
  Serial.println(rawValue);

  // 3. Wait for 1 second before the next reading
  delay(1000);
}
