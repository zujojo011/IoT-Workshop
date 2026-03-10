# Smart Greenhouse IoT-Projekt

##  Einleitung
Dieses Projekt ist ein **automatisiertes Smart-Greenhouse-System**, das mit IoT-Technologie **Umweltbedingungen überwacht und steuert**. Es nutzt einen **ESP32 MicroMod Processor** mit Sensoren und Aktoren, um die idealen Wachstumsbedingungen für Pflanzen sicherzustellen. Das System **kommuniziert über MQTT**, sammelt Wetterdaten und **optimiert den Wasserverbrauch** durch intelligente Steuerung.

**Hinweis:** Aufgrund der **Übersichtlichkeit des Projekts** wurde auf separate `.cpp`-Dateien verzichtet und stattdessen nur `.h`-Dateien verwendet. Dies erleichtert die Verwaltung und Anpassung des Codes in kleineren Projekten.

##  Funktionen
- **Zonenbasierte Steuerung**: Mehrere Pflanzenbereiche werden individuell überwacht & gesteuert.
- **Sensorintegration**: Temperatur, Luftfeuchtigkeit, CO₂-Gehalt und Bodenfeuchtigkeit werden gemessen.
- **Aktuatorsteuerung**: Fenster öffnen, CO₂-Ventil aktivieren, Pflanzenbewässerung starten.
- **Trinkwasser-Nachfüllung**: Automatisches Auffüllen des Reservoirs bei niedrigem Wasserstand.
- **Intelligente Wetterintegration**: Überprüfung der Niederschlagswahrscheinlichkeit aus der OpenWeather API.
- **Optimierte MQTT-Kommunikation**: Daten werden **nur bei Änderungen** gesendet, um Bandbreite zu sparen.
- **Nicht-blockierender Code**: Steuerung über `StopWatch`, keine blockierenden `delay()`-Funktionen.

---

##  Installation
###  Voraussetzungen
- **ESP32 MicroMod Processor** + **SparkFun ATP Carrier Board**
- **Arduino IDE oder PlatformIO**
- **MQTT-Broker** (z. B. Mosquitto)
- **Sensormodule** für Temperatur, Feuchtigkeit, Wasserstand, CO₂
- **Internetverbindung für OpenWeather API** *(eigener API-Key erforderlich!)*

###  Einrichtung
1️ **Code auf ESP32 hochladen**  
   - Installiere alle benötigten **Bibliotheken** in PlatformIO oder der Arduino IDE.
   - Trage in `config.h` die **WLAN-Zugangsdaten & MQTT-Server-Infos** ein.
   - Erstelle einen **eigenen API-Key für OpenWeather** und trage ihn in `config.h` ein.
   - Kompiliere und lade den Code auf den ESP32 hoch.

   2️ **MQTT-Server einrichten**  
     - Falls Mosquitto lokal verwendet wird, installiere es:
     ```sh
     sudo apt install mosquitto mosquitto-clients
     ```
     - Bearbeite `/etc/mosquitto/mosquitto.conf` und aktiviere anonyme Verbindungen:
     ```
     listener 1883
     allow_anonymous true
     ```
     - Starte Mosquitto neu:
     ```sh
     sudo systemctl restart mosquitto
     ```
     - **Firewall für MQTT-Port (1883) freigeben:**
     ```sh
     sudo ufw allow 1883/tcp
     ```

      3 **Hardware verbinden**  
         - Verbinde Sensoren & Aktoren gemäß folgender GPIO-Belegung:
         ```c
         #define ZONE1_WINDOW_ACTUATOR_PIN       12
         #define ZONE1_CO2_ACTUATOR_PIN          18
         #define ZONE1_TANK_ACTUATOR_PIN         19
         #define ZONE1_DRINKWATER_ACTUATOR_PIN   23
         ```

         4 **I²C-Adressen für Sensoren ermitteln** *(falls andere Sensoren verwendet werden)*  
                - Bevor die I²C-Adresse des **Moisture Sensors** (`ZONE1_MOISTURE_ADDR`) oder anderer Sensoren festgelegt wird, sollte die Adresse mit `setupI2C()` ausgelesen werden.
                - Falls die Adresse abweicht, muss sie in `config.h` angepasst werden:
                ```c
                #define ZONE1_MOISTURE_ADDR 0x28 // Ändern, wenn nötig
                ```

---

##  Nutzung
- Nach dem Hochladen verbindet sich der ESP32 automatisch mit dem WLAN & MQTT-Broker.
- Sensorwerte werden regelmäßig über **MQTT** an den Server gesendet.
- Aktoren können über MQTT-Befehle gesteuert werden.
- OpenWeather API wird **alle 12 Stunden** abgefragt, um die Niederschlagswahrscheinlichkeit zu ermitteln.

###  MQTT-Test
Überprüfe die Sensordaten mit Mosquitto:
```sh
mosquitto_sub -h <MQTT_SERVER_IP> -t "greenhouse/#" -v
```
Beispielwerte:
```
greenhouse/strawberry/Temperature: 24.5°C
greenhouse/strawberry/Humidity: 60%
greenhouse/strawberry/Window: OPEN
```

---

##  Wichtige Hinweise & Fehlerbehebung
###  Probleme mit MQTT-Verbindung
Falls der ESP32 sich nicht mit dem MQTT-Server verbindet:
1. Prüfe, ob Mosquitto läuft:
   ```sh
   systemctl status mosquitto
   ```
2. Teste eine Verbindung:
   ```sh
   mosquitto_pub -h <MQTT_SERVER_IP> -t "test" -m "Hello"
   ```

###  OpenWeather API funktioniert nicht
- Stelle sicher, dass der **API-Key korrekt in `config.h`** eingetragen ist.
- Prüfe die **Internetverbindung des ESP32**.

###  Sensorwerte aktualisieren nicht
Falls Sensorwerte **nicht aktualisiert werden**:
- Prüfe die **Verbindungen & Pins** der I²C-Sensoren.
- Starte den ESP32 neu.
- Lese die **I²C-Adressen** mit `setupI2C()` aus und passe `config.h` an.

---

