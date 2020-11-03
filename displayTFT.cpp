#include "displayTFT.h"

float _temperature, _humidity, _eco2, _tvoc, _dustcon;
float oldDustcon, oldEco2, oldHumidity, oldTemperature;
int oldVirus;
int _hist0 = 0;
int _hist1 = 0;
int _hist2 = 0;
int _hist3 = 0;
int _hist4 = 0;
int _hist5 = 0;
int _hist6 = 0;
int _hist7 = 0;
int _hist8 = 0;
int _hist9 = 0;
int _hist10 = 0;
int histArray[11];          //virus index history length
int _histArray[11];
int vColor;
int h0Color, h1Color;



#define TFT_RST       2
#define TFT_Lite      3
#define TFT_DC        4
#define TFT_CS        5

/* Color Definitions */
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define ORANGE   0xFCA0
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define GREY     0xAD75


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

void startTFT() {
  /*TFT Backlight*/
  pinMode(TFT_Lite, OUTPUT);
  digitalWrite(TFT_Lite, HIGH);
  
  tft.drawBitmap(0, 0, startScreen, 128, 128, WHITE, BLACK);
}


void clearTFT() {
  tft.fillRect(0, 0, 128, 128, ST77XX_BLACK);
}


void displayValuesOnTFT() {
  extern int btnPos;

  clearTFT();
  
  if (btnPos == 0) {
    scrnMain();
  }
  
  if (btnPos == 1) {
    scrnTemp();
  }

  if (btnPos == 2) {
    scrnHumidity();
  }

  if (btnPos == 3) {
    scrnCo2();
  }

  if (btnPos == 4) {
    scrnVOC();
  }
  
  if (btnPos == 5) {
    scrnPM25();
  }
  if (btnPos == 6) {
    scrnLight();
  }

  if (btnPos == 7) {
    scrnSound();
  }
}

