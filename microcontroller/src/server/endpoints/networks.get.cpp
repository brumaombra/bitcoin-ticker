#include "../server.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <algorithm>
#include <vector>
#include "../../config/config.h"

namespace {
	// Struct to hold scanned network information
	struct ScannedNetwork {
		String ssid;
		int32_t rssi;
		uint8_t channel;
		bool secured;
		uint8_t quality;
	};

	// Calculate signal quality percentage from RSSI
	uint8_t calculateSignalQuality(int32_t rssi) {
		if (rssi <= -100) return 0;
		if (rssi >= -50) return 100;
		return static_cast<uint8_t>(2 * (rssi + 100));
	}

	// Check if a network with the given SSID already exists in the list
	bool hasNetworkWithSsid(const std::vector<ScannedNetwork>& networks, const String& ssid) {
		// Loop the networks and check if any has the same SSID
		for (const ScannedNetwork& network : networks) {
			if (network.ssid.equals(ssid)) {
				return true;
			}
		}

		// No network with the same SSID found
		return false;
	}

	// Build the JSON payload from the list of scanned networks
	JsonDocument buildNetworksData(const std::vector<ScannedNetwork>& networks) {
		JsonDocument doc;
		JsonObject data = doc.to<JsonObject>();
		JsonArray array = data["networks"].to<JsonArray>();

		// Add each network to the JSON array
		for (const ScannedNetwork& network : networks) {
			JsonObject item = array.add<JsonObject>();
			item["ssid"] = network.ssid;
			item["signal"] = network.rssi;
			item["quality"] = network.quality;
			item["channel"] = network.channel;
			item["secured"] = network.secured;
		}

		// Add the count
		data["count"] = networks.size();
		data["currentSsid"] = WiFi.status() == WL_CONNECTED ? WiFi.SSID() : "";

		// Return the JSON document
		return doc;
	}
}

// Get the list of available Wi-Fi networks
void setupNetworksGetRoute() {
	server.on("/api/networks", HTTP_GET, [](AsyncWebServerRequest *request) {
		WiFi.scanNetworksAsync([request](int numNetworks) {
			std::vector<ScannedNetwork> scannedNetworks;
			scannedNetworks.reserve(numNetworks);

			// Add the networks to the JSON object
			for (int i = 0; i < numNetworks; i++) {
				// Skip networks with empty SSID
				const String ssid = WiFi.SSID(i);
				if (ssid.length() == 0) {
					continue;
				}

				// Add the network to the list
				ScannedNetwork network;
				network.ssid = ssid;
				network.rssi = WiFi.RSSI(i);
				network.channel = WiFi.channel(i);
				network.secured = WiFi.encryptionType(i) != ENC_TYPE_NONE;
				network.quality = calculateSignalQuality(network.rssi);
				scannedNetworks.push_back(network);
			}

			// Sort strongest networks first
			std::sort(scannedNetworks.begin(), scannedNetworks.end(), [](const ScannedNetwork& left, const ScannedNetwork& right) {
				if (left.rssi != right.rssi) return left.rssi > right.rssi;
				return left.ssid < right.ssid;
			});

			// Remove duplicate SSIDs
			std::vector<ScannedNetwork> uniqueNetworks;
			uniqueNetworks.reserve(scannedNetworks.size());
			for (const ScannedNetwork& network : scannedNetworks) {
				if (hasNetworkWithSsid(uniqueNetworks, network.ssid)) continue;
				uniqueNetworks.push_back(network);
			}

			// Send the JSON object
			JsonDocument doc = buildNetworksData(uniqueNetworks);
			sendSuccessResponse(request, 200, &doc);
		});
	});
}