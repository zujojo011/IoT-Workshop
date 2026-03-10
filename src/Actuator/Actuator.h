#pragma once

#include <Arduino.h>
#include "Utils/StopWatch.h"


class Actuator 
{
protected:
  int       actuatorPin;
  bool      actuatorValue;
  StopWatch stopWatch;

public:
  Actuator(int pin)
    : actuatorPin(pin)
    , actuatorValue(false)
    , stopWatch("Actuator", 0)
  {
  }
  
  virtual void setup()
  {
    ::pinMode(actuatorPin, OUTPUT);
  }

  virtual bool digitalRead()
  {
    return actuatorValue;
  }

  virtual void digitalWrite(bool state)
  {
    ::digitalWrite(actuatorPin, state ? HIGH : LOW);
    actuatorValue = state;

  }

  virtual void setHighSec(int sec)
  {
    stopWatch.setSeconds(sec);
    stopWatch.reset();
    digitalWrite(HIGH);
  }

  virtual void update()
  {
    if (stopWatch.expired()) {
      stopWatch.reset();
      digitalWrite(LOW);
    }
  }
};
