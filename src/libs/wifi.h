#ifdef BOARD_ESP32
#include "wifi/wifi-esp32.h"
#elif BOARD_ESP8266
#include "wifi/wifi-esp8266.h"
#endif

#ifndef ESP_OK
#define ESP_OK 0
#endif

namespace WiFiWrapper
{
    const char *SSID = "AndroidAP558F";
    const char *PASSWORD = "asdf1234";

    String getMacAddress()
    {
        return WiFi.macAddress();
    }

    IPAddress getLocalIP()
    {
        return WiFi.localIP();
    }

    bool addPeer(uint8_t broadcastAddress[6]);

    void setSelfRole(bool isController);

    void setWiFiMode(WiFiMode_t mode)
    {
        WiFi.mode(mode);
    }

    void registerRecvCallback(esp_now_recv_cb_t callback)
    {
        esp_now_register_recv_cb(callback);
    }

    void registerSendCallback(esp_now_send_cb_t callback)
    {
        esp_now_register_send_cb(callback);
    }

    bool setupESPNow(bool isController)
    {
        WiFi.macAddress(); // Somehow adding this line fixes the issue. I don't know why.
        if (esp_now_init() != ESP_OK)
        {
            return false;
        }
        setSelfRole(isController);
        return true;
    }

    void setupWiFi()
    {
        WiFi.begin(SSID, PASSWORD);
        while (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("Connecting...");
            Serial.printf("Connection Status: %d\n", WiFi.status());
            delay(1000);
        }
        Serial.print("Wi-Fi connected.");
    }

    bool sendData(uint8_t *broadcastAddress, uint8_t *data, size_t len)
    {
        return esp_now_send(broadcastAddress, data, len) == ESP_OK;
    }
}
