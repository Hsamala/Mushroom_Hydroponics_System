#ifndef OLEDSetup_H
#define OLEDSetup_H

#include <Adafruit_SSD1306.h>
#include <memory>

// --- OLED SCREEN CONFIGURATION ---
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C


extern std::unique_ptr<Adafruit_SSD1306> display;

bool setupDisplay(TwoWire& w);
void drawCO2(uint16_t co2Value);
void drawWarmupCo2();
void drawInitCo2();
void invalidCo2();
void drawHumidity(float humidity);
void drawNavDots(int activePage);
void drawTemperature(float temp);
void drawSoilMoisture(int moistPercentage);


#endif