#include "dataread.h"
#include "Plant.h"
#include "Prints.h"
#include <Arduino.h>

void print_light(int light_level)  
{
    Serial.print("Light Level (ADC): ");
    Serial.println(light_level);
}

void print_moisture(Plant *plants)  
{
    // Read all plant moisture levels
    for (int i = 0; i < Plant::get_count(); i++) {
        int moisture = plants[i].moisture;
        Serial.print("Moisture for ");
        Serial.print(plants[i].name);
        Serial.print(": ");
        Serial.println(moisture);
    }
}

void print_temp_humidity(TempHumidity &th)
{
    // Print out temperature and humidity
    Serial.print("Humidity: ");
    Serial.print(th.humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(th.temperature);
    Serial.println(" *C ");
}