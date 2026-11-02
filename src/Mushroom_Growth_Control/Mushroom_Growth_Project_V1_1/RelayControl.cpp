#include "RelayControl.h"

GrowthStage currentStage = PINNING;

void setupRelays() {
  pinMode(FAN_RELAY_PIN, OUTPUT);
  pinMode(MISTER_PIN, OUTPUT);
  pinMode(MISTER_PIN2, OUTPUT);
  digitalWrite(MISTER_PIN, RELAY_OFF); 
  digitalWrite(MISTER_PIN2, RELAY_OFF); 
  digitalWrite(FAN_RELAY_PIN, RELAY_OFF); 
}

template <typename T>
int calc_withinRange(T currentReading, T targetReading, T margin) {
  
  if(currentReading == 1001) {
    return 0; // Warmup CO2 reading, disregard
  } else if (currentReading > (targetReading + margin)) {
    return 2; // Upper Limit Breached
  } 
  else if (currentReading < (targetReading - margin)) {
    return 1; // Lower Limit Breached
  }
  return 0; // Inside Dead Zone 
}

void relayControl() {

  // Switch growth stage when the vision system detects pins
  if (pinsDetected) {
    currentStage = FRUITING;
  }
  else {
    currentStage = PINNING;
  }

  // Select control values based on growth stage
  uint16_t co2Target;
  uint16_t co2Margin;
  float humidityTarget;
  float humidityMargin;

  if (currentStage == PINNING) {
    co2Target = PINNING_CO2_TARGET;
    co2Margin = PINNING_CO2_MARGIN;
    humidityTarget = PINNING_HUMIDITY_TARGET;
    humidityMargin = PINNING_HUMIDITY_MARGIN;
  }
  else {
    co2Target = FRUITING_CO2_TARGET;
    co2Margin = FRUITING_CO2_MARGIN;
    humidityTarget = FRUITING_HUMIDITY_TARGET;
    humidityMargin = FRUITING_HUMIDITY_MARGIN;
  }

  // --- 1. CO2 LOGIC (Low Trigger: ON at Lower Limit) ---
  int co2Event = calc_withinRange<unsigned int>(co2Level, co2Target, co2Margin); 
  if (co2Event == 2) {
    digitalWrite(FAN_RELAY_PIN, RELAY_ON);
  }  
  if (co2Event == 1) {
    digitalWrite(FAN_RELAY_PIN, RELAY_OFF);
  }  

  // --- 2. MISTER LOGIC (Low Trigger: ON at Lower Limit) ---
  int humEvent = calc_withinRange<float>(humidity, humidityTarget, humidityMargin); 
  if (humEvent == 1) {
    digitalWrite(MISTER_PIN, RELAY_ON);
    digitalWrite(MISTER_PIN2, RELAY_ON);
    digitalWrite(FAN_RELAY_PIN, RELAY_OFF);
  } 
  if (humEvent == 2) {
    digitalWrite(MISTER_PIN, RELAY_OFF);
    digitalWrite(MISTER_PIN2, RELAY_OFF); 
    digitalWrite(FAN_RELAY_PIN, RELAY_ON);
  } 
}