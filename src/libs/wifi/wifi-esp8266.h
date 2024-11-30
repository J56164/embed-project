#include <ESP8266WiFi.h>
#include <espnow.h>

namespace WiFiWrapper
{
    bool addPeer(uint8_t *broadcastAddress)
    {
        return esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0) == 0;
    }

    void setSelfRole(bool isController)
    {
        esp_now_set_self_role(isController ? ESP_NOW_ROLE_CONTROLLER : ESP_NOW_ROLE_SLAVE);
    }
}