/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include "libs/wifi.h"

uint8_t broadcastAddress[6] = {0xC4, 0x5B, 0xBE, 0x6F, 0xBD, 0x0B};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void sendSensorData()
{
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1, 20);
  myData.c = 1.2;
  myData.d = "Hello";
  myData.e = false;

  // Send message via ESP-NOW
  bool isDataSent = WiFiWrapper::sendData(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (isDataSent)
  {
    Serial.println("Data sent successfully");
  }
  else
  {
    Serial.println("Error sending data");
  }
}

void setup()
{
  // Init Serial Monitor
  Serial.begin(9600);
  WiFiWrapper::setWiFiMode(WIFI_STA);
  WiFiWrapper::setupWiFi();
  WiFiWrapper::setupESPNow(false);
  WiFiWrapper::addPeer(broadcastAddress);
  WiFiWrapper::registerSendCallback(OnDataSent);
}

void loop()
{
  Serial.println(WiFiWrapper::getMacAddress());
  sendSensorData();
  delay(1000);
}