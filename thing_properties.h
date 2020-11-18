#ifndef THINGPROPERTIES_H
#define THINGPROPERTIES_H

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>


const char THING_ID[] = "82a22de3-4fa9-44e7-a127-2949b1e37a84";

const char SSID[]     = "dial_Z";       // Network SSID (name)
const char PASS[]     = "kingFelix";    // Network password (use for WPA, or use as key for WEP)

float aveVirusIndex, aveTemperature, aveHumidity, aveCo2, aveVoc, aveDust, aveLight, aveSound;
//int virusIndex = round(aveVirusIndex);

void initProperties(){

  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(aveVirusIndex, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(aveTemperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(aveHumidity, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(aveCo2, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(aveVoc, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(aveDust, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(aveLight, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(aveSound, READ, ON_CHANGE, NULL);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

#endif /* THINGPROPERTIES_H */
