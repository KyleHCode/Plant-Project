#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "datasender.h"
#include "Plant.h"

// Function to send data to server
void send_data(const TempHumidity &th, int light, Plant plants[], const char* server_url, const char* device_id) {
    if (WiFi.status() != WL_CONNECTED) return;

    StaticJsonDocument<256> doc;

    doc["device"] = device_id;
    doc["light"] = light;
    doc["temp"] = th.temperature;
    doc["humidity"] = th.humidity;

    JsonObject soil_moisture = doc.createNestedObject("soil_moisture");
    for (int i = 0; i < Plant::get_count(); i++) {
        soil_moisture[plants[i].name] = plants[i].moisture;
    }

    String json;
    serializeJson(doc, json);

    WiFiClient client;
    HTTPClient http;
    http.begin(client, server_url);
    http.setTimeout(5000);
    http.addHeader("Content-Type", "application/json");

    int http_post = http.POST(json);
    String body = http.getString();
    http.end();

    Serial.print("Data sent with response code: ");
    Serial.println(http_post);
    if (http_post > 0) {
        Serial.println(body);
    }
}
