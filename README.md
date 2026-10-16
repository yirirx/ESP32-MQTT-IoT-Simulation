# ESP32 MQTT IoT Simulation

An ESP32-based Internet of Things (IoT) simulation exploring MQTT messaging, sensor telemetry, and remote control of simulated components.

## Interactive demo

[Open the original Wokwi project](https://wokwi.com/projects/349943659510104658)

## Project overview

The ESP32 connects to Wi-Fi and an MQTT broker. It reads temperature and humidity from a DHT22 sensor, publishes telemetry, and listens for MQTT messages that control an LED, servo motor, and NeoPixel strip.

## Repository files

- `sketch.ino` — documented Arduino-style ESP32 sketch with input validation for servo and RGB commands, plus sensor-read checks.
- `diagram.json` — starter Wokwi circuit diagram using the GPIO assignments in the sketch. **Open and test this diagram in Wokwi; adjust any wiring differences to match the original project.**
- `libraries.txt` — Arduino libraries used by the sketch.

## Hardware simulated

- ESP32 development board
- DHT22 temperature and humidity sensor
- LED and current-limiting resistor
- Servo motor
- WS2812/NeoPixel LED strip

## Technologies

- **ESP32 / Arduino C++**
- **MQTT** with PubSubClient
- **Wi-Fi** using the ESP32 Wi-Fi library
- **DHT sensor** using Adafruit DHT libraries
- **Servo control** using ESP32Servo
- **Addressable LEDs** using FastLED
- **Wokwi** for simulation

## MQTT message interface

The sketch uses the public HiveMQ broker at `broker.hivemq.com` on port `1883`.

| Topic | Direction | Payload / purpose |
|---|---|---|
| `Tempdata` | ESP32 → broker | Temperature and humidity as comma-separated values: `temperature,humidity` |
| `lights` | broker → ESP32 | Send `ON` to turn the LED on; other payloads turn it off |
| `servo` | broker → ESP32 | Send an integer angle; the sketch clamps it to 0–180 |
| `lights/neopixel` | broker → ESP32 | Send RGB values as `R,G,B`, for example `255,0,0` |

MQTT topics and payload formats are case-sensitive.

## How to run

1. Open the [Wokwi simulation](https://wokwi.com/projects/349943659510104658) to compare with the original circuit.
2. In this repository, open `sketch.ino`, `diagram.json`, and `libraries.txt`.
3. Import or recreate the circuit in Wokwi and verify that the pin connections match the sketch.
4. Start the simulation and inspect the serial monitor for connection status and sensor readings.
5. Use an MQTT client connected to the same broker to publish control messages to the listed topics, and subscribe to `Tempdata` to inspect telemetry.

## Testing

See [TESTING.md](TESTING.md) for a functional test checklist and a place to record actual results. The checklist is not evidence that tests have passed; run the simulation and document what you observe.

## What this project demonstrates

- Embedded programming with ESP32
- IoT connectivity over Wi-Fi
- MQTT publish/subscribe communication
- Sensor telemetry and message-driven output control
- Input validation and basic error handling
- Simulation-based debugging

## Security and reliability

This is a learning simulation, not a production-ready deployment. The current setup uses a public MQTT broker and unencrypted MQTT on port 1883. Public topics may be visible to other broker users, so do not send private data or use this setup for safety-critical control. A real deployment should use an appropriately secured broker, authentication, TLS, unique topic namespaces, and stronger reconnect/error handling.

## Next improvements

- Verify the circuit diagram by running it in Wokwi.
- Capture a screenshot of the circuit and serial output.
- Record a small test table with example MQTT commands and observed responses.
- Consider using a unique topic prefix instead of generic public topics.

## Attribution

The original interactive project is linked above. This GitHub repository includes a documented sketch and a starter circuit diagram for learning and further development. Credit the original creator if adapting code or design from another source.
