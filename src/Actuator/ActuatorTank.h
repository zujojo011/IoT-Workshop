#pragma once

#include "Actuator.h"


class TankActuator : public Actuator 
{ 
public: 
  TankActuator(int pin) 
    : Actuator(pin) 
  {
  } 
};
