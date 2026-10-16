#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

// Pin configuration
#define DHTPIN 12
#define LED_PIN_BUILTIN 26
#define SERVO_PIN 2
#define NEOPIXEL_PIN 4
#define NUM_LEDS 16
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
Servo servo;
Adafruit_NeoPixel pixels(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Wokwi guest Wi-Fi is for simulation only.
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";
const char* clientID = "esp32-mqtt-iot-simulation";
const char* publishTopic = "Tempdata";

unsigned long previousMillis = 0;
const unsigned long interval = 2000;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected");
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
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("; retrying in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String data;
  data.reserve(length);
  for (unsigned int i = 0; i < length; i++) {
    data += static_cast<char>(payload[i]);
  }

  Serial.print("MQTT message [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(data);

  if (String(topic) == "lights") {
    digitalWrite(LED_PIN_BUILTIN, data == "ON" ? HIGH : LOW);
  } else if (String(topic) == "servo") {
    int degree = constrain(data.toInt(), 0, 180);
    servo.write(degree);
  } else if (String(topic) == "lights/neopixel") {
    int red, green, blue;
    if (sscanf(data.c_str(), "%d,%d,%d", &red, &green, &blue) == 3) {
      red = constrain(red, 0, 255);
      green = constrain(green, 0, 255);
      blue = constrain(blue, 0, 255);

      for (int i = 0; i < NUM_LEDS; i++) {
        pixels.setPixelColor(i, pixels.Color(red, green, blue));
      }
      pixels.show();
    } else {
      Serial.println("Invalid RGB payload. Expected R,G,B");
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN_BUILTIN, OUTPUT);
  digitalWrite(LED_PIN_BUILTIN, LOW);

  dht.begin();
  servo.attach(SERVO_PIN, 500, 2400);
  servo.write(0);

  pixels.begin();
  pixels.clear();
  pixels.show();

  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  const unsigned long now = millis();
  if (now - previousMillis >= interval) {
    previousMillis = now;

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("DHT22 read failed; skipping this publish");
      return;
    }

    char payload[32];
    snprintf(payload, sizeof(payload), "%.1f,%.1f", temperature, humidity);

    Serial.print("Publishing to ");
    Serial.print(publishTopic);
    Serial.print(": ");
    Serial.println(payload);

    if (!client.publish(publishTopic, payload)) {
      Serial.println("MQTT publish failed");
    }
  }
}
