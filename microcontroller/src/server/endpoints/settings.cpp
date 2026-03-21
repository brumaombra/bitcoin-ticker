#include "endpoints.h"
#include <ArduinoJson.h>
#include "../../config/config.h"
#include "../../utils/utils.h"
#include "../../storage/storage.h"
#include "../../matrix/matrix.h"
#include "../../serial/serial.h"

static String postData = "";

void setupSettingsRoute() {
	// Get the values visibility settings
	server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		JsonDocument doc;

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

		size_t jsonLength = measureJson(doc) + 1;
		char json[jsonLength];
		serializeJson(doc, json, jsonLength);
		// Send the JSON object
		request->send(200, "application/json", json);
	});

	// POST request for preflighted requests
	server.on("/api/settings", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
		request->send(200);
	});

	// Clear all saved settings from EEPROM and restart
	server.on("/api/reset-settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		// Clear EEPROM
		if (!clearEEPROM()) {
			request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to clear EEPROM\"}");
			return;
		}

		// Send response and restart
		request->send(200, "application/json", "{\"status\":\"OK\",\"message\":\"EEPROM cleared. Restarting...\"}");
		delay(500);
		ESP.restart();
	});

	// Save the values visibility settings
	server.on("/api/settings", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		// Accumulate chunks (ESP8266 splits data in multiple chunks)
		if (index == 0) postData = "";
		for (size_t i = 0; i < len; i++) {
			postData += static_cast<char>(data[i]);
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
			writeEEPROM();
			setMatrixIntensity(matrixIntensity);
			setMatrixSpeed(scrollSpeed);

			// Send the JSON object
			request->send(200, "application/json", "{\"status\":\"OK\"}");
			printLogfln("Values settings changed");
		}
	});
}