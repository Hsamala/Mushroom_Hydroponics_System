#include "Receiver.h"

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len != sizeof(PinDetectionMessage)) {
    char errorScreen[] = "Wrong message size";
    printScreen(errorScreen);
    return;
  }

  PinDetectionMessage msg;
  memcpy(&msg, incomingData, sizeof(msg));

  pinsDetected = msg.pinsDetected;
  newPinDetectionMessage = true;

  /* Maybe find you can write to Blynk V2 pin later on...*/
  //Serial.print("ESP-NOW received pinsDetected = ");
  //Serial.println(pinsDetected ? "true" : "false");
  if (newPinDetectionMessage) {
    newPinDetectionMessage = false;
    char pinStatus[32];
    snprintf(pinStatus, sizeof(pinStatus), "Pins detected:\n%s", pinsDetected ? "YES" : "NO");
    printScreen(pinStatus);
  } 
}

bool setupEspNowReceiver() {

  if (esp_now_init() != ESP_OK) {
    char errorScreen[] = "ESPNow Init Failed";
    printScreen(errorScreen);
    return false;
  }

  if (esp_now_register_recv_cb(OnDataRecv) != ESP_OK) {
    char errorScreen[] = "ESP-NOW receive callback registration failed";
    printScreen(errorScreen);
    return false;
  }

  return true;

}