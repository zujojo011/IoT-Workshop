#include <stdio.h>
#include <stdint.h>

#ifdef SIMULATION_MODE
	#include <random>
	#include <chrono>
	#include <thread>
#else
	#include <Arduino.h>
#endif

static const char* FW_NAME = "GreenHouse";
static const char* FW_VERSION = "0.1.0";

#ifdef SIMULATION_MODE

static double randRange(std::mt19937 &rng, double minV, double maxV)
{
	std::uniform_real_distribution<double> dist(minV, maxV);
	return dist(rng);
}

static int randInt(std::mt19937 &rng, int minV, int maxV)
{
	std::uniform_int_distribution<int> dist(minV, maxV);
	return dist(rng);
}

int main()
{
	std::random_device rd;
	std::mt19937 rng(rd());

	printf("{\"type\":\"boot\",\"fw\":\"%s\",\"version\":\"%s\",\"mode\":\"simulation\"}\n", FW_NAME, FW_VERSION);

	for (;;)
	{
		// Plausible Demo-Werte
		double temperature = randRange(rng, 16.0, 32.0);
		double moisture = randRange(rng, 0.0, 100.0);
		double co2 = randRange(rng, 350.0, 1600.0);
		double waterLevel = randRange(rng, 0.0, 100.0);
		int rainForecast12h = randInt(rng, 0, 1);

		// Simple states (Demo)
		const char* window = (temperature > 28.0) ? "open" : "close";
		const char* pump = (moisture < 25.0 && waterLevel > 10.0) ? "on" : "off";

		printf("{\"type\":\"telemetry\",\"zone\":\"zone1\","
			   "\"temperature\":%.2f,\"moisture\":%.1f,\"co2\":%.0f,\"waterLevel\":%.1f,"
			   "\"rain_forecast_12h\":%d,\"window\":\"%s\",\"pump\":\"%s\"}\n",
			   temperature, moisture, co2, waterLevel, rainForecast12h, window, pump);

		fflush(stdout);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}

#else

unsigned long lastPrint = 0;

void setup()
{
	Serial.begin(115200);
	delay(200);

	Serial.println("--------------------------------------------------");
	Serial.print(FW_NAME);
	Serial.print(" Boot OK | Version ");
	Serial.println(FW_VERSION);
	Serial.println("--------------------------------------------------");
}

void loop()
{
	// Minimaler Heartbeat, damit man im Monitor sieht: läuft
	if (millis() - lastPrint >= 1000)
	{
		lastPrint = millis();
		Serial.println("GreenHouse running...");
	}
}

#endif