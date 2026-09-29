// ---------- BLYNK INFORMATION -------------
#define BLYNK_PRINT Serial 
#define BLYNK_TEMPLATE_ID "TMPL28D24NlX3"
#define BLYNK_TEMPLATE_NAME "Mushroom Hydroponics Chamber"
#define BLYNK_AUTH_TOKEN "yAs7jPlP9J0zwGTFs3Bq_yVSh7Ryh06Y"

// ---------- NETWORK INFORMATIOn ------------
#define WIFI_SSID "Green2026"
#define WIFI_PASS "12345678"
#define HOSTNAME "esp32cam"

#define FLASH_PIN 4

#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/esp32/wifi/sta.h>
#include <eloquent_esp32cam/viz/image_collection.h>
#include "AI_Detection_Logic.h"

#include <WiFi.h>
#include <WebServer.h>
#include <ElegantOTA.h>


// ---------- STATIC IP SETTINGS ------------
IPAddress LOCAL_IP(10, 210, 249, 50);
IPAddress GATEWAY(10, 210, 248, 1);
IPAddress SUBNET(255, 255, 248, 0);
IPAddress PRIMARY_DNS(8, 8, 8, 8);
IPAddress SECONDARY_DNS(8, 8, 4, 4);

using eloq::camera;
using eloq::wifi;
using eloq::viz::collectionServer;

// Use a separate port for OTA so it does not conflict
// with the image collection server.
WebServer otaServer(8080);
BlynkTimer timer;

void setup() {
  delay(3000);

  Serial.begin(115200);
  Serial.println();
  Serial.println("___IMAGE COLLECTION SERVER WITH OTA___");

  WiFi.setHostname(HOSTNAME);
  WiFi.mode(WIFI_STA);

   if (!WiFi.config(LOCAL_IP, GATEWAY, SUBNET, PRIMARY_DNS, SECONDARY_DNS)) {
        Serial.println("Static IP configuration failed");
   }

  // Connect to WiFi
  while (!wifi.connect().isOk()) {
    Serial.println(wifi.exception.toString());
    delay(1000);
  }

  // Start OTA server on port 8080
  ElegantOTA.begin(&otaServer);
  otaServer.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

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
  while (!collectionServer.begin().isOk()) {
    Serial.println(collectionServer.exception.toString());
    delay(1000);
  }

  Serial.println("Camera OK");
  Serial.println("WiFi OK");
  Serial.println("Image Collection Server OK");

}

void loop() {
  // Handle OTA server requests
  otaServer.handleClient();

  // Required by ElegantOTA
  ElegantOTA.loop();

  delay(2000);
}