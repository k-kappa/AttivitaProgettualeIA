#include <Arduino.h>
#include <UltrasonicSensorK.h>

#define LED 2
#define TRIGPIN 22
#define ECHOPIN 23

UltrasonicSensorK UltraS(TRIGPIN,ECHOPIN);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  UltraS.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED, HIGH);
  Serial.println("LED is on");
  //delay(1000);
  digitalWrite(LED, LOW);
  //Serial.println("LED is off");
  delay(1000);
  Serial.println("distanza " + String(UltraS.avgMeasurementsDistance(15,200,0)) + " cm");

}