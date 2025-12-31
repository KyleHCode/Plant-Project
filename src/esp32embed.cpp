#include <Arduino.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_sleep.h>

// WiFi credentials
const char* ssid     = "your_SSID";
const char* password = "your_PASSWORD";

// Define the pin and sensor type
#define DHTPIN 4       // GPIO pin connected to DATA
#define DHTTYPE DHT11   // DHT 11

#define ARM_PIN 27 // GPIO pin connected to arm control
RTC_DATA_ATTR bool armed = false;

#define HAS_BASIL 1
#define HAS_SPRING_ONION 0
#define HAS_ROSEMARY 0

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

// Function prototypes
bool humidity_temp_read(TempHumidity &th);
int light_level_read();
void soil_moisture_read(SoilMoisture &sm);
bool connect_to_wifi(unsigned long timeout_ms = 15000);
void send_data(const TempHumidity &th, int light, const SoilMoisture &sm);
int read_avg(int pin);

// Setup and Loop
void setup() 
{
    pinMode(ARM_PIN, INPUT_PULLUP);
    Serial.begin(115200); 
    dht.begin();

    WiFi.setHostname("plant_device_001");
}

void loop()
{
// If not armed yet, wait here until button press
    if (!armed) {
        Serial.println("DISARMED: press button to arm and start logging.");

        // Wait for press
        while (digitalRead(ARM_PIN) == HIGH) { delay(50); }

        // Debounce + wait for release
        while (digitalRead(ARM_PIN) == LOW) { delay(50); }
        delay(200);

        armed = true;
        Serial.println("ARMED! Starting logging cycle...");
    }
    
    if (!connect_to_wifi()) {
        Serial.println("WiFi failed, sleeping...");
        esp_sleep_enable_timer_wakeup(60ULL * 60ULL * 1000000ULL); // 1 hour
        esp_deep_sleep_start();
    }
    
    delay(2000); // Wait a sec for DHT sensor to stabilize

    TempHumidity temperature_humidity;
    SoilMoisture soil_moisture;

    if (!humidity_temp_read(temperature_humidity)) {
        Serial.println("Sensor read failed, skipping data send.");
        // Break the loop and go to sleep
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        esp_sleep_enable_timer_wakeup(60ULL * 60ULL * 1000000ULL); // 1 hour
        esp_deep_sleep_start();
    };
    
    int light = light_level_read();
    soil_moisture_read(soil_moisture);

    send_data(temperature_humidity, light, soil_moisture);

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    esp_sleep_enable_timer_wakeup(60ULL * 60ULL * 1000000ULL); // 1 hour
    esp_deep_sleep_start();

}

// Function implementations
bool humidity_temp_read(TempHumidity &th)
{
    th.humidity = dht.readHumidity();
    th.temperature = dht.readTemperature();

    if (isnan(th.humidity) || isnan(th.temperature))
    {
        Serial.println("Failed to read from DHT sensor!");
        return false;
    }

    Serial.print("Humidity: ");
    Serial.print(th.humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(th.temperature);
    Serial.println(" *C ");

    return true;
}

int light_level_read()
{
    int light_level = analogRead(LIGHT_SENSOR_PIN);
    Serial.print("Light Level (ADC): ");
    Serial.println(light_level);
    return light_level;
}

void soil_moisture_read(SoilMoisture &soil_moisture)
{
    #if HAS_BASIL
    soil_moisture.basil = map(read_avg(SOIL_MOISTURE_BASIL_PIN), 2400, 900, 0, 100);
    soil_moisture.basil = constrain(soil_moisture.basil, 0, 100);
    #else
    soil_moisture.basil = -1;
    #endif

    #if HAS_SPRING_ONION
    soil_moisture.spring_onion = map(read_avg(SOIL_MOISTURE_SPRING_ONION_PIN), 2400, 900, 0, 100);
    soil_moisture.spring_onion = constrain(soil_moisture.spring_onion, 0, 100);
    #else
    soil_moisture.spring_onion = -1;
    #endif

    #if HAS_ROSEMARY
    soil_moisture.rosemary = map(read_avg(SOIL_MOISTURE_ROSEMARY_PIN), 2400, 900, 0, 100);
    soil_moisture.rosemary = constrain(soil_moisture.rosemary, 0, 100);
    #else
    soil_moisture.rosemary = -1;
    #endif
    // Print soil moisture levels
    int values[3];
    soil_moisture.to_array(values);

    for (int i = 0; i < 3; i++)
    {
        Serial.print(soil_moisture.plant_names[i]);
        Serial.print(" Level (%): ");
        Serial.println(values[i]);
    }
}

// Function to read average analog value
int read_avg(int pin){
    const int samples = 10;
    long total = 0;
    for(int i = 0; i < samples; i++){
        total += analogRead(pin);
        delay(10);
    }
    return total / samples;
}

bool connect_to_wifi(unsigned long timeout_ms = 15000) {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname("plant_device_001");
    WiFi.begin(ssid, password);

    unsigned long start = millis();
    Serial.print("Connecting to WiFi");
    while(WiFi.status() != WL_CONNECTED && millis() - start < timeout_ms) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("Connected, IP: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    return false;
}

// Function to send data to server
void send_data(const TempHumidity &th, int light, const SoilMoisture &sm) {
    if (WiFi.status() != WL_CONNECTED) return;

    StaticJsonDocument<256> doc;

    doc["device"] = "device_001";
    doc["light"] = light;
    doc["temp"] = th.temperature;
    doc["humidity"] = th.humidity;

    JsonObject soil_moisture = doc.createNestedObject("soil_moisture");
    soil_moisture["basil"] = sm.basil;
    soil_moisture["spring_onion"] = sm.spring_onion;
    soil_moisture["rosemary"] = sm.rosemary;

    String json;
    serializeJson(doc, json);

    WiFiClient client;
    HTTPClient http;
    http.begin(client, "http://192.168.254.153:5000/sensor");
    http.setTimeout(5000);
    http.addHeader("Content-Type", "application/json");

    int code = http.POST(json);
    String body = http.getString();
    http.end();

    Serial.print("Data sent with response code: ");
    Serial.println(code);
    if (code > 0) {
        Serial.println(body);
    }
}