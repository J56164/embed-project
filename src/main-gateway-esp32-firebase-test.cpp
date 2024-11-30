#include <Arduino.h>
#include "libs/firebase.h"

void setup()
{
  Serial.begin(9600);
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