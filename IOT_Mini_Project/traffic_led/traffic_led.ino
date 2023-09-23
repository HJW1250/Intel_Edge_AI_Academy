
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define DRIVE_R 5
#define DRIVE_Y 6
#define DRIVE_G 7

#define HUMAN_R 14
#define HUMAN_G 16

#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST  8
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_CLK 13

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  pinMode(DRIVE_R, OUTPUT);
  pinMode(DRIVE_Y, OUTPUT);
  pinMode(DRIVE_G, OUTPUT);

  pinMode(HUMAN_R, OUTPUT);
  pinMode(HUMAN_G, OUTPUT);
 
  
}


void loop() {
  digitalWrite(HUMAN_R, HIGH);
  digitalWrite(HUMAN_G, LOW);
  digitalWrite(DRIVE_G, HIGH);
  digitalWrite(DRIVE_R, LOW);
  digitalWrite(DRIVE_Y, LOW);
  delay(2000); 
  
  digitalWrite(DRIVE_G, LOW);
  digitalWrite(DRIVE_R, LOW);
  digitalWrite(DRIVE_Y, HIGH);
  delay(1000);

  digitalWrite(DRIVE_G, LOW);
  digitalWrite(DRIVE_R, HIGH);
  digitalWrite(DRIVE_Y, LOW);
  digitalWrite(HUMAN_R, LOW);
  digitalWrite(HUMAN_G, HIGH);
  delay(2000);
  digitalWrite(HUMAN_G, LOW);
  delay(500);
  digitalWrite(HUMAN_G, HIGH);
  delay(500);
  digitalWrite(HUMAN_G, LOW);
  delay(500);
  digitalWrite(HUMAN_G, HIGH);
  delay(500);
  digitalWrite(HUMAN_G, LOW);
  digitalWrite(HUMAN_R, HIGH);

  




 
                 
}
