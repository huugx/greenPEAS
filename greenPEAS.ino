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
//#include "thingProperties.h"
//#include "display_helpers.h"

#include "data_helpers.h"
#include "sensor_helpers.h"

/* Global Variables */
const unsigned long postInterval = 5000;
unsigned long lastPostInterval;
//const unsigned longlastReadInterval;
//const unsigned long readInterval = 100;
//unsigned long aveSensorReadings = (postInterval/readInterval) - 1;
//const unsigned long displayInterval = 0;
int smoothInterval = 0;

void setup() {
  Serial.begin(115200);
  while(Serial == false){}; 
  
  beginSensors();
//  initSmoothSensorData();

  delay(3000);
}


void loop() {

  readSensors();
  printSensors();
  smoothSensorData();
  smoothInterval = smoothInterval + 1;




  if (smoothInterval >= 10) {
    printSensorsAve();
    smoothInterval = 0;
  }
  
}
