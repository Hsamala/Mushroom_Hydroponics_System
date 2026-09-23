//------ BLYNK IMPORTANT --- 
#define BLYNK_PRINT Serial 
#define BLYNK_TEMPLATE_ID           "TMPL28D24NlX3"
#define BLYNK_TEMPLATE_NAME         "Mushroom Hydroponics Chamber"
#define BLYNK_AUTH_TOKEN            "D90PEM_WTvVfeE_OT91LTXlH85fYwN8k"


// ---------- NETWORK INFORMATIOn ------------
char ssid[] = "Green2026";
char pass[] = "12345678";
#define HOSTNAME "esp32control"
IPAddress LOCAL_IP(10, 210, 249, 51);
IPAddress GATEWAY(10, 210, 248, 1);
IPAddress SUBNET(255, 255, 248, 0);
IPAddress PRIMARY_DNS(8, 8, 8, 8);
IPAddress SECONDARY_DNS(8, 8, 4, 4);

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>
#include <BlynkSimpleEsp32.h>

#include "OLEDSetup.h"
#include "MultiGasSensor.h"
#include "RelayControl.h"

// -- DEBUG MODE: Remove Before Flight -- 	     	      <
bool isDebugMode = false;

// --- PIN DEFINITIONS ---
#define SOIL_PIN   8  // Analog pin for the soil sensor
#define I2C_SDA    6  // I2C SDA pin
#define I2C_SCL    7  // I2C SCL pin

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
WebServer otaServer(8080);


//--- GLOBAL sensor values ---
uint16_t co2Level = 0;
float temperature = 0.0;
float humidity = 0.0;
int soilMoisture = 0, state = 0;

// -- Relays Turned On ---			     	      <
bool isFanOn, isMisterOn;

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
  syncBlynk(isFanOn, isMisterOn);
}

BLYNK_WRITE(V10) { if(isDebugMode) co2Level = param.asInt(); }
BLYNK_WRITE(V11) { if(isDebugMode) temperature = param.asFloat(); }
BLYNK_WRITE(V12) { if(isDebugMode) humidity = param.asFloat(); }

void OLEDStateMachine() {
switch(state) {
        case 0:
          if (!isDebugMode) co2Level = readCO2(Serial);
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
          if (!isDebugMode) temperature = readTemp(Serial);
          drawTemperature(temperature);
          Serial.println(temperature);
          break;
        case 2:
          humidity = readHumidity(Serial);
          drawHumidity(humidity);
          break;
        case 3:
          if (!isDebugMode) soilMoisture = readSoilSensor(Serial);
          drawSoilMoisture(soilMoisture);
          break;
  }
  state = (state + 1) % 4;
}

void syncBlynk(bool fan, bool mister) {			      
  Blynk.virtualWrite(V0, fan);				      
  Blynk.virtualWrite(V1, mister);			      
}

void setup() {
  // 1. Initialize the I2C bus with your custom pins
  if (Wire.begin(I2C_SDA, I2C_SCL)) {
    Serial.println("I2C bus initialized successfully!");
  } else {
    Serial.println("I2C initialization failed. Check your pin assignments.");
    for(;;);
  }
  delay(200);

  if(!setupDisplay(Wire)) {
    for(;;);
  }

  //2. Intialize the screen
  char confirmScreen[] = "Screen is on and ready to rock :)!";
  printScreen(confirmScreen);
  delay(500);

  //3. Setup the relays
  setupRelays();
  char relayConfirm[] = "Relays are set up...";
  printScreen(relayConfirm);
  delay(500);

  //4. Intialize the C02 and Temperature, and Humidity sensor.
  if(!sensorSetup(Wire, Serial)) {
    char sensorError[] = "Sensor Error! Please check sensor wiring, especially on the I2C bus.";
    printScreen(sensorError);
    for(;;);
  }
  
  char confirmSensors[] = "Sensors are ready :)";
  printScreen(confirmSensors);
  delay(500);

  WiFi.setHostname(HOSTNAME);
  WiFi.mode(WIFI_STA);

  if (!WiFi.config(LOCAL_IP, GATEWAY, SUBNET, PRIMARY_DNS, SECONDARY_DNS)) {
    char ipError[] = "Static IP configuration failed";
    printScreen(ipError);
    for(;;);
  }

  // Connect to WiFi
  while (!WiFi.begin(ssid, pass)) {
    char wifiError[] = "Cannot connect to wifi and create a static IP Address";
    printScreen(wifiError);
    for(;;);
  }
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  if(!Blynk.connected()) {
    char errorBlynk[] = "ERROR: Blynk connection not working!";
    printScreen(errorBlynk);
    for(;;);
  }

  char BlynkOk[] = "Connection to Blynk is stable :)";
  printScreen(BlynkOk);

  ElegantOTA.begin(&otaServer);
  otaServer.begin();

  timer.setInterval(10000L, OLEDStateMachine);
  timer.setInterval(11000L, relayControl);
  timer.setInterval(241000L, sendData2Blynky);

  delay(2000);
}

void loop() {
  otaServer.handleClient();
  Blynk.run();
  timer.run();
  ElegantOTA.loop();
}