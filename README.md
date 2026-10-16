# ESP32 MQTT IoT Simulation

An ESP32-based Internet of Things (IoT) simulation exploring MQTT messaging, sensor telemetry, and remote control of simulated components.

## Interactive demo

[Open the project in Wokwi](https://wokwi.com/projects/349943659510104658)

## Project overview

The simulated ESP32 connects to Wi-Fi and an MQTT broker. It periodically reads temperature and humidity from a DHT22 sensor, publishes the readings, and listens for MQTT messages that control output components.

## Hardware simulated

- ESP32 development board
- DHT22 temperature and humidity sensor
- LED
- Servo motor
- WS2812/NeoPixel LED strip

## Technologies and libraries

- **ESP32 / Arduino C++**
- **MQTT** using the PubSubClient library
- **Wi-Fi** using the ESP32 Wi-Fi library
- **DHT sensor** using Adafruit DHT libraries
- **Servo control** using ESP32Servo
- **Addressable LEDs** using FastLED
- **Wokwi** for circuit simulation

## MQTT message interface

The current Wokwi sketch uses the public HiveMQ broker at `broker.hivemq.com` on port `1883`.

| Topic | Direction | Payload / purpose |
|---|---|---|
| `Tempdata` | ESP32 → broker | Publishes temperature and humidity as comma-separated values |
| `lights` | broker → ESP32 | Send `ON` to turn the LED on; other payloads turn it off |
| `servo` | broker → ESP32 | Send an angle as an integer |
| `lights/neopixel` | broker → ESP32 | Send RGB values as `R,G,B`, for example `255,0,0` |

MQTT topics and payload formats are case-sensitive and must match the sketch.

## How to run

1. Open the [Wokwi simulation](https://wokwi.com/projects/349943659510104658).
2. Start the simulation.
3. Open the serial monitor to inspect Wi-Fi/MQTT connection messages and sensor readings.
4. Use an MQTT client that can connect to the same broker to publish control messages to the topics listed above.
5. Subscribe to `Tempdata` to inspect the telemetry published by the ESP32.

## What this project demonstrates

- Basic embedded programming with ESP32
- IoT device connectivity over Wi-Fi
- MQTT publish/subscribe communication
- Sensor telemetry and message-driven output control
- Simulating and debugging an embedded IoT workflow

## Security and reliability notes

This is a learning simulation, not a production-ready deployment. The current sketch uses a public MQTT broker and unencrypted MQTT on port 1883. Public topics may be visible to other broker users, so do not send private data or use this setup for safety-critical control. A real deployment should use an appropriately secured broker, authentication, TLS, unique topic namespaces, and input validation.

The sketch should also be reviewed for reconnect behavior and sensor-read error handling before being used beyond simulation.

## Next steps

- Add the Wokwi source files (`sketch.ino`, `diagram.json`, and `libraries.txt`) to this repository.
- Capture a screenshot of the running simulation and MQTT messages.
- Add a short test log showing example telemetry and control commands.
- Consider a small improvement, such as validating servo angles and RGB ranges before applying received values.

## Attribution

The interactive circuit and sketch are linked from the Wokwi project above. Any code or design adapted from another source should retain appropriate attribution.
