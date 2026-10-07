#ifndef Transmitter_H
#define Transmitter_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Arduino.h>

extern uint8_t receiverMac[];

typedef struct {
  bool pinsDetected;
} PinDetectionMessage;

void onEspNowSend(const esp_now_send_info_t *txInfo, esp_now_send_status_t status);
bool setupEspNowTransmitter();
bool sendPinsDetected(bool pinsDetected);

#endif