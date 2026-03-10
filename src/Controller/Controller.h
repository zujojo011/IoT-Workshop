#pragma once

#include <WiFi.h>
#include "../Zone/Zone.h"
#include "../Mqtt/MqttPublisher.h"
#include "../Weather/Weather.h"
#include <vector>

#define PUBLISH_INTERVAL_SEC          10
#define ZONE_UPDATE_INTERVAL_SEC      10
#define RAIN_UPDATE_INTERVAL_SEC      3600
#define WINDOW_OPEN_INTERVAL_SEC      60
#define IRRIGATION_INTERVAL_SEC       60
#define DRINKWATER_OPEN_INTERVAL_SEC  60
#define MIN_WATER_LEVEL               20
#define MIN_RESERVOIR_LEVEL           30

class Controller
{
protected:
  std::vector<Zone *> zones;
  WiFiClient          &wifiClient;
  MqttPublisher       &mqttPublisher;
  StopWatch           zoneUpdateStopWatch;
  StopWatch           publishStopWatch;
  StopWatch           rainUpdateStopWatch;
  StopWatch           drinkwaterStopWatch;
  bool                rainInNext12Hours;
  bool                waterShortage;
  float               reservoirLevel;
  bool                temperatureAboveFreezing;

public:
  Controller(MqttPublisher &mqttPublisher, WiFiClient &wifiClient)
    : mqttPublisher(mqttPublisher)
    , wifiClient(wifiClient)
    , publishStopWatch("publish", PUBLISH_INTERVAL_SEC)
    , zoneUpdateStopWatch("zoneUpdate", ZONE_UPDATE_INTERVAL_SEC)
    , rainUpdateStopWatch("rainUpdaet", RAIN_UPDATE_INTERVAL_SEC)
    , drinkwaterStopWatch("trinkWater", DRINKWATER_OPEN_INTERVAL_SEC)
    , rainInNext12Hours(false)
    , waterShortage(false)
    , reservoirLevel(50) // Beispielwert
    , temperatureAboveFreezing(true) // Beispielwert
  {
  }

  void addZone(Zone *zone)
  {
    if (zone != nullptr) {
      Serial.println("Controller::addZone(" + zone->getName() + ")");
      zones.push_back(zone);
    }
  }

  void setup()
  {
    Serial.println("Controller::setup()");
    publishStopWatch.reset();
    for (Zone *zone : zones) {
      if (zone != nullptr) {
        zone->setup();
      }
    }
    rainInNext12Hours = Weather::rainInNext12Hours(wifiClient);
  }

  void publishData()
  {
    Serial.println("Controller - publishStopWatch.expired()");

    if (!mqttPublisher.isConnected()) {
      mqttPublisher.connect("greenhouse");
    }

    if (mqttPublisher.isConnected()) {
      for (Zone *zone : zones) {
        if (zone != nullptr) {
          String zoneName               = zone->getName();
          zoneName.toLowerCase();
          String baseTopic              = "greenhouse/" + zoneName;

          String topicCO2Valve          = baseTopic + "/co2valve";
          String topicWindow            = baseTopic + "/window";
          String topicTank              = baseTopic + "/tank";
          String topicCO2               = baseTopic + "/co2";
          String topicMoisture          = baseTopic + "/moisture";
          String topicTemperature       = baseTopic + "/temperature";
          String topicWaterLevel        = baseTopic + "/waterlevel";
          String topicDrinkWaterValve   = baseTopic + "/drinkwatervalve";

          String valueCO2Valve          = zone->isCO2ValveOpen() ? "OPEN" : "CLOSED";
          String valueWindow            = zone->isWindowOpen() ? "OPEN" : "CLOSED";
          String valueTank              = zone->isTankValveOpen() ? "OPEN" : "CLOSED";
          String valueDrinkWaterValve   = zone->isDrinkWaterValveOpen() ? "OPEN" : "CLOSED";
          String valueCO2               = String(zone->getCO2Value());
          String valueMoisture          = String(zone->getMoistureValue());
          String valueTemperature       = String(zone->getTemperatureValue());
          String valueWaterLevel        = String(zone->getWaterLevelValue());

          mqttPublisher.publish(topicCO2Valve.c_str(),          valueCO2Valve.c_str());
          mqttPublisher.publish(topicWindow.c_str(),            valueWindow.c_str());
          mqttPublisher.publish(topicTank.c_str(),              valueTank.c_str());
          mqttPublisher.publish(topicCO2.c_str(),               valueCO2.c_str());
          mqttPublisher.publish(topicMoisture.c_str(),          valueMoisture.c_str());
          mqttPublisher.publish(topicTemperature.c_str(),       valueTemperature.c_str());
          mqttPublisher.publish(topicWaterLevel.c_str(),        valueWaterLevel.c_str());
          mqttPublisher.publish(topicDrinkWaterValve.c_str(),   valueDrinkWaterValve.c_str());
        }
      }
    }
  }

  void update()
  {
    if (rainUpdateStopWatch.expired()) {
      rainUpdateStopWatch.reset();
      rainInNext12Hours = Weather::rainInNext12Hours(wifiClient);
      Serial.println(rainInNext12Hours ? "Regen in den nächsten 12 Stunden!" : "Kein Regen in den nächsten 12 Stunden!");
    }

    if (zoneUpdateStopWatch.expired()) {
      zoneUpdateStopWatch.reset();

      for (Zone *zone : zones) {
        if (zone != nullptr) {
          zone->update();

          // CO2-Kontrolle
          if (zone->getCO2Value() < 200) {
            Serial.println("Controller - CO2-Wert unter 200ppm, Fenster öffnen");
            if (!zone->isWindowOpen()) {
              zone->openWindow(WINDOW_OPEN_INTERVAL_SEC);
            }
          }

          // Bewässerungskontrolle
          float moisture = zone->getMoistureValue();
          if (moisture != -1) {
            Serial.println("Feuchtigkeitswert: " + String((int) moisture));
            if (moisture < 30) { // Feuchtigkeit unter 30 %
              Serial.println("Boden ist zu trocken, Bewässerung prüfen");
              if (reservoirLevel > MIN_WATER_LEVEL || rainInNext12Hours) {
                zone->startIrrigation(IRRIGATION_INTERVAL_SEC);
              } else {
                Serial.println("Reservoir zu niedrig und es kein Regen");
              }
            }
          }
          // Reservoir-Kontrolle
          if (reservoirLevel < MIN_RESERVOIR_LEVEL) {
            if (!rainInNext12Hours && temperatureAboveFreezing) {
              Serial.println("Reservoir unter Mindestlevel, Trinkwassernachfüllung starten");
              Serial.println("Controller - Trinkwasser wird für 60 Sekunden aktiviert");
              zone->openDrinkWaterValve(60); // 60 Sekunden Wasserzufuhr  
            } else {
              Serial.println("Niederschlag erwartet, Nachfüllung ausgesetzt");
            }
          }
        }
      }
    }
    if (publishStopWatch.expired()) {
      publishStopWatch.reset();
      publishData();
    }

    mqttPublisher.update();
  }
};
