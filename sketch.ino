#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <FastLED.h>

// Pin configuration
#define DHTPIN 12
#define LED 26
#define SERVO_PIN 2
#define LED_PIN 4
#define NUM_LEDS 16
#define DHTTYPE DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);
Servo servo;
CRGB leds[NUM_LEDS];

// Wokwi guest Wi-Fi is for simulation only.
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";
const char* clientID = "esp32-mqtt-iot-simulation";
const char* publishTopic = "Tempdata";

unsigned long previousMillis = 0;
const unsigned long interval = 1000;

float temp = NAN;
float hum = NAN;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");

    if (client.connect(clientID)) {
      Serial.println("connected");
      client.subscribe("lights");
      client.subscribe("servo");
      client.subscribe("lights/neopixel");
      Serial.println("Subscribed to control topics");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("; retrying in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  String data;
  data.reserve(length);
  for (unsigned int i = 0; i < length; i++) {
    data += static_cast<char>(payload[i]);
  }

  Serial.print("Payload: ");
  Serial.println(data);

  if (String(topic) == "lights") {
    digitalWrite(LED, data == "ON" ? HIGH : LOW);
  } else if (String(topic) == "servo") {
    // Keep the requested angle within the servo's usual range.
    int degree = constrain(data.toInt(), 0, 180);
    Serial.print("Moving servo to: ");
    Serial.println(degree);
    servo.write(degree);
  } else if (String(topic) == "lights/neopixel") {
    int red = 0, green = 0, blue = 0;

    if (sscanf(data.c_str(), "%d,%d,%d", &red, &green, &blue) == 3) {
      red = constrain(red, 0, 255);
      green = constrain(green, 0, 255);
      blue = constrain(blue, 0, 255);

      fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
      FastLED.show();
      Serial.printf("NeoPixel RGB: %d,%d,%d\n", red, green, blue);
    } else {
      Serial.println("Invalid RGB payload. Expected R,G,B");
    }
  }
}

void setup() {
  Serial.begin(115200);

  dht.begin();
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  servo.attach(SERVO_PIN, 500, 2400);
  servo.write(0);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear(true);

  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  const unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Reset readings so a failed sensor read cannot reuse stale values.
    temp = NAN;
    hum = NAN;

    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println("Error reading temperature");
    } else {
      temp = event.temperature;
      Serial.printf("Temperature: %.1f C\n", temp);
    }

    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println("Error reading humidity");
    } else {
      hum = event.relative_humidity;
      Serial.printf("Humidity: %.1f %%\n", hum);
    }

    // Publish only when both readings are valid.
    if (!isnan(temp) && !isnan(hum)) {
      String payload = String(temp, 1) + "," + String(hum, 1);
      Serial.print("Publishing to ");
      Serial.print(publishTopic);
      Serial.print(": ");
      Serial.println(payload);
      client.publish(publishTopic, payload.c_str());
    }
  }
}
