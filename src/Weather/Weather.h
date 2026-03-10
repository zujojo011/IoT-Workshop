#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char *city   = "Zürich";
const char *apiKey = "44fa6a1e90b1b87fca2019af95ada4e4";

class Weather
{
public:
  static bool rainInNext12Hours(WiFiClient &client)
  {
    Serial.println("rainInNext12Hours");

    HTTPClient http;

    // Baue die API-URL korrekt zusammen
    String apiUrl = "http://api.openweathermap.org/data/2.5/forecast?q=" + String(city) + "&appid=" + String(apiKey) + "&units=metric";

    Serial.println("OpenWeatherCall: " + apiUrl);
    
    http.begin(client, apiUrl);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(4096);
      deserializeJson(doc, payload);

      // Prüfe die Regenwahrscheinlichkeit in den nächsten 12 Stunden (4 Einträge à 3h)
      for (int i = 0; i < 4; i++) {
        if (doc["list"][i]["weather"][0]["main"] == "Rain") {
          http.end();
          Serial.println("Regen in den nächsten 12 Stunden");
          return true;
        }
      }
    }
    http.end();
    Serial.println("Kein Regen in den nächsten 12 Stunden");
    return false;
  }
};
