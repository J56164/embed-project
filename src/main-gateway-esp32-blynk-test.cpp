/*************************************************************

  Modularized Blynk example for ESP32

  This program demonstrates how values can be pushed from 
  Arduino to the Blynk App using a modularized structure.

  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer

  App dashboard setup:
    Value Display widget attached to Virtual Pin V5.

 *************************************************************/

#include "blynk-esp32.h"

// Timer instance
BlynkTimer timer;

// This function sends Arduino's uptime every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH.
void myTimerEvent()
{
  // Send uptime in seconds to Blynk Virtual Pin V5
  Blynk.virtualWrite(V5, (double)millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  // Initialize Blynk and WiFi
  setupBlynk();

  // Setup a timer to call `myTimerEvent` every 10 seconds
  timer.setInterval(10000L, myTimerEvent);
}

void loop()
{
  // Run Blynk
  Blynk.run();

  // Run the timer
  timer.run();
}
