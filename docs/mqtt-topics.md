# MQTT Topic-Spezifikation (GreenHouse)

## Basis
- Prefix: `greenhouse/`
- Zonen: `zone1` (Demo), später `zone2`, ...
- Payload: Standard = String/Number, optional JSON

## Telemetrie (Sensoren)
- `greenhouse/<zone>/temperature` (°C)
- `greenhouse/<zone>/moisture` (%)
- `greenhouse/<zone>/co2` (ppm)
- `greenhouse/<zone>/waterLevel` (%)

## Virtueller Sensor (Wetter)
- `greenhouse/weather/rain_forecast_12h` (`0` oder `1`)

## Aktor-Status (Demo)
- `greenhouse/<zone>/window` (`open`/`close`)
- `greenhouse/<zone>/pump` (`on`/`off`)

## Logging (optional später)
- `greenhouse/log/info`
- `greenhouse/log/warn`
- `greenhouse/log/error`