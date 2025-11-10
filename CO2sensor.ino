#include <Wire.h>

#define SDA_PIN 8     // our chosen pins
#define SCL_PIN 9

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\nESP32-S3 I2C Scanner");
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(200);
}

void loop() {
  Serial.println("Scanning I2C bus...");
  byte error, address;
  int nDevices = 0;

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknown error at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) Serial.println("No I2C devices found.\n");
  else Serial.printf("Done. %d device(s) found.\n\n", nDevices);
  delay(4000);
}
