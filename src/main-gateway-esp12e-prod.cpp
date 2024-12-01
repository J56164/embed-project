#include <Arduino.h>
#include <LittleFS.h>
#include <SD.h>
#include "libs/wifi.h"
#include "libs/blynk.h"
#include "libs/firebase.h"
#include "libs/pump.h"

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

SensorData sensorData;

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

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&sensorData, incomingData, sizeof(sensorData));
  Serial.print("Bytes received: ");
  Serial.println(len);

  printSensorData();

  checkHumidityAndControlPump();
}

void checkHumidityAndControlPump()
{
  if (sensorData.soilMoisture > soilMoistureThreshold && !isPumpActive)
  {
    Serial.println("Activating pump.");
    PumpWrapper::enablePump();
    isPumpActive = true;
    pumpActivationStartTime = millis();
  }
}

void handlePumpDeactivation()
{
  if (isPumpActive && (millis() - pumpActivationStartTime >= pumpActiveDuration))
  {
    Serial.println("Turning off pump after duration.");
    PumpWrapper::disablePump();
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

// TODO: Try to retrieve the soil moisture threshold from Firebase and store it in the ESP32.
void getHumidityThresholdFromFirebase()
{
  soilMoistureThreshold = FirebaseWrapper::readIntData("Config/SoilMoistureThreshold");
}

// TODO: When the user changes the soil moisture threshold on the Blynk app, the value is stored
// both in the ESP32 and in Firebase.
void updateHumidityThreshold(float newThreshold)
{
  soilMoistureThreshold = newThreshold;
  Serial.println(soilMoistureThreshold);

  FirebaseWrapper::sendFloatData("Config/SoilMoistureThreshold", soilMoistureThreshold);
  Serial.println("Soil Moisture Threshold saved to Firebase");
}

// TODO: When the sensor readings are received, the ESP32 compares the soil moisture reading with
// the soil moisture threshold. If the soil moisture reading is greater than the threshold, the ESP32
// communicates with the ESP8266 to turn on the water pump for a certain amount of time
// and then turn it off. Then, the ESP32 sends a notification to the user's phone via Blynk.

void setup()
{
  Serial.begin(9600);

  // Setup WiFi
  WiFiWrapper::setWiFiMode(WIFI_AP_STA);
  WiFiWrapper::setupWiFi();
  WiFiWrapper::setupESPNow(); // ESPNow must be initialized after WiFi for some reason
  WiFiWrapper::addPeer(broadcastAddress);
  WiFiWrapper::registerRecvCallback(OnDataRecv);

  // Setup Blynk
  BlynkWrapper::setupBlynk();
  BlynkWrapper::setTimerInterval(10000L, sendDataToBlynk);
  BlynkWrapper::setTimerInterval(10000L, sendDataToFirebase);

  // Setup Firebase
  FirebaseWrapper::setupFirebase();
  getHumidityThresholdFromFirebase();

  // Setup Pump
  PumpWrapper::setupPump();
}

void loop()
{
  BlynkWrapper::run();
  handlePumpDeactivation();
}