/*
  MotorL293DControllerK.h - Library for controlling one motor via L293D.
  Created by Krystian D. Koss, january 3, 2024.
*/
#ifndef MotorL293DControllerK_h
#define MotorL293DControllerK_h

#include "Arduino.h"


class MotorL293DControllerK {
public:

  MotorL293DControllerK(int forwardMotorPin, int BackwardMotorPin, int enablePin, int resolutionPWM);
  bool begin();
  bool setPWM();
  void moveForward();
  void moveBackward();
  /**
  * Sync function (delay function involved)
  */
  bool moveRightwithAngle(float angle);
  bool moveLeftwithAngle(float angle);
private:
  int _forwardMotorPin, _BackwardMotorPin, _enablePin;
  int _resolutionPWM;
  long _duration;
};

#endif