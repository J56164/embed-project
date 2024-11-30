#include <Arduino.h>
#include "libs/sensors.h"

void setup()
{
  Serial.begin(9600);
  SensorsWrapper::setupSensors();
}

void loop()
{
  Serial.print("Soil reading: ");
  Serial.println(SensorsWrapper::getSoilReading());

  Serial.print("Is soil wet?: ");
  Serial.println(SensorsWrapper::isSoilWet());

  Serial.print("Humidity: ");
  Serial.print(SensorsWrapper::getHumidityReading());
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(SensorsWrapper::getTemperatureReading());
  Serial.println(" Celsius");

  Serial.print("Light reading: ");
  Serial.println(SensorsWrapper::getLightReading());

  Serial.println("---------------");

  SensorsWrapper::enablePump();
  Serial.println("Pump enabled.");
  delay(1000);
  SensorsWrapper::disablePump();
  Serial.println("Pump disabled.");

  delay(3000);
}