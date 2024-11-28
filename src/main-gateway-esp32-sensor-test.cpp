#include <Arduino.h>
#include <DHT.h>

#define SOIL_ANALOG_PIN 2
#define SOIL_DIGITAL_PIN 4

#define DHT_PIN 5
#define DHT_TYPE DHT21
DHT dht(DHT_PIN, DHT_TYPE);

#define LIGHT_PIN 15

#define PUMP_PIN 19

void setupSoilSensor()
{
  pinMode(SOIL_DIGITAL_PIN, INPUT);
}

int getSoilReading()
{
  return analogRead(SOIL_ANALOG_PIN);
}

bool isSoilWet()
{
  return !(bool)digitalRead(SOIL_DIGITAL_PIN);
}

float getHumidityReading()
{
  return dht.readHumidity();
}

float getTemperatureReading()
{
  return dht.readTemperature();
}

uint16_t getLightReading()
{
  return analogRead(LIGHT_PIN);
}

void enablePump()
{
  digitalWrite(PUMP_PIN, HIGH);
}

void disablePump()
{
  digitalWrite(PUMP_PIN, LOW);
}

void setup()
{
  Serial.begin(9600);
  setupSoilSensor();
  dht.begin();
}

void loop()
{
  Serial.print("Soil reading: ");
  Serial.println(getSoilReading());

  Serial.print("Is soil wet?: ");
  Serial.println(isSoilWet());

  Serial.print("Humidity: ");
  Serial.print(getHumidityReading());
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(getTemperatureReading());
  Serial.println(" Celsius");

  Serial.print("Light reading: ");
  Serial.println(getLightReading());

  Serial.println("---------------");

  enablePump();
  Serial.println("Pump enabled.");
  delay(1000);
  disablePump();
  Serial.println("Pump disabled.");

  delay(3000);
}