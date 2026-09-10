#include "RelayControl.h"


void setupRelays() {
  pinMode(FAN_RELAY_PIN, OUTPUT);
  digitalWrite(FAN_RELAY_PIN, RELAY_OFF);
  pinMode(MISTER_PIN, OUTPUT);
  digitalWrite(MISTER_PIN, RELAY_OFF);
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
  // --- 1. CO2 LOGIC (Low Trigger: ON at Lower Limit) ---
  int co2Event = calc_withinRange<unsigned int>(co2Level, 800, 50); 
  if (co2Event == 2) digitalWrite(FAN_RELAY_PIN, RELAY_ON); isFanOn = true;// ON
  if (co2Event == 1) digitalWrite(FAN_RELAY_PIN, RELAY_OFF); isFanOn = false;// OFF

  // --- 2. MISTER LOGIC (Low Trigger: ON at Lower Limit) ---
  int humEvent = calc_withinRange<float>(humidity, 90.0, 3.0); 
  if (humEvent == 1) digitalWrite(MISTER_PIN, RELAY_ON); digitalWrite(MISTER_PIN2, RELAY_ON); isMisterOn = true;  // ON
  if (humEvent == 2) digitalWrite(MISTER_PIN, RELAY_OFF); digitalWrite(MISTER_PIN2, RELAY_OFF); isMisterOn = false;// OFF
  syncBlynk(isFanOn, isMisterOn);
}