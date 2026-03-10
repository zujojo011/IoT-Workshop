#pragma once

#include <WiFi.h>
#include <PubSubClient.h>

class MqttPublisher
{
private:
  WiFiClient  &espClient;
  PubSubClient client;
  const char  *mqttServer;
  int          mqttPort;
  const char  *mqttUser;
  const char  *mqttPassword;

public:
  MqttPublisher(WiFiClient &espClient, const char *server, int port, const char *user, const char *password)
    : espClient(espClient)
    , client(espClient)
    , mqttServer(server)
    , mqttPort(port)
    , mqttUser(user)
    , mqttPassword(password)
  {
    client.setServer(mqttServer, mqttPort);
  }

  bool isConnected()
  {
    return client.connected();
  }

  bool connect(const char *clientId)
  {
    Serial.println("MqttPublisher::connect(" + String(clientId) + ")");
    
    int retries = 3; // Maximal 3 Verbindungsversuche
    while (!client.connected() && retries > 0) {
      Serial.print("Versuche MQTT-Verbindung... ");
      
      bool success;
      if (mqttUser && mqttPassword) {
        success = client.connect(clientId, mqttUser, mqttPassword);
      } else {
        success = client.connect(clientId);
      }

      if (success) {
        Serial.println("✔ Verbunden mit MQTT-Broker!");
        return true;
      } else {
        Serial.print("❌ Verbindung fehlgeschlagen (rc=");
        Serial.print(client.state());
        Serial.println("), versuche erneut...");
        retries--;
        delay(2000);  // Wartezeit vor erneutem Versuch
      }
    }

    Serial.println("❌ Konnte keine MQTT-Verbindung aufbauen!");
    return false;
  }
  
  void publish(const char *topic, const char *payload)
  {
    Serial.println("MqttPublisher::publish(" + String(topic) + ", " + String(payload) + ")");

    if (client.connected()) {
      client.publish(topic, payload);
    } else {
      Serial.println("MQTT Client not connected. Unable to publish.");
    }
  }

  void update()
  {
    client.loop();
  }
};
