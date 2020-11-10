#ifndef SENSOR_HELPERS_H
#define SENSOR_HELPERS_H

#include "Adafruit_SGP30.h"
#include "Adafruit_VEML7700.h"
#include "Adafruit_SHTC3.h"

const int smoothDataInterval = 20;

extern float aveVirusIndex, aveTemperature, aveHumidity, aveCo2, aveVoc, aveDust, aveLight, aveSound;

void beginSensors();
void readSensors();
void printSensors();
void printSensorsAve();
void printSafeIndex();
void postSensorsToCloud();
float getVirusIndex();
float getVoc();
float getCo2();
float getLight();
float getSound();
float getTemperature();
float getHumidity();
float getDust();
int calcDustFilter(int m);
void configureLight();
uint32_t getAbsoluteHumidity(float temperature, float humidity);
void triggerAveDustToPost();
void getVocCo2Baseline();
void getAbsoluteHumidityCompensation();



void initSmoothSensorData();
void smoothSensorData(); 
int calculateSensorIndex (float a, float b, float x);


#endif /* SENSOR_HELPERS_H */
