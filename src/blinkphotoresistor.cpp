#include <Arduino.h>
#define LED_BUILTIN 2
#define PHOTORESISTOR_INPUT_PIN 35

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    int delay_val = analogRead(PHOTORESISTOR_INPUT_PIN)
    digitalWrite(LED_BUILTIN, HIGH);
    delay(delay_val);
    digitalWrite(LED_BUILTIN, LOW);
    delay(delay_val);
}