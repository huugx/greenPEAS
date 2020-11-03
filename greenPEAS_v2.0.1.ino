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
#include "thingProperties.h"
#include "displayTFT.h"
#include "virusIndex.h"
#include "Adafruit_VEML7700.h"
#include "Adafruit_SHTC3.h"
#include "Adafruit_SGP30.h"

/* Definitions */
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();
Adafruit_VEML7700 veml = Adafruit_VEML7700();
Adafruit_SGP30 sgp;


/* Global Variables */
const int iled = 1;                       //drive the Dust Sensor led 
const int vout = A0;                      //analog input
const int buttonPin = 6;                  //button for screen rotating
const int pinSound = A3;

#define COV_RATIO  0.2                    //ug/mmm / mv
#define NO_DUST_VOLTAGE 400               //mv
#define SYS_VOLTAGE 3300

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int btnPos = 0;                         //position of btnScreen
int   adcvalue;
int counter = 0;              //tvoc baseline counter

float density;
float voMeasured = 0;
float calcVoltage = 0;
float raw_eco2;
float pre_eco2 = 400;

bool wificon = false;

unsigned long lastConnectionTime = 0;              // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 1000;       // delay between updates, in milliseconds
unsigned long lastDisplayTime = 0;                //last time you updated the display
const unsigned long displayInterval = 5000;        //delay between screen updates
unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long lastScrnTime = 0;
const unsigned long scrnDisplayReset = 15000;


float value;
float voltage;


/* Functions */
float DustDensity();
uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
}

void setup() {
//  digitalWrite(pin, LOW);
  
  Serial.begin(9600);
  delay(1500); 

  initProperties();                                   // Initialize Sensors (see thingProperties.h)
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);  // Connect to Arduino IoT Cloud
   
  tft.initR(INITR_144GREENTAB);                       // Initialize TFT Display
  tft.fillScreen(ST77XX_WHITE);
  
  if (!veml.begin()) {
    Serial.println("Light sensor not found");
    while (1);
  }
  Serial.println("Light sensor found");
  
  startTFT();
  configureSensor();

  if (! shtc3.begin()) {
    Serial.println("Couldn't find SHTC3");
    while (1) delay(1);
  }
  Serial.println("Found SHTC3 sensor");

  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);
  
  pinMode(buttonPin, INPUT);
  pinMode(iled, OUTPUT);
  digitalWrite(iled, LOW);                            //iled default closed

//  /*PIR SENSOR*/
//  pinMode(inputPin, INPUT);
//  takeLowTime = LOW; 

  
//  LowPower.attachInterruptWakeup(inputPin, wakeUp, CHANGE);
  
  delay(10000);                                       // Sensor calibration delay

} /* void setup() */



void loop() {
  digitalWrite(iled, LOW);                            //iled default closed
//  digitalWrite(LED_PIN, HIGH);


//button and screen rotating
   int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

//    // if the button state has changed:
//    if (reading != buttonState) {
//      buttonState = reading;
//
//      // only toggle the LED if the new button state is HIGH
//      if (buttonState == HIGH) {
//        btnPos++;
//      }
//    }

    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH && btnPos < 7) {
        btnPos++;
        lastScrnTime = millis();
        displayValuesOnTFT();
      } else if (buttonState == HIGH && btnPos == 7) {
        btnPos = 0;
        lastScrnTime = millis();
        displayValuesOnTFT();
      }
    }
  }

  if ((millis() - lastScrnTime) > scrnDisplayReset) {
    btnPos = 0;
  }
  
  lastButtonState = reading;
//end button and screen rotating


  ArduinoCloud.update();
//  delay(2000);
  

  if (millis() - lastConnectionTime > postingInterval) {
    readSensors();
    safeIndex();
//    histIndex();
//    histDraw();
    
      /*
    get adcvalue
    */
    digitalWrite(iled, HIGH);
    delayMicroseconds(280);
    adcvalue = analogRead(vout);
    digitalWrite(iled, LOW);
  
  
    adcvalue = Filter(adcvalue);
    
//    Serial.print("The current voltage is: ");
//    Serial.println(adcvalue);
   
    /*
    covert voltage (mv)
    */
    voltage = (SYS_VOLTAGE / 1024.0) * adcvalue * 11;
    
    /*
    voltage to density
    */
    if(voltage >= NO_DUST_VOLTAGE)
    {
      voltage -= NO_DUST_VOLTAGE;
      
      density = voltage * COV_RATIO;
    }
    else
      density = 0;
      
    /*
    display the result
    */
  
    
    Serial.print("Dust: "); Serial.print(density); Serial.println(" ug/m3");  
    Serial.print("Temp: "); Serial.println(temperature);
    Serial.print("Humidity: "); Serial.println(humidity);
    Serial.print("VOC: "); Serial.print(tvoc); Serial.println(" ppb");
    Serial.print("CO2: "); Serial.print(eco2); Serial.println( "ppm");
    Serial.print("Lux: "); Serial.println(lux);
    Serial.print("Sound: "); Serial.println(sound);
    
    dustcon = density;
    
    lastConnectionTime = millis();

  }
  
  if (millis() - lastDisplayTime > displayInterval) {
    displayValuesOnTFT();

    lastDisplayTime = millis();
  }
  
}  /* void loop() */



