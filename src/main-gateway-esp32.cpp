#include <Arduino.h>
#include <esp_wifi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FirebaseClient.h>
const char *SSID = "AndroidAP558F";
const char *PASSWORD = "asdf1234";

#define API_KEY "AIzaSyCOS3vFkL0sX48M99Vh3mVlPFMDPytNbeY"
#define DATABASE_URL "https://waterpot-a1e79-default-rtdb.firebaseio.com/"

#include <esp_now.h>
#include <WiFi.h>

#include <WiFiClientSecure.h>
WiFiClientSecure ssl;
DefaultNetwork network;
AsyncClientClass client(ssl, getNetwork(network));

FirebaseApp app;
RealtimeDatabase Database;
AsyncResult result;
NoAuth noAuth;

typedef struct struct_message{
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;
void printError(int code, const String &msg)
{
  Firebase.printf("Error, msg: %s, code: %d\n", msg.c_str(), code);
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("Bool: ");
  Serial.println(myData.d);
  Serial.println();
}

void setupFirebase()
{
  // Initialize Firebase
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  ssl.setInsecure();
#if defined(ESP8266)
  ssl.setBufferSizes(1024, 1024);
#endif

  // Initialize the authentication handler.
  initializeApp(client, app, getAuth(noAuth));

  // Binding the authentication handler with your Database class object.
  app.getApp<RealtimeDatabase>(Database);

  // Set your database URL
  Database.url(DATABASE_URL);

  // In sync functions, we have to set the operating result for the client that works with the function.
  client.setAsyncResult(result);

  // Set, push and get integer value

  Serial.print("Set int... ");
  bool status = Database.set<int>(client, "/test/int", 12345);
  if (status)
    Serial.println("ok");
  else
    printError(client.lastError().code(), client.lastError().message());

  Serial.print("Push int... ");
  String name = Database.push<int>(client, "/test/push", 12345);
  if (client.lastError().code() == 0)
    Firebase.printf("ok, name: %s\n", name.c_str());
  else
    printError(client.lastError().code(), client.lastError().message());

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
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void getMacAddress()
{
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void setup()
{
  Serial.begin(9600);
  //setupWiFi();
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  setupFirebase();
}

void loop()
{
  getMacAddress();

  delay(1000);
}