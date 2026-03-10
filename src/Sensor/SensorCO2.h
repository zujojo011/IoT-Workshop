#pragma once

// https://learn.sparkfun.com/tutorials/soil-moisture-sensor-hookup-guide

#include "Sensor.h"


class CO2Sensor : public Sensor 
{ 
public: 
  CO2Sensor(TwoWire &wire, int addr) 
    : Sensor(wire, addr) 
  {
  } 

  virtual float getValueAnalog()
  {
    // Zufälligen Werte zwischen 50 und 500 generieren
    return (float) random(50, 500);
  }
};
