#pragma once

// https://learn.sparkfun.com/tutorials/soil-moisture-sensor-hookup-guide

#include "Sensor.h"

#define COMMAND_LED_OFF     0x00
#define COMMAND_LED_ON      0x01
#define COMMAND_GET_VALUE   0x05
#define COMMAND_NOTHING_NEW 0x99

class MoistureSensor : public Sensor 
{ 
public: 
  MoistureSensor(TwoWire &wire, int addr) 
    : Sensor(wire, addr) 
  {
  } 

  float getValueAnalog()
  {
    Serial.println("MoistureSensor::getValueAnalog(" + String(sensorAddr) + ")");

    sensorWire.beginTransmission(sensorAddr);
    sensorWire.write(COMMAND_GET_VALUE);
    if (sensorWire.endTransmission() != 0) {
      Serial.println("Fehler beim Abrufen des Feuchtigkeitswerts");
      return -1; // Fehler bei der Übertragung
    }

    sensorWire.requestFrom(sensorAddr, 2); // Annahme: 2 Bytes für Feuchtigkeitswert
    while (sensorWire.available() == 2) {
      uint16_t moisture = sensorWire.read() << 8 | sensorWire.read();  // F

      sensorWire.read();
      return (float) 100.0 - (100.0 * moisture / 65535); // Rückgabe des Feuchtigkeitswerts in %
    }

    Serial.println("MoistureSensor error");
    return -1; // Fehler oder keine Daten verfügbar  }
  }
};
