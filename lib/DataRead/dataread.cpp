#include <Arduino.h>
#include <DHT.h>
#include "dataread.h"
#include "config.h"

extern DHT dht;

// Function implementations
bool humidity_temp_read(TempHumidity &th)
{
    th.humidity = dht.readHumidity();
    th.temperature = dht.readTemperature();

    if (isnan(th.humidity) || isnan(th.temperature))
    {
        Serial.println("Failed to read from DHT sensor!");
        return false;
    }

    Serial.print("Humidity: ");
    Serial.print(th.humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(th.temperature);
    Serial.println(" *C ");

    return true;
}

int light_level_read()
{
    int light_level = analogRead(LIGHT_SENSOR_PIN);
    Serial.print("Light Level (ADC): ");
    Serial.println(light_level);
    return light_level;
}