#include <DHT.h>

#define LED 2
#define SOIL_1 A0
#define SOIL_2 D1

#define DHTPIN D1
#define DHTTYPE DHT21
DHT dht(DHTPIN, DHTTYPE);

#define LIGHT_1 A0

#define PUMP D3

void getSoil()
{
    Serial.println("Soil 1");
    Serial.println(analogRead(SOIL_1));
    Serial.println("Soil 2");
    Serial.println(analogRead(SOIL_2));
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
    Serial.println(analogRead(LIGHT_1));
}

void enablePump()
{
    digitalWrite(PUMP, HIGH);
    Serial.println("Enable");
}

void disablePump()
{
    digitalWrite(PUMP, LOW);
    Serial.println("Disable");
}