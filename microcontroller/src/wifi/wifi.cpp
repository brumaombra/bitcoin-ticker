#include "wifi.h"
#include <Arduino.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include "../config/config.h"
#include "../storage/storage.h"
#include "../matrix/matrix.h"
#include "../serial/serial.h"

namespace {
	DNSServer dnsServer; // DNS server for captive portal
	constexpr byte DNS_PORT = 53; // DNS port for captive portal
	bool mdnsActive = false;

	void stopMdns() {
		if (!mdnsActive) {
			return;
		}

		MDNS.close();
		mdnsActive = false;
		printLogfln("mDNS stopped");
	}

	void startMdns() {
		if (WiFi.status() != WL_CONNECTED) {
			stopMdns();
			return;
		}

		if (mdnsActive) {
			MDNS.update();
			return;
		}

		if (!MDNS.begin(mdnsHostname)) {
			printLogfln("Failed to start mDNS on http://%s.local", mdnsHostname);
			return;
		}

		MDNS.addService("http", "tcp", 80);
		mdnsActive = true;
		printLogfln("mDNS ready on http://%s.local", mdnsHostname);
	}
}

// Connecting to WiFi
bool connectToWiFi() {
    WiFi.begin(wiFiSSID, wiFiPassword);
    byte maxTry = 50;
    byte count = 0;
    printLog("Connecting to WiFi");
    
	// Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        if (count >= maxTry) {
            wiFiSSID[0] = '\0';
            wiFiPassword[0] = '\0';
            return false;
        }
        count++;
        printLog(".");
        delay(250);
    }

	// Check if connected
    if (WiFi.status() == WL_CONNECTED) {
        printLogln(" connected!");
		startMdns();
        return true;
    }
    
    return false;
}

// Setting up the access point
bool setupAccessPoint() {
	stopMdns();
	if (accessPointEnabled) // Check if already enabled
		return true; // If enabled exit the function
	accessPointEnabled = WiFi.softAP(accessPointSSID); // Start the access point
	if (!accessPointEnabled) // Check if enabled
		return false; // If not enabled exit the function
	dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
	printLogfln("Access point ready on http://%s", WiFi.softAPIP().toString().c_str());
	return true; // Access point enabled
}

// Manage WiFi connection
bool manageWiFiConnection() {
	if (accessPointEnabled) {
		dnsServer.processNextRequest();
	}

	if (wiFiConnectionStatus == WIFI_TRY) { // Check if already trying to connect
		if (connectToWiFi()) { // Connecting to WiFi
			wiFiConnectionStatus = WIFI_OK; // Update connection status
			writeEEPROM(); // Save the new credentials
			return true; // Connection success
		} else {
			wiFiConnectionStatus = WIFI_KO; // Update connection status
			return false; // Connection failed
		}
	}

	// Every 2 seconds
	if (millis() - timestampWiFiConnection > 2000) {
		timestampWiFiConnection = millis(); // Save timestamp
		
		// Check if I need to disable the access point
		if (disableAccessPoint && accessPointEnabled) {
			dnsServer.stop();
			accessPointEnabled = !WiFi.softAPdisconnect(); // Disable access point
			if (!accessPointEnabled) { // Check if disabled
				disableAccessPoint = false; // Mark as disabled
			}
		}

		// Check if connected to WiFi
		if (WiFi.status() == WL_CONNECTED) {
			wiFiConnectionStatus = WIFI_OK; // Update connection status
			startMdns();
			return true; // Connection success
		}

		stopMdns();
		
		// Check if credentials are already present
		if (wiFiSSID[0] != '\0' && wiFiPassword[0] != '\0') {
			if (connectToWiFi()) { // Connecting to WiFi
				return true; // Connection success
			} else {
				setupAccessPoint(); // Setup access point
			}
		} else {
			setupAccessPoint(); // Setup access point
		}
		return false; // Connection failed
	}
	return true; // Connection success
}

// Check if connected to WiFi
bool checkWifiConnection() {
	if (WiFi.status() != WL_CONNECTED) {
		const char errorMessage[] = "Not connected to Wi-Fi. Use the 'Bitcoin-Ticker' access point to enter the Wi-Fi credentials.";
		printLogfln(errorMessage);
		printOnLedMatrix(errorMessage, sizeof(errorMessage)); // Print the message on the matrix
		return false; // Not connected
	}
	return true; // Connected
}