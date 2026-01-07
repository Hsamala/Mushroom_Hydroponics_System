#include <Wire.h>

#include "OLEDSetup.h"
#include "MultiGasSensor.h"

// --- PIN DEFINITIONS ---
#define SOIL_PIN   0  // Analog pin for the soil sensor
#define I2C_SDA    5  // I2C SDA pin
#define I2C_SCL    6  // I2C SCL pin

// --- SENSOR CALIBRATION (IMPORTANT!) ---
// You must find these values yourself for accurate readings
// 1. Upload this code and check the Serial Monitor for the "Raw Value".
// 2. Put the sensor in bone-dry soil and note the "Raw Value". This is your DRY_VALUE.
// 3. Submerge the sensor in a glass of water. Note the "Raw Value". This is your WET_VALUE.
// 4. Update the values below.
const int DRY_VALUE = 3300; // Placeholder: ESP32 raw value in dry soil (approx.)
const int WET_VALUE = 1300; // Placeholder: ESP32 raw value in water (approx.)


// --- Countdown and States for OLED Display --- 
int countdownSeconds = 60;
unsigned long previousMillis = 0;
const long interval = 1000;
int state = 0;

byte scanI2CBus() {
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning I2C bus...");

  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Wire.endTransmission to see if
    // a device acknowledged the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("Scan finished.\n");
  }
  
  return nDevices;
}

void setup() {
  // 1. Start the Serial Monitor for debugging
  Serial.begin(115200); // Wait for native USB serial to connect
  Serial.println("System booting up... Soil Moisture Test.");

  // 2. Initialize the I2C bus with your custom pins
  // This MUST be called before display.begin() on an ESP32
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.println("I2C Bus Initialized.");

  delay(200);
  scanI2CBus();

  //3. Intialize the C02 and Temperature, and Humidity sensor.
  if(!sensorSetup(Wire, Serial)) {
    for(;;);
  }
  Serial.println("Onto OLED display setup");
  // 4. Initialize the OLED display
  if(!setupDisplay(Wire)) {
    for(;;);
  }

  // 5. Show a startup message on the OLED
  delay(2000);
}

void loop() {

  unsigned long currentMillies = millis();
  uint16_t co2Level;
  float temperature, humidity;
  int soilMoisture;

  if(currentMillies - previousMillis >= interval) {
    previousMillis = currentMillies;

    if(countdownSeconds > 0) {
      countdownSeconds -= 10;

      switch(state) {
        case 0:
          co2Level = readCO2(Serial);
          Serial.print("CO2 Level:");
          Serial.println(co2Level);
          if(co2Level == 1001) {
            drawWarmupCo2();
            break;
          } else if(co2Level == 1002) {
            drawInitCo2();
            break;
          } else if(co2Level == 1003) {
            invalidCo2();
            break;
          } else {
            drawCO2(co2Level);
            break;
          } 
        case 1:
          temperature = readTemp(Serial);
          drawTemperature(temperature);
          Serial.print("Temp: ");
          Serial.println(temperature);
          break;
        case 2:
          humidity = readHumidity(Serial);
          drawHumidity(humidity);
          Serial.print("Humidity: ");
          Serial.println(humidity);
          break;
        case 3:
          soilMoisture = readSoilSensor(Serial);
          Serial.print("Moisture: ");
          Serial.println(soilMoisture);
          drawSoilMoisture(soilMoisture);
          
          break;
      }
    } else {
        countdownSeconds = 60;
        previousMillis = 0;
        switch(state) {
          case 0: 
            state = 1;
            break;
          case 1:
            state = 2;
            break;
          case 2:
            state = 3;
            break;
          case 3:
            state = 0;
            break;
        }
    }
  }

  /* 
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
  */

  delay(2000);
}