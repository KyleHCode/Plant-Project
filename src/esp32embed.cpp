#include <Arduino.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_sleep.h>
#include "config.h"
#include "connect.h"
#include "Plant.h"
#include "dataread.h"
#include "datasender.h"

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Setup and Loop
void setup() 
{
    Serial.begin(115200); 
    dht.begin();

    WiFi.setHostname("plant_device_001");
}

void loop()
{  
    if (!connect_to_wifi()) {
        Serial.println("WiFi failed, sleeping...");
        esp_sleep_enable_timer_wakeup(60ULL * 60ULL * 1000000ULL); // 1 hour
        esp_deep_sleep_start();
    }
    
    delay(2000); // Wait a sec for DHT sensor to stabilize

    TempHumidity temperature_humidity;

    if (!humidity_temp_read(temperature_humidity)) {
        Serial.println("Sensor read failed, skipping data send.");
        // Break the loop and go to sleep
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        esp_sleep_enable_timer_wakeup(60ULL * 60ULL * 1000000ULL); // 1 hour
        esp_deep_sleep_start();
    };
    
    int light = light_level_read();
    
    // Read all plant moisture levels
    for (int i = 0; i < NUM_PLANTS; i++) {
        plants[i].readMoisture();
    }

    send_data(temperature_humidity, light);

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    Serial.println("Disconnected from WiFi. Going to sleep for 1 hour...");
    esp_sleep_enable_timer_wakeup(60ULL * 60ULL * 1000000ULL); // 1 hour
    esp_deep_sleep_start();

}