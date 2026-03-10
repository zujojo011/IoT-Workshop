#pragma once

#include "Actuator.h"


class DrinkWaterActuator : public Actuator 
{ 
public: 
  DrinkWaterActuator(int pin) 
    : Actuator(pin) 
  {
  } 

};

