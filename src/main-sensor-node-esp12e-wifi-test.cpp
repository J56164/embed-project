#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <espnow.h>

uint8_t broadcastAddress[6] = {0x88, 0x13, 0xBF, 0x0C, 0x31, 0xA4};

typedef struct struct_message
{
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

struct_message myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0)
  {
    Serial.println("Delivery success");
  }
  else
  {
    Serial.println("Delivery fail");
  }
}

// void setupWiFi()
// {
//   WiFi.begin(SSID, PASSWORD);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     Serial.println("Connecting...");
//     Serial.printf("Connection Status: %d\n", WiFi.status());
//     delay(1000);
//   }

//   Serial.print("Wi-Fi connected.");
//   Serial.print("IP Address: ");
//   Serial.println(WiFi.localIP());
// }

void getMacAddress()
{
  Serial.print("ESP Board MAC Address: ");
  Serial.println(WiFi.macAddress());
}

void setup()
{
  Serial.begin(9600);
  // setupWiFi();

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

void loop()
{
  getMacAddress();

  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1, 20);
  myData.c = 1.2;
  myData.d = "Hello";
  myData.e = false;
  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  delay(1000);
}