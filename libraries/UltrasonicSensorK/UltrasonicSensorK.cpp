/*
  UltrasonicSensorK.cpp - Library for Ultrasonic Sensor code.
  Created by Krystian D. Koss, December 26, 2023.
*/

#include "Arduino.h"
#include "UltrasonicSensorK.h"

UltrasonicSensorK::UltrasonicSensorK(int trigPin, int echoPin)
{
  _trigPin = trigPin;
  _echoPin = echoPin;
}

void UltrasonicSensorK::begin()
{
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

float UltrasonicSensorK::measureDistance()
{
  // Clears the trigPin
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  _duration = pulseIn(_echoPin, HIGH);
  return _duration * SOUND_SPEED/2;
}

float UltrasonicSensorK::avgMeasurementsDistance(int num, int UB, int LB, long microsecondsDelay){ //num= number of measurements
  int tempCount = 0;
  float tot=0;
  float measure;
  Serial.println("xxxx");
  for(int i=0;i<num;i++){
    measure=measureDistance();
    Serial.println(measure);
    if ((measure<=UB)&&(measure>=LB)){
      tot=tot+measure;
      tempCount++;
    }
    delayMicroseconds(microsecondsDelay);
  }
  return tot/tempCount;
}

float UltrasonicSensorK::avgMeasurementsDistance(int num, int UB, int LB){ //num= number of measurements
  int tempCount = 0;
  float tot=0;
  float measure;
  Serial.println("xxxx");
  for(int i=0;i<num;i++){
    measure=measureDistance();
    Serial.println(measure);
    if ((measure<=UB)&&(measure>=LB)){
      tot=tot+measure;
      tempCount++;
    }
    delayMicroseconds(2000);     //default delay
  }
  return tot/tempCount;
}
