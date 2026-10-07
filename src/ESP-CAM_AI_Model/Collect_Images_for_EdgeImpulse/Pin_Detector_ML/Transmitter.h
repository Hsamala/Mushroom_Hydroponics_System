#ifndef Transmitter_H
#define Transmitter_H

#include <WiFi.h>
#include <esp_now.h>
#include <Arduino.h>

extern uint8_t receiverMac[];

typedef struct {
  bool pinsDetected;
} PinDetectionMessage;

bool setupEspNowTransmitter();
bool sendPinsDetected(bool pinsDetected);

#endif