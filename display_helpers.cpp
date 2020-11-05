#include "display_helpers.h"


/* Instances */
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

float _temperature, _humidity, _eco2, _tvoc, _dustcon;
float oldDustcon, oldEco2, oldHumidity, oldTemperature;
int oldVirus;
int vColor;


/////////////////////////////////
// NAVIGATION
/////////////////////////////////
/* 
 *  Monitors the Button pin and cycles
 *  
 *  through the screen
 *  
 */

const int buttonPin = 6;                  //button for screen rotating
int buttonState;                          // the current reading from the input pin
int lastButtonState = LOW;                // the previous reading from the input pin
int buttonPos = 0;                           //position of btnScreen
unsigned long lastDebounceTime = 0;       // the last time the output pin was toggled
unsigned long debounceDelay = 30;         // the debounce time; increase if the output flickers
unsigned long lastScrnTime = 0;
const unsigned long scrnDisplayReset = 15000;



void debounceButton() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        displayValuesOnTFT();
        lastScrnTime = millis();
      } 
    }
  }

  if ((millis() - lastScrnTime) > scrnDisplayReset) {
    buttonPos = 0;
  }
  
  lastButtonState = reading;
  
}


/////////////////////////////////
// Initiate Screen
/////////////////////////////////

void initTFT() {
  tft.initR(INITR_144GREENTAB);                       // Initialize TFT Display
  tft.fillScreen(ST77XX_WHITE);
  
  delay(100);

  pinMode(buttonPin, INPUT);
  pinMode(TFT_Lite, OUTPUT);          //Backlight
  digitalWrite(TFT_Lite, HIGH);
  
  tft.drawBitmap(0, 0, startScreen, 128, 128, WHITE, BLACK);
}



/////////////////////////////////
// Clear Screen
/////////////////////////////////

void clearTFT() {
  
  tft.fillRect(0, 0, 128, 128, ST77XX_BLACK);
  
}



/////////////////////////////////
// Display
/////////////////////////////////
void (*displayMenuPointer[])() = {screenMain, screenTemperature, screenHumidity, screenCo2, screenVoc, screenDust, screenLight, screenSound};

void displayValuesOnTFT() {
  clearTFT();
  
  displayMenuPointer[buttonPos++]();
  if (buttonPos >= 8) {
    buttonPos = 0;
  }
  Serial.println(buttonPos);
}


void screenMain() {
  Serial.println("main screen");
  
//  extern int virusIndex, tempIndex, humIndex, co2Index, dustIndex;

//  char _virusIndex[6];      // float to char conversion

//  dtostrf(virusIndex, 1, 0, _virusIndex);


//  // tft.fillScreen(ST77XX_BLACK);
//  // tft.fillRect(0, 50, 120, 50, indexColor);
//  if (virusIndex <= 2) {
//    vColor=GREEN;
//  } else if (virusIndex > 2 && virusIndex <= 6) {
//    vColor=YELLOW;
//  } else if (virusIndex >= 7 && virusIndex < 9) {
//    vColor=ORANGE;
//  } else {
//    vColor=RED;
//  }
//
//  tft.fillCircle(64,62,24, vColor); //use with history
//  tft.fillCircle(64,62,28, vColor);
//  tft.drawCircle(64,62,30, vColor);
//  tft.drawCircle(64,62,31, vColor);
//  
//  
//  if (tempIndex < 1) {
//    tft.drawBitmap(0, 0, tempIcon, 35, 35, GREEN);
//  } else if (tempIndex = 1) {
//    tft.drawBitmap(0, 0, tempIcon, 35, 35, YELLOW);
//  } else {
//    tft.drawBitmap(0, 0, tempIcon, 35, 35, RED);
//  }
//
//  if (humIndex < 1) {
//    tft.drawBitmap(93, 0, humIcon, 35, 35, GREEN);
//  } else if (humIndex = 1) {
//    tft.drawBitmap(93, 0, humIcon, 35, 35, YELLOW);
//  } else {
//    tft.drawBitmap(93, 0, humIcon, 35, 35, RED);
//  }
//  
//  if (dustIndex < 1) {
//    tft.drawBitmap(93, 93, gasIcon, 35, 35, GREEN);
//  } else if (dustIndex = 1) {
//    tft.drawBitmap(93, 93, gasIcon, 35, 35, YELLOW);
//  } else if (dustIndex = 2) {
//    tft.drawBitmap(93, 93, gasIcon, 35, 35, ORANGE);
//  } else {
//    tft.drawBitmap(93, 93, gasIcon, 35, 35, RED);
//  }
//  
//  if (co2Index < 2) {
//    tft.drawBitmap(0, 93, coIcon, 35, 35, GREEN);
//  } else if (co2Index = 2) {
//    tft.drawBitmap(0, 93, coIcon, 35, 35, ORANGE);
//  } else {
//    tft.drawBitmap(0, 93, coIcon, 35, 35, RED);
//  }
//  
//  drawText(50,45,_virusIndex, BLACK,2);
//  drawText(63,53,"/10", BLACK,1);
//  drawText(50,63,"VIRUS", BLACK,1);
//  drawText(50,73,"SCORE", BLACK,1);  
} /* void displayValuesOnTFT() */


