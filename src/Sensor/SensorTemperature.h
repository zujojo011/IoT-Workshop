#pragma once

#include "Sensor.h"


class TemperatureSensor : public Sensor 
{ 
public: 
  TemperatureSensor(TwoWire &wire, int addr) 
    : Sensor(wire, addr) 
  {
  } 
};
