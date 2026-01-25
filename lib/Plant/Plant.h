#ifndef PLANT_H
#define PLANT_H

#include <Arduino.h>

class Plant {
public:
    const char* name;
    int pin;
    int moisture;
    bool enabled;
    
    Plant(const char* plant_name, int pin_number, bool is_enabled = true) 
        : name(plant_name), pin(pin_number), enabled(is_enabled), moisture(-1) {}
    
    void readMoisture() {
        if (!enabled) {
            moisture = -1;
            return;
        }
        int raw = read_avg(pin);
        moisture = map(raw, 900, 2400, 0, 100);
        moisture = constrain(moisture, 0, 100);
        
        Serial.print(name);
        Serial.print(" Level (%): ");
        Serial.println(moisture);
    }

private:
    int read_avg(int analog_pin) {
        const int samples = 10;
        long total = 0;
        for(int i = 0; i < samples; i++){
            total += analogRead(analog_pin);
            delay(200);
        }
        return total / samples;
    }
};

#endif
