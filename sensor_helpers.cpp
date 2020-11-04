#include "sensor_helpers.h"

/* Instances */
Adafruit_SGP30 sgp;
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();


float currDust, currCo2, currVoc, currTemperature, currHumidity, currLight, currSound;
int counter = 0;      //change VOC sensor to baseline via millis()


/* IO Pins */
const int pinSound = A3;
const int pinDustVolt = A0;
const int pinDustLed = 1;




/////////////////////////////////
// Setup Sensors
/////////////////////////////////

void beginSensors() {

  /* Initialize VOC and CO2 Sensor */
  if (! sgp.begin()){
    Serial.println("VOC/CO2 sensor not found");
    while (1);
  } 
  Serial.println("VOC/CO2 sensor found");

  /* Initialize Light Sensor */
  if (! veml.begin()) {
    Serial.println("Light sensor not found");
    while (1);
  } 
  Serial.println("Light sensor found");

  configureLight();

  /* Initialize Temp and Humidity Sensor */
  if (! shtc3.begin()) {
    Serial.println("Temperature/Humidity sensor not found");
    while (1) delay(1);
  }
  Serial.println("Temperature/Humidity sensor found");


  /* Prepare Dust Sensor */
  pinMode(pinDustLed, OUTPUT);
  digitalWrite(pinDustLed, LOW);  
 
} /* end beginSensors() */




/////////////////////////////////
// Initiate Sensor Data Collection
/////////////////////////////////

void readSensors() {
  getVOC();
  getLight();
  getSound();
  getTemp();
  getDust();

} /* end readSensors() */




/////////////////////////////////
// Print Current Sensor Data
/////////////////////////////////

void printSensors() {
    Serial.print("Dust: "); Serial.print(currDust); Serial.println(" ug/m3");  
    Serial.print("Temp: "); Serial.print(currTemperature); Serial.println(" F");
    Serial.print("Humidity: "); Serial.print(currHumidity); Serial.println("%");
    Serial.print("VOC: "); Serial.print(currVoc); Serial.println(" ppb");
    Serial.print("CO2: "); Serial.print(currCo2); Serial.println( " ppm");
    Serial.print("Lux: "); Serial.print(currLight); Serial.println( " lux");
    Serial.print("Sound: "); Serial.println(currSound);
} /* end printSensors() */


/////////////////////////////////
// Print Average Sensor Data
/////////////////////////////////

void printSensorsAve() {

    Serial.println("/////////////////////////////////");
    Serial.print("Dust Average: "); Serial.print(sensorArrayAve[0]); Serial.println(" ug/m3");  
    Serial.print("Temp Average: "); Serial.print(sensorArrayAve[3]); Serial.println(" F");
    Serial.print("Humidity Average: "); Serial.print(sensorArrayAve[4]); Serial.println("%");
    Serial.print("VOC Average: "); Serial.print(sensorArrayAve[2]); Serial.println(" ppb");
    Serial.print("CO2 Average: "); Serial.print(sensorArrayAve[1]); Serial.println( " ppm");
    Serial.print("Lux Average: "); Serial.print(sensorArrayAve[5]); Serial.println( " lux");
    Serial.print("Sound Average: "); Serial.println(sensorArrayAve[6]);
    Serial.println("/////////////////////////////////");
} /* end printSensors() */



/////////////////////////////////
// Get Sensor Data 
/////////////////////////////////
/////////////////////////////////
// VOC AND CO2
void getVOC() {
  // If you have a temperature / humidity sensor, you can set the absolute humidity to enable the humditiy compensation for the air quality signals
  //float temperature = 22.1; // [°C]
  //float humidity = 45.2; // [%RH]
  //sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));

  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }

  if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }

  currVoc = sgp.TVOC;
  currCo2 = sgp.eCO2;
  
  counter++;
  if (counter == 30) {
    counter = 0;

    uint16_t TVOC_base, eCO2_base;
    if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }
    Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
    Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
  }
  
}
// end VOC AND CO2
/////////////////////////////////




/////////////////////////////////
// LIGHT
void getLight() {
  currLight = float(veml.readLux());
   
  uint16_t irq = veml.interruptStatus();
  if (irq & VEML7700_INTERRUPT_LOW) {
    Serial.println("** Low threshold"); 
  }
  if (irq & VEML7700_INTERRUPT_HIGH) {
    Serial.println("** High threshold"); 
  } 
}
// end LIGHT 
/////////////////////////////////
/////////////////////////////////
// LIGHT HELPER
void configureLight() {
  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_800MS);
 
  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);
}
// end LIGHT HELPER
/////////////////////////////////




/////////////////////////////////
// SOUND
void getSound() {
  long sum = 0;
  
    for(int i=0; i<32; i++) {
        sum += analogRead(pinSound);
    }
 
    sum >>= 5;
    currSound = sum;
 
}
// end SOUND
/////////////////////////////////




/////////////////////////////////
// TEMPERATURE AND HUMIDITY
void getTemp() {
  float celsTemperature;
  
  sensors_event_t hum, temp;
  shtc3.getEvent(&hum, &temp);// populate temp and humidity objects with fresh data

  celsTemperature = float(temp.temperature);
  currTemperature = ((celsTemperature * 9/5) + 32);
  currHumidity = float(hum.relative_humidity);
}
// end TEMPERATURE AND HUMIDITY 
/////////////////////////////////




/////////////////////////////////
// DUST
void getDust() {
  const float voltRatio = 0.2;
  const int noDustVoltage = 400;
  const int sysVoltage = 5000;
  float density, voltage;
  int adcValue;

  digitalWrite(pinDustLed, HIGH);
  delayMicroseconds(280);
  
  adcValue = analogRead(pinDustVolt);
  digitalWrite(pinDustLed, LOW);

  adcValue = calcDustFilter(adcValue);

  voltage = (sysVoltage / 1024.0) * adcValue * 11;   //convert voltage (mv)

  if(voltage >= noDustVoltage) {                    //convert voltage to dust density
    voltage -= noDustVoltage;
    density = voltage * voltRatio;
  } else {
    density = 0;
  }
  currDust = density;
}
// end DUST
/////////////////////////////////
/////////////////////////////////
// DUST HELPER
int calcDustFilter(int m) {
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  
  if(flag_first == 0) {
    flag_first = 1;
    for(i = 0, sum = 0; i < _buff_max; i++) {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  } else {
    sum -= _buff[0];
    for(i = 0; i < (_buff_max - 1); i++) {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    i = sum / 10.0;
    return i;
  }
  
}
// end DUST HELPER 
/////////////////////////////////
