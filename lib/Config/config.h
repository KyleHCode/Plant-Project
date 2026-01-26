#ifndef CONFIG_H
#define CONFIG_H

#include "Plant.h"

// WiFi credentials
extern const char* ssid;
extern const char* password;
extern const char* hostname;

// Server configuration
extern const char* server_url;
extern const char* device_id;

// Pin definitions
#define DHTPIN 4
#define DHTTYPE DHT11

#define LIGHT_SENSOR_PIN 35
#define SOIL_MOISTURE_BASIL_PIN 34
#define SOIL_MOISTURE_SPRING_ONION_PIN 33
#define SOIL_MOISTURE_ROSEMARY_PIN 32

// Plant array
extern Plant plants[];

#endif // CONFIG_H