#include <DHT.h>

#ifdef BOARD_ESP32
#include "sensors/sensors-esp32.h"
#elif BOARD_ESP8266
#include "sensors/sensors-esp8266.h"
#endif

#define DHT_TYPE DHT21

namespace SensorsWrapper
{
    DHT dht(DHT_PIN, DHT_TYPE);

    void setupSoilSensor()
    {
        pinMode(SOIL_DIGITAL_PIN, INPUT);
    }

    int getSoilMoisture()
    {
        return analogRead(SOIL_ANALOG_PIN);
    }

    bool isSoilWet()
    {
        return !(bool)digitalRead(SOIL_DIGITAL_PIN);
    }

    float getAirHumidity()
    {
        return dht.readHumidity();
    }

    float getAirTemperature()
    {
        return dht.readTemperature();
    }

    uint16_t getLightLevel()
    {
        return analogRead(LIGHT_PIN);
    }

    void setupSensors()
    {
        setupSoilSensor();
        dht.begin();
    }
}