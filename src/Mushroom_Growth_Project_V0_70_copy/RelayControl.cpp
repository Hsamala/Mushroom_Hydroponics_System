#include "RelayControl.h"


void setupRelays() {
  pinMode(FAN_RELAY_PIN, OUTPUT);
}

template <typename T>
int calc_withinRange(T currentReading, T targetReading, T margin) {
  if (currentReading > (targetReading + margin)) {
    return 2; // Upper Limit Breached
  } 
  else if (currentReading < (targetReading - margin)) {
    return 1; // Lower Limit Breached
  }
  return 0; // Inside Dead Zone 
}

void relayControl() {
  // --- 1. CO2 LOGIC (Low Trigger: ON at Lower Limit) ---
  int co2Event = calc_withinRange<unsigned int>(co2Level, 600, 50); 
  if (co2Event == 2) digitalWrite(FAN_RELAY_PIN, HIGH); isFanOn = true;// ON
  if (co2Event == 1) digitalWrite(FAN_RELAY_PIN, LOW); isFanOn = false;// OFF

  // --- 2. MISTER LOGIC (Low Trigger: ON at Lower Limit) ---
  int humEvent = calc_withinRange<float>(soilMoisture, 75.0, 3.0); 
  if (humEvent == 1) digitalWrite(MISTER_PIN, HIGH); isMisterOn = true;  // ON
  if (humEvent == 2) digitalWrite(MISTER_PIN, LOW); isMisterOn = false;// OFF
}