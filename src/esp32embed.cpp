#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <esp_sleep.h>
#include "config.h"
#include "connect.h"
#include "dataread.h"
#include "datasender.h"
#include "Prints.h"

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Setup and Loop
void setup() 
{
    Serial.begin(115200); 
    dht.begin();
}

void loop()
{  
    if (!connect_to_wifi(ssid, password, hostname)) {
        Serial.println("WiFi failed, sleeping...");
        esp_sleep_enable_timer_wakeup(60ULL * 60ULL * 1000000ULL); // 1 hour
        esp_deep_sleep_start();
    }
    
    delay(2000); // Wait a sec for DHT sensor to stabilize

    TempHumidity temperature_humidity;

    if (!humidity_temp_read(dht, temperature_humidity)) {
        Serial.println("Sensor read failed, skipping data send.");
        // Break the loop and go to sleep
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        esp_sleep_enable_timer_wakeup(60ULL * 60ULL * 1000000ULL); // 1 hour
        esp_deep_sleep_start();
    };

    int light_level = light_level_read(LIGHT_SENSOR_PIN);
    
    // Read all plant moisture levels
    for (int i = 0; i < Plant::get_count(); i++) {
        plants[i].read_moisture();
    }

    print_temp_humidity(temperature_humidity);
    print_light(light_level);
    print_moisture(plants);
    
    send_data(temperature_humidity, light_level, plants, server_url, device_id);


    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    Serial.println("Disconnected from WiFi. Going to sleep for 1 hour...");
    esp_sleep_enable_timer_wakeup(60ULL * 60ULL * 1000000ULL); // 1 hour
    esp_deep_sleep_start();

}