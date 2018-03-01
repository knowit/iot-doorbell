#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "secrets.h"

const int DOORBELL_PIN = D3;

int lastDoorbellClickState = 0;

void connectToWifi(const char *ssid, const char *password) {
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("\nConnected. Got IP: %s\n", WiFi.localIP().toString());
}

void setup() {
  Serial.begin(9600);
  connectToWifi(SSID, WIFI_PW);

  pinMode(DOORBELL_PIN, INPUT_PULLUP);
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from WiFi");
    connectToWifi(SSID, WIFI_PW);
  }

  int doorbellState = digitalRead(DOORBELL_PIN);

  Serial.print("Current state: ");
  Serial.println(doorbellState);

  if (doorbellState != lastDoorbellClickState) {
    Serial.println("Doorbell ring ring!");
  }

  lastDoorbellClickState = doorbellState;
}
