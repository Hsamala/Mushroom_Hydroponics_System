#ifndef RelayControl_H
#define RelayControl_H

#include <Arduino.h>

#define FAN_RELAY_PIN 11 
#define MISTER_PIN 9
#define MISTER_PIN2 10
#define RELAY_ON  LOW
#define RELAY_OFF HIGH

// ===== PINNING CONTROL VALUES =====
#define PINNING_CO2_TARGET       1000
#define PINNING_CO2_MARGIN       50
#define PINNING_HUMIDITY_TARGET  93.0
#define PINNING_HUMIDITY_MARGIN  3.0

// ===== FRUITING CONTROL VALUES =====
#define FRUITING_CO2_TARGET       800
#define FRUITING_CO2_MARGIN       50
#define FRUITING_HUMIDITY_TARGET  88.0
#define FRUITING_HUMIDITY_MARGIN  3.0

enum GrowthStage {
  PINNING,
  FRUITING
};

extern GrowthStage currentStage;
extern uint16_t co2Level;
extern float temperature, humidity;
extern int soilMoisture;
extern bool pinsDetected;

void setupRelays();
template <typename T>
int calc_withinRange(T currentReading, T targetReading, T margin); 
void relayControl();

#endif