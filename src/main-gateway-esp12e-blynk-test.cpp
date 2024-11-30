#include <Arduino.h>
#include "libs/blynk.h"

void sendDataToBlynk()
{
  Blynk.virtualWrite(V5, (double)millis() / 1000);
}

void setup()
{
  Serial.begin(9600);
  BlynkWrapper::setupBlynk();
  BlynkWrapper::setTimerInterval(10000L, sendDataToBlynk);
}

void loop()
{
  BlynkWrapper::run();
}