void scrnMain() {

  extern int virusIndex, tempIndex, humIndex, co2Index, dustIndex;
  extern float dustcon, eco2, humidity, temperature;
  
  char _temperature[6];     // float to char conversion
  char _humidity[6];
  char _dustcon[6];
  char _eco2[6];
  char _virusIndex[6];
  
  dtostrf(temperature, 2, 0, _temperature);
  dtostrf(humidity, 2, 0, _humidity);
  dtostrf(dustcon, 2, 0, _dustcon);
  dtostrf(eco2, 3, 0, _eco2);
  dtostrf(virusIndex, 1, 0, _virusIndex);

  // Display on TFT
  // tft.fillScreen(ST77XX_BLACK);
  // tft.fillRect(0, 50, 120, 50, indexColor);
  if (virusIndex <= 2) {
    vColor=GREEN;
    h0Color=GREEN;
  } else if (virusIndex > 2 && virusIndex <= 6) {
    vColor=YELLOW;
    h0Color=YELLOW;
  } else if (virusIndex >= 7 && virusIndex < 9) {
    vColor=ORANGE;
    h0Color=ORANGE;
  } else {
    vColor=RED;
    h0Color=RED;
  }

  tft.fillCircle(64,62,24, vColor); //use with history
  tft.fillCircle(64,62,28, vColor);
  tft.drawCircle(64,62,30, vColor);
  tft.drawCircle(64,62,31, vColor);
  
//  tft.drawBitmap(0, 0, hist0, 128, 128, h0Color);
//  
//  if (_hist1 = 0) {
//    h1Color = BLACK;
//  } 
// if (_hist1 > 0 && _hist1 <= 2) {
//    h1Color = GREEN;
//  } 
// if (_hist1 >= 2 && _hist1 < 5) {
//    h1Color = YELLOW;
//  } 
// if (_hist1 >= 5 && _hist1 <= 7) {
//    h1Color = ORANGE;
//  } 
// if (_hist1 > 7) {
//    h1Color = RED;
//  }


//  tft.drawBitmap(0, 0, hist1, 128, 128, h1Color);
  
  if (tempIndex < 1) {
    tft.drawBitmap(0, 0, tempIcon, 35, 35, GREEN);
  } else if (tempIndex = 1) {
    tft.drawBitmap(0, 0, tempIcon, 35, 35, YELLOW);
  } else {
    tft.drawBitmap(0, 0, tempIcon, 35, 35, RED);
  }

  if (humIndex < 1) {
    tft.drawBitmap(93, 0, humIcon, 35, 35, GREEN);
  } else if (humIndex = 1) {
    tft.drawBitmap(93, 0, humIcon, 35, 35, YELLOW);
  } else {
    tft.drawBitmap(93, 0, humIcon, 35, 35, RED);
  }
  
  if (dustIndex < 1) {
    tft.drawBitmap(93, 93, gasIcon, 35, 35, GREEN);
  } else if (dustIndex = 1) {
    tft.drawBitmap(93, 93, gasIcon, 35, 35, YELLOW);
  } else if (dustIndex = 2) {
    tft.drawBitmap(93, 93, gasIcon, 35, 35, ORANGE);
  } else {
    tft.drawBitmap(93, 93, gasIcon, 35, 35, RED);
  }
  
  if (co2Index < 2) {
    tft.drawBitmap(0, 93, coIcon, 35, 35, GREEN);
  } else if (co2Index = 2) {
    tft.drawBitmap(0, 93, coIcon, 35, 35, ORANGE);
  } else {
    tft.drawBitmap(0, 93, coIcon, 35, 35, RED);
  }
  




//  if (histArray[0] = 0) {
//    tft.drawBitmap(0, 0, hist0, 128, 128, BLACK);
//  } else if (histArray[0] > 0 && histArray[0] < 3) {
//    tft.drawBitmap(0, 0, hist0, 128, 128, GREEN);
//  } else if ( histArray[0] >= 3 && histArray[0] < 5) {
//    tft.drawBitmap(0, 0, hist0, 128, 128, YELLOW);
//  } else if ( histArray[0] >= 5 && histArray[0] < 8) {
//    tft.drawBitmap(0, 0, hist0, 128, 128, ORANGE);
//  } else {
//    tft.drawBitmap(0, 0, hist0, 128, 128, RED);
//  }
//  
//  if (histArray[1] = 0) {
//    tft.drawBitmap(0, 0, hist1, 128, 128, BLACK);
//  } else if (histArray[1] > 0 && histArray[1] < 3) {
//    tft.drawBitmap(0, 0, hist1, 128, 128, GREEN);
//  } else if ( histArray[1] >= 3 && histArray[1] < 5) {
//    tft.drawBitmap(0, 0, hist1, 128, 128, YELLOW);
//  } else if ( histArray[1] >= 5 && histArray[1] < 8) {
//    tft.drawBitmap(0, 0, hist1, 128, 128, ORANGE);
//  } else {
//    tft.drawBitmap(0, 0, hist1, 128, 128, RED);
//  }



//  drawText(5,55,"-9", ST77XX_WHITE,1);
//  drawText(5,65,"HR", ST77XX_WHITE,1);
//
//  drawText(58,4,"-6", ST77XX_WHITE,1);
//  drawText(58,14,"HR", ST77XX_WHITE,1);
//
//  drawText(112,55,"-3", ST77XX_WHITE,1);
//  drawText(112,65,"HR", ST77XX_WHITE,1);
//
//  drawText(58,107,"NOW", ST77XX_WHITE,1);

  drawText(50,45,_virusIndex, BLACK,2);
  drawText(63,53,"/10", BLACK,1);
  drawText(50,63,"VIRUS", BLACK,1);
  drawText(50,73,"SCORE", BLACK,1);  
} /* void displayValuesOnTFT() */


void scrnTemp() {
  extern int tempIndex;
  extern float temperature;

  char _temperature[6];     // float to char conversion
  
  dtostrf(temperature, 2, 0, _temperature);
  
  if (tempIndex < 1) {
    tft.drawBitmap(24, 12, tempIconLg, 80, 80, BLACK, GREEN);
  } else if (tempIndex = 1) {
    tft.drawBitmap(24, 12, tempIconLg, 80, 80, BLACK, YELLOW);
  } else {
    tft.drawBitmap(24, 12, tempIconLg, 80, 80, BLACK, RED);
  }  
  
  drawText(20,100, "Temp", WHITE, 1);
  drawText(45,100, _temperature, WHITE, 3);
  drawText(85,100,"C", WHITE, 2);
}

