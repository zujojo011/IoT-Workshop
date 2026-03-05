# GreenHouse (IoT Workshop)

Automatisierung eines Gewächshauses mit ESP32, PlatformIO und MQTT.

Dieses Projekt implementiert ein IoT‑System zur Überwachung und Steuerung eines Gewächshauses mit mehreren Zonen.
Das System kann sowohl auf echter Hardware (ESP32) als auch als Simulation auf dem PC ausgeführt werden.

---

# Projektüberblick

Ziel ist die Entwicklung eines modularen Systems zur Überwachung und automatisierten Steuerung eines Gewächshauses.

Über mehrere logische Zonen werden Sensorwerte erfasst und Aktoren gesteuert.

Beispiele für Sensorwerte:

- Temperatur
- Bodenfeuchtigkeit
- CO₂
- Wasserstand
- Wettervorhersage

Beispiele für Aktoren:

- Bewässerungspumpe
- Fensteröffnung
- Lüftung
- Beschattung

Die Kommunikation erfolgt über MQTT.

---

# Technologie Stack

- ESP32
- PlatformIO
- Arduino Framework
- MQTT (Mosquitto)
- GitHub
- GitHub Actions (CI)
- C++

Für Entwicklung ohne Hardware wird zusätzlich eine native Simulation verwendet.

---

# Repository Struktur

.
├── src/
│   └── main.cpp
│
├── docs/
│   ├── mqtt-topics.md
│   └── Arbeitsjournal
│
├── tools/
│   ├── mqtt_bridge.py
│   └── requirements.txt
│
├── .github/
│   └── workflows/
│       └── ci.yml
│
├── platformio.ini
└── README.md

---

# Quickstart (VS Code + PlatformIO)

1. Repository klonen

git clone <repository-url>

2. Projekt in VS Code öffnen

3. PlatformIO Extension installieren

4. Projekt wird automatisch erkannt

---

# Build (ESP32)

Firmware bauen

pio run -e esp32dev

Firmware hochladen

pio run -e esp32dev -t upload

Seriellen Monitor öffnen

pio device monitor

Baudrate

115200

---

# Simulation ohne Hardware

Die Software kann auch ohne ESP32 getestet werden.

Build

pio run -e native

Simulation starten

pio run -e native -t exec

Die Simulation erzeugt JSON Telemetrie.

---

# MSYS2 Installation (für Simulation)

Windows benötigt einen C++ Compiler.

Deshalb wird MSYS2 installiert.

Download

https://www.msys2.org/

Toolchain installieren

pacman -Syu

Terminal neu öffnen

pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain

PATH hinzufügen

C:\msys64\ucrt64\bin

Installation prüfen

g++ --version

---

# MQTT Integration

Das System nutzt einen Mosquitto Broker.

Standard Adresse

localhost:1883

---

# MQTT testen

Subscriber

mosquitto_sub -h localhost -t "greenhouse/#" -v

Publisher Beispiel

mosquitto_pub -h localhost -t greenhouse/test -m "hello"

---

# Simulation → MQTT Bridge

Python Abhängigkeiten installieren

python -m pip install -r tools/requirements.txt

Simulation starten und MQTT Bridge nutzen

pio run -e native -t exec | python tools/mqtt_bridge.py

---

# Beispiel MQTT Topics

greenhouse/zone1/temperature
greenhouse/zone1/moisture
greenhouse/zone1/co2
greenhouse/zone1/waterLevel
greenhouse/weather/rain_forecast_12h
greenhouse/zone1/window
greenhouse/zone1/pump

---

# Continuous Integration

GitHub Actions führt automatische Builds aus.

CI prüft:

- ESP32 Build
- Simulation Build

Workflow Datei

.github/workflows/ci.yml

---

# Dokumentation

Weitere Dokumentation

docs/

Arbeitsjournal


---

# Git Workflow

Branches

main
develop
feature/*

Regeln

- Commit early
- Commit often
- Push daily

---

# Lizenz

Educational IoT Workshop Project
