#include <Arduino.h>
#include <LittleFS.h>
#include "libs/sensors.h"
#include "libs/pump.h"

void setup()
{
  Serial.begin(9600);
  Sensors::setupSensors();
}

void loop()
{
  Serial.print("Soil reading: ");
  Serial.println(Sensors::getSoilReading());

  Serial.print("Is soil wet?: ");
  Serial.println(Sensors::isSoilWet());

  Serial.print("Humidity: ");
  Serial.print(Sensors::getHumidityReading());
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(Sensors::getTemperatureReading());
  Serial.println(" Celsius");

  Serial.print("Light reading: ");
  Serial.println(Sensors::getLightReading());

  Serial.println("---------------");

  Pump::enablePump();
  Serial.println("Pump enabled.");
  delay(1000);
  Pump::disablePump();
  Serial.println("Pump disabled.");

  delay(3000);
}