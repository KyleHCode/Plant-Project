#include <Arduino.h>

int input_pins[] = {18,19};
int led_pins[] = {25,26};

void setup()
{
    for (int i = 0; i < 2; i++)
    {
        pinMode(led_pins[i], OUTPUT);
        pinMode(input_pins[i], INPUT_PULLUP);
    }
}

void loop()
{
    for (int i = 0; i < 2; i++)
    {
        int val = digitalRead(input_pins[i]);
        if(val == LOW)
        {
            digitalWrite(led_pins[i], HIGH);
        }
        else
        {
            digitalWrite(led_pins[i], LOW);
        }
    }
}