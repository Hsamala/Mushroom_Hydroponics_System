#include "MultiGasSensor.h"
#include <ScioSense_ENS16x.h>  
#include <Adafruit_AHTX0.h>

ENS160 ens16x; 
Adafruit_AHTX0 aht;

bool sensorSetup(TwoWire& w, Stream& s) {

  ens16x.begin(&w, ENS160_I2C_ADDR);
  if (ens16x.init() != true) {
    s.println("ENS160 C02 sensor is not working, check wiring");
    return false;
  } else {
    s.println("ENS160 Sensor ready for use");
    ens16x.wait();
  }
  if (!aht.begin()) {
    uint8_t st = aht.getStatus(); // will return 0xFF on I2C read failure
    s.print("AHT begin() failed. getStatus() = 0x");
    s.println(st, HEX);

    if (st == 0xFF) {
      s.println("I2C read failed -> check wiring, power, and I2C address (use I2C scanner).");
    } else {
      if (st & 0x80) s.println("Status: BUSY (bit 7)");
      if (st & 0x08) s.println("Status: CALIBRATED (bit 3) -> sensor reports calibrated");
      else s.println("Status: NOT CALIBRATED -> calibration failed");
    }
    return false;
  }
  ens16x.startStandardMeasure();
  s.println("ENS160 and AHT11 sensor ready to go");

  pinMode(SOIL_PIN, INPUT);
  return true;
}

uint16_t readCO2(Stream& s) {
  if (ens16x.update() == RESULT_OK) {
    Ens16x_DeviceStatus status = ens16x.getDeviceStatus();
    uint8_t registerCheck = ENS16X_DEVICE_STATUS_VALID_HIGH | ENS16X_DEVICE_STATUS_VALID_LOW;
    s.print("Ens16x Status ");
    s.println(status);
    s.print("Register Check: ");
    s.println(registerCheck);
    if( (status & registerCheck) == 0x00) {
      return ens16x.getEco2();
    } else if ( (status & registerCheck)  == 0x04)  {
      return 1001;
    } else if ( (status & registerCheck) == 0x08) {
      return 1002;
    } else if( (status & registerCheck) == 0x0C) {
      return 1003;
    }
  }
}


float readTemp(Stream& s) {
  sensors_event_t temp;
  if(aht.getEvent(NULL, &temp)) {
    return temp.temperature;
  } else {
    s.println("Temperature polling went haywire, check delays/getEvent checks");
    return -1100.0;
  }
}
 

float readHumidity(Stream& s) {
  sensors_event_t humidity;
  if(aht.getEvent(NULL, &humidity)) {
    return humidity.relative_humidity; 
  } else {
    s.println("Humidity polling went haywire, check delays/getEvent checks");
    return -1100.0;
  }
}

int readSoilSensor(Stream& s) {
  int rawValue = analogRead(SOIL_PIN);
  int moisturePercentage = map(rawValue, dryValue, wetValue, 0, 100);
  s.printf("Raw: %d | Mapped: %d%%\n", rawValue, moisturePercentage);
  moisturePercentage = constrain(moisturePercentage, 0, 100);
  return moisturePercentage;
}
