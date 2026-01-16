#ifndef RelayControl_H
#define RelayControl_H

#include <Arduino.h>

#define FAN_RELAY_PIN 9 
#define MISTER_PIN 8
#define RELAY_ON  LOW
#define RELAY_OFF HIGH

extern uint16_t co2Level;
extern float temperature, humidity;
extern int soilMoisture;
extern bool isFanOn, isMisterOn;

void setupRelays();
template <typename T>
int calc_withinRange(T currentReading, T targetReading, T margin); 
void relayControl();

#endif