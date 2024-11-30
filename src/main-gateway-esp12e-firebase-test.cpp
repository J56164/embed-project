#include <Arduino.h>
#include "libs/firebase.h"
#include "libs/wifi.h"

void setup()
{
  Serial.begin(9600);
  WiFiWrapper::setWiFiMode(WIFI_STA);
  WiFiWrapper::setupWiFi();
  FirebaseWrapper::setupFirebase();
}

void loop()
{
  FirebaseWrapper::sendFloatData("Sensors/Humidity", 3.14);
  FirebaseWrapper::sendIntData("Sensors/SoilMoisture", 1234);
  FirebaseWrapper::readFloatData("Sensors/Humidity");
  FirebaseWrapper::readIntData("Sensors/SoilMoisture");

  delay(1000);
}