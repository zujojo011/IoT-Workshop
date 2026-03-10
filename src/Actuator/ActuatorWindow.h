#pragma once

#include "Actuator.h"


class WindowActuator : public Actuator 
{ 
public: 
  WindowActuator(int pin) 
    : Actuator(pin) 
  {
  } 
};

