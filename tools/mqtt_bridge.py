import sys
import json
import time

import paho.mqtt.client as mqtt

BROKER_HOST = "127.0.0.1"
BROKER_PORT = 1883

TOPIC_PREFIX = "greenhouse"

def publish_telemetry(client, msg: dict):
	zone = msg.get("zone", "zone1")

	# Wetter (global)
	if "rain_forecast_12h" in msg:
		client.publish(f"{TOPIC_PREFIX}/weather/rain_forecast_12h", str(int(msg["rain_forecast_12h"])), retain=True)

	# Zone values
	for key in ["temperature", "moisture", "co2", "waterLevel", "window", "pump"]:
		if key in msg:
			client.publish(f"{TOPIC_PREFIX}/{zone}/{key}", str(msg[key]), retain=False)

def main():
	client = mqtt.Client()
	client.connect(BROKER_HOST, BROKER_PORT, keepalive=60)
	client.loop_start()

	print(f"[bridge] connected to mqtt://{BROKER_HOST}:{BROKER_PORT}")
	print("[bridge] reading JSON lines from stdin...")

	for line in sys.stdin:
		line = line.strip()
		if not line:
			continue

		try:
			msg = json.loads(line)
		except Exception as e:
			print(f"[bridge] invalid json: {e} | line={line[:120]}")
			continue

		if msg.get("type") == "boot":
			client.publish(f"{TOPIC_PREFIX}/sim/boot", json.dumps(msg), retain=True)
			continue

		if msg.get("type") == "telemetry":
			publish_telemetry(client, msg)
			continue

	# graceful shutdown
	client.loop_stop()
	client.disconnect()

if __name__ == "__main__":
	main()