#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define LED 2
#define SOIL_1 A0
#define SOIL_2 D1

#define DHTPIN D1
#define DHTTYPE DHT21
DHT dht(DHTPIN, DHTTYPE);

#define LIGHT_1 A0

#define PUMP D3

const char *SSID = "AndroidAP558F";
const char *PASSWORD = "asdf1234";

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

void blinkLed()
{
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}

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

void setup()
{
  pinMode(LED, OUTPUT);
  dht.begin();
  // pinMode(SOIL_1, INPUT);
  // pinMode(SOIL_2, INPUT);
  pinMode(PUMP, OUTPUT);
  Serial.begin(9600);
  setupWiFi();
}

void loop()
{
  // blinkLed();
  // getSoil();
  // getDht();
  // getLight();
  // enablePump();
  getMacAddress();
  delay(1000);
}