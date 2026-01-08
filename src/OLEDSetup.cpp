#include <memory>
#include "OLEDSetup.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

std::unique_ptr<Adafruit_SSD1306> display = NULL;

bool setupDisplay(TwoWire& w) {

   display = std::unique_ptr<Adafruit_SSD1306> ( new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &w, OLED_RESET) );

  if (!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    return false;
  }

  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(0, 0);
  display->println("Soil Sensor Test");
  display->println("Reading value...");
  display->display();

  return true;

}

void printScreen(char text[]) {
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(WHITE);
  display->setCursor(0, 0);
  display->print(text);
  display->display();
}

void drawInitCo2() {
  display->clearDisplay();
  display->setTextColor(SSD1306_WHITE);

  // 1. YELLOW ZONE (Y: 0-15) - Status Header
  display->setTextSize(2);
  display->setCursor(0, 0);
  display->print(F("CO2 INIT")); 

  // 2. BLUE ZONE (Y: 16-63) - Primary Instruction
  display->setCursor(0, 20);
  display->setTextSize(1);
  display->print(F("First time setup..."));
  
  display->setCursor(0, 30);
  display->print(F("May take 1 hour"));

  drawNavDots(0);
  display->display();
}

void drawWarmupCo2() {
  display->clearDisplay();
  display->setTextColor(SSD1306_WHITE);

  // 1. YELLOW ZONE (Y: 0-15) - Status Header
  display->setTextSize(2);
  display->setCursor(0, 0);
  display->print(F("CO2 WARMUP")); 

  // 2. BLUE ZONE (Y: 16-63) - Primary Instruction

  display->setCursor(0, 20);
  display->setTextSize(1);
  display->print(F("Warming up CO2 sensor..."));
  
  display->setCursor(0, 30);
  display->print(F("Will take 3 mins"));

  drawNavDots(0);
  display->display();
}

void invalidCo2() {
  display->clearDisplay();
  display->setTextColor(SSD1306_WHITE);

  // 1. YELLOW ZONE (Y: 0-15) - Status Header
  display->setTextSize(2);
  display->setCursor(0, 0);
  display->print(F("CO2 INVALID")); 

  // 2. BLUE ZONE (Y: 16-63) - Primary Instruction
  display->setCursor(0, 42);
  display->setTextSize(1);
  display->print(F("Something is wrong with the CO2 sensor"));
  
  display->setCursor(0, 54);
  display->print(F("Consult Manual or replace"));

  drawNavDots(0);
  display->display();
}

void drawCO2(uint16_t co2Value) {
  display->clearDisplay();
  display->setTextColor(SSD1306_WHITE);

  // 1. YELLOW ZONE (Y: 0-15)
  display->setTextSize(2); 
  display->setCursor(0, 0); // Top of the screen
  display->print(F("CO2"));

  // 2. BLUE ZONE (Y: 16-63)
  display->setCursor(0, 16); // Pushed down into the blue area
  display->setTextSize(4); 
  display->print(co2Value, 1);
  
  display->setTextSize(1);
  display->print(F(" ppm"));

  drawNavDots(0);
  display->display();

}


void drawTemperature(float temp) {
  display->clearDisplay();
  display->setTextColor(SSD1306_WHITE);

  // YELLOW ZONE
  display->setTextSize(2);
  display->setCursor(0, 0);
  display->print(F("TEMP"));

  // BLUE ZONE
  display->setCursor(0, 22);
  display->setTextSize(4);
  display->print(temp, 1);
  display->setTextSize(2);
  display->print(F(" C"));

  drawNavDots(1);
  display->display();
}

void drawHumidity(float humidity) {
  display->clearDisplay();
  display->setTextColor(SSD1306_WHITE);

  // YELLOW ZONE
  display->setTextSize(2);
  display->setCursor(0, 0);
  display->print(F("HUMIDITY"));

  // BLUE ZONE
  display->setCursor(0, 22);
  display->setTextSize(4);
  display->print(humidity, 1);
  display->setTextSize(2);
  display->print(F(" %"));

  drawNavDots(2);
  display->display();
}

void drawSoilMoisture(int moistPercentage) {
  display->clearDisplay();
  display->setTextColor(SSD1306_WHITE);

  // YELLOW ZONE
  display->setTextSize(2);
  display->setCursor(0, 0);
  display->print(F("MOISTURE"));

  // BLUE ZONE
  display->setCursor(0, 22);
  display->setTextSize(4);
  display->print(moistPercentage);
  display->setTextSize(2);
  display->print(F(" %"));

  drawNavDots(3);
  display->display();
}


void drawNavDots(int activePage) {
  int dotY = 58;
  int centerX = SCREEN_WIDTH / 2;
  int spacing = 20;
  int totalDots = 4; // Updated from 3 to 4

  // Offset calculation: (Total Spacing) / 2
  // For 4 dots, there are 3 "spaces" between them.
  int startX = centerX - (((totalDots - 1) * spacing) / 2);

  for (int i = 0; i < totalDots; i++) {
    int x = startX + (i * spacing);
    
    display->fillCircle(x, dotY, 2, SSD1306_WHITE); // Draw the small dot
    
    if (i == activePage) {
      display->drawCircle(x, dotY, 5, SSD1306_WHITE); // Draw the selection ring
    }
  }
}