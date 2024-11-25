#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "libs/sensors.h"

void blinkLed()
{
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}

void setup()
{
  pinMode(LED, OUTPUT);
  dht.begin();
  // pinMode(SOIL_1, INPUT);
  // pinMode(SOIL_2, INPUT);
  pinMode(PUMP, OUTPUT);
  Serial.begin(9600);
  setupWiFi();
}

void loop()
{
  // blinkLed();
  // getSoil();
  // getDht();
  // getLight();
  // enablePump();
  getMacAddress();
  delay(1000);
}