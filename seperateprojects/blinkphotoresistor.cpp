#include <Arduino.h>
#define LED_BUILTIN 2

const int sensor_pin = 35;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    int delay_val = analogRead(sensor_pin);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delay_val);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delay_val);
}