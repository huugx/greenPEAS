#ifndef DISPLAY_HELPERS_H
#define DISPLAY_HELPERS_H

/* TFT PINS */
#define TFT_RST       2
#define TFT_Lite      3
#define TFT_DC        4
#define TFT_CS        5

/* Color Definitions */
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define ORANGE   0xFCA0
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define GREY     0x39E7


#include <avr/dtostrf.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include "bitmap_mono.h"


extern Adafruit_ST7735 tft;
extern float aveDust, aveCo2, aveVoc, aveTemperature, aveHumidity, aveLight, aveSound;
extern float aveSensorIAQIndex[];
extern float aveSensorData[];
extern float histogramSensorIAQIndex[];
//extern float oldDustcon, oldEco2, oldHumidity, oldTemperature;
//extern int oldVirus;


//Functions
void initTFT();
void clearTFT();
void histIndex();
void histDraw();
void screenMain();
void screenTemperature();
void screenHumidity();
void screenCo2();
void screenDust();
void screenVoc();
void screenLight();
void screenSound();
void displayValuesOnTFT();
void drawText(int x, int y, char *text, uint16_t color, int size);
void wifiStatus();
void debounceButton();
void updateValuesOnTFT();

#endif /* DISPLAY_HELPERS_H */
