#pragma once

#include <Arduino.h>

class StopWatch 
{
protected:
  String name;
  int    seconds;
  int    lastMillis;

public:
  StopWatch(String nameSet, int secondsSet)
    : name(nameSet)
    , seconds(secondsSet)
    , lastMillis(0)
  {
  }

  void setSeconds(int sec)
  {
    seconds = sec;
  }

  bool expired()
  {
    if (seconds > 0) {
      bool expired = (millis() - lastMillis) >= seconds * 1000;

      if (expired) {
        Serial.println(name + " - StopWatch::expired()");
        return true;
      }
    }
    return false;
  }

  void reset()
  {
    Serial.println(name + " - StopWatch::reset()");
    
    lastMillis = millis();
  }
};