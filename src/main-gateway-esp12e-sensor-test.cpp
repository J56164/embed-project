#include <Arduino.h>
#include "libs/sensors.h"

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

  Sensors::enablePump();
  Serial.println("Pump enabled.");
  delay(1000);
  Sensors::disablePump();
  Serial.println("Pump disabled.");

  delay(3000);
}