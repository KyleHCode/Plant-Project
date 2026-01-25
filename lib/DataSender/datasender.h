#ifndef DATASENDER_H
#define DATASENDER_H

#include "dataread.h"
#include "Plant.h"

// Function declaration
void send_data(const TempHumidity &th, int light, Plant plants[], int num_plants, const char* server_url, const char* device_id);

#endif // DATASENDER_H
