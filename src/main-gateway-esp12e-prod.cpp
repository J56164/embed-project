#include <Arduino.h>
#include <LittleFS.h>
#include <SD.h>
#include "libs/wifi.h"
#include "libs/blynk.h"
#include "libs/firebase.h"

struct SensorData
{
  int soilReading;
  bool isSoilWet;
  float humidity;
  float temperature;
  uint16_t light;
};

SensorData sensorData;

void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&sensorData, incomingData, sizeof(sensorData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Soil Wetness: ");
  Serial.println(sensorData.soilReading);
  Serial.print("Is Soil Wet: ");
  Serial.println(sensorData.isSoilWet);
  Serial.print("Humidity: ");
  Serial.println(sensorData.humidity);
  Serial.print("Temperature: ");
  Serial.println(sensorData.temperature);
  Serial.print("Light: ");
  Serial.println(sensorData.light);
  Serial.println();
}

void sendDataToBlynk()
{
  Blynk.virtualWrite(V1, sensorData.soilReading);
  Blynk.virtualWrite(V2, sensorData.isSoilWet);
  Blynk.virtualWrite(V3, sensorData.humidity);
  Blynk.virtualWrite(V4, sensorData.temperature);
  Blynk.virtualWrite(V5, sensorData.light);
  Serial.println("Data sent to Blynk");
}

void sendDataToFirebase()
{
  FirebaseWrapper::sendIntData("Sensors/SoilReading", sensorData.soilReading);
  FirebaseWrapper::sendBoolData("Sensors/IsSoilWet", sensorData.isSoilWet);
  FirebaseWrapper::sendFloatData("Sensors/Humidity", sensorData.humidity);
  FirebaseWrapper::sendFloatData("Sensors/Temperature", sensorData.temperature);
  FirebaseWrapper::sendIntData("Sensors/Light", sensorData.light);
  Serial.println("Data sent to Firebase");
}

// TODO: Try to retrieve the humidity threshold from Firebase and store it in the ESP32.

// TODO: When the user changes the humidity threshold on the Blynk app, the value is stored
// both in the ESP32 and in Firebase.

// TODO: When the sensor readings are received, the ESP32 compares the humidity reading with
// the humidity threshold. If the humidity reading is greater than the threshold, the ESP32
// communicates with the ESP8266 to turn on the water pump for a certain amount of time
// and then turn it off. Then, the ESP32 sends a notification to the user's phone via Blynk.

void setup()
{
  Serial.begin(9600);

  // Setup WiFi
  WiFiWrapper::setWiFiMode(WIFI_AP_STA);
  WiFiWrapper::setupWiFi();
  WiFiWrapper::setupESPNow(true); // ESPNow must be initialized after WiFi for some reason
  WiFiWrapper::registerRecvCallback(OnDataRecv);

  // Setup Blynk
  BlynkWrapper::setupBlynk();
  BlynkWrapper::setTimerInterval(10000L, sendDataToBlynk);
  BlynkWrapper::setTimerInterval(10000L, sendDataToFirebase);

  // Setup Firebase
  FirebaseWrapper::setupFirebase();
}

void loop()
{
  BlynkWrapper::run();
}