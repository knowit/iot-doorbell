#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

#define FW_VERSION "1.0.0"
#define MQTT_SERVER "192.168.150.99"

const int DOORBELL_PIN = D3;
unsigned long lastDoorBellRing = 0;

const char* baseTopic = "sensor/stavanger/door/bell/";

PubSubClient* client;
WiFiClient espClient;

void connectMqtt() {
  if (!client->connected()) {
    client->connect("doorbell", MQTT_USER, MQTT_PASS);
    Serial.println(client->state());
  }
}

void publishMqtt(const char *topic, const char *value, bool retain) {
  connectMqtt();
  String finalTopic = baseTopic;
  finalTopic.concat( topic );
  client->publish(finalTopic.c_str(), value, retain);
}

void connectToWifi(const char *ssid, const char *password) {
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  connectMqtt();
  Serial.printf("\nConnected. Got IP: %s\n", WiFi.localIP().toString().c_str());
  publishMqtt("ip", WiFi.localIP().toString().c_str(), true);
}

bool doorBellTriggered() {
  unsigned long currentTime = millis();

  if (currentTime - lastDoorBellRing > 15000 && digitalRead(DOORBELL_PIN) != 1) {
    lastDoorBellRing = currentTime;
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  pinMode(DOORBELL_PIN, INPUT_PULLUP);
  client = new PubSubClient(espClient);
  client->setServer(MQTT_SERVER, 1883);
  connectToWifi(SSID, WIFI_PW);
  publishMqtt("firmware", FW_VERSION, true);
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from WiFi");
    connectToWifi(SSID, WIFI_PW);
  }

  if (doorBellTriggered()) {
    Serial.println("Doorbell ring ring!");
    publishMqtt("ring", "ding", false);
  }
}
