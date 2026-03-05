#include <stdio.h>
#include <stdint.h>

#ifndef SIMULATION_MODE
#include <Arduino.h>
#endif


static const char* FW_NAME = "GreenHouse";
static const char* FW_VERSION = "0.1.0";


#ifdef SIMULATION_MODE

int main()
{
    printf("----------------------------------\n");
    printf("%s Simulation Boot\n", FW_NAME);
    printf("Version: %s\n", FW_VERSION);
    printf("----------------------------------\n");

    int counter = 0;

    while (counter < 5)
    {
        printf("GreenHouse Simulation Alive %d\n", counter);
        counter++;
    }

    return 0;
}

#else

unsigned long lastHeartbeat = 0;

void setup()
{
    Serial.begin(115200);

    Serial.println("----------------------------------");
    Serial.print(FW_NAME);
    Serial.print(" Boot OK Version ");
    Serial.println(FW_VERSION);
    Serial.println("----------------------------------");
}

void loop()
{
    if (millis() - lastHeartbeat > 1000)
    {
        lastHeartbeat = millis();
        Serial.println("GreenHouse Alive");
    }
}

#endif