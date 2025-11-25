#include <Arduino.h>
#include <DHT.h>

// Define the pin and sensor type
#define DHTPIN 4       // GPIO pin connected to DATA
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

float temperature;
float humidity;

void setup() 
{
  Serial.begin(9600);
  dht.begin();
}

void loop() 
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C ");

    delay(2000); // Wait a few seconds between measurements
}