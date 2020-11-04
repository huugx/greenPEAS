#ifndef DISPLAY_HELPERS_H
#define DISPLAY_HELPERS_H

#include <avr/dtostrf.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include "bitmap_mono.h"


extern float aveDust, aveCo2, aveVoc, aveTemperature, aveHumidity, aveLight, aveSound;
//extern float oldDustcon, oldEco2, oldHumidity, oldTemperature;
//extern int oldVirus;


//Functions
void startTFT();
void clearTFT();
void histIndex();
void histDraw();
void scrnMain();
void scrnTemp();
void scrnHumidity();
void scrnCo2();
void scrnPM25();
void scrnVOC();
void scrnLight();
void scrnSound();
void displayValuesOnTFT();
void drawText(int x, int y, char *text, uint16_t color, int size);
void wifiStatus();

#endif /* DISPLAY_HELPERS_H */
