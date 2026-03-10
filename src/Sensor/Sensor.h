#pragma once

#include <Arduino.h>
#include <Wire.h> 


class Sensor 
{
protected:
  TwoWire &sensorWire;
  int      sensorAddr;
  
public:
  Sensor(TwoWire &wire, int addr)
    : sensorWire(wire)
    , sensorAddr(addr)
  {
  }
  
  virtual void setup()
  {
  }

  virtual bool getValueDigital()
  {
    if (random(0, 100) > 50) {
      return true;
    } else {
      return false;
    }

    return false;
  }

  virtual float getValueAnalog()
  {
    // Zufälligen Werte zwischen 0.0 und 100.0 generieren
    return random(0, 1000) / 10.0;
  }
};
