#include <Arduino.h>
#include "dataread.h"

// Function implementations
bool humidity_temp_read(DHT &dht, TempHumidity &th)
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

int light_level_read(int pin)
{
    int light_level = analogRead(pin);
    Serial.print("Light Level (ADC): ");
    Serial.println(light_level);
    return light_level;
}