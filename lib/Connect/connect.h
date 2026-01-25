#ifndef CONNECT_H
#define CONNECT_H

// Function declaration
bool connect_to_wifi(const char* ssid, const char* password, const char* hostname, unsigned long timeout_ms = 15000);

#endif // CONNECT_H