#include <Wire.h>
#include <ScioSense_ENS16x.h>   // from sciosense / ens16x-arduino (or use DFRobot_ENS160)
#include <Adafruit_AHTX0.h>    // AHT10/AHT20 library

// I2C pins you used in your scanner
#define SDA_PIN 8
#define SCL_PIN 9

// ENS160 I2C address (change to 0x52 if your board uses ADD low)
#define ENS160_I2C_ADDR 0x52

ENS160 ens16x;
Adafruit_AHTX0 aht;

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("ESP32-S3 ENS160 + AHT2x example\n");
  ens16x.enableDebugging(Serial);

  // init Wire on chosen pins
  Wire.begin(SDA_PIN, SCL_PIN);
  delay(200);
  ens16x.begin(&Wire, ENS160_I2C_ADDR);

  // init AHT (temp & humidity)
  if (!aht.begin()) {
    Serial.println("Could not find AHT sensor - check wiring!");
  } else {
    Serial.println("AHT sensor found");
  }

  // init ENS160
  Serial.print("Initializing ENS160...");
  if (ens16x.init() != true) { // true -> enable debug prints (optional)
    Serial.println(" ENS160 not found or init failed. Check wiring/address.");
    // don't return — we'll still try but readings will fail until init.
  } else {
    Serial.println(" ENS160 initialized");
  }

  // start ENS160 standard measurement mode
  ens16x.startStandardMeasure();
  Serial.println("Started ENS160 standard measurement (warm up may take some time)");
}

void loop() {
  // 1) read temperature & humidity from AHT
  float temperatureC = NAN;
  float humidityRH = NAN;

  if (aht.begin()) { // aht.begin() is cheap; you can instead check once in setup and track a flag
    sensors_event_t humEvent, tempEvent;
    aht.getEvent(&humEvent, &tempEvent); // populate events
    temperatureC = tempEvent.temperature;
    humidityRH = humEvent.relative_humidity;
    Serial.printf("T = %.2f °C, RH = %.2f %%\n", temperatureC, humidityRH);
  } else {
    Serial.println("AHT read failed");
  }

  // 3) trigger/read ENS160 data (uses library's measurement flow)
  ens16x.wait(); // wait until sensor measurement ready (library helper)
  if (ens16x.update() == RESULT_OK) {
    if (ens16x.hasNewData()) {
      // read useful outputs:
      uint8_t aqi_uba = ens16x.getAirQualityIndex_UBA();
      uint16_t tvoc_ppb = ens16x.getTvoc();
      uint16_t eco2_ppm = ens16x.getEco2();

      Serial.printf("ENS160 -> AQI(UBA): %u\tTVOC: %u ppb\teCO2: %u ppm\n",
                    (unsigned)aqi_uba, (unsigned)tvoc_ppb, (unsigned)eco2_ppm);
    } else {
      Serial.println("ENS160: no new data this cycle");
    }
  } else {
    Serial.println("ENS160 update() returned error or not ready");
  }

  // wait before next loop
  delay(2000);
}