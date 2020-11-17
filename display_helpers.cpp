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

  if ((millis() - lastDebounceTime) >= debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        displayValuesOnTFT();
        lastScrnTime = millis();
      } 
    }
  }

  if (buttonPos > 0) {
    if ((millis() - lastScrnTime) >= scrnDisplayReset) {
      buttonPos = 0;
      displayValuesOnTFT();
      lastScrnTime = millis();
    }
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

int colorPicker (int index) {
  int color;
  index = round(index);
  
  if (index == -1) {
      color=GREY;
  } else if (index == 0) {
      color=GREEN;
  } else if (index == 1) {
      color=YELLOW;
  } else if (index == 2) {
      color=ORANGE;
  } else {
      color=RED;
  }

  return color;
}


void (*displayMenuPointer[])() = {screenMain, screenTemperature, screenHumidity, screenCo2, screenVoc, screenDust, screenLight, screenSound};

void displayValuesOnTFT() {
  clearTFT();

  displayMenuPointer[buttonPos++]();
  if (buttonPos >= 8) {
    buttonPos = 0;
  }
}

void updateValuesOnTFT() {
  screenMain();
}


void screenMain() {
  
  char _virusIndex[6];      // float to char conversion
  dtostrf(aveSensorData[0], 1, 0, _virusIndex);

  histDraw();
  tft.fillCircle(64,62,24, colorPicker(aveSensorIAQIndex[0]));

  tft.drawBitmap(0,0, tempIcon, 35, 35, colorPicker(aveSensorIAQIndex[1]));
  tft.drawBitmap(93, 0, humIcon, 35, 35, colorPicker(aveSensorIAQIndex[2]));
  tft.drawBitmap(93, 93, gasIcon, 35, 35, colorPicker(aveSensorIAQIndex[3]));
  tft.drawBitmap(0, 93, coIcon, 35, 35, colorPicker(aveSensorIAQIndex[4]));

  drawText(50,45,_virusIndex, BLACK,2);
  drawText(63,53,"/10", BLACK,1);
  drawText(50,63,"VIRUS", BLACK,1);
  drawText(50,73,"SCORE", BLACK,1);  

  drawText(5,55,"-9", ST77XX_WHITE,1);
  drawText(5,65,"HR", ST77XX_WHITE,1);

  drawText(58,4,"-6", ST77XX_WHITE,1);
  drawText(58,14,"HR", ST77XX_WHITE,1);

  drawText(112,55,"-3", ST77XX_WHITE,1);
  drawText(112,65,"HR", ST77XX_WHITE,1);

  drawText(58,107,"NOW", ST77XX_WHITE,1);
  
} /* void displayValuesOnTFT() */


void screenTemperature() {
   
  char _temperature[6];     // float to char conversion
  dtostrf(aveSensorData[1], 2, 0, _temperature);
  
  tft.drawBitmap(24, 12, tempIconLg, 80, 80, BLACK, colorPicker(aveSensorIAQIndex[1]));
 
  drawText(20,100, "Temp", WHITE, 1);
  drawText(45,100, _temperature, WHITE, 3);
  drawText(85,100,"C", WHITE, 2);
}

void screenHumidity() {
  
  char _humidity[6];     // float to char conversion
  dtostrf(aveSensorData[2], 2, 0, _humidity);

  tft.drawBitmap(24, 12, humIconLg, 80, 80, BLACK, colorPicker(aveSensorIAQIndex[2]));

  drawText(20,100, "Hum", WHITE, 1);
  drawText(45,100, _humidity, WHITE, 3);
  drawText(85,100,"%", WHITE, 2);
}

void screenCo2() {
 
  char _eco2[6];     // float to char conversion
  dtostrf(aveSensorData[3], 2, 0, _eco2);

   tft.drawBitmap(24, 12, coIconLg, 80, 80, BLACK, colorPicker(aveSensorIAQIndex[3]));
  
  drawText(20,100, "CO2", WHITE, 1);
  drawText(40,100, _eco2, WHITE, 3);
  drawText(95,100,"ppm", WHITE, 1);
}

void screenDust() {
  
  char _dustcon[6];     // float to char conversion
  dtostrf(aveSensorData[4], 2, 0, _dustcon);

  tft.drawBitmap(24, 12, dustIconLg, 80, 80, BLACK, colorPicker(aveSensorIAQIndex[4]));
  
  drawText(20,100, "PM2.5: ", WHITE, 1);
  drawText(45,100, _dustcon, WHITE, 3);
  drawText(85,100,"um/m3", WHITE, 1);
}

void screenVoc() {
  
  char _tvoc[6];     // float to char conversion
  dtostrf(aveSensorData[5], 2, 0, _tvoc);

  tft.drawBitmap(24, 12, vocIconLg, 80, 80, BLACK, colorPicker(aveSensorIAQIndex[5]));

  drawText(20,100, "VOC", WHITE, 1);
  drawText(40,100, _tvoc, WHITE, 3);
  drawText(92,100,"ppb", WHITE, 1);
}

void screenLight() {
  char _lux[6];     // float to char conversion
  dtostrf(aveSensorData[6], 2, 0, _lux);

  tft.drawBitmap(24, 12, lightIconLg, 80, 80, BLACK, colorPicker(aveSensorIAQIndex[6]));
  
  drawText(20,100, "Light ", WHITE, 1);
  drawText(45,100, _lux, WHITE, 3);
  drawText(92,100,"lux", WHITE, 2);
}

void screenSound() {
  char _sound[6];     // float to char conversion
  dtostrf(aveSensorData[7], 2, 0, _sound);

  tft.drawBitmap(24, 12, soundIconLg, 80, 80, BLACK, colorPicker(aveSensorIAQIndex[7]));
  
  drawText(20,100, "Sound", WHITE, 1);
  drawText(45,100, _sound, WHITE, 3);
  drawText(95,100," ", WHITE, 2);
}

void histIndex() {

}

void histDraw() {
  tft.drawBitmap(0, 0, hist0, 128, 128, colorPicker(histogramSensorIAQIndex[0]));
  tft.drawBitmap(0, 0, hist1, 128, 128, colorPicker(histogramSensorIAQIndex[1]));
  tft.drawBitmap(0, 0, hist2, 128, 128, colorPicker(histogramSensorIAQIndex[2]));
  tft.drawBitmap(0, 0, hist3, 128, 128, colorPicker(histogramSensorIAQIndex[3]));
  tft.drawBitmap(0, 0, hist4, 128, 128, colorPicker(histogramSensorIAQIndex[4]));
  tft.drawBitmap(0, 0, hist5, 128, 128, colorPicker(histogramSensorIAQIndex[5]));
  tft.drawBitmap(0, 0, hist6, 128, 128, colorPicker(histogramSensorIAQIndex[6]));
  tft.drawBitmap(0, 0, hist7, 128, 128, colorPicker(histogramSensorIAQIndex[7]));
  tft.drawBitmap(0, 0, hist8, 128, 128, colorPicker(histogramSensorIAQIndex[8]));
  tft.drawBitmap(0, 0, hist9, 128, 128, colorPicker(histogramSensorIAQIndex[9]));
  tft.drawBitmap(0, 0, hist10, 128, 128, colorPicker(histogramSensorIAQIndex[10]));
}

void drawText(int x, int y, char *text, uint16_t color, int size) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setTextSize(size);
  tft.print(text);
  
} /* void drawText() */
