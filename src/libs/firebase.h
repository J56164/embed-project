#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyCOS3vFkL0sX48M99Vh3mVlPFMDPytNbeY"
#define DATABASE_URL "https://waterpot-a1e79-default-rtdb.asia-southeast1.firebasedatabase.app/"

namespace FirebaseWrapper // Renamed to avoid conflict
{
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    unsigned long sendDataPrevMillis = 0;
    const long sendDataIntervalMillis = 10000;
    bool signupOK = false;

    void setupFirebase()
    {
        config.api_key = API_KEY;
        config.database_url = DATABASE_URL;

        Serial.println("Signing up...");
        if (Firebase.signUp(&config, &auth, "", ""))
        {
            Serial.println("Signup successful");
            signupOK = true;
        }
        else
        {
            Serial.printf("Signup Error: %s\n", config.signer.signupError.message.c_str());
        }

        config.token_status_callback = tokenStatusCallback; // Monitor token generation
        Firebase.begin(&config, &auth);
        Firebase.reconnectWiFi(true);
    }

    void sendFloatData(const char *path, float value)
    {
        if (Firebase.ready() && signupOK)
        {
            Serial.printf("Sending data to Firebase: %s\n", path);
            if (Firebase.RTDB.setFloat(&fbdo, path, value))
            {
                Serial.println("Data sent successfully.");
            }
            else
            {
                Serial.printf("Failed to send data: %s\n", fbdo.errorReason().c_str());
            }
        }
        else
        {
            Serial.println("Firebase is not ready or signup failed!");
        }
    }

    void sendBoolData(const char *path, bool value)
    {
        if (Firebase.ready() && signupOK)
        {
            Serial.printf("Sending data to Firebase: %s\n", path);
            if (Firebase.RTDB.setBool(&fbdo, path, value))
            {
                Serial.println("Data sent successfully.");
            }
            else
            {
                Serial.printf("Failed to send data: %s\n", fbdo.errorReason().c_str());
            }
        }
        else
        {
            Serial.println("Firebase is not ready or signup failed!");
        }
    }

    void sendIntData(const char *path, int value)
    {
        if (Firebase.ready() && signupOK)
        {
            Serial.printf("Sending data to Firebase: %s\n", path);
            if (Firebase.RTDB.setInt(&fbdo, path, value))
            {
                Serial.println("Data sent successfully.");
            }
            else
            {
                Serial.printf("Failed to send data: %s\n", fbdo.errorReason().c_str());
            }
        }
        else
        {
            Serial.println("Firebase is not ready or signup failed!");
        }
    }

    // void sendData()
    // {
    //     if (Firebase.ready() && signupOK)
    //     {
    //         Serial.println("Sending data to Firebase...");

    //         // Collect sensor readings
    //         float humidity = Sensors::getHumidityReading();
    //         float temperature = Sensors::getTemperatureReading();
    //         int soilMoisture = Sensors::getSoilReading();
    //         bool isSoilWet = Sensors::isSoilWet();
    //         uint16_t lightLevel = Sensors::getLightReading();

    //         // Send humidity to Firebase
    //         if (Firebase.RTDB.setFloat(&fbdo, "Sensors/Humidity", humidity))
    //         {
    //             Serial.println("Humidity sent successfully.");
    //         }
    //         else
    //         {
    //             Serial.printf("Failed to send humidity: %s\n", fbdo.errorReason().c_str());
    //         }

    //         // Send temperature to Firebase
    //         if (Firebase.RTDB.setFloat(&fbdo, "Sensors/Temperature", temperature))
    //         {
    //             Serial.println("Temperature sent successfully.");
    //         }
    //         else
    //         {
    //             Serial.printf("Failed to send temperature: %s\n", fbdo.errorReason().c_str());
    //         }

    //         // Send soil moisture to Firebase
    //         if (Firebase.RTDB.setInt(&fbdo, "Sensors/SoilMoisture", soilMoisture))
    //         {
    //             Serial.println("Soil moisture sent successfully.");
    //         }
    //         else
    //         {
    //             Serial.printf("Failed to send soil moisture: %s\n", fbdo.errorReason().c_str());
    //         }

    //         // Send soil wet state to Firebase
    //         if (Firebase.RTDB.setInt(&fbdo, "Sensors/SoilWet", isSoilWet ? 1 : 0))
    //         {
    //             Serial.println("Soil wet state sent successfully.");
    //         }
    //         else
    //         {
    //             Serial.printf("Failed to send soil wet state: %s\n", fbdo.errorReason().c_str());
    //         }

