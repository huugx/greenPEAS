#ifndef SENSOR_HELPERS_H
#define SENSOR_HELPERS_H

#include "Adafruit_SGP30.h"
#include "Adafruit_VEML7700.h"
#include "Adafruit_SHTC3.h"

extern const unsigned long readInterval;

const int smoothDataInterval = 20;
const int prevHourDataInterval = 180;       // Average past hour of readings (3600)/(60/(readInterval*smoothDataInterval)/1000)
const int histogramDataInterval = 11;       // Show 10 hours on the histogram


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
int Filter(int m);
void configureLight();
uint32_t getAbsoluteHumidity(float temperature, float humidity);
void triggerAveDustToPost();
void getVocCo2Baseline();
void getAbsoluteHumidityCompensation();
void storeIAQIndex();
void storeHistogramIndex();

void initSmoothSensorData();
void smoothSensorData(); 
float calculateSensorIndex (float a, float b, float x);


#endif /* SENSOR_HELPERS_H */
