#include "config.h"

const char* ssid = "Frontier8864";
const char* password = "2727914437";

// Plant array definition
Plant plants[] = {
    Plant("basil", SOIL_MOISTURE_BASIL_PIN, HAS_BASIL),
    Plant("spring_onion", SOIL_MOISTURE_SPRING_ONION_PIN, HAS_SPRING_ONION),
    Plant("rosemary", SOIL_MOISTURE_ROSEMARY_PIN, HAS_ROSEMARY)
};
const int NUM_PLANTS = sizeof(plants) / sizeof(plants[0]);
