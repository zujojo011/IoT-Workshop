#pragma once
#include <Arduino.h>
#include <Wire.h> 


// Scanne alle Adressen von 1 bis 127 (Standard I2C-Adressbereich)
void ScanI2C(TwoWire &wire)
{
  int     nDevices = 0;
  uint8_t error, address;

  for (address = 1; address < 127; address++) {
    wire.beginTransmission(address);  // Versuche eine Verbindung mit der Adresse
    error = wire.endTransmission();   // Prüfe, ob die Verbindung erfolgreich war
    
    if (error == 0) { // Erfolgreiche Kommunikation
      Serial.print("Gerät gefunden bei Adresse 0x");
      Serial.println(address, HEX);  // Gebe die Adresse im Hex-Format aus
      nDevices++;
    }
    delay(10);  // Kurze Verzögerung für Stabilität
  }

  if (nDevices == 0) {
    Serial.println("Keine I2C-Geräte gefunden");
  } else {
    Serial.println("Scan abgeschlossen");
  }
}
