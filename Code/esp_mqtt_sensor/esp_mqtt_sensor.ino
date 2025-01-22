#include <WiFi.h>
#include <PubSubClient.h>
#include <string>
#include <vector>
#include "sensor.h"
#include "../WiFiCredentials.h"

const char* wifi_ssid = WIFI_SSID;
const char* wifi_password = WIFI_PASSWORD;
const char* mqtt_client_username = MQTT_CLIENT_USERNAME;
const char* mqtt_client_password = MQTT_CLIENT_PASSWORD;
const char* mqtt_device_name = "esp32";
const char* mqtt_topic = "sensor/plant/coffea/";

// MQTT variables
IPAddress mqttServer(192, 168, 1, 50);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  Serial.begin(115200);
  initSensors();

  Serial.println("Connecting to WiFi and MQTT");
  mqttClient.setServer(mqttServer, 1883);
  WiFi.begin(wifi_ssid, wifi_password);
}

void mqttReconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(mqtt_device_name, mqtt_client_username, mqtt_client_password)) {
      Serial.println("Mqtt connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

float hum = 0, temp = 0, press = 0, lum = 0;
std::vector<char *> topic_names = {"humidity", "temperature", "pressure", "luminosity"};

void loop() {
  if (!mqttClient.connected()) {
    mqttReconnect();
  } else {
    getReadings(hum, temp, press, lum);

    mqttClient.publish((std::string(mqtt_topic) + "humidity").c_str(), std::to_string(hum).c_str(), true);
    mqttClient.publish((std::string(mqtt_topic) + "temperature").c_str(), std::to_string(temp).c_str(), true);
    mqttClient.publish((std::string(mqtt_topic) + "pressure").c_str(), std::to_string(press).c_str(), true);
    mqttClient.publish((std::string(mqtt_topic) + "luminosity").c_str(), std::to_string(lum).c_str(), true);

    sleep(60);
  }
}
