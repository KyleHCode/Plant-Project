#include <Arduino.h>
#define LED_BUILTIN 2
#define LED_EXTERNAL 25

void setup() // Runs once
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_EXTERNAL, OUTPUT);
}

void loop() // Runs main code
{
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_EXTERNAL, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_EXTERNAL, HIGH);
  delay(1000);
}

