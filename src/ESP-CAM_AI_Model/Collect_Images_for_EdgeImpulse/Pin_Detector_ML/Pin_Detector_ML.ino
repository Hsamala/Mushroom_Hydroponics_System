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

#include "Transmitter.h"
#include "CV_Model.h"

bool pinDetectedState = false;

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
  if(runModelInference) {
    Serial.println("Detected Anomoly");
  }

  delay(10);
}
