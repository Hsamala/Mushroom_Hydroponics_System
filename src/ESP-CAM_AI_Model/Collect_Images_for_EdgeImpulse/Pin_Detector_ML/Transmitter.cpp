#include "Transmitter.h"

void onEspNowSend(const esp_now_send_info_t *txInfo, esp_now_send_status_t status) {
  Serial.print("ESP-NOW send status: ");

  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("SUCCESS");
  } else {
    Serial.println("FAILED");
  }
}

bool setupEspNowTransmitter() { 
  Serial.print("Transmitter STA MAC: ");
  Serial.println(WiFi.macAddress());

  Serial.print("WiFi channel: ");
  Serial.println(WiFi.channel());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return false;
  }

  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  peerInfo.channel = WiFi.channel();
  peerInfo.encrypt = false;
  // register first peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

}

bool sendPinsDetected(bool pinsDetected) {
  PinDetectionMessage msg;
  msg.pinsDetected = pinsDetected;

  esp_err_t result = esp_now_send(receiverMac, (uint8_t *) &msg, sizeof(msg));

  if (result == ESP_OK) {
    Serial.print("Sent pinsDetected = ");
    Serial.println(pinsDetected ? "true" : "false");
    return true;
  } else {
    Serial.print("esp_now_send failed with error: ");
    Serial.println(result);
    return false;
  }
}