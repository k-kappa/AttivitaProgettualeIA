/*
  UltrasonicSensorK.h - Library for Ultrasonic Sensor code.
  Created by Krystian D. Koss, December 26, 2023.
*/
#ifndef UltrasonicSensorK_h
#define UltrasonicSensorK_h

#include "Arduino.h"

#define SOUND_SPEED 0.034

class UltrasonicSensorK
{
  public:
    
    UltrasonicSensorK(int trigPin, int echoPin);
    void begin();
    float measureDistance();
    float avgMeasurementsDistance(int num, int UB, int LB);
    float avgMeasurementsDistance(int num, int UB, int LB, long microsecondsDelay);
  private:
    int _trigPin, _echoPin;
    long _duration;
};

#endif