#pragma once

#include "Sensor.h"


class WaterLevelSensor : public Sensor 
{ 
public: 
  WaterLevelSensor(TwoWire &wire, int addr) 
    : Sensor(wire, addr) 
  {
  } 
};
