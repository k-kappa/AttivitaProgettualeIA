#include <Arduino.h>
#include <UltrasonicSensorK.h>
#include <MotorL293DControllerK.h>
//#include <WiFi.h>
#include <ESP8266WiFi.h>


const char *ssid = "";
const char *password = "";

/*#define LED 2
#define TRIGPIN1 18
#define ECHOPIN1 39
#define TRIGPIN2 22
#define ECHOPIN2 23
#define TRIGPIN3 16
#define ECHOPIN3 4
#define TRIGPIN4 19
#define ECHOPIN4 36
#define TRIGPIN5 17
#define ECHOPIN5 35
#define TRIGPIN6 5
#define ECHOPIN6 34*/

#define MOTOR1FOWARDPIN 0//25
#define MOTOR1BACKWARDPIN 2//33
#define MOTOR1ENABLEPIN 5//26
#define MOTOR2FOWARDPIN 12//27
#define MOTOR2BACKWARDPIN 14//14
#define MOTOR2ENABLEPIN 4//12


//------------------------------------------------------------------------------//
//define the server
WiFiServer server(80);

//define ultrasonic sensors
/*UltrasonicSensorK UltraS1(TRIGPIN1, ECHOPIN1);
UltrasonicSensorK UltraS2(TRIGPIN2, ECHOPIN2);
UltrasonicSensorK UltraS3(TRIGPIN3, ECHOPIN3);
UltrasonicSensorK UltraS4(TRIGPIN4, ECHOPIN4);
UltrasonicSensorK UltraS5(TRIGPIN5, ECHOPIN5);
UltrasonicSensorK UltraS6(TRIGPIN6, ECHOPIN6);*/

//define 2 motors
MotorL293DControllerK motor1(MOTOR1FOWARDPIN, MOTOR1BACKWARDPIN, MOTOR1ENABLEPIN, 8, 30000);
MotorL293DControllerK motor2(MOTOR2FOWARDPIN, MOTOR2BACKWARDPIN, MOTOR2ENABLEPIN, 8, 30000);

//------------------------------------------------------------------------------//
//define functions used
void startServer();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  /*pinMode(LED, OUTPUT);
  UltraS1.begin();
  UltraS2.begin();
  UltraS3.begin();
  UltraS4.begin();
  UltraS5.begin();
  UltraS6.begin();*/

  //configure 2 motors
  motor1.begin();
  motor2.begin();
  motor1.setPWM(255);  //momentarily unavailable
  motor2.setPWM(150);

  //connect wifi and start server
  //startServer();  //make only wifi connection, exlude the server creation
}

void loop() {
  Serial.println("----------------------------");
  //motor1.setPWM(250);  //momentarily unavailable
  /*Serial.print("ultra 1: ");
  Serial.println(UltraS1.measureDistance());
  delay(30);
  Serial.print("ultra 2: ");
  Serial.println(UltraS1.measureDistance());
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
  Serial.println(UltraS6.measureDistance());*/
  motor1.moveForward();
  motor2.moveForward();
  delay(500);
}



void startServer() {
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
  // Start the server
  server.begin();
  Serial.println("Server started");
  // Print the IP address on serial monitor
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");  //URL IP to be typed in mobile/desktop browser
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
