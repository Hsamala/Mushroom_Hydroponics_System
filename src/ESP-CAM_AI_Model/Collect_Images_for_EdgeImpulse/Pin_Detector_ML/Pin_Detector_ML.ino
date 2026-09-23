/**
 * Collect images for Edge Impulse image
 * classification / object detection
 *
 * BE SURE TO SET "TOOLS > CORE DEBUG LEVEL = INFO"
 * to turn on debug messages
 *
 * Image Collection Server:
 *   http://ESP32_IP/
 *
 * OTA Update Page:
 *   http://ESP32_IP:8080/update
 */

// ---------- NETWORK INFORMATIOn ------------
#define WIFI_SSID "Green2026"
#define WIFI_PASS "12345678"
#define HOSTNAME "esp32cam"

#define FLASH_PIN 4

#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/esp32/wifi/sta.h>
#include <eloquent_esp32cam/viz/image_collection.h>

#include <WiFi.h>
#include <WiFiClient.h>
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

  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(WiFi.getHostname());

  // Start OTA server on port 8080
  ElegantOTA.begin(&otaServer);
  otaServer.begin();

  Serial.println("OTA server started");
  Serial.print("OTA update URL: http://");
  Serial.print(WiFi.localIP());
  Serial.println(":8080/update");

  // Camera settings
  camera.pinout.aithinker();
  camera.brownout.disable();

  // Edge Impulse models work on square images.
  // Face resolution is 240x240.
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
  Serial.println(collectionServer.address());

    pinMode(FLASH_PIN, OUTPUT);
    digitalWrite(FLASH_PIN, LOW);   // turn flash ON
}

void loop() {
  // Handle OTA server requests
  otaServer.handleClient();

  // Required by ElegantOTA
  ElegantOTA.loop();

  Serial.println("OTA test Herald...");
  delay(2000);
}