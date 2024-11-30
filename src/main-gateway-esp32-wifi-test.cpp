#include <Arduino.h>
#include "libs/wifi.h"

typedef struct struct_message
{
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

struct_message myData;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("String: ");
  Serial.println(myData.d);
  Serial.print("Bool: ");
  Serial.println(myData.e);
  Serial.println();
}

void setup()
{
  Serial.begin(9600);
  WiFiWrapper::setWiFiMode(WIFI_AP_STA);
  WiFiWrapper::setupWiFi();
  WiFiWrapper::setupESPNow(true); // ESPNow must be initialized after WiFi for some reason
  WiFiWrapper::registerRecvCallback(OnDataRecv);
}

void loop()
{
  Serial.println(WiFiWrapper::getMacAddress());
  delay(1000);
}