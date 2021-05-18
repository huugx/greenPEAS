/* 
INDOOR ENVIRONMENTAL SENSOR
*/

/* Libraries */
#include <Wire.h>
#include <SPI.h>
#include <Arduino_MKRENV.h>
#include <Adafruit_Sensor.h>
#include <ArduinoLowPower.h>

#include <ArduinoJson.h>  
#include <avr/dtostrf.h>


/* Headers */
#include "config.h"
#include "display_helpers.h"
#include "sensor_helpers.h"

/* Global Variables */
int postSensorCounter; 
//int sampleSensorCounter;
//unsigned long lastPostTime;
unsigned long prevReadTime;
const unsigned long readInterval = 1000;             //DO NOT CHANGE
unsigned long prevHistogramTime;
const unsigned long histogramInterval = 3600000;  //FIXME: CHANGE TO 3600000
//unsigned long aveSensorReadings = (postInterval/readInterval) - 1;

//const unsigned long displayInterval = 0;



void setup() {
  Serial.begin(115200);
//  while(Serial == false){};                           //TODO: remove
  
                                   // Initialize Sensors (see thingProperties.h)
//  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // Connect to Arduino IoT Cloud
  
  initTFT();
  beginSensors();
  initSmoothSensorData();


//  connectToWIFI();

  
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
//    ArduinoCloud.update();
    postSensorCounter = 0;
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("reconnecting to network");
      status = WiFi.begin(ssid, pass);
      delay(500);
      printWifiStatus();
    }
    httpRequest();
//    Serial.println(currentTime);
  }

  if ((unsigned long)(currentTime - prevHistogramTime) >= histogramInterval) {
    storeHistogramIndex();
    Serial.println("histogram");
    histDraw();
    prevHistogramTime = currentTime;
  }
  
}

void connectToWIFI()
{
   // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void wifiStatus() {
  if (WiFi.status() == 3) {
    tft.fillCircle(5,5,1, GREEN);
  } else {
    tft.fillCircle(5,5,1, RED);
  }
}
