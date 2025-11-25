#include <Arduino.h>
#include <DHT.h>

// Define the pin and sensor type
#define DHTPIN 4       // GPIO pin connected to DATA
#define DHTTYPE DHT11   // DHT 11
const int LIGHT_SENSOR_PIN = 35; // GPIO pin connected to photoresistor
const int SOIL_MOISTURE_PIN = 34; // GPIO pin connected to soil moisture sensor

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Temp/Humidity Struct
struct TempHumidity {
    float temperature;
    float humidity;
};

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  pinMode(LIGHT_SENSOR_PIN, INPUT);
}

// Function prototypes
void humidity_temp_read(TempHumidity &th);
void light_level_read();
void soil_moisture_read();

void loop()
{
    TempHumidity temperature_humidity;
    humidity_temp_read(temperature_humidity);
    delay(1000);
    light_level_read();
    delay(1000);
    soil_moisture_read();
    delay(1000);
}

void humidity_temp_read(TempHumidity &th)
{
    th.humidity = dht.readHumidity();
    th.temperature = dht.readTemperature();

    if (isnan(th.humidity) || isnan(th.temperature)) 
    {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    Serial.print("Humidity: ");
    Serial.print(th.humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(th.temperature);
    Serial.println(" *C ");
}

void light_level_read()
{
    int light_level = analogRead(LIGHT_SENSOR_PIN);
    Serial.print("Light Level (ADC): ");
    Serial.println(light_level);
}

void soil_moisture_read()
{
    int soil_moisture = analogRead(SOIL_MOISTURE_PIN);
    Serial.print("Soil Moisture Level (ADC): ");
    Serial.println(soil_moisture);
}