/* 
INDOOR ENVIRONMENTAL SENSOR
*/

/* Libraries */
#include <Wire.h>
#include <SPI.h>
#include <Arduino_MKRENV.h>
#include <Adafruit_Sensor.h>
#include <ArduinoLowPower.h>


/* Headers */
#include "thing_properties.h"
#include "display_helpers.h"
#include "sensor_helpers.h"

/* Global Variables */
int postSensorCounter; 
//int sampleSensorCounter;
//unsigned long lastPostTime;
unsigned long prevReadTime;
const unsigned long readInterval = 250;             //DO NOT CHANGE
unsigned long prevHistogramTime;
const unsigned long histogramInterval = 3600000;  //FIXME: CHANGE TO 3600000
//unsigned long aveSensorReadings = (postInterval/readInterval) - 1;
//const unsigned long displayInterval = 0;



void setup() {
  Serial.begin(115200);
//  while(Serial == false){};                           //TODO: remove
  
  initProperties();                                   // Initialize Sensors (see thingProperties.h)
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // Connect to Arduino IoT Cloud
  
  initTFT();
  beginSensors();
  initSmoothSensorData();
 
  
  delay(2500);
  clearTFT();
}


void loop() {
  debounceButton();
  
//  int adc = analogRead(A3);
//  Serial.println(adc);

  unsigned long currentTime = millis();
  if ((unsigned long)(currentTime - prevReadTime) >= readInterval) {
    wifiStatus();
    smoothSensorData();       //read and average sensors
    postSensorCounter++;      //steps until cloud post
//    printSensors();           //DEBUG
    prevReadTime = currentTime;
  }
 
  if (postSensorCounter >= smoothDataInterval) {
    postSensorsToCloud();       //convert array to variables - ensures only post on change
//    printSensorsAve();          
//    printSafeIndex();
    updateValuesOnTFT();
    ArduinoCloud.update();
    postSensorCounter = 0;
    Serial.println(currentTime);
  }

  if ((unsigned long)(currentTime - prevHistogramTime) >= histogramInterval) {
    storeHistogramIndex();
    Serial.println("histogram");
    histDraw();
    prevHistogramTime = currentTime;
  }
  
}


void wifiStatus() {
  if (WiFi.status() == 3) {
    tft.fillCircle(64,5,1, GREEN);
  } else {
    tft.fillCircle(64,5,1, RED);
  }
}
