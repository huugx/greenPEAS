#ifndef THINGPROPERTIES_H
#define THINGPROPERTIES_H

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>


const char THING_ID[] = "82a22de3-4fa9-44e7-a127-2949b1e37a84";

const char SSID[]     = "dial_Z";    // Network SSID (name)
const char PASS[]     = "kingFelix";    // Network password (use for WPA, or use as key for WEP)


float dustcon;
float eco2;
float humidity;
float temperature;
float tvoc;
float lux;
float sound;

void initProperties(){

  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(dustcon, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(eco2, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(humidity, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(tvoc, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(lux, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(sound, READ, ON_CHANGE, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

#endif /* THINGPROPERTIES_H */
