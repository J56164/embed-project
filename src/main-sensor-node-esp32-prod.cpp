#include <Arduino.h>
#include "libs/sensors.h"
#include "libs/pump.h"
#include "libs/wifi.h"

uint8_t broadcastAddress[6] = {0xC4, 0x5B, 0xBE, 0x6F, 0xBD, 0x0B};

struct SensorData
{
  int soilReading;
  bool isSoilWet;
  float humidity;
  float temperature;
  uint16_t light;
};

SensorData sensorData;

void readSensors()
{
  sensorData.soilReading = SensorsWrapper::getSoilReading();
  sensorData.isSoilWet = SensorsWrapper::isSoilWet();
  sensorData.humidity = SensorsWrapper::getHumidityReading();
  sensorData.temperature = SensorsWrapper::getTemperatureReading();
  sensorData.light = SensorsWrapper::getLightReading();
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void sendSensorData()
{
  bool isDataSent = WiFiWrapper::sendData(broadcastAddress, (uint8_t *)&sensorData, sizeof(sensorData));

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

  // Setup sensors
  SensorsWrapper::setupSensors();

  // Setup WiFi
  WiFiWrapper::setWiFiMode(WIFI_STA);
  WiFiWrapper::setupWiFi();
  WiFiWrapper::setupESPNow(false);
  WiFiWrapper::addPeer(broadcastAddress);
  WiFiWrapper::registerSendCallback(OnDataSent);
}

void loop()
{
  readSensors();
  sendSensorData();

  delay(3000);
}