#include <Arduino.h>
#include "libs/sensors.h"
#include "libs/pump.h"
#include "libs/wifi.h"

uint8_t broadcastAddress[6] = {0xC4, 0x5B, 0xBE, 0x6F, 0xBD, 0x0B};

bool isPumpOn = false;

struct SensorData
{
  int soilMoisture;
  bool isSoilWet;
  float airHumidity;
  float airTemperature;
  uint16_t lightLevel;
};

struct PumpCommand
{
  bool isPumpActive;
};

SensorData sensorData;
PumpCommand pumpCommand;

void readSensors()
{
  sensorData.soilMoisture = SensorsWrapper::getSoilMoisture();
  sensorData.isSoilWet = SensorsWrapper::isSoilWet();
  sensorData.airHumidity = SensorsWrapper::getAirHumidity();
  sensorData.airTemperature = SensorsWrapper::getAirTemperature();
  sensorData.lightLevel = SensorsWrapper::getLightLevel();
}

void printSensorData()
{
  Serial.print("Soil Moisture: ");
  Serial.println(sensorData.soilMoisture);
  Serial.print("Is Soil Wet: ");
  Serial.println(sensorData.isSoilWet);
  Serial.print("Air Humidity: ");
  Serial.println(sensorData.airHumidity);
  Serial.print("Air Temperature: ");
  Serial.println(sensorData.airTemperature);
  Serial.print("Light Level: ");
  Serial.println(sensorData.lightLevel);
  Serial.println();
}

void printPumpCommand()
{
  Serial.print("Pump active: ");
  Serial.println(pumpCommand.isPumpActive);
}

void handlePumpCommand()
{
  if (pumpCommand.isPumpActive)
  {
    PumpWrapper::enablePump();
  }
  else
  {
    PumpWrapper::disablePump();
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&pumpCommand, incomingData, sizeof(pumpCommand));
  Serial.print("Bytes received: ");
  Serial.println(len);

  printPumpCommand();
  handlePumpCommand();
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

  // Setup pump
  PumpWrapper::setupPump();

  // Setup WiFi
  WiFiWrapper::setWiFiMode(WIFI_AP_STA);
  WiFi.disconnect();
  delay(100);

  WiFiWrapper::setupWiFi();
  WiFiWrapper::setupESPNow();
  WiFiWrapper::addPeer(broadcastAddress);
  WiFiWrapper::registerSendCallback(OnDataSent);
  WiFiWrapper::registerRecvCallback(OnDataRecv);
  WiFi.printDiag(Serial);
}

void loop()
{
  readSensors();
  printSensorData();
  sendSensorData();

  // For demo only
  if (sensorData.soilMoisture >= 2000 & !isPumpOn)
  {
    PumpWrapper::enablePump();
    Serial.println("Pump High");
  }
  else if (sensorData.soilMoisture < 2000 & isPumpOn)
  {
    PumpWrapper::disablePump();
    Serial.println("Pump Low");
  }

  delay(3000);
}