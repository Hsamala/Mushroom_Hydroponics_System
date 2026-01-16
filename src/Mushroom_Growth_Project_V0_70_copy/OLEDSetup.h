#ifndef OLEDSetup_H
#define OLEDSetup_H

#include <memory>

// --- OLED SCREEN CONFIGURATION ---
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C


class Adafruit_SSD1306;
class TwoWire;


bool setupDisplay(TwoWire& w);
void drawCO2(uint16_t co2Value);
void drawWarmupCo2();
void drawInitCo2();
void invalidCo2();
void printScreen(char text[]);
void drawHumidity(float humidity);
void drawNavDots(int activePage);
void drawTemperature(float temp);
void drawSoilMoisture(int moistPercentage);


#endif