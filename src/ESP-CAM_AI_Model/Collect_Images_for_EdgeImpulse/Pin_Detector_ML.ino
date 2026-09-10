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

#define WIFI_SSID "Green2026"
#define WIFI_PASS "12345678"
#define HOSTNAME "esp32cam"

#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/extra/esp32/wifi/sta.h>
#include <eloquent_esp32cam/viz/image_collection.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ElegantOTA.h>

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
}

void loop() {
    // Handle OTA server requests
    otaServer.handleClient();

    // Required by ElegantOTA
    ElegantOTA.loop();

    Serial.println("OTA update test...");
    delay(1000);

}