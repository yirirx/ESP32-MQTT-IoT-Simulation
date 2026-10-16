# Test Plan

This checklist is a test plan, not a claim that the simulation has already passed. Run the tests in Wokwi and record the actual results before presenting the project as verified.

## Setup checks

- [ ] Open the original [Wokwi project](https://wokwi.com/projects/349943659510104658).
- [ ] Open the repository's `sketch.ino`, `diagram.json`, and `libraries.txt`.
- [ ] Import/recreate the repository circuit in Wokwi and verify every connection matches the pin assignments in the sketch.
- [ ] Start the simulation and confirm Wi-Fi and MQTT connection messages appear in the serial monitor.

## Functional tests

| Test | Action | Expected result |
|---|---|---|
| Sensor telemetry | Run the simulation and observe the serial monitor | Temperature and humidity are printed when sensor readings are valid |
| MQTT publish | Subscribe to `Tempdata` on the same MQTT broker | Receive a payload in `temperature,humidity` format |
| LED on | Publish `ON` to topic `lights` | LED turns on |
| LED off | Publish `OFF` to topic `lights` | LED turns off |
| Servo | Publish `90` to topic `servo` | Servo moves toward 90 degrees |
| Servo bounds | Publish `200` to topic `servo` | Sketch clamps the angle to 180 degrees |
| NeoPixel | Publish `255,0,0` to `lights/neopixel` | LED strip displays red |
| Invalid RGB | Publish a malformed payload to `lights/neopixel` | Sketch reports an invalid RGB payload instead of applying it |

## Record your results

Fill this in after running the tests:

- **Date tested:**
- **Wokwi circuit verified:** Yes / No
- **Wi-Fi connection:** Pass / Fail
- **MQTT connection:** Pass / Fail
- **Sensor telemetry:** Pass / Fail
- **LED control:** Pass / Fail
- **Servo control:** Pass / Fail
- **NeoPixel control:** Pass / Fail
- **Issues noticed:**

## Important limitation

The project uses a public MQTT broker over unencrypted port 1883 for learning purposes. Avoid sending sensitive data and do not use this setup for safety-critical control. Public topic names may collide with messages from other users.
