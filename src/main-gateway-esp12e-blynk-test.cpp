#include <Arduino.h>
#include "libs/blynk.h"

void myTimerEvent()
{
  Blynk.virtualWrite(V5, (double)millis() / 1000);
}

void setup()
{
  Serial.begin(9600);
  BlynkWrapper::setupBlynk();
  BlynkWrapper::setTimerInterval(10000L, myTimerEvent);
}

void loop()
{
  BlynkWrapper::run();
}
