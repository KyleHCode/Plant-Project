#include <Arduino.h>
#include <DHT.h>

// Define the pin and sensor type
#define DHTPIN 4       // GPIO pin connected to DATA
#define DHTTYPE DHT11   // DHT 11
const int LIGHT_SENSOR_PIN = 35; // GPIO pin connected to photoresistor
const int SOIL_MOISTURE_BASIL_PIN = 34; // GPIO pin connected to soil moisture sensor
const int SOIL_MOISTURE_SPRING_ONION_PIN = 33; // GPIO pin connected to soil moisture sensor
const int SOIL_MOISTURE_ROSEMARY_PIN = 32; // GPIO pin connected to soil moisture sensor


// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Temp/Humidity Struct
struct TempHumidity {
    float temperature;
    float humidity;
};

// Soil Moisture Struct
struct SoilMoisture {
    int basil;
    int spring_onion;
    int rosemary;

    const char* plant_names[3] = {"Basil", "Spring Onion", "Rosemary"};

    void to_array(int out[3]) const
    {
        out[0] = basil;
        out[1] = spring_onion;
        out[2] = rosemary;
    };
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
    SoilMoisture soil_moisture;
    soil_moisture.basil        = map(analogRead(SOIL_MOISTURE_BASIL_PIN), 2400, 900, 0, 100);
    soil_moisture.spring_onion = map(analogRead(SOIL_MOISTURE_SPRING_ONION_PIN), 2400, 900, 0, 100);
    soil_moisture.rosemary     = map(analogRead(SOIL_MOISTURE_ROSEMARY_PIN), 2400, 900, 0, 100);
    soil_moisture.basil        = constrain(soil_moisture.basil,        0, 100);
    soil_moisture.spring_onion = constrain(soil_moisture.spring_onion, 0, 100);
    soil_moisture.rosemary     = constrain(soil_moisture.rosemary,     0, 100); 

    int values[3];
    soil_moisture.to_array(values);

    for (int i = 0; i < 3; i++)
    {
        Serial.print(soil_moisture.plant_names[i]);
        Serial.print(" Level (%): ");
        Serial.println(values[i]);
    }
}