    //         // Send light level to Firebase
    //         if (Firebase.RTDB.setInt(&fbdo, "Sensors/LightLevel", lightLevel))
    //         {
    //             Serial.println("Light level sent successfully.");
    //         }
    //         else
    //         {
    //             Serial.printf("Failed to send light level: %s\n", fbdo.errorReason().c_str());
    //         }
    //     }
    //     else
    //     {
    //         Serial.println("Firebase is not ready or signup failed!");
    //     }
    // }

    float readFloatData(const char *path)
    {
        if (Firebase.ready() && signupOK)
        {
            Serial.printf("Reading data from Firebase: %s\n", path);
            if (Firebase.RTDB.getFloat(&fbdo, path))
            {
                float value = fbdo.floatData();
                Serial.printf("Data read successfully: %.2f\n", value);
                return value;
            }
            else
            {
                Serial.printf("Failed to read data: %s\n", fbdo.errorReason().c_str());
            }
        }
        else
        {
            Serial.println("Firebase is not ready or signup failed!");
        }
        return 0.0;
    }

    bool readBoolData(const char *path)
    {
        if (Firebase.ready() && signupOK)
        {
            Serial.printf("Reading data from Firebase: %s\n", path);
            if (Firebase.RTDB.getBool(&fbdo, path))
            {
                bool value = fbdo.boolData();
                Serial.printf("Data read successfully: %s\n", value ? "true" : "false");
                return value;
            }
            else
            {
                Serial.printf("Failed to read data: %s\n", fbdo.errorReason().c_str());
            }
        }
        else
        {
            Serial.println("Firebase is not ready or signup failed!");
        }
        return false;
    }

    int readIntData(const char *path)
    {
        if (Firebase.ready() && signupOK)
        {
            Serial.printf("Reading data from Firebase: %s\n", path);
            if (Firebase.RTDB.getInt(&fbdo, path))
            {
                int value = fbdo.intData();
                Serial.printf("Data read successfully: %d\n", value);
                return value;
            }
            else
            {
                Serial.printf("Failed to read data: %s\n", fbdo.errorReason().c_str());
            }
        }
        else
        {
            Serial.println("Firebase is not ready or signup failed!");
        }
        return 0;
    }

    // void readData()
    // {
    //     if (Firebase.ready() && signupOK)
    //     {
    //         Serial.println("Reading data from Firebase...");

    //         // Read humidity threshold from Firebase
    //         if (Firebase.RTDB.getFloat(&fbdo, "Control/HumidityThreshold"))
    //         {
    //             float humidityThreshold = fbdo.floatData();
    //             Serial.printf("Humidity Threshold: %.2f\n", humidityThreshold);
    //             // You can use this threshold for decision-making
    //         }
    //         else
    //         {
    //             Serial.printf("Failed to read HumidityThreshold: %s\n", fbdo.errorReason().c_str());
    //         }

    //         // Read temperature threshold from Firebase
    //         if (Firebase.RTDB.getFloat(&fbdo, "Control/TemperatureThreshold"))
    //         {
    //             float temperatureThreshold = fbdo.floatData();
    //             Serial.printf("Temperature Threshold: %.2f\n", temperatureThreshold);
    //             // Use this value as needed
    //         }
    //         else
    //         {
    //             Serial.printf("Failed to read TemperatureThreshold: %s\n", fbdo.errorReason().c_str());
    //         }

    //         // Read light control state from Firebase
    //         if (Firebase.RTDB.getInt(&fbdo, "Control/LightState"))
    //         {
    //             int lightState = fbdo.intData();
    //             Serial.printf("Light State: %d\n", lightState);
    //             // Example: Control light based on the state
    //             digitalWrite(LIGHT_PIN, lightState);
    //         }
    //         else
    //         {
    //             Serial.printf("Failed to read LightState: %s\n", fbdo.errorReason().c_str());
    //         }

    //         // Read irrigation state from Firebase
    //         // if (Firebase.RTDB.getInt(&fbdo, "Control/Irrigation"))
    //         //{
    //         // int irrigationState = fbdo.intData();
    //         // Serial.printf("Irrigation State: %d\n", irrigationState);
    //         // Example: Control irrigation system
    //         // digitalWrite(IRRIGATION_PIN, irrigationState);
    //         //}
    //         // else
    //         //{
    //         // Serial.printf("Failed to read Irrigation: %s\n", fbdo.errorReason().c_str());
    //         //}

    //         Serial.println("Finished reading data from Firebase.");
    //     }
    //     else
    //     {
    //         Serial.println("Firebase is not ready or signup failed!");
    //     }
    // }
}