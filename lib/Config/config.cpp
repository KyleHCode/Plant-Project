#include "config.h"

const char* ssid = "Frontier8864";
const char* password = "2727914437";
const char* hostname = "plant_device_001";
const char* server_url = "http://192.168.254.153:5000/sensor";
const char* device_id = "device_001";

// Initialize Plant static member
int Plant::count = 0;

// Plant array definition
Plant plants[] = {
    Plant("basil", SOIL_MOISTURE_BASIL_PIN),
    //Plant("spring_onion", SOIL_MOISTURE_SPRING_ONION_PIN),
    //Plant("rosemary", SOIL_MOISTURE_ROSEMARY_PIN)
};