void readSensors() {
////  pressure = float(ENV.readPressure());
////  lux = float(ENV.readLux());
//  ess.measureIAQ(); // measure first to ensure proper timing
//  ess.measureRHT();
// 
//  tvoc = ess.getTVOC();
//  raw_eco2 = ess.getECO2(); // SGP30 only
//  humidity = ess.getHumidity();
//  temperature = ess.getTemperature();
//  
//  if (raw_eco2 == 65535.00){
//    eco2 = pre_eco2;
//  } else {
//    eco2 = raw_eco2;
//  }
//  pre_eco2 = eco2;

  //temp and humidity sensor
  sensors_event_t hum, temp;

  shtc3.getEvent(&hum, &temp);

  temperature = float(temp.temperature);
  humidity = float(hum.relative_humidity);

  //light sensor
  lux = float(veml.readLux());
 
  uint16_t irq = veml.interruptStatus();
  if (irq & VEML7700_INTERRUPT_LOW) {
    Serial.println("** Low threshold"); 
  }
  if (irq & VEML7700_INTERRUPT_HIGH) {
    Serial.println("** High threshold"); 
  }
  delay(500);

  //sound sensor
  long sum = 0;
    for(int i=0; i<32; i++)
    {
        sum += analogRead(pinSound);
    }
 
    sum >>= 5;
    sound = sum;

  //voc & co2 sensor
  sgp.setHumidity(getAbsoluteHumidity(temperature, humidity));
  
  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }


  tvoc = sgp.TVOC;
  eco2 = sgp.eCO2; 

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

  

  
//
//  dustcon = DustDensity();
//  Serial.println(dustcon);
}

void storeData() {
  
  oldDustcon = dustcon;
  oldEco2 = eco2;
  oldHumidity = humidity; 
  oldTemperature = temperature;
  oldVirus = virusIndex;


//  Serial.print("NEW CO2: ");
//  Serial.println(oldDustcon);
//  
//  Serial.print("OLD CO2: ");
//  Serial.println(dustcon);
  
} /* void storeData() */




int Filter(int m)
{
  static int flag_first = 0, _buff[10], sum;
  const int _buff_max = 10;
  int i;
  
  if(flag_first == 0)
  {
    flag_first = 1;

    for(i = 0, sum = 0; i < _buff_max; i++)
    {
      _buff[i] = m;
      sum += _buff[i];
    }
    return m;
  }
  else
  {
    sum -= _buff[0];
    for(i = 0; i < (_buff_max - 1); i++)
    {
      _buff[i] = _buff[i + 1];
    }
    _buff[9] = m;
    sum += _buff[9];
    
    i = sum / 10.0;
    return i;
  }
}

float DustDensity() {
  /*
  get adcvalue
  */
  digitalWrite(iled, HIGH);
  delayMicroseconds(280);
  adcvalue = analogRead(vout);
  digitalWrite(iled, LOW);


  adcvalue = Filter(adcvalue);

  /*
  covert voltage (mv)
  */
  voltage = (SYS_VOLTAGE / 1024.0) * adcvalue * 11;
  
  /*
  voltage to density
  */
  if(voltage >= NO_DUST_VOLTAGE)
  {
    voltage -= NO_DUST_VOLTAGE;
    
    density = voltage * COV_RATIO;
  }
  else
    density = 0;

  return density;
  /*
  display the result
  */

//  
  Serial.print("The current dust concentration is: ");
  Serial.print(density);
  Serial.print(" ug/m3\n");  
//  
//  delay(1000);
}

void configureSensor() {
  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_800MS);
 
  Serial.print(F("Gain: "));
  switch (veml.getGain()) {
    case VEML7700_GAIN_1: Serial.println("1"); break;
    case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }
 
  Serial.print(F("Integration Time (ms): "));
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS: Serial.println("25"); break;
    case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    case VEML7700_IT_200MS: Serial.println("200"); break;
    case VEML7700_IT_400MS: Serial.println("400"); break;
    case VEML7700_IT_800MS: Serial.println("800"); break;
  }
 
  //veml.powerSaveEnable(true);
  //veml.setPowerSaveMode(VEML7700_POWERSAVE_MODE4);
 
  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);
}
