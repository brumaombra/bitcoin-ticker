#include "../server.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "../../config/config.h"

namespace {
	// Create the JSON response for the check connection endpoint
	String getCheckConnectionResponse() {
		JsonDocument doc;

		// Add the Wi-Fi connection status to the JSON response
		doc["status"] = static_cast<int>(wiFiConnectionStatus);

		// If connected, add the SSID and IP address to the JSON response
		if (wiFiConnectionStatus == WIFI_OK && WiFi.status() == WL_CONNECTED) {
			doc["ssid"] = WiFi.SSID();
			doc["ip"] = WiFi.localIP().toString();
			doc["hostname"] = String(mdnsHostname) + ".local";
		}

		// Serialize the JSON response to a string
		String jsonResponse;
		serializeJson(doc, jsonResponse);
		return jsonResponse;
	}
}

void setupCheckConnectionGetRoute() {
	// Route to check the Wi-Fi connection status
	server.on("/api/check-connection", HTTP_GET, [](AsyncWebServerRequest *request) {
		// Create the JSON response
		const String jsonResponse = getCheckConnectionResponse();

		// Once connected, mark the access point for shutdown
		if (wiFiConnectionStatus == WIFI_OK) {
			disableAccessPoint = true;
		}

		// Send the JSON response
		request->send(200, "application/json", jsonResponse);
	});
}