#pragma once

#include "Sensor/SensorCO2.h"
#include "Sensor/SensorMoisture.h"
#include "Sensor/SensorTemperature.h"
#include "Sensor/SensorWaterLevel.h"
#include "Actuator/ActuatorCO2.h"
#include "Actuator/ActuatorTank.h"
#include "Actuator/ActuatorDrinkWater.h"
#include "Actuator/ActuatorWindow.h"
#include "Utils/StopWatch.h"


class Zone 
{
protected:
  String              zoneName;
  CO2Sensor           &co2Sensor;
  TemperatureSensor   &temperatureSensor;
  MoistureSensor      &moistureSensor;
  WaterLevelSensor    &waterSensor;
  WindowActuator      &windowActuator;
  CO2Actuator         &co2Actuator;
  TankActuator        &tankActuator;
  DrinkWaterActuator  &drinkwaterActuator;

public:
  Zone(String name,
       CO2Sensor &co2Sensor, TemperatureSensor &temperatureSensor, MoistureSensor &moistureSensor, WaterLevelSensor &waterSensor,
       WindowActuator &windowActuator, CO2Actuator &co2Actuator, TankActuator &tankActuator, DrinkWaterActuator &drinkwaterActuator)
    : zoneName(name)
    , co2Sensor(co2Sensor)
    , temperatureSensor(temperatureSensor)
    , moistureSensor(moistureSensor)
    , waterSensor(waterSensor)
    , windowActuator(windowActuator)
    , co2Actuator(co2Actuator)
    , tankActuator(tankActuator)
    , drinkwaterActuator(drinkwaterActuator)
  {
  }
  
  void setup()
  {
    Serial.println("Zone::setup(" + zoneName + ")");

    co2Sensor.setup();
    temperatureSensor.setup();
    moistureSensor.setup();
    waterSensor.setup();
    /*
    windowActuator.setup();
    co2Actuator.setup();
    tankActuator.setup();
    drinkwaterActuator.setup();
    */
  }

  void update()
  {
    windowActuator.update();
    co2Actuator.update();
    tankActuator.update();
    drinkwaterActuator.update();
  }

  String getName()
  {
    return zoneName;
  }

  bool isWindowOpen()
  {
    return windowActuator.digitalRead() == HIGH;
  }

  bool isCO2ValveOpen()
  {
    return co2Actuator.digitalRead() == HIGH;
  }

  bool isTankValveOpen()
  {
    return tankActuator.digitalRead() == HIGH;
  }

  bool isDrinkWaterValveOpen()
  {
    return drinkwaterActuator.digitalRead() == HIGH;
  }

  float getCO2Value()
  {
    return co2Sensor.getValueAnalog();
  }

  float getTemperatureValue()
  {
    return temperatureSensor.getValueAnalog();
  }

  float getMoistureValue()
  {
    return moistureSensor.getValueAnalog();
  }

  float getWaterLevelValue()
  {
    return waterSensor.getValueAnalog();
  }

  void openWindow(int sec)
  {
    Serial.println("Zone::openWindow()");
    if (!isWindowOpen()) {
      windowActuator.setHighSec(sec);
    }
  }

  void openDrinkWaterValve(int sec)
  {
    Serial.println("Zone::openDrinkWaterValve()");
    if (!isDrinkWaterValveOpen()) {
      drinkwaterActuator.setHighSec(sec);
    }
  }

  void startIrrigation(int sec) 
  {
    Serial.println("Zone::startIrrigation() - Bewässerung starten");
    if (!isTankValveOpen()) {
      tankActuator.setHighSec(sec);
    }
  }

};