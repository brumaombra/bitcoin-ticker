#include "wifi.h"
#include <Arduino.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include "../config/config.h"
#include "../eeprom/eeprom.h"
#include "../matrix/matrix.h"
#include "../serial/serial.h"

namespace {
	DNSServer dnsServer; // DNS server for captive portal
	constexpr byte DNS_PORT = 53; // DNS port for captive portal
	bool mdnsActive = false;
	bool wifiStackInitialized = false;

	// Stop mDNS service if active
	void stopMdns() {
		if (!mdnsActive) return;
		MDNS.end();
		mdnsActive = false;
		printLogfln("mDNS stopped");
	}

	// Start mDNS service
	void startMdns() {
		// Don't start mDNS if not connected to Wi-Fi
		if (WiFi.status() != WL_CONNECTED) {
			stopMdns();
			return;
		}

		// Don't start mDNS if it's already active
		if (mdnsActive) {
			return;
		}

		// Start mDNS with the configured hostname
		if (!MDNS.begin(mdnsHostname)) {
			printLogfln("Failed to start mDNS on http://%s.local", mdnsHostname);
			return;
		}

		// Advertise the HTTP service on mDNS
		MDNS.addService("http", "tcp", 80);
		mdnsActive = true;
		printLogfln("mDNS ready on http://%s.local", mdnsHostname);
	}
}

// Initialize the Wi-Fi stack
void initializeWiFiStack() {
	if (wifiStackInitialized) return;
	WiFi.mode(WIFI_AP_STA);
	wifiStackInitialized = true;
}

// Connecting to WiFi
bool connectToWiFi() {
	initializeWiFiStack();
	const DeviceConfig& config = getDeviceConfig();
	WiFi.begin(config.ssid, config.password);
    byte maxTry = 50;
    byte count = 0;
    printLog("Connecting to WiFi");
    
	// Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
		// Check if we have tried enough times
        if (count >= maxTry) {
			DeviceConfig nextConfig = getDeviceConfig();
			nextConfig.ssid[0] = '\0';
			nextConfig.password[0] = '\0';
			setDeviceConfig(nextConfig);
            return false;
        }

		// Wait a bit before retrying
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

	// Connection failed
    return false;
}

// Setting up the access point
bool setupAccessPoint() {
	initializeWiFiStack();
	stopMdns();

	// If the access point is already enabled, return success
	if (accessPointEnabled) {
		return true; // If enabled exit the function
	}

	// Start the access point
	accessPointEnabled = WiFi.softAP(accessPointSSID);
	if (!accessPointEnabled) {
		printLogfln("Failed to start access point with SSID: %s", accessPointSSID);
		return false; // If failed to start access point exit the function
	}

	// Start the DNS server for captive portal
	dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
	printLogfln("Access point ready on http://%s", WiFi.softAPIP().toString().c_str());
	return true; // Access point enabled
}

// Manage WiFi connection
bool manageWiFiConnection() {
	initializeWiFiStack();

	// Process DNS requests if the access point is enabled
	if (accessPointEnabled) {
		dnsServer.processNextRequest();
	}

	// Check if already trying to connect
	if (wiFiConnectionStatus == WIFI_TRY) {
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

		// Stop mDNS if not connected to Wi-Fi
		stopMdns();
		
		// Check if credentials are already present
		const DeviceConfig& config = getDeviceConfig();
		if (config.ssid[0] != '\0' && config.password[0] != '\0') {
			if (connectToWiFi()) { // Connecting to WiFi
				return true; // Connection success
			} else {
				setupAccessPoint(); // Setup access point
			}
		} else {
			setupAccessPoint(); // Setup access point
		}

		// Connection failed
		return false;
	}

	// Connection success
	return true;
}

// Check if connected to WiFi
bool checkWifiConnection() {
	// If not connected, show a notice on the matrix
	if (WiFi.status() != WL_CONNECTED) {
		const char errorMessage[] = "Not connected to Wi-Fi. Use the 'Bitcoin-Ticker' access point to enter the Wi-Fi credentials.";
		printLogfln(errorMessage);
		printOnLedMatrix(errorMessage, sizeof(errorMessage)); // Print the message on the matrix

		// Not connected
		return false;
	}

	// Connected
	return true;
}