void scrnHumidity() {
  extern int humIndex;
  extern float humidity;

  char _humidity[6];     // float to char conversion
  
  dtostrf(humidity, 2, 0, _humidity);

  if (humIndex < 1) {
    tft.drawBitmap(24, 12, humIconLg, 80, 80, BLACK, GREEN);
  } else if (humIndex = 1) {
    tft.drawBitmap(24, 12, humIconLg, 80, 80, BLACK, YELLOW);
  } else {
    tft.drawBitmap(24, 12, humIconLg, 80, 80, BLACK, RED);
  }  


  drawText(20,100, "Hum", WHITE, 1);
  drawText(45,100, _humidity, WHITE, 3);
  drawText(85,100,"%", WHITE, 2);
}

void scrnCo2() {
  extern int co2Index;
  extern float eco2;

  char _eco2[6];     // float to char conversion
  
  dtostrf(eco2, 2, 0, _eco2);

  if (co2Index < 2) {
    tft.drawBitmap(24, 12, coIconLg, 80, 80, BLACK, GREEN);
  } else if (co2Index = 2) {
    tft.drawBitmap(24, 12, coIconLg, 80, 80, BLACK, YELLOW);
  } else {
    tft.drawBitmap(24, 12, coIconLg, 80, 80, BLACK, RED);
  } 
  
  drawText(20,100, "CO2", WHITE, 1);
  drawText(40,100, _eco2, WHITE, 3);
  drawText(95,100,"ppm", WHITE, 1);
}

void scrnPM25() {
  extern int dustIndex;
  extern float dustcon;

  char _dustcon[6];     // float to char conversion
  
  dtostrf(dustcon, 2, 0, _dustcon);

  if (dustIndex < 1) {
    tft.drawBitmap(24, 12, gasIconLg, 80, 80, BLACK, GREEN);
  } else if (dustIndex = 1) {
    tft.drawBitmap(24, 12, gasIconLg, 80, 80, BLACK, YELLOW);
  } else if (dustIndex = 2) {
    tft.drawBitmap(24, 12, gasIconLg, 80, 80, BLACK, ORANGE);
  } else {
    tft.drawBitmap(24, 12, gasIconLg, 80, 80, BLACK, RED);
  }
  
  drawText(20,100, "PM2.5: ", WHITE, 1);
  drawText(45,100, _dustcon, WHITE, 3);
  drawText(85,100,"um/m3", WHITE, 1);
}

void scrnVOC() {
  extern float tvoc;

  char _tvoc[6];     // float to char conversion
  
  dtostrf(tvoc, 2, 0, _tvoc);

  drawText(20,100, "VOC", WHITE, 1);
  drawText(40,100, _tvoc, WHITE, 3);
  drawText(92,100,"ppb", WHITE, 1);
}

void scrnLight() {
  extern float lux;

  char _lux[6];     // float to char conversion
  
  dtostrf(lux, 2, 0, _lux);
  
  drawText(20,100, "Light ", WHITE, 1);
  drawText(45,100, _lux, WHITE, 3);
  drawText(92,100,"lux", WHITE, 2);
}

void scrnSound() {
  extern float sound;

  char _sound[6];     // float to char conversion
  
  dtostrf(sound, 2, 0, _sound);
  
  drawText(20,100, "Sound", WHITE, 1);
  drawText(45,100, _sound, WHITE, 3);
  drawText(95,100," ", WHITE, 2);
}

