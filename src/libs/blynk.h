#include <libs/wifi.h>

#define BLYNK_TEMPLATE_ID "TMPL67zPTaR6h"
#define BLYNK_TEMPLATE_NAME "SmartPot"
#define BLYNK_AUTH_TOKEN "TDYXaafosmqcKyT9PZprEZzw6WHfxG8f"

#ifdef BOARD_ESP32
#include <BlynkSimpleEsp32.h>
#elif BOARD_ESP8266
#include <BlynkSimpleEsp8266.h>
#endif

#define BLYNK_PRINT Serial

namespace BlynkWrapper
{
    BlynkTimer timer;

    void setTimerInterval(unsigned long interval, std::function<void()> function)
    {
        timer.setInterval(interval, function);
    }

    void setupBlynk()
    {
        Blynk.begin(BLYNK_AUTH_TOKEN, WiFiWrapper::SSID, WiFiWrapper::PASSWORD);
    }

    void run()
    {
        Blynk.run();
        timer.run();
    }
}