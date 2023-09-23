#include <DHT.h>
#include <WiFiEsp.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define trigPin1 2
#define echoPin1 3
#define trigPin2 4
#define echoPin2 5
#define DHTPIN 8
#define V_LED 9   
#define Vo A0

// 와이파이세팅
#define DEBUG
#define WIFIRX 6  //6:RX-->ESP8266 TX
#define WIFITX 7  //7:TX -->ESP8266 RX

#define AP_SSID "embA"
#define AP_PASS "embA1234"
#define SERVER_NAME "10.10.14.176"
#define SERVER_PORT 5000
#define LOGID "HJW_ARD"
#define PASSWD "PASSWD"

#define CMD_SIZE 50
#define ARR_CNT 5

char send_ARD[10] = "EKC_ARD";
char send_DB[10] = "EKC_UBU";

char sendBuf[CMD_SIZE];

char getSensorId[10];

SoftwareSerial wifiSerial(WIFIRX, WIFITX);
WiFiEspClient client;

// 초음파 센서
long duration,sensor1val, sensor2val;
int distance;

// 온습도 센서
#define DHTTYPE DHT11
float temp = 0.0;
float humi = 0.0;
DHT dht(DHTPIN, DHTTYPE);

// 미세먼지 센서
float Vo_value = 0;
float Voltage = 0;
float dustDensity = 0;

void setup(){
  //와이파이 셋업
#ifdef DEBUG
  Serial.begin(115200); //DEBUG
#endif
  wifi_Setup();

  //초음파 셋업
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);  

  //온습도 셋업
  dht.begin();

  //미세먼지 셋업
  pinMode(V_LED, OUTPUT);
  pinMode(Vo, INPUT);

}

void loop(){
  //와이파이 연결
  if (client.available()) {
    socketEvent();
  }

#ifdef DEBUG

#endif
      if (!client.connected()) {
        Serial.println( "Server Down");
        server_Connect();
      }

  //미세먼지 루프
  digitalWrite(V_LED,LOW);
  delayMicroseconds(280);

  Vo_value = analogRead(Vo); 
  delayMicroseconds(40);

  digitalWrite(V_LED,HIGH); 
  delayMicroseconds(9680);

  Voltage = Vo_value * 5.0 / 1024.0;
  dustDensity = (Voltage - 0.3)/0.005;

  // 초음파 루프
  driveSensor(trigPin1, echoPin1);
  sensor1val = distance;
  driveSensor(trigPin2, echoPin2);
  sensor2val = distance;

  //온습도 루프
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  
  //와이파이를 통해 서버로 데이터 보내기
  sprintf(sendBuf, "[%s]SENSOR@%d@%d@%d\n", send_ARD,(int)humi,(int)temp,(int)dustDensity);
  client.write(sendBuf, strlen(sendBuf));
  client.flush();

  sprintf(sendBuf, "[%s]ECHO@%d@%d\n", send_ARD, (int)sensor1val,(int)sensor2val);
  client.write(sendBuf, strlen(sendBuf));
  client.flush();

  sprintf(sendBuf, "[%s]SENSOR@%d@%d@%d\n", send_DB, (int)humi,(int)temp,(int)dustDensity);
  client.write(sendBuf, strlen(sendBuf));
  client.flush();

  //sprintf(sendBuf, "[iotdb]ECHO@%d@%d\n", (int)sensor1val,(int)sensor2val);
  //client.write(sendBuf, strlen(sendBuf));
  //client.flush();

  delay(5000);
}

void driveSensor(int trigPin,int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.4;
}

void socketEvent()
{
  int i = 0;
  char * pToken;
  char * pArray[ARR_CNT] = {0};
  char recvBuf[CMD_SIZE] = {0};
  int len;

  sendBuf[0] = '\0';
  len = client.readBytesUntil('\n', recvBuf, CMD_SIZE);
  client.flush();
#ifdef DEBUG
  Serial.print("recv : ");
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

  if (!strncmp(pArray[1], " Alr", 4)) //Already logged
  {
#ifdef DEBUG
    Serial.write('\n');
#endif
    client.stop();
    server_Connect();
    return ;
  }
  else
    return;

  client.write(sendBuf, strlen(sendBuf));
  client.flush();

#ifdef DEBUG
  Serial.print(", send : ");
  Serial.print(sendBuf);
#endif
}

void wifi_Setup() {
  wifiSerial.begin(19200);
  wifi_Init();
  server_Connect();
}
void wifi_Init()
{
  do {
    WiFi.init(&wifiSerial);
    if (WiFi.status() == WL_NO_SHIELD) {
#ifdef DEBUG_WIFI
      Serial.println("WiFi shield not present");
#endif
    }
    else
      break;
  } while (1);

#ifdef DEBUG_WIFI
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(AP_SSID);
#endif
  while (WiFi.begin(AP_SSID, AP_PASS) != WL_CONNECTED) {
#ifdef DEBUG_WIFI
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(AP_SSID);
#endif
  }

#ifdef DEBUG_WIFI
  Serial.println("You're connected to the network");
  printWifiStatus();
#endif
}

int server_Connect()
{
#ifdef DEBUG_WIFI
  Serial.println("Starting connection to server...");
#endif

  if (client.connect(SERVER_NAME, SERVER_PORT)) {
#ifdef DEBUG_WIFI
    Serial.println("Connect to server");
#endif
    client.print("["LOGID":"PASSWD"]");
  }
  else
  {
#ifdef DEBUG_WIFI
    Serial.println("server connection failure");
#endif
  }
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}