#include <Arduino.h>
#include "libs/firebase.h"

void setup()
{
  Serial.begin(9600);
  MyFirebase::setupFirebase();
}

void loop()
{
  MyFirebase::sendFloatData("Sensors/Humidity", 3.14);
  MyFirebase::sendIntData("Sensors/SoilMoisture", 1234);
  MyFirebase::readFloatData("Sensors/Humidity");
  MyFirebase::readIntData("Sensors/SoilMoisture");

  delay(1000);
}