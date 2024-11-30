#include <Arduino.h>
#include "libs/firebase.h"

void setup()
{
  Serial.begin(9600);
  MyFirebase::setupFirebase();
}

void loop()
{
  MyFirebase::sendData();
  MyFirebase::readData();

  delay(1000);
}