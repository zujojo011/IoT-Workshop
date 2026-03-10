#pragma once

#include "Actuator.h"


class CO2Actuator : public Actuator 
{ 
public: 
  CO2Actuator (int pin) 
    : Actuator(pin) 
  {
  } 
};
