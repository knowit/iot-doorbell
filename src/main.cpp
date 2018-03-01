#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "secrets.h"

const char *ssid = SSID;
const char *password = WIFI_PW;
const int DOORBELL_PIN = D3;

int lastDoorbellClickState = 0;

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
}

void loop()
{
  delay(1000);

  int doorbellState = digitalRead(DOORBELL_PIN);

  Serial.print("Current state: ");
  Serial.println(doorbellState);

  if (doorbellState != lastDoorbellClickState) {
    Serial.println("Doorbell ring ring!");
  }

  lastDoorbellClickState = doorbellState;
}
