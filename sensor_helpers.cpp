#include "sensor_helpers.h"

/* Instances */
Adafruit_SGP30 sgp;
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();




float currSensorData[8];
const int sensorQty = (sizeof(currSensorData)/sizeof(currSensorData[0]));
float aveSensorData[sensorQty];
float aveSensorIAQIndex[sensorQty];

int readSensorIndex = 0;
int storeIAQIndexCounter = 0;
float storeIAQIndexTotal = 0;
int storeHistogramIAQIndexCounter = 0;
int avePrevHourIAQIndex = 0;

float readSensorTotal[sensorQty];
float dataSensorArray[smoothDataInterval][sensorQty];
float prevHourSensorIAQIndex[prevHourDataInterval];
float histogramSensorIAQIndex[histogramDataInterval];


//float currDust, currCo2, currVoc, currTemperature, currHumidity, currLight, currSound;        //FIXME: Remove
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





float linearRegressionIAQ [][2] = {
    {0.30137, 0.06849},              // Virus Index
    {0.134741784, -9.335680751},      // Temperature
    {0.1, -5},                        // Humidity
    {0.001640832, 0.152173913},       // CO2
    {0.001331435, 0.67023051},        // VOC
    {0.005246116, 0.771539296},       // Dust
    {-0.007215529, 3.613119143},      // Light
    {0.066435185, -2.34375}           // Sound
};


/** 
 *  Data Smooting Functions
 *  
 *  Averages the sensor data collected over a period
 *  
 *  @param 
 *  @param
 *  
 */

 
void initSmoothSensorData() {
  for (int i =0; i < smoothDataInterval; i++) {
    for (int j =0; j < sensorQty; j++) {
      dataSensorArray[i][j] = 0;
    }
  }

  for (int k =0; k < sensorQty; k++) {
      readSensorTotal[k] = 0;
    }

  for (int k =0; k < prevHourDataInterval; k++) {
      prevHourSensorIAQIndex[k] = 0;
    }

  for (int k =0; k < histogramDataInterval; k++) {
      histogramSensorIAQIndex[k] = -1;
    }
}


float (*getSensorCurr[])() = {getVirusIndex, getTemperature, getHumidity, getCo2, getVoc, getDust, getLight, getSound};


void smoothSensorData() {
  
  for (int i=0; i < sensorQty; i++) {
    currSensorData[i] = getSensorCurr[i]();     //Debug only - printSensor();
    // subtract the last reading:
    readSensorTotal[i] = readSensorTotal[i] - dataSensorArray[readSensorIndex][i];
    // read from the sensor:
    dataSensorArray[readSensorIndex][i] = getSensorCurr[i]();
    // add the reading to the readSensorTotal:
    readSensorTotal[i] = readSensorTotal[i] + dataSensorArray[readSensorIndex][i];
      
    aveSensorData[i] = readSensorTotal[i] / smoothDataInterval;

    if (aveSensorData[i] < 0) {
      aveSensorData[i] = 0;
    }
      
      aveSensorIAQIndex[i] = calculateSensorIndex (linearRegressionIAQ [i][0], linearRegressionIAQ [i][1], aveSensorData[i]);
  } //close i  
  
  // advance to the next position in the array:
  readSensorIndex++; 

    // if we're at the end of the array...
  if (readSensorIndex >= smoothDataInterval) {
    // ...wrap around to the beginning:
    readSensorIndex = 0;      
  } //close if (readSensorIndex....)

  storeIAQIndex();
    
}


void storeIAQIndex() {
   
  storeIAQIndexTotal = storeIAQIndexTotal - prevHourSensorIAQIndex[storeIAQIndexCounter];

  prevHourSensorIAQIndex[storeIAQIndexCounter] = aveSensorIAQIndex[0];

  storeIAQIndexTotal = storeIAQIndexTotal + prevHourSensorIAQIndex[storeIAQIndexCounter];
  
  storeIAQIndexCounter++;

  if (storeIAQIndexCounter >= prevHourDataInterval) {
      storeIAQIndexCounter = 0;        
  }  

  avePrevHourIAQIndex = storeIAQIndexTotal / prevHourDataInterval;
}