void histIndex() {
  extern int virusIndex;

//  _hist10 = _hist9;
//  _hist9 = _hist8;
//  _hist8 = _hist7;
//  _hist7 = _hist6;
//  _hist6 = _hist5;
//  _hist5 = _hist4;
//  _hist4 = _hist3;
//  _hist3 = _hist2;
//  _hist2 = _hist1;
  _hist1 = 2;
  _hist0 = virusIndex;
 
  
//  for (int i=1; i < 11; i++) {
//    _histArray[i] = histArray[i-1];
//  }
//  
//  _histArray[0] = virusIndex; 
//  
//  for (int j=0; j < 11; j++) {
//    histArray[j] = _histArray[j];
//  }
//
//  _hist0 = histArray[0];
//  _hist1 = histArray[1];
//  _hist2 = histArray[2];
//  _hist3 = histArray[3];
//  _hist4 = histArray[4];
//  _hist5 = histArray[5];
//  _hist6 = histArray[6];
//  _hist7 = histArray[7];
//  _hist8 = histArray[8];
//  _hist9 = histArray[9];
//  _hist10 = histArray[10];

//  Serial.print("hist: ");
//  Serial.print(_hist0);
//  Serial.print(": ");
//  Serial.print(_hist1);
//  Serial.print(": ");
//  Serial.print(_hist2);
//  Serial.print(": ");
//  Serial.print(_hist3);
//  Serial.print(": ");
//  Serial.print(_hist4);
//  Serial.print(": ");
//  Serial.print(_hist5);
//  Serial.print(": ");
//  Serial.print(_hist6);
//  Serial.print(": ");
//  Serial.print(_hist7);
//  Serial.print(": ");
//  Serial.print(_hist8);
//  Serial.print(": ");
//  Serial.print(_hist9);
//  Serial.print(": ");
//  Serial.println(_hist10);

}


void histDraw() {

//  if (_hist0 = 0) {
//    tft.drawBitmap(0, 0, hist0, 128, 128, BLACK);
//  } else if (_hist0 > 0 && _hist0 <= 2) {
//    tft.drawBitmap(0, 0, hist0, 128, 128, GREEN);
//  } else if (_hist0 >= 2 && _hist0 < 5) {
//    tft.drawBitmap(0, 0, hist0, 128, 128, YELLOW);
//  } else if (_hist0 >= 5 && _hist0 < 7) {
//    tft.drawBitmap(0, 0, hist0, 128, 128, ORANGE);
//  } else if (_hist0 >= 7){
//    tft.drawBitmap(0, 0, hist0, 128, 128, RED);
//  }
//
//  if (_hist1 = 0) {
//    tft.drawBitmap(0, 0, hist1, 128, 128, BLACK);
//  } else if (_hist1 > 0 && _hist1 <= 2) {
//    tft.drawBitmap(0, 0, hist1, 128, 128, GREEN);
//  } else if (_hist1 >= 2 && _hist1 < 5) {
//    tft.drawBitmap(0, 0, hist1, 128, 128, YELLOW);
//  } else if (_hist1 >= 5 && _hist1 < 7) {
//    tft.drawBitmap(0, 0, hist1, 128, 128, ORANGE);
//  } else if (_hist1 >= 7) {
//    tft.drawBitmap(0, 0, hist1, 128, 128, RED);
//  }
//  tft.drawBitmap(0, 0, hist0, 128, 128, histColor[0]);
//  tft.drawBitmap(0, 0, hist1, 128, 128, histColor[1]);
//  tft.drawBitmap(0, 0, hist2, 128, 128, histColor[2]);
//  tft.drawBitmap(0, 0, hist3, 128, 128, histColor[3]);
//  tft.drawBitmap(0, 0, hist4, 128, 128, histColor[4]);
//  tft.drawBitmap(0, 0, hist5, 128, 128, histColor[5]);
//  tft.drawBitmap(0, 0, hist6, 128, 128, histColor[6]);
//  tft.drawBitmap(0, 0, hist7, 128, 128, histColor[7]);
//  tft.drawBitmap(0, 0, hist8, 128, 128, histColor[8]);
//  tft.drawBitmap(0, 0, hist9, 128, 128, histColor[9]);
//  tft.drawBitmap(0, 0, his10, 128, 128, histColor[10]);
}

void drawText(int x, int y, char *text, uint16_t color, int size) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setTextSize(size);
  tft.print(text);
  
} /* void drawText() */
