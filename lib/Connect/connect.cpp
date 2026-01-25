#include "connect.h"
#include <Arduino.h>
#include <WiFi.h>

bool connect_to_wifi(const char* ssid, const char* password, const char* hostname, unsigned long timeout_ms) {
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(hostname);
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
