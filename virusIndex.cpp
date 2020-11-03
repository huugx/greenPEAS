#include "virusIndex.h"


  float _virusIndex;
  int tempIndex, humIndex, co2Index, dustIndex;
  int virusIndex = 0;
//  extern int virusIndex;
  extern float dustcon;
  extern float eco2;
  extern float humidity;
  extern float pressure;
  extern float temperature;
  extern float tvoc;
  extern float lux;

int safeIndex() {
    
  if (temperature >= 19 && temperature <= 24) {
    tempIndex = 0;
  } else if (temperature > 24 && temperature < 30) {
    tempIndex = 1;
  } else if (temperature < 19) {
    tempIndex = 2;
  } else {
    tempIndex = 2;
  }
  
  if (humidity >= 40 && humidity <= 60) {
    humIndex = 0;
  } else if (humidity > 60 && humidity < 70) {
    humIndex = 1; 
  } else if (humidity < 40 || humidity > 70) {
    humIndex = 2;
  } 

  if (eco2 > 800) {
    co2Index = 3;
  } else if (eco2 > 700 && eco2 <=800) {
    co2Index = 2;
  } else {
    co2Index = 0;
  }
  
  if (dustcon <= 15) {
    dustIndex = 0;
  } else if (dustcon > 15 && dustcon <= 35) {
    dustIndex = 1;
  } else if (dustcon > 35 && dustcon <= 100) {
    dustIndex = 2;
  } else {
    dustIndex = 3;
  }

  virusIndex = tempIndex + humIndex + co2Index + dustIndex;

  return virusIndex;
}


int resetIndex() {
  
  virusIndex = 0;
}
