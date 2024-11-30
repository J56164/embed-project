#include <DHT.h>

#ifdef BOARD_ESP32
#include "sensors/sensors-esp32.h"
#else
#include "sensors/sensors-esp8266.h"
#endif

#define DHT_TYPE DHT21

namespace Sensors
{
    DHT dht(DHT_PIN, DHT_TYPE);

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

    void setupSensors()
    {
        setupSoilSensor();
        dht.begin();
    }
}