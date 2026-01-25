#ifndef DATAREAD_H
#define DATAREAD_H

// Temp/Humidity Struct
struct TempHumidity {
    float temperature;
    float humidity;
};

// Function declarations
bool humidity_temp_read(TempHumidity &th);
int light_level_read();

#endif // DATAREAD_H
