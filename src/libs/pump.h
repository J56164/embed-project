#include <Arduino.h>

#ifdef BOARD_ESP32
#include "pump/pump-esp32.h"
#elif BOARD_ESP8266
#include "pump/pump-esp8266.h"
#endif

namespace PumpWrapper
{
    void enablePump()
    {
        digitalWrite(PUMP_PIN, HIGH);
        digitalWrite(PUMP_CONTROL_A_PIN, HIGH);
        digitalWrite(PUMP_CONTROL_B_PIN, LOW);
    }

    void disablePump()
    {
        digitalWrite(PUMP_PIN, LOW);
        digitalWrite(PUMP_CONTROL_A_PIN, HIGH);
        digitalWrite(PUMP_CONTROL_B_PIN, LOW);
    }

    void setupPump()
    {
        pinMode(PUMP_PIN, OUTPUT);
        pinMode(PUMP_CONTROL_A_PIN, OUTPUT);
        pinMode(PUMP_CONTROL_B_PIN, OUTPUT);
    }
}