float calculateSensorIndex (float a, float b, float x) {        // y = ax + b
  float y = 0;
  
  y = abs((a*x) + b);


  return y; 
}

void storeHistogramIndex() {

  for (int i = 1; i < histogramDataInterval; i++) {
    histogramSensorIAQIndex[histogramDataInterval-(i)] = histogramSensorIAQIndex[histogramDataInterval-(i+1)];
  }
  
  histogramSensorIAQIndex[0] = avePrevHourIAQIndex;
  
  for (int i = 0; i < histogramDataInterval; i++) {
    Serial.println(histogramSensorIAQIndex[i]);
  }
}




/////////////////////////////////
// DEBUGGING
/////////////////////////////////
// Print Current Sensor Data
void printSensors() {
//    Serial.print("Virus Index: "); Serial.println(currSensorData[0]);
//    Serial.print("Temp: "); Serial.print(currSensorData[1]); Serial.println(" F");
//    Serial.print("Humidity: "); Serial.print(currSensorData[2]); Serial.println("%");
    Serial.print("CO2: "); Serial.print(currSensorData[3]); Serial.println( " ppm");
    Serial.print("VOC: "); Serial.print(currSensorData[4]); Serial.println(" ppb");
//    Serial.print("Dust: "); Serial.print(currSensorData[5]); Serial.println(" ug/m3");  
//    Serial.print("Lux: "); Serial.print(currSensorData[6]); Serial.println( " lux");
//    Serial.print("Sound: "); Serial.print(currSensorData[7]); Serial.println( " dB");
} /* end printSensors() */


/////////////////////////////////
// Print Average Sensor Data
/////////////////////////////////

void printSensorsAve() {

    Serial.println("/////////////////////////////////");
//    Serial.print("Virus Index: "); Serial.println(aveSensorData[0]);
//    Serial.print("Temp Average: "); Serial.print(aveSensorData[1]); Serial.println(" F");
//    Serial.print("Humidity Average: "); Serial.print(aveSensorData[2]); Serial.println("%");
    Serial.print("CO2 Average: "); Serial.print(aveSensorData[3]); Serial.println( " ppm");
    Serial.print("VOC Average: "); Serial.print(aveSensorData[4]); Serial.println(" ppb");
//    Serial.print("Dust Average: "); Serial.print(aveSensorData[5]); Serial.println(" ug/m3");  
//    Serial.print("Lux Average: "); Serial.print(aveSensorData[6]); Serial.println( " lux");
//    Serial.print("Sound Average: "); Serial.println(aveSensorData[7]);
    Serial.println("/////////////////////////////////");
} /* end printSensors() */

void printSafeIndex() {

    Serial.println("/////////////////////////////////");
    Serial.print("Virus Index: "); Serial.println(aveSensorData[0]);
//    Serial.print("Temp Average: "); Serial.println(aveSensorIAQIndex[1]);
//    Serial.print("Humidity Average: "); Serial.println(aveSensorIAQIndex[2]);
//    Serial.print("CO2 Average: "); Serial.println(aveSensorIAQIndex[3]);
//    Serial.print("VOC Average: "); Serial.println(aveSensorIAQIndex[4]);
//    Serial.print("Dust Average: "); Serial.println(aveSensorIAQIndex[5]);
//    Serial.print("Lux Average: "); Serial.println(aveSensorIAQIndex[6]);
//    Serial.print("Sound Average: "); Serial.println(aveSensorIAQIndex[7]);
    Serial.println("/////////////////////////////////");
} /* end printSensors() */


/////////////////////////////////
// POSTING
/////////////////////////////////


