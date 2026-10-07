#ifndef Receiver_H
#define Receiver_H

#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "OLEDSetup.h"
#include "RelayControl.h"

typedef struct {
  bool pinsDetected;
} PinDetectionMessage;

extern bool newPinDetectionMessage;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len);
bool setupEspNowReceiver();

#endif