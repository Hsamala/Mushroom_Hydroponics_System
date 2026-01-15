//------ BLYNK IMPORTANT --- 
#define BLYNK_PRINT Serial 
#define BLYNK_TEMPLATE_ID           "TMPL28D24NlX3"
#define BLYNK_TEMPLATE_NAME         "Mushroom Hydroponics Chamber"
#define BLYNK_AUTH_TOKEN            "D90PEM_WTvVfeE_OT91LTXlH85fYwN8k"
char ssid[] = "HHESS";
char pass[] = "!";

#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

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
// 5 . Also adjust on Blinkly for correct logging
const int DRY_VALUE = 3300; // Placeholder: ESP32 raw value in dry soil (approx.)
const int WET_VALUE = 1300; // Placeholder: ESP32 raw value in water (approx.)


BlynkTimer timer;

//--- GLOBAL sensor values ---
uint16_t co2Level;
float temperature, humidity;
int soilMoisture, state = 0;

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


void sendData2Blynky() {
  Blynk.virtualWrite(V3, co2Level);
  Blynk.virtualWrite(V4, temperature);
  Blynk.virtualWrite(V5, humidity);
  Blynk.virtualWrite(V6, soilMoisture);
}

void OLEDStateMachine() {
  switch(state) {
        case 0:
          co2Level = readCO2(Serial);
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
          Serial.println(temperature);
          break;
        case 2:
          humidity = readHumidity(Serial);
          drawHumidity(humidity);
          break;
        case 3:
          soilMoisture = readSoilSensor(Serial);
          drawSoilMoisture(soilMoisture);
          break;
  }
  state = (state + 1) % 4;
}

void setup() {
  // 1. Start the Serial Monitor for debugging
  Serial.begin(115200); // Wait for native USB serial to connect
  // 2. Initialize the I2C bus with your custom pins
  Wire.begin(I2C_SDA, I2C_SCL);
  delay(200);

  Serial.println("Starting program....");

  if(!setupDisplay(Wire)) {
    for(;;);
  }
  char confirmScreen[] = "Screen is on and ready to rock :)!";
  printScreen(confirmScreen);

  //3. Intialize the C02 and Temperature, and Humidity sensor.
  if(!sensorSetup(Wire, Serial)) {
    char sensorError[] = "Sensor Error! Please check sensor wiring, especially on the I2C bus.";
    printScreen(sensorError);
    for(;;);
  }
  
  char confirmSensors[] = "Sensors are ready :)";
  printScreen(confirmSensors);
  // 4. Initialize the OLED display
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  if(!Blynk.connected()) {
    char errorBlynk[] = "ERROR: Blynk connection not working!";
    printScreen(errorBlynk);
    for(;;);
  }
  char BlynkOk[] = "Connection to Blynk is stable :)";
  printScreen(BlynkOk);

  timer.setInterval(10000L, OLEDStateMachine);
  timer.setInterval(241000L, sendData2Blynky);

  delay(2000);
}

void loop() {
  Blynk.run();
  timer.run();
}