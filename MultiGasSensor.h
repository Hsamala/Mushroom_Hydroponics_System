#ifndef MultiGasSensor_H
#define MultiGasSensor_H

#include <Arduino.h>
#include <Wire.h>

#define ENS160_I2C_ADDR 0x53

#define SOIL_PIN   8
const int dryValue = 3500;
const int wetValue = 1600;


bool sensorSetup(TwoWire& w, Stream& s);
uint16_t readCO2(Stream& s);
float readTemp(Stream& s);
float readHumidity(Stream& s);
int  readSoilSensor(Stream& s);


#endif 
