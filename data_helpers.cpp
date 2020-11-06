#include "data_helpers.h"


float currSensorData[8];
const int sensorQty = (sizeof(currSensorData)/sizeof(currSensorData[0]));
float sensorArrayAve[sensorQty];
int readSensorIndex = 0;
float readSensorTotal[sensorQty];
float dataSensorArray[smoothDataInterval][sensorQty];


void initSmoothSensorData() {
  for (int i =0; i < smoothDataInterval; i++) {
    for (int j =0; j < sensorQty; j++) {
      dataSensorArray[i][j] = 0;
    }
  }

}

float (*getSensorCurr[])() = {getVirusIndex, getTemperature, getHumidity, getCo2, getVoc, getDust, getLight, getSound};

void smoothSensorData() {
  
  for (int i=0; i < sensorQty; i++) {
    currSensorData[i] = getSensorCurr[i]();
    // subtract the last reading:
    readSensorTotal[i] = readSensorTotal[i] - dataSensorArray[readSensorIndex][i];
    // read from the sensor:
    dataSensorArray[readSensorIndex][i] = currSensorData[i];
    // add the reading to the readSensorTotal:
    readSensorTotal[i] = readSensorTotal[i] + dataSensorArray[readSensorIndex][i];
    // advance to the next position in the array:
    readSensorIndex = readSensorIndex + 1;
  
    // if we're at the end of the array...
    if (readSensorIndex >= smoothDataInterval) {
      // ...wrap around to the beginning:
      readSensorIndex = 0;
      sensorArrayAve[i] = readSensorTotal[i] / smoothDataInterval;
    } //close if (readSensorIndex....)
  } //close i
}

//void smoothSensorData() {
////    currSensorData[0] = currVirusIndex;
//    currSensorData[1] = currTemperature;
//    currSensorData[2] = currHumidity;
//    currSensorData[3] = currCo2;
//    currSensorData[4] = currVoc;
//    currSensorData[5] = currDust;
//    currSensorData[6] = currLight;
//    currSensorData[7] = currSound;
//    
//    for (int i=0; i < sensorQty; i++) {
//        // subtract the last reading:
//        readSensorTotal[i] = readSensorTotal[i] - dataSensorArray[readSensorIndex][i];
//        // read from the sensor:
//        dataSensorArray[readSensorIndex][i] = currSensorData[i];
//        // add the reading to the readSensorTotal:
//        readSensorTotal[i] = readSensorTotal[i] + dataSensorArray[readSensorIndex][i];
//        // advance to the next position in the array:
//        readSensorIndex = readSensorIndex + 1;
//      
//        // if we're at the end of the array...
//        if (readSensorIndex >= smoothDataInterval) {
//          // ...wrap around to the beginning:
//          readSensorIndex = 0;
//          sensorArrayAve[i] = readSensorTotal[i] / smoothDataInterval;
//        } //close if (readSensorIndex....)
//    } //close i
//}




//
//
//#include "data_helpers.h"
//
//
//
//
//
///////////////////////////////////
//// Calculate Virus Index
///////////////////////////////////
////  float _virusIndex;
////  int tempIndex, humIndex, co2Index, dustIndex;
////  int virusIndex = 0;
//////  extern int virusIndex;
////  extern float dustcon;
////  extern float eco2;
////  extern float humidity;
////  extern float pressure;
////  extern float temperature;
////  extern float tvoc;
////  extern float lux;
////
////int safeIndex() {
////    
////  if (temperature >= 19 && temperature <= 24) {
////    tempIndex = 0;
////  } else if (temperature > 24 && temperature < 30) {
////    tempIndex = 1;
////  } else if (temperature < 19) {
////    tempIndex = 2;
////  } else {
////    tempIndex = 2;
////  }
////  
////  if (humidity >= 40 && humidity <= 60) {
////    humIndex = 0;
////  } else if (humidity > 60 && humidity < 70) {
////    humIndex = 1; 
////  } else if (humidity < 40 || humidity > 70) {
////    humIndex = 2;
////  } 
////
////  if (eco2 > 800) {
////    co2Index = 3;
////  } else if (eco2 > 700 && eco2 <=800) {
////    co2Index = 2;
////  } else {
////    co2Index = 0;
////  }
////  
////  if (dustcon <= 15) {
////    dustIndex = 0;
////  } else if (dustcon > 15 && dustcon <= 35) {
////    dustIndex = 1;
////  } else if (dustcon > 35 && dustcon <= 100) {
////    dustIndex = 2;
////  } else {
////    dustIndex = 3;
////  }
////
////  virusIndex = tempIndex + humIndex + co2Index + dustIndex;
////
////  return virusIndex;
////}
////
////
////int resetIndex() {
////  
////  virusIndex = 0;
////}
//
//
//
///** 
// *  Data Smooting Functions
// *  
// *  Averages the sensor data collected over a period
// *  
// *  @param 
// *  @param
// *  
// */
//
//float currVirusIndex = 0;     //TODO: make this variable a function
//float currSensorData[8];
//
//const int sensorQty = 8;
////(sizeof(currSensorData)/sizeof(currSensorData[0]));
//float sensorArrayAve[sensorQty];
//int readSensorIndex = 0;
//float readSensorTotal[sensorQty];
//float dataSensorArray[smoothDataInterval][sensorQty];
//
//
//void initSmoothSensorData() {
//  
//  for (int i =0; i < smoothDataInterval; i++) {
//    
//    for (int j =0; j < sensorQty; j++) {
//      
//      dataSensorArray[i][j] = 0;
//      //clear array of any data
//      
//    }
//  }
//}
//
//
//void smoothSensorData() {
//    
//    currSensorData[0] = currVirusIndex;
//    currSensorData[1] = currTemperature;
//    currSensorData[2] = currHumidity;
//    currSensorData[3] = currCo2;
//    currSensorData[4] = currVoc;
//    currSensorData[5] = currDust;
//    currSensorData[6] = currLight;
//    currSensorData[7] = currSound;
//    
//    for (int i=0; i < sensorQty; i++) {
//        // subtract the last reading:
//        readSensorTotal[i] = readSensorTotal[i] - dataSensorArray[readSensorIndex][i];
//        // read from the sensor:
//        dataSensorArray[readSensorIndex][i] = currSensorData[i];
//        // add the reading to the readSensorTotal:
//        readSensorTotal[i] = readSensorTotal[i] + dataSensorArray[readSensorIndex][i];
//        // advance to the next position in the array:
//        readSensorIndex = readSensorIndex + 1;
//      
//        // if we're at the end of the array...
//        if (readSensorIndex >= smoothDataInterval) {
//          // ...wrap around to the beginning:
//          readSensorIndex = 0;
//          sensorArrayAve[i] = readSensorTotal[i] / smoothDataInterval;
//        } //close if (readSensorIndex....)
//    } //close i
//}
