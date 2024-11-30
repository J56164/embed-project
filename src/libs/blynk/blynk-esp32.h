/*************************************************************

  Header file for Blynk configuration and initialization
  for ESP32-based projects.

  This file modularizes WiFi credentials, Blynk template 
  configuration, and utility declarations.

 *************************************************************/

#ifndef BLYNK_ESP32_H
#define BLYNK_ESP32_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <BlynkTimer.h>

// Blynk template credentials
#define BLYNK_TEMPLATE_ID "TMPL67zPTaR6h"
#define BLYNK_TEMPLATE_NAME "SmartPot"
#define BLYNK_AUTH_TOKEN "TDYXaafosmqcKyT9PZprEZzw6WHfxG8f"

// WiFi credentials
#define WIFI_SSID "AndroidAP558F"
#define WIFI_PASS "asdf1234"

// Optional: Uncomment to specify a custom Blynk server
// #define BLYNK_SERVER "blynk.cloud"
// #define BLYNK_PORT 80

// Debugging
#define BLYNK_PRINT Serial

// Timer instance
extern BlynkTimer timer;

// Function to initialize Blynk and WiFi
void setupBlynk();

// Example user-defined timer function
void myTimerEvent();

#endif // BLYNK_ESP32_H
