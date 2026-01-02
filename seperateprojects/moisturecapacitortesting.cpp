#include <Arduino.h>

#define TESTPIN 34 // GPIO pin connected to moisture capacitor

void setup() 
{
    Serial.begin(115200);
    delay(1000); 
    Serial.println("Moisture Capacitor Testing");
}
void loop() 
{
    int sensorValue = analogRead(TESTPIN);
    Serial.print("Moisture Capacitor Value: ");
    Serial.println(sensorValue);
    delay(2000);
}