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
unsigned long lastReadInterval;
const unsigned long readInterval = 250;
//unsigned long aveSensorReadings = (postInterval/readInterval) - 1;
//const unsigned long displayInterval = 0;



void setup() {
  Serial.begin(115200);
  while(Serial == false){};                           //TODO: remove
  
  initProperties();                                   // Initialize Sensors (see thingProperties.h)
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // Connect to Arduino IoT Cloud
  
  initTFT();
  beginSensors();
  initSmoothSensorData();
 
  
  delay(2500);
}


void loop() {

  debounceButton();
  
//  int adc = analogRead(A3);
//  Serial.println(adc);

  
  if ((millis() - lastReadInterval) >= readInterval) {
    wifiStatus();
    smoothSensorData();       //read and average sensors
    postSensorCounter++;      //steps until cloud post
    printSensors();           //DEBUG
    lastReadInterval = millis();
  }
 
  if (postSensorCounter >= smoothDataInterval) {
    postSensorsToCloud();       //convert array to variables - ensures only post on change
//    printSensorsAve();          
    printSafeIndex();
    ArduinoCloud.update();
    postSensorCounter = 0;
  }
}


void wifiStatus() {
  if (WiFi.status() == 3) {
    tft.fillCircle(64,5,1, GREEN);
  } else {
    tft.fillCircle(64,5,1, RED);
  }
}
