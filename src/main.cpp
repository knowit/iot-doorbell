#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
#include "secrets.h"

#define FW_VERSION "1.0.0"
#define MQTT_SERVER "192.168.150.99"

const char *ssid = SSID;
const char *password = WIFI_PW;
const int DOORBELL_PIN = D3;
int lastDoorbellClickState = 0;

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

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  pinMode(DOORBELL_PIN, INPUT_PULLUP);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client = new PubSubClient(espClient);
  client->setServer(MQTT_SERVER, 1883);

  connectMqtt();
  publishMqtt("firmware", FW_VERSION, true);
  publishMqtt("ip", WiFi.localIP().toString().c_str(), true);
}

void loop()
{
  delay(1000);

  int doorbellState = digitalRead(DOORBELL_PIN);

  Serial.print("Current state: ");
  Serial.println(doorbellState);

  if (doorbellState != lastDoorbellClickState) {

    connectMqtt();
    Serial.println("Doorbell ring ring!");
    publishMqtt("ring", "ding", false);
  }

  lastDoorbellClickState = doorbellState;
}