void screenTemperature() {
  Serial.println("temp screen");
  
//  extern int tempIndex;
//  extern float temperature;
//
//  char _temperature[6];     // float to char conversion
//  
//  dtostrf(temperature, 2, 0, _temperature);
//  
//  if (tempIndex < 1) {
//    tft.drawBitmap(24, 12, tempIconLg, 80, 80, BLACK, GREEN);
//  } else if (tempIndex = 1) {
//    tft.drawBitmap(24, 12, tempIconLg, 80, 80, BLACK, YELLOW);
//  } else {
//    tft.drawBitmap(24, 12, tempIconLg, 80, 80, BLACK, RED);
//  }  
//  
//  drawText(20,100, "Temp", WHITE, 1);
//  drawText(45,100, _temperature, WHITE, 3);
//  drawText(85,100,"C", WHITE, 2);
}

void screenHumidity() {
  Serial.println("hum screen");
  
//  extern int humIndex;
//  extern float humidity;
//
//  char _humidity[6];     // float to char conversion
//  
//  dtostrf(humidity, 2, 0, _humidity);
//
//  if (humIndex < 1) {
//    tft.drawBitmap(24, 12, humIconLg, 80, 80, BLACK, GREEN);
//  } else if (humIndex = 1) {
//    tft.drawBitmap(24, 12, humIconLg, 80, 80, BLACK, YELLOW);
//  } else {
//    tft.drawBitmap(24, 12, humIconLg, 80, 80, BLACK, RED);
//  }  
//
//
//  drawText(20,100, "Hum", WHITE, 1);
//  drawText(45,100, _humidity, WHITE, 3);
//  drawText(85,100,"%", WHITE, 2);
}

void screenCo2() {
  Serial.println("co2 screen");
  
//  extern int co2Index;
//  extern float eco2;
//
//  char _eco2[6];     // float to char conversion
//  
//  dtostrf(eco2, 2, 0, _eco2);
//
//  if (co2Index < 2) {
//    tft.drawBitmap(24, 12, coIconLg, 80, 80, BLACK, GREEN);
//  } else if (co2Index = 2) {
//    tft.drawBitmap(24, 12, coIconLg, 80, 80, BLACK, YELLOW);
//  } else {
//    tft.drawBitmap(24, 12, coIconLg, 80, 80, BLACK, RED);
//  } 
//  
//  drawText(20,100, "CO2", WHITE, 1);
//  drawText(40,100, _eco2, WHITE, 3);
//  drawText(95,100,"ppm", WHITE, 1);
}

void screenDust() {
  Serial.println("dust screen");
  
//  extern int dustIndex;
//  extern float dustcon;
//
//  char _dustcon[6];     // float to char conversion
//  
//  dtostrf(dustcon, 2, 0, _dustcon);
//
//  if (dustIndex < 1) {
//    tft.drawBitmap(24, 12, gasIconLg, 80, 80, BLACK, GREEN);
//  } else if (dustIndex = 1) {
//    tft.drawBitmap(24, 12, gasIconLg, 80, 80, BLACK, YELLOW);
//  } else if (dustIndex = 2) {
//    tft.drawBitmap(24, 12, gasIconLg, 80, 80, BLACK, ORANGE);
//  } else {
//    tft.drawBitmap(24, 12, gasIconLg, 80, 80, BLACK, RED);
//  }
//  
//  drawText(20,100, "PM2.5: ", WHITE, 1);
//  drawText(45,100, _dustcon, WHITE, 3);
//  drawText(85,100,"um/m3", WHITE, 1);
}

void screenVoc() {
  Serial.println("voc screen");
  
//  extern float tvoc;
//
//  char _tvoc[6];     // float to char conversion
//  
//  dtostrf(tvoc, 2, 0, _tvoc);
//
//  drawText(20,100, "VOC", WHITE, 1);
//  drawText(40,100, _tvoc, WHITE, 3);
//  drawText(92,100,"ppb", WHITE, 1);
}

void screenLight() {
  Serial.println("light screen");
  
//  extern float lux;
//
//  char _lux[6];     // float to char conversion
//  
//  dtostrf(lux, 2, 0, _lux);
//  
//  drawText(20,100, "Light ", WHITE, 1);
//  drawText(45,100, _lux, WHITE, 3);
//  drawText(92,100,"lux", WHITE, 2);
}

void screenSound() {
  Serial.println("sound screen");
  
//  extern float sound;
//
//  char _sound[6];     // float to char conversion
//  
//  dtostrf(sound, 2, 0, _sound);
//  
//  drawText(20,100, "Sound", WHITE, 1);
//  drawText(45,100, _sound, WHITE, 3);
//  drawText(95,100," ", WHITE, 2);
}

void histIndex() {

}


void histDraw() {


}

void drawText(int x, int y, char *text, uint16_t color, int size) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setTextSize(size);
  tft.print(text);
  
} /* void drawText() */


void wifiStatus() {
  if (WiFi.status() == 3) {
    tft.fillCircle(64,5,2, GREEN);
  } else {
    tft.fillCircle(64,5,2, RED);
  }
}
