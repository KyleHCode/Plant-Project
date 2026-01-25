#ifndef CONNECT_H
#define CONNECT_H

#include <WiFi.h>

// Function declaration
bool connect_to_wifi(unsigned long timeout_ms = 15000);

#endif // CONNECT_H