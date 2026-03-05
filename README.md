# GreenHouse (IoT Workshop)

Projekt: Automatisierung eines Gewächshauses (ESP32 + PlatformIO + MQTT) für zonenbasierte Überwachung/Steuerung.

## Quickstart (VS Code + PlatformIO)
1. Repository klonen
2. In VS Code öffnen
3. PlatformIO: **Build** (Environment: `esp32dev`)
4. Optional: **Monitor** für Logs (115200)

## Projektziele (Kurz)
- Zonen-Regelung für Bewässerung, Klima und CO₂
- MQTT-Vernetzung (Topics dokumentiert unter `docs/mqtt-topics.md`)
- Simulation ohne Hardware (später)
- Tests + CI (später)

## Doku
- Arbeitsjournal: `docs/journal/`

## Quickstart (Windows)

Dieses Projekt ist ein PlatformIO-Projekt (ESP32 / Arduino). Es kann entweder
1) auf echter Hardware gebaut werden (`esp32dev`) oder
2) ohne Hardware als Simulation auf dem PC gebaut werden (`native`).

### Warum brauche ich MSYS2 / MinGW?
Für die Simulation (`pio run -e native`) kompiliert PlatformIO den Code als normales PC-Programm.
Dafür wird ein C/C++-Compiler benötigt (`g++`). Windows bringt standardmäßig keinen `g++` mit.

Wir installieren deshalb **MSYS2 (UCRT64)** und darüber die **MinGW Toolchain**:
- `g++` (C++ Compiler)
- `make` / Build-Tools
- Debug/Runtime-Komponenten

### Installation (nur nötig für Simulation `native`)
1. MSYS2 installieren (UCRT64)
2. In der **MSYS2 UCRT64** Konsole ausführen:
   - `pacman -Syu`
   - danach (Konsole ggf. neu öffnen):
     `pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain`
3. Optional (empfohlen): `C:\msys64\ucrt64\bin` zur Windows-Umgebungsvariable `Path` hinzufügen.
4. Prüfen:
   - `g++ --version`

### Builds

#### ESP32 Build (Hardware)
```powershell
pio run -e esp32dev