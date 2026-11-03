// ---------- NETWORK INFORMATIOn ------------
#define WIFI_SSID "Velocity Wi-Fi"
#define WIFI_PASS "pywihldg"


#include "Transmitter.h"
#include "CV_Model.h"

bool pinDetectedState = false;

void setup() {
  delay(3000);

  Serial.begin(115200);
  Serial.println();
  Serial.println("___IMAGE COLLECTION SERVER WITH OTA___");

  WiFi.begin(WIFI_SSID, WIFI_PASS);

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
  Serial.println("ESP-NOW OK");

  if (ei_camera_init() == false) {
        Serial.println("Failed to initialize Camera!\r\n");
    }
    else {
        Serial.println("Camera initialized\r\n");
  }

  Serial.println("\nStarting anomoly detection in 2 seconds...\n");
  delay(2000);

}

void loop() {
  bool pinsDetected = runModelInference();
  if (pinsDetected) {
    Serial.println("Detected Anomaly / Pins!");
  }
  else {
    Serial.println("No anomaly detected");
  }
  delay(1000);
}