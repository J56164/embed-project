#include <Arduino.h>
#include "blynk-esp8266.h"

// Define the timer instance
BlynkTimer timer;

void myTimerEvent()
{
  Blynk.virtualWrite(V5, (double)millis() / 1000);
}

void setup()
{
  Serial.begin(9600);
  setupBlynk();
  timer.setInterval(10000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}
