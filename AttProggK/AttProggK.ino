#include <Arduino.h>
#include <UltrasonicSensorK.h>
#include <MotorL293DControllerK.h>
#include <WiFi.h>
#include <WiFiUdp.h>

WiFiUDP Udp;

const char *ssid = "";
const char *password = "";

#define LED 2
#define TRIGPIN1 15
#define ECHOPIN1 14
#define TRIGPIN2 19
#define ECHOPIN2 21
#define TRIGPIN3 22
#define ECHOPIN3 35
#define TRIGPIN4 25
#define ECHOPIN4 26
#define TRIGPIN5 27
#define ECHOPIN5 32
#define TRIGPIN6 33
#define ECHOPIN6 34

#define MOTOR1FOWARDPIN 17
#define MOTOR1BACKWARDPIN 18
#define MOTOR1ENABLEPIN 13
#define MOTOR2FOWARDPIN 5
#define MOTOR2BACKWARDPIN 4
#define MOTOR2ENABLEPIN 16

#define MINMOTORVALUE 40
#define MAXMOTORVALUE 255


//------------------------------------------------------------------------------//
//define the server
WiFiServer server(80);

//define ultrasonic sensors
UltrasonicSensorK UltraS1(TRIGPIN1, ECHOPIN1);
UltrasonicSensorK UltraS2(TRIGPIN2, ECHOPIN2);
UltrasonicSensorK UltraS3(TRIGPIN3, ECHOPIN3);
UltrasonicSensorK UltraS4(TRIGPIN4, ECHOPIN4);
UltrasonicSensorK UltraS5(TRIGPIN5, ECHOPIN5);
UltrasonicSensorK UltraS6(TRIGPIN6, ECHOPIN6);

//define 2 motors
MotorL293DControllerK motor1(MOTOR1FOWARDPIN, MOTOR1BACKWARDPIN, MOTOR1ENABLEPIN, 8, 30000);
MotorL293DControllerK motor2(MOTOR2FOWARDPIN, MOTOR2BACKWARDPIN, MOTOR2ENABLEPIN, 8, 30000);

//------------------------------------------------------------------------------//
//define functions used
void startWIFI();

void inviaDati(char* dato) {
  Udp.beginPacket("192.168.1.3", 2025);
  Udp.print(dato);
  Udp.endPacket();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  
  UltraS1.begin();
  UltraS2.begin();
  UltraS3.begin();
  UltraS4.begin();
  UltraS5.begin();
  UltraS6.begin();

  //configure 2 motors
  motor1.begin();
  motor2.begin();
  motor1.setPWM(255);  //momentarily unavailable
  motor2.setPWM(255);  //momentarily unavailable

  digitalWrite(LED,HIGH);

  //connect wifi
  startWIFI();
}

void loop() {
  Serial.println("----------------------------");
  float d1 = UltraS1.measureDistance();
  Serial.print("ultra 1: ");
  Serial.println(d1);
  delay(30);
  /*float d2 = UltraS2.measureDistance();
  Serial.print("ultra 2: ");
  Serial.println(d2);
  delay(30);*/
  float d3 = UltraS3.measureDistance();
  Serial.print("ultra 3: ");
  Serial.println(d3);
  delay(30);
  /*float d4 = UltraS4.measureDistance();
  Serial.print("ultra 4: ");
  Serial.println(d4);
  delay(30);
  float d5 = UltraS5.measureDistance();
  Serial.print("ultra 5: ");
  Serial.println(d5);
  delay(30);
  float d6 = UltraS6.measureDistance();
  Serial.print("ultra 6: ");
  Serial.println(d6);*/

  int m1 = map(d1, 6, 60, MINMOTORVALUE, MAXMOTORVALUE);
  int m2 = map(d3, 6, 60, MINMOTORVALUE, MAXMOTORVALUE);

  m1 = constrain(m1, MINMOTORVALUE, MAXMOTORVALUE);
  m2 = constrain(m2, MINMOTORVALUE, MAXMOTORVALUE);

  motor1.setPWM(m1);
  motor2.setPWM(m2);

  
  motor1.moveForward();
  motor2.moveForward();
  char buffer[100];
  sprintf(buffer,"%f - %f",m1,m2);
  inviaDati(buffer);
  /*
  delay(10000);
  Serial.println("----------------------------");
  Serial.print("ultra 1: ");
  Serial.println(UltraS1.measureDistance());
  delay(30);
  Serial.print("ultra 2: ");
  Serial.println(UltraS2.measureDistance());
  delay(30);
  Serial.print("ultra 3: ");
  Serial.println(UltraS3.measureDistance());
  delay(30);
  Serial.print("ultra 4: ");
  Serial.println(UltraS4.measureDistance());
  delay(30);
  Serial.print("ultra 5: ");
  Serial.println(UltraS5.measureDistance());
  delay(30);
  Serial.print("ultra 6: ");
  Serial.println(UltraS6.measureDistance());
  motor1.moveBackward();
  motor2.moveBackward();
  delay(10000);*/
}



void startWIFI() {
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address on serial monitor
  Serial.print("My IP is: ");
  Serial.print(WiFi.localIP());
}
