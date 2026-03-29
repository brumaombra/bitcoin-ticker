#include "serial/serial.h"
#include "eeprom/eeprom.h"
#include "api/api.h"
#include "wifi/wifi.h"
#include "matrix/matrix.h"
#include "server/server.h"

// Setup
void setup() {
	initSerial(); // Init serial
	setupEEPROM(); // Setup EEPROM
	initializeWiFiStack(); // Init ESP32 TCP/IP stack before AsyncWebServer starts
	setupServer(); // Setup server
	manageWiFiConnection(); // Manage WiFi connection
	setupWebClient(); // Setup web client
	setupLedMatrix(); // Setup LED matrix
}

// Loop
void loop() {
	manageWiFiConnection(); // Manage WiFi connection
	manageLedMatrix(); // Manage the LED matrix
}