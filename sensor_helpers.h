#ifndef SENSOR_HELPERS_H
#define SENSOR_HELPERS_H

#include "Adafruit_SGP30.h"
#include "Adafruit_VEML7700.h"
#include "Adafruit_SHTC3.h"

extern float sensorArrayAve[];
extern float aveDust, aveCo2, aveVoc, aveTemperature, aveHumidity, aveLight, aveSound;

void beginSensors();
void readSensors();
void printSensors();
void printSensorsAve();
void postSensorsToCloud();
void getVOC();
void getLight();
void getSound();
void getTemp();
void getDust();
int calcDustFilter(int m);
void configureLight();
uint32_t getAbsoluteHumidity(float temperature, float humidity);

#endif /* SENSOR_HELPERS_H */
