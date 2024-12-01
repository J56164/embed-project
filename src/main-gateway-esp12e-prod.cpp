#include <Arduino.h>
#include <LittleFS.h>
#include <SD.h>
#include "libs/wifi.h"
#include "libs/blynk.h"
#include "libs/firebase.h"

void checkHumidityAndControlPump();

uint8_t broadcastAddress[6] = {0x88, 0x13, 0xBF, 0x0C, 0x31, 0xA4};

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

float soilMoistureThreshold = 0;

unsigned long pumpActivationStartTime = 0;
bool isPumpActive = false;
const unsigned long pumpActiveDuration = 10000; // 10 seconds

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

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(sendStatus == 0 ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&sensorData, incomingData, sizeof(sensorData));
  Serial.print("Bytes received: ");
  Serial.println(len);

  printSensorData();
}

void sendPumpCommand()
{
  Serial.println("Sending pump command to sensor node...");

  bool isCommandSent = WiFiWrapper::sendData(broadcastAddress, (uint8_t *)&pumpCommand, sizeof(pumpCommand));

  if (isCommandSent)
  {
    Serial.println("Command sent successfully");
  }
  else
  {
    Serial.println("Error sending command");
  }
}

void checkHumidityAndControlPump()
{
  if (sensorData.soilMoisture > soilMoistureThreshold && !isPumpActive)
  {
    Serial.println("Activating pump.");
    pumpCommand.isPumpActive = true;
    sendPumpCommand();
    pumpActivationStartTime = millis();
    Blynk.logEvent("pump", "The pump has been activated.");
  }
}

void handlePumpDeactivation()
{
  if (isPumpActive && (millis() - pumpActivationStartTime >= pumpActiveDuration))
  {
    Serial.println("Turning off pump after duration.");
    sendPumpCommand();
    isPumpActive = false;
  }
}

void sendDataToBlynk()
{
  Blynk.virtualWrite(V1, sensorData.soilMoisture);
  Blynk.virtualWrite(V2, sensorData.isSoilWet);
  Blynk.virtualWrite(V3, sensorData.airHumidity);
  Blynk.virtualWrite(V4, sensorData.airTemperature);
  Blynk.virtualWrite(V5, sensorData.lightLevel);
  Serial.println("Data sent to Blynk");
}

void sendDataToFirebase()
{
  FirebaseWrapper::sendIntData("Sensors/SoilMoisture", sensorData.soilMoisture);
  FirebaseWrapper::sendBoolData("Sensors/IsSoilWet", sensorData.isSoilWet);
  FirebaseWrapper::sendFloatData("Sensors/AirHumidity", sensorData.airHumidity);
  FirebaseWrapper::sendFloatData("Sensors/AirTemperature", sensorData.airTemperature);
  FirebaseWrapper::sendIntData("Sensors/LightLevel", sensorData.lightLevel);
  Serial.println("Data sent to Firebase");
}

void getHumidityThresholdFromFirebase()
{
  soilMoistureThreshold = FirebaseWrapper::readIntData("Config/SoilMoistureThreshold");
}

void updateHumidityThreshold(float newThreshold)
{
  soilMoistureThreshold = newThreshold;
  Serial.println(soilMoistureThreshold);

  FirebaseWrapper::sendFloatData("Config/SoilMoistureThreshold", soilMoistureThreshold);
  Serial.println("Soil Moisture Threshold saved to Firebase");
}

BLYNK_WRITE(V6)
{
  String value = param.asString();
  updateHumidityThreshold(value.toDouble());
}

void setup()
{
  Serial.begin(9600);

  // Setup WiFi
  WiFiWrapper::setWiFiMode(WIFI_AP_STA);
  WiFi.disconnect();
  delay(100);

  WiFiWrapper::setupWiFi();
  WiFiWrapper::setupESPNow();
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  WiFiWrapper::addPeer(broadcastAddress);
  WiFiWrapper::registerSendCallback(OnDataSent);
  WiFiWrapper::registerRecvCallback(OnDataRecv);
  WiFi.printDiag(Serial);

  // Setup Blynk
  BlynkWrapper::setupBlynk();
  BlynkWrapper::setTimerInterval(10000L, sendDataToBlynk);
  BlynkWrapper::setTimerInterval(10000L, sendDataToFirebase);
  BlynkWrapper::setTimerInterval(10000L, checkHumidityAndControlPump);
  BlynkWrapper::setTimerInterval(10000L, handlePumpDeactivation);

  // Setup Firebase
  FirebaseWrapper::setupFirebase();
  getHumidityThresholdFromFirebase();
}

void loop()
{
  BlynkWrapper::run();
}