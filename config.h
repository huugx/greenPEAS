#ifndef THINGPROPERTIES_H
#define THINGPROPERTIES_H



#include <WiFiNINA.h>

// visit io.adafruit.com if you need tocreate an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME ""
#define IO_KEY ""
#define IO_GROUP "greenpeas"

char ssid[]     = "";       // Network SSID (name)
char pass[]     = "";       // Network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;
char server[] = "io.adafruit.com"; // name address for Adafruit IOT Cloud

WiFiClient client;

float aveVirusIndex, aveTemperature, aveHumidity, aveCo2, aveVoc, aveDust, aveLight, aveSound;
int virusIndex = round(aveVirusIndex);


void httpRequest() {
      
   /*
 * https://io.adafruit.com/api/docs/#operation/createGroupData
 * 
 * POST /{username}/groups/{group_key}/data
 * 
 * JSON:
 * 
{
  "location": {
    "lat": 0,
    "lon": 0,
    "ele": 0
  },
  "feeds": [
    {
      "key": "string",
      "value": "string"
    }
  ],
  "created_at": "string"
}
 */
//  const size_t capacity = JSON_ARRAY_SIZE(3) + 3*JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(3) + 130;
  const size_t capacity = JSON_ARRAY_SIZE(8) + 9*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 159;
  StaticJsonDocument<capacity> doc;
   // Add the "location" object
  JsonObject location = doc.createNestedObject("location");
  location["lat"] = 0;
  location["lon"] = 0;
  location["ele"] = 0;
  
  // Add the "feeds" array
  JsonArray feeds = doc.createNestedArray("feeds");
  
  JsonObject feed1 = feeds.createNestedObject();
  feed1["key"] = "temperature";
  feed1["value"] = aveTemperature;
  
  JsonObject feed2 = feeds.createNestedObject();
  feed2["key"] = "humidity";
  feed2["value"] = aveHumidity;
  
  JsonObject feed3 = feeds.createNestedObject();
  feed3["key"] = "co2";
  feed3["value"] = aveCo2;
  
  JsonObject feed4 = feeds.createNestedObject();
  feed4["key"] = "voc";
  feed4["value"] = aveVoc;
  
  JsonObject feed5 = feeds.createNestedObject();
  feed5["key"] = "dust";
  feed5["value"] = aveDust;
  
  JsonObject feed6 = feeds.createNestedObject();
  feed6["key"] = "light";
  feed6["value"] = aveLight;
  
  JsonObject feed7 = feeds.createNestedObject();
  feed7["key"] = "sound";
  feed7["value"] = aveSound;
  
  JsonObject feed8 = feeds.createNestedObject();
  feed8["key"] = "index";
  feed8["value"] = aveVirusIndex;

  
  // close any connection before send a new request.
  // This will free the socket on the Nina module
  client.stop();
  Serial.println("\nStarting connection to server...");
  if (client.connect(server, 80)) 
  {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("POST /api/v2/" IO_USERNAME "/groups/" IO_GROUP "/data HTTP/1.1"); 
    client.println("Host: io.adafruit.com");  
    client.println("Connection: close");  
    client.print("Content-Length: ");  
    client.println(measureJson(doc));  
    client.println("Content-Type: application/json");  
    client.println("X-AIO-Key: " IO_KEY); 
    // Terminate headers with a blank line
    client.println();
    // Send JSON document in body
    serializeJson(doc, client);
    // note the time that the connection was made:
    Serial.println("data sent!");
    
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed!");
  }
    
}

#endif /* THINGPROPERTIES_H */
