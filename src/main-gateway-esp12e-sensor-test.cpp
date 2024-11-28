#include <Arduino.h>
#include <DHT.h>

#define LED 2
#define SOIL_ANALOG_PIN A0
#define SOIL_DIGITAL_PIN D1

#define DHT_PIN D1
#define DHT_TYPE DHT21
DHT dht(DHT_PIN, DHT_TYPE);

#define LIGHT_PIN A0

#define PUMP_PIN D3

void getSoil()
{
  Serial.println("Soil Analog Pin");
  Serial.println(analogRead(SOIL_ANALOG_PIN));
  Serial.println("Soil Digital Pin");
  Serial.println(analogRead(SOIL_DIGITAL_PIN));
}

void getDht()
{
  // Read data and store it to variables hum and temp
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  // Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print("%, Temperature: ");
  Serial.print(temp);
  Serial.println(" Celsius");
}

void getLight()
{
  Serial.println("Light 1");
  Serial.println(analogRead(LIGHT_PIN));
}

void enablePump()
{
  digitalWrite(PUMP_PIN, HIGH);
  Serial.println("Enable");
}

void disablePump()
{
  digitalWrite(PUMP_PIN, LOW);
  Serial.println("Disable");
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // H
  delay(1000);
}