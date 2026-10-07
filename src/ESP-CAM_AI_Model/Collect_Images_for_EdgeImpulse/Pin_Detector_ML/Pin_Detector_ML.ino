// ---------- NETWORK INFORMATIOn ------------
#define WIFI_SSID "Green2026"
#define WIFI_PASS "12345678"
#define HOSTNAME "esp32cam"

#define FLASH_PIN 4

// ---------- STATIC IP SETTINGS ------------
IPAddress LOCAL_IP(10, 210, 249, 50);
IPAddress GATEWAY(10, 210, 248, 1);
IPAddress SUBNET(255, 255, 248, 0);
IPAddress PRIMARY_DNS(8, 8, 8, 8);
IPAddress SECONDARY_DNS(8, 8, 4, 4);

#include <eloquent_esp32cam.h>
//#include <eloquent_esp32cam/extra/esp32/wifi/sta.h>
//#include <eloquent_esp32cam/viz/image_collection.h>

#include "Transmitter.h"
#include <WebServer.h>
#include <ElegantOTA.h>

using eloq::camera;
//using eloq::wifi;
//using eloq::viz::collectionServer;

// Use a separate port for OTA so it does not conflict
// with the image collection server.
WebServer otaServer(8080);

bool pinDetectedState = false;
unsigned long lastPinDetectionToggleMs = 0;
const unsigned long PIN_DETECTION_TOGGLE_INTERVAL_MS = 5000;

void updatePinDetectionToggle() {
  unsigned long now = millis();
  if (now - lastPinDetectionToggleMs < PIN_DETECTION_TOGGLE_INTERVAL_MS) {
    return;
  }
  

  lastPinDetectionToggleMs = now;
  pinDetectedState = !pinDetectedState;
}

void setup() {
  delay(3000);

  Serial.begin(115200);
  Serial.println();
  Serial.println("___IMAGE COLLECTION SERVER WITH OTA___");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  WiFi.setHostname(HOSTNAME);
  if (!WiFi.config(LOCAL_IP, GATEWAY, SUBNET, PRIMARY_DNS, SECONDARY_DNS)) {
        Serial.println("Static IP configuration failed");
  }

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  Serial.print("Connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("WiFi channel after connect: ");
  Serial.println(WiFi.channel());

  setupEspNowTransmitter();

  /* Connect to WiFi
  while (!wifi.connect().isOk()) {
    Serial.println(wifi.exception.toString());
    delay(1000);
  } */

  // Start OTA server on port 8080
  ElegantOTA.begin(&otaServer);
  otaServer.begin();


  // Camera settings
  camera.pinout.aithinker();
  camera.brownout.disable();
  camera.resolution.face();
  camera.quality.high();

  // Initialize camera
  while (!camera.begin().isOk()) {
    Serial.println(camera.exception.toString());
    delay(1000);
  }

  // Start image collection server
  //while (!collectionServer.begin().isOk()) {
  //  Serial.println(collectionServer.exception.toString());
  //  delay(1000);
  //}

  Serial.println("Camera OK");
  Serial.println("WiFi OK");
  //Serial.println("Image Collection Server OK");
  Serial.println(pinDetectedState);

}

void loop() {
  static bool testPinsDetected = false;
  //Serial.println(pinDetectedState);

  /*----- ESP-NOW Test Baseline ------ */
  /*
  static unsigned long lastSendMs = 0;

  if (millis() - lastSendMs >= 5000) {
    lastSendMs = millis();

    testPinsDetected = !testPinsDetected;
    sendPinsDetected(testPinsDetected);
    Serial.print("Sent Pins Detected: ");
    Serial.println(testPinsDetected);
  } */

  // Required by ElegantOTA & Handle OTA server requests
  otaServer.handleClient();
  ElegantOTA.loop();

  delay(10);
}
