#include <SoftwareSerial.h>
#include <Wire.h>
#include <MsTimer2.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define DEBUG

#define CMD_SIZE 60
char lcdLine1[17] = "Smart IoT By KSH";
char lcdLine2[17] = "";
char sendBuf[CMD_SIZE];
char recvId[10] = "EKC_ARD";  // SQL 저장 클라이이언트 ID
bool lastButton = HIGH;       // 버튼의 이전 상태 저장
bool currentButton = HIGH;    // 버튼의 현재 상태 저장
bool ledOn = false;      // LED의 현재 상태 (on/off)
bool timerIsrFlag = false;
unsigned int secCount;
int getSensorTime;
int dust;
int humi;
int temp;
int echo1;
int echo2;
#define BUZZER 4
#define DRIVE_R 5
#define DRIVE_Y 6
#define DRIVE_G 7

#define HUMAN_R 14
#define HUMAN_G 16

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10
#define TFT_RST  8
#define TFT_MOSI 11
#define TFT_MISO 12
#define TFT_CLK 13
#define ARR_CNT 5
//bool timerIsrFlag = false;
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

SoftwareSerial BTSerial(2, 3); //bt모듈tx = 아두이노2번 , bt모듈rx = 아두이노3번

void lcdDisplay(int x, int y, char * str);
unsigned long LED();
unsigned long testText();
void bluetoothEvent();
void timerIsr();
void beep();



void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("setup() start!");
#endif
  lcd.init();
  lcd.backlight();
  lcdDisplay(0, 0, lcdLine1);
  lcdDisplay(0, 1, lcdLine2);
 
  BTSerial.begin(9600); // set the data rate for the SoftwareSerial port
  MsTimer2::set(1000, timerIsr); // 1000ms period
  MsTimer2::start();
  
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  
  pinMode(DRIVE_R, OUTPUT);
  pinMode(DRIVE_Y, OUTPUT);
  pinMode(DRIVE_G, OUTPUT);

  pinMode(HUMAN_R, OUTPUT);
  pinMode(HUMAN_G, OUTPUT);

  MsTimer2::set(1000, timerIsr); // 1000ms period
  MsTimer2::start();
}

void loop() 
{
  // put your main code here, to run repeatedly:
  testText();
  LED();

  if (BTSerial.available()){
    bluetoothEvent();
    
  #ifdef DEBUG
    Serial.println(lcdLine2);
  #endif
    
    lcdDisplay(0, 1, lcdLine2);

//    if(getSensorTime != 0 && !(secCount % getSensorTime)) {
//      sprintf(sendBuf,"[%s]SENSOR@%d@%d@%d\n",recvId,(int)humi,(int)temp,(int)dust);
//      BTSerial.write(sendBuf);   
//    }    
  }
   beep();


#ifdef DEBUG
//    Serial.println(sendBuf);
#endif
//    Serial.println(sendBuf);

#ifdef DEBUG
  if (Serial.available()){
    BTSerial.write(Serial.read());
  }
#endif
}


unsigned long LED() {
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
  digitalWrite(DRIVE_G, HIGH);
  digitalWrite(DRIVE_R, LOW);
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  char tempStr[20];
  char humiStr[20];
  char dustStr[20];
  char echo1Str[20];
  char echo2Str[20];
  sprintf(tempStr, "humi: %d%%", humi);
  sprintf(humiStr, "temp: %d'C", temp); 
  sprintf(dustStr, "dust: %dug/m3", dust);
  sprintf(echo1Str, "echo1: %dcm", echo1);
  sprintf(echo2Str, "echo2: %dcm", echo2);
  tft.setCursor(0, 0);
  tft.setTextSize(5); tft.setTextColor(ILI9341_WHITE); tft.println("IoT mini Project");
  tft.setTextSize(4); tft.setTextColor(ILI9341_GREEN); tft.println("HJW & EKC");
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(3);
  tft.println("       ");
  tft.println(tempStr);
  tft.println(humiStr);
  tft.println(dustStr);
  tft.println(echo1Str);
  tft.println(echo2Str);
  

  return micros() - start;
}

void bluetoothEvent(){
  int i = 0;
  char * pToken;
  char * pArray[ARR_CNT] = {0};
  char recvBuf[CMD_SIZE] = {0};
  int len = BTSerial.readBytesUntil('\n', recvBuf, sizeof(recvBuf) - 1);

#ifdef DEBUG
  Serial.print("Recv : ");
  Serial.println(recvBuf);
#endif

  pToken = strtok(recvBuf, "[@]");
  while (pToken != NULL)
  {
    pArray[i] =  pToken;
    if (++i >= ARR_CNT)
      break;
    pToken = strtok(NULL, "[@]");
  }
  //recvBuf : [XXX_BTM]LED@ON
  //pArray[0] = "XXX_LIN"   : 송신자 ID
  //pArray[1] = "LED"
  //pArray[2] = "ON"
  //pArray[3] = 0x0


  //블루투스를 통해 서버에서 데이터 받아오기
  if(!strncmp(pArray[1],"SENSOR",4))
  {
    humi=atoi(pArray[2]);
    temp=atoi(pArray[3]);
    dust=atoi(pArray[4]);

    if(pArray[2] == NULL){
      getSensorTime = 0;
    }else {

    strcpy(recvId,pArray[0]);
    }
    sprintf(sendBuf,"[%s]SENSOR@%d@%d@%d\n",pArray[0],dust,temp,humi);
  }

  else if(!strncmp(pArray[1],"ECHO",4))
  {
    echo1=atoi(pArray[2]);
    echo2=atoi(pArray[3]);
  }
  
  else if (!strncmp(pArray[1], " New", 4)) // New Connected
  {
    return ;
  }
  else if (!strncmp(pArray[1], " Alr", 4)) //Already logged
  {
    return ;
  }

#ifdef DEBUG
  Serial.print("Send : ");
  Serial.print(sendBuf);
#endif
  BTSerial.write(sendBuf);
}
void timerIsr()
{
  timerIsrFlag = true;
  secCount++;
}
void lcdDisplay(int x, int y, char * str)
{
  int len = 16 - strlen(str);
  lcd.setCursor(x, y);
  lcd.print(str);
  for (int i = len; i > 0; i--)
    lcd.write(' ');
}

void beep()
{
  if(echo1<10 || echo2<10)
  {
    tone(BUZZER,261);
    tone(BUZZER,261);
    tone(BUZZER,261);
  }
}



