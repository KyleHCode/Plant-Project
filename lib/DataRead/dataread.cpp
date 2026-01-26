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

    return true;
}

int light_level_read(int pin)
{
    int light_level = analogRead(pin);
    return light_level;
}