/////////////////////////////////
// Post Sensors to Cloud
/////////////////////////////////

float preDust = 0;



void postSensorsToCloud() {
    
    aveVirusIndex = aveSensorData[0];
    aveTemperature = aveSensorData[1];
    aveHumidity = aveSensorData[2];
    aveCo2 = aveSensorData[3];
    aveVoc = aveSensorData[4];
    triggerAveDustToPost();
    aveLight = aveSensorData[6];
    aveSound = aveSensorData[7];
}
/////////////////////////////////
// Trigger Dust Change for Posting
/////////////////////////////////
void triggerAveDustToPost() {
    float sampleDust = aveSensorData[5];

    if (sampleDust <= 0.01 && preDust == 0) {
        aveDust = 0.1;
    } else if (sampleDust <= 0.01 && preDust == 0.1) {
        aveDust = 0.00;
    } else {
      aveDust = sampleDust;
    } 
    preDust = aveDust;

}
// END POSTING
/////////////////////////////////



float getVirusIndex() {
  float currSample = 0;

  currSample = aveSensorIAQIndex[1] + aveSensorIAQIndex[2] + aveSensorIAQIndex[3] + aveSensorIAQIndex[5];

  return currSample;
}

/////////////////////////////////
// GET SENSOR DATA 
/////////////////////////////////
/////////////////////////////////
// VOC AND CO2
float currVoc;
float rawCo2;
float preCo2 = 400;     //debugging the effects of wifi chip on Co2

float getCo2() { 
  float currCo2;
  
  getAbsoluteHumidityCompensation();
  
  sgp.IAQmeasure();
  
  currVoc = sgp.TVOC;
  rawCo2 = sgp.eCO2;
 
  if (rawCo2 == 65535.00){
    currCo2 = preCo2;
  } else {
    currCo2 = rawCo2;
  }
  preCo2 = currCo2;

 float currSample = currCo2;
 return currSample;
}


float getVoc() { 
  
  float currSample = currVoc;          //FIXME: change this variable name
  return currSample;
  
}


void getAbsoluteHumidityCompensation() {
  
  float temperature = aveTemperature; // [°F]
  float humidity = aveHumidity; // [%RH]
  sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));
}


void getVocCo2Baseline() {
  
  counter++;
  if (counter == 30) {
    counter = 0;

    uint16_t TVOC_base, eCO2_base;
    if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }
//    Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
//    Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
  }
}
// end VOC AND CO2
/////////////////////////////////
/////////////////////////////////
// VOC AND CO2 HELPER
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}
// end VOC AND CO2 HELPER
/////////////////////////////////




/////////////////////////////////
// LIGHT
float getLight() {
 
  float currSample = float(veml.readLux());
  return currSample;

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
float getSound() {
  
  long sum = 0;
  
  for(int i=0; i<32; i++) {
      sum += analogRead(pinSound);
  }

  sum >>= 5;

//  long sum = analogRead(pinSound);
//  sum = (adc+83.2073)/11.003;         //rough conversion formula: https://circuitdigest.com/microcontroller-projects/arduino-sound-level-measurement
    

  int currSample = sum;
  return currSample;
}
// end SOUND
/////////////////////////////////




/////////////////////////////////
// TEMPERATURE AND HUMIDITY

float currHumidity;

float getTemperature() {
  
  float celsTemperature;
  
  sensors_event_t hum, temp;
  shtc3.getEvent(&hum, &temp);// populate temp and humidity objects with fresh data
  
  celsTemperature = float(temp.temperature);
  currHumidity = float(hum.relative_humidity);
  float currSample = ((celsTemperature * 9/5) + 32);
  return currSample;
  
}

float getHumidity() {

  float currSample = currHumidity;
  return currSample;
}
// end TEMPERATURE AND HUMIDITY 
/////////////////////////////////


/////////////////////////////////
// DUST
float getDust() {
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

  float currSample = density;
  return currSample;
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
