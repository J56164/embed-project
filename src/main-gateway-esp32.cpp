#include <Arduino.h>
#include <esp_wifi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char *SSID = "AndroidAP558F";
const char *PASSWORD = "asdf1234";

void setupWiFi()
{
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting...");
    Serial.printf("Connection Status: %d\n", WiFi.status());
    delay(1000);
  }

  Serial.print("Wi-Fi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void getMacAddress()
{
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void setup()
{
  Serial.begin(9600);
  setupWiFi();
}

void loop()
{
  getMacAddress();
  delay(1000);
}