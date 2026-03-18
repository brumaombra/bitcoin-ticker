#include "server.h"
#include <ArduinoJson.h>
// #include <LittleFS.h>
#include "../wifi/wifi.h"
#include "../config/config.h"
#include "../utils/utils.h"
#include "../storage/storage.h"
#include "../matrix/matrix.h"
#include "../serial/serial.h"
#include "../webui/generated_assets.h"

// Global buffer for POST data
String postData = "";

// Normalize request paths so route lookups handle optional trailing slashes
String normalizeAssetPath(const String& requestPath) {
	if (requestPath.length() <= 1 || !requestPath.endsWith("/")) {
		return requestPath;
	}

	return requestPath.substring(0, requestPath.length() - 1);
}

// Find the embedded asset that matches the requested route
const EmbeddedAsset* findEmbeddedAsset(const String& requestPath) {
	const String normalizedPath = normalizeAssetPath(requestPath);

	for (size_t i = 0; i < EMBEDDED_ASSET_COUNT; i++) {
		if (normalizedPath.equals(EMBEDDED_ASSETS[i].routePath)) {
			return &EMBEDDED_ASSETS[i];
		}
	}

	return NULL;
}

// Send an embedded asset from PROGMEM with the required gzip header
bool sendEmbeddedAsset(AsyncWebServerRequest *request, const String& requestPath) {
	const EmbeddedAsset* asset = findEmbeddedAsset(requestPath);
	if (asset == NULL) {
		return false;
	}

	AsyncWebServerResponse *response = request->beginResponse_P(200, asset->contentType, reinterpret_cast<PGM_P>(asset->data), asset->size);
	if (asset->gzip) {
		response->addHeader("Content-Encoding", "gzip");
	}

	request->send(response);
	return true;
}

