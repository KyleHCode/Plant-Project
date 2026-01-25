#ifndef DATAREAD_H
#define DATAREAD_H

#include <DHT.h>

// Temp/Humidity Struct
struct TempHumidity {
    float temperature;
    float humidity;
};

// Function declarations
bool humidity_temp_read(DHT &dht, TempHumidity &th);
int light_level_read(int pin);

#endif // DATAREAD_H
