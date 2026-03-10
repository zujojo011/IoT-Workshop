// Hauptdatei für das Gewächshaussteuerungssystem
#include <Arduino.h> // Standardfunktionen der Arduino-Plattform
#include <Wire.h> 
#include "Utils/ScanI2C.h"
#include "Sensor/SensorCO2.h"
#include "Sensor/SensorMoisture.h"
#include "Sensor/SensorTemperature.h"
#include "Sensor/SensorWaterLevel.h"
#include "Actuator/ActuatorCO2.h"
#include "Actuator/ActuatorTank.h"
#include "Actuator/ActuatorDrinkWater.h"
#include "Actuator/ActuatorWindow.h"
#include "Zone/Zone.h"
#include "Mqtt/MqttPublisher.h"
#include "Controller/Controller.h"

#define SDA1_PIN                        21
#define SCL1_PIN                        22
#define SDA2_PIN                        26
#define SCL2_PIN                        25

#define ZONE1_CO2_ADDR                  60
#define ZONE1_TEMPERATURE_ADDR          61
#define ZONE1_MOISTURE_ADDR             0x28
#define ZONE1_WATER_LEVEL_ADDR          63
#define ZONE1_WINDOW_ACTUATOR_PIN       12
#define ZONE1_CO2_ACTUATOR_PIN          18
#define ZONE1_TANK_ACTUATOR_PIN         19
#define ZONE1_DRINKWATER_ACTUATOR_PIN   23

#define ZONE2_CO2_ADDR                  70
#define ZONE2_TEMPERATURE_ADDR          71
#define ZONE2_MOISTURE_ADDR             0x28
#define ZONE2_WATER_LEVEL_ADDR          73
#define ZONE2_WINDOW_ACTUATOR_PIN       13
#define ZONE2_CO2_ACTUATOR_PIN          20
#define ZONE2_TANK_ACTUATOR_PIN         24
#define ZONE2_DRINKWATER_ACTUATOR_PIN   16

const char *ssid     = "SSID hier eintragen";
const char *password = "Passwort hier eintragen";

#define MQTT_SERVER   "192.168.1.236"
#define MQTT_PORT     1883
#define MQTT_USER     "" 
#define MQTT_PASSWORD ""

TwoWire I2C_1 = TwoWire(0);  // I2C-Bus 1
TwoWire I2C_2 = TwoWire(1);  // I2C-Bus 2

CO2Sensor           zone1CO2Sensor          (I2C_1, ZONE1_CO2_ADDR);
TemperatureSensor   zone1TemperatureSensor  (I2C_1, ZONE1_TEMPERATURE_ADDR);
MoistureSensor      zone1MoistureSensor     (I2C_1, ZONE1_MOISTURE_ADDR);
WaterLevelSensor    zone1WaterLevelSensor   (I2C_1, ZONE1_WATER_LEVEL_ADDR);
WindowActuator      zone1WindowActuator     (ZONE1_WINDOW_ACTUATOR_PIN);
CO2Actuator         zone1CO2Actuator        (ZONE1_CO2_ACTUATOR_PIN);
TankActuator        zone1TankActuator       (ZONE1_TANK_ACTUATOR_PIN);
DrinkWaterActuator  zone1DrinkWaterActuator (ZONE1_DRINKWATER_ACTUATOR_PIN);

CO2Sensor           zone2CO2Sensor          (I2C_2, ZONE2_CO2_ADDR);
TemperatureSensor   zone2TemperatureSensor  (I2C_2, ZONE2_TEMPERATURE_ADDR);
MoistureSensor      zone2MoistureSensor     (I2C_2, ZONE2_MOISTURE_ADDR);
WaterLevelSensor    zone2WaterLevelSensor   (I2C_2, ZONE2_WATER_LEVEL_ADDR);
WindowActuator      zone2WindowActuator     (ZONE2_WINDOW_ACTUATOR_PIN);
CO2Actuator         zone2CO2Actuator        (ZONE2_CO2_ACTUATOR_PIN);
TankActuator        zone2TankActuator       (ZONE2_TANK_ACTUATOR_PIN);
DrinkWaterActuator  zone2DrinkWaterActuator (ZONE2_DRINKWATER_ACTUATOR_PIN);

Zone zone1("Strawberry",
           zone1CO2Sensor, zone1TemperatureSensor, zone1MoistureSensor, zone1WaterLevelSensor,
           zone1WindowActuator, zone1CO2Actuator, zone1TankActuator, zone1DrinkWaterActuator);

Zone zone2("Blackberry",
           zone2CO2Sensor, zone2TemperatureSensor, zone2MoistureSensor, zone2WaterLevelSensor,
           zone2WindowActuator, zone2CO2Actuator, zone2TankActuator, zone2DrinkWaterActuator);

WiFiClient    wifiClient;
MqttPublisher mqttPublisher(wifiClient, MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);
Controller    controller(mqttPublisher, wifiClient);

         
void wifiConnect()
{
  Serial.print("Verbinde mit WLAN...");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("\nVerbunden mit WLAN!");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());  // Zeigt di
}

void setupI2C()
{
  // Starte I2C_1 mit 100kHz
  if (!I2C_1.begin(SDA1_PIN, SCL1_PIN, 100000)) {
    Serial.println("Fehler beim Start von I2C_1");
  } else {
    Serial.println("I2C_1 gestartet");
  }

  // Starte I2C_2 mit 100kHz
  if (!I2C_2.begin(SDA2_PIN, SCL2_PIN, 100000)) {
    Serial.println("Fehler beim Start von I2C_2");
  } else {
    Serial.println("I2C_2 gestartet");
  }  
}

void setup() 
{
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("GreenHouse Start"));

  setupI2C();
  wifiConnect();

  controller.addZone(&zone1);
  controller.addZone(&zone2);
  controller.setup();

  /*
  Serial.println("ScanI2C 1");
  ScanI2C(I2C_1);
  Serial.println("ScanI2C 2");
  ScanI2C(I2C_2);
  */
}

void loop() 
{
  controller.update();
  delay(10);
}
