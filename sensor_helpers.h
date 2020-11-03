#ifndef SENSOR_HELPERS_H
#define SENSOR_HELPERS_H

#include "Adafruit_SGP30.h"
#include "Adafruit_VEML7700.h"
#include "Adafruit_SHTC3.h"

extern float dust, co2, voc, temperature, humidity, light, sound;

void beginSensors();
void readSensors();
void printSensors();
void getVOC();
void getLight();
void getSound();
void getTemp();
void getDust();
int calcDustFilter(int m);
void configureLight();

#endif /* SENSOR_HELPERS_H */