// Setup delle route
void setupRoutes() {
	// CORS
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

	// Static files
	// server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html"); // Serve web page

	// Connect to WiFi
	server.on("/api/connect", HTTP_GET, [](AsyncWebServerRequest *request) {
		if (!request->hasParam("ssid") || !request->hasParam("password")) { // Check required fields
			request->send(400, "application/json", "{\"status\":\"KO\"}"); // Response
			return;
		}

		// Save the new credentials into temp variables
		stringCopy(wiFiSSID, request->getParam("ssid")->value().c_str(), 35); // Save the SSID
		stringCopy(wiFiPassword, request->getParam("password")->value().c_str(), 70); // Save the password
		wiFiConnectionStatus = WIFI_TRY; // Trying to connect

		// Print the new credentials
		printLogfln("SSID: %s, Password: %s\n", wiFiSSID, wiFiPassword);

		// Response
		char jsonResponse[20]; // JSON response
		snprintf(jsonResponse, sizeof(jsonResponse), "{\"status\":\"%d\"}", wiFiConnectionStatus); // Create response
		request->send(200, "application/json", jsonResponse); // Response
	});

	// Check the WiFi connection status
	server.on("/api/check-connection", HTTP_GET, [](AsyncWebServerRequest *request) {
		char jsonResponse[20]; // JSON response
		snprintf(jsonResponse, sizeof(jsonResponse), "{\"status\":\"%d\"}", wiFiConnectionStatus);
		switch(wiFiConnectionStatus) {
			case WIFI_TRY:
				request->send(200, "application/json", jsonResponse); // Response
				break;
			case WIFI_OK:
				disableAccessPoint = true; // I need to disable the access point
				request->send(200, "application/json", jsonResponse); // Response
				break;
			case WIFI_KO:
				request->send(400, "application/json", jsonResponse); // Response
				break;
		}
	});

	// Send the list of networks
	server.on("/api/networks", HTTP_GET, [](AsyncWebServerRequest *request) {
		WiFi.scanNetworksAsync([request](int numNetworks) {
			JsonDocument doc; // JSON object
			JsonArray networks = doc["networks"].to<JsonArray>();

			// Add the networks to the JSON object
			for (int i = 0; i < numNetworks; i++) {
				JsonObject network = networks.add<JsonObject>();
				network["ssid"] = WiFi.SSID(i);
				network["signal"] = WiFi.RSSI(i);
			}

			// Send the JSON object
			size_t jsonLength = measureJson(doc) + 1; // Get the size of the JSON object
			char json[jsonLength];
			serializeJson(doc, json, jsonLength);
			request->send(200, "application/json", json); // Send the JSON object
		});
    });

	// Save the API key
	server.on("/api/api-key", HTTP_GET, [](AsyncWebServerRequest *request) {
		if (!request->hasParam("apiKey")) { // Check if the API key is missing
        	request->send(400, "application/json", "{\"status\":\"KO\"}"); // Send the JSON object
        	return;
    	}

		// Save the new API key
		stringCopy(apiKey, request->getParam("apiKey")->value().c_str(), 35); // Save the API key
		writeEEPROM(); // Save the API key

		// Send the JSON object
		request->send(200, "application/json", "{\"status\":\"OK\"}"); // Send the JSON object
		printLogfln("API key changed");
	});

	// Get the values visibility settings
	server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		JsonDocument doc; // JSON object

		// Add the visibility settings to the JSON object
		doc["currentPrice"] = currentPriceVisible;
		doc["priceChange"] = priceChangeVisible;
		doc["marketCap"] = marketCapVisible;
		doc["dailyHighLow"] = dailyHighLowVisible;
		doc["yearHighLow"] = yearHighLowVisible;
		doc["openPrice"] = openPriceVisible;
		doc["volume"] = volumeVisible;
		doc["formatType"] = formatType == FORMAT_US ? "US" : "EU";
		doc["matrixIntensity"] = matrixIntensity;
		doc["scrollSpeed"] = scrollSpeed;

		// Send the JSON object
		size_t jsonLength = measureJson(doc) + 1; // Get the size of the JSON object
		char json[jsonLength];
		serializeJson(doc, json, jsonLength);
		request->send(200, "application/json", json); // Send the JSON object
	});

	// POST request for preflighted requests
	server.on("/api/settings", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
		request->send(200);
	});

	// Save the values visibility settings
	server.on("/api/settings", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		// Accumulate chunks (ESP8266 splits data in multiple chunks)
		if (index == 0) postData = "";
		for(size_t i = 0; i < len; i++) {
			postData += (char)data[i];
		}
		
		// Process only when all chunks received
		if (index + len == total) {
			printLogfln("Complete data: %s", postData.c_str());

			// Parse the JSON object
			JsonDocument doc;
			DeserializationError error = deserializeJson(doc, postData);
			if (error) {
				request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
				return;
			}
			
			// Get the new settings
			if (!doc["currentPrice"].isNull())
				currentPriceVisible = doc["currentPrice"].as<bool>();
			if (!doc["priceChange"].isNull())
				priceChangeVisible = doc["priceChange"].as<bool>();
			if (!doc["marketCap"].isNull())
				marketCapVisible = doc["marketCap"].as<bool>();
			if (!doc["dailyHighLow"].isNull())
				dailyHighLowVisible = doc["dailyHighLow"].as<bool>();
			if (!doc["yearHighLow"].isNull())
				yearHighLowVisible = doc["yearHighLow"].as<bool>();
			if (!doc["openPrice"].isNull())
				openPriceVisible = doc["openPrice"].as<bool>();
			if (!doc["volume"].isNull())
				volumeVisible = doc["volume"].as<bool>();
			if (!doc["formatType"].isNull())
				formatType = doc["formatType"].as<String>() == "US" ? FORMAT_US : FORMAT_EU;
			if (!doc["matrixIntensity"].isNull())
				matrixIntensity = doc["matrixIntensity"].as<uint8_t>();
			if (!doc["scrollSpeed"].isNull())
				scrollSpeed = doc["scrollSpeed"].as<uint8_t>();
			
			// Apply changes
			writeEEPROM(); // Save the settings
			setMatrixIntensity(matrixIntensity); // Set the intensity of the matrix
			setMatrixSpeed(scrollSpeed); // Set the speed of the matrix

			// Send the JSON object
			request->send(200, "application/json", "{\"status\":\"OK\"}");
			printLogfln("Values settings changed");
		}
	});

	// Embedded Nuxt routes and generated assets
	server.onNotFound([](AsyncWebServerRequest *request) {
		if (request->method() == HTTP_GET && sendEmbeddedAsset(request, request->url())) {
			return;
		}

		request->send(404);
	});
}

// Setup server
void setupServer() {
	setupRoutes(); // Setup routes
	server.begin(); // Start the server
}