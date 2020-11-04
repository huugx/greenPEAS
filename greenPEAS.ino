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
//#include "display_helpers.h"
#include "data_helpers.h"
#include "sensor_helpers.h"

/* Global Variables */
int postDataInterval;
//unsigned long lastPostTime;
//const unsigned longlastReadInterval;
//const unsigned long readInterval = 100;
//unsigned long aveSensorReadings = (postInterval/readInterval) - 1;
//const unsigned long displayInterval = 0;


void setup() {
  Serial.begin(115200);
  while(Serial == false){}; 
  
  initProperties();                                   // Initialize Sensors (see thingProperties.h)
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // Connect to Arduino IoT Cloud
  
//  tft.initR(INITR_144GREENTAB);                       // Initialize TFT Display
//  tft.fillScreen(ST77XX_WHITE);
//  startTFT();
  
  beginSensors();
  initSmoothSensorData();

  delay(2500);
}


void loop() {
  ArduinoCloud.update();
  
  readSensors();
//  printSensors();
  smoothSensorData();
  postDataInterval++;


  if (postDataInterval >= smoothDataInterval) {
    printSensorsAve();
    postSensorsToCloud();
//    displayValuesOnTFT();
    postDataInterval = 0;
  }
}


//void wifiStatus() {
//  if (WiFi.status() == 3) {
//    tft.fillCircle(64,5,2, GREEN);
//  } else {
//    tft.fillCircle(64,5,2, RED);
//  }
//}
