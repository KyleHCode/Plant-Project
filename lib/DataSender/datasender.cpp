#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "datasender.h"
#include "config.h"

// Function to send data to server
void send_data(const TempHumidity &th, int light) {
    if (WiFi.status() != WL_CONNECTED) return;

    StaticJsonDocument<256> doc;

    doc["device"] = "device_001";
    doc["light"] = light;
    doc["temp"] = th.temperature;
    doc["humidity"] = th.humidity;

    JsonObject soil_moisture = doc.createNestedObject("soil_moisture");
    for (int i = 0; i < NUM_PLANTS; i++) {
        soil_moisture[plants[i].name] = plants[i].moisture;
    }

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
