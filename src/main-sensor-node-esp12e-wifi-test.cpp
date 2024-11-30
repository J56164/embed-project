#include <Arduino.h>
#include "libs/wifi.h"

uint8_t broadcastAddress[6] = {0x88, 0x13, 0xBF, 0x0C, 0x31, 0xA4};

typedef struct struct_message
{
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0)
  {
    Serial.println("Delivery success");
  }
  else
  {
    Serial.println("Delivery fail");
  }
}

void sendSensorData()
{
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
  Serial.begin(9600);
  WiFiWrapper::setWiFiMode(WIFI_STA);
  WiFiWrapper::setupWiFi();
  WiFiWrapper::setupESPNow(false);
  WiFiWrapper::registerSendCallback(OnDataSent);
  WiFiWrapper::addPeer(broadcastAddress);
}

void loop()
{
  Serial.println(WiFiWrapper::getMacAddress());
  sendSensorData();
  delay(1000);
}