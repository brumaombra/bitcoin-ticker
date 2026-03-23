#include "../server.h"
#include <ArduinoJson.h>
#include "../../config/config.h"
#include "../../utils/utils.h"
#include "../../storage/storage.h"
#include "../../matrix/matrix.h"
#include "../../serial/serial.h"

namespace {
	String postData = "";
}

// Save the settings
void setupSettingsPostRoute() {
	// POST to update the settings
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
			if (!doc["cryptoCoin"].isNull())
				stringCopy(cryptoCoin, doc["cryptoCoin"].as<const char*>(), sizeof(cryptoCoin));
			if (!doc["formatType"].isNull())
				formatType = doc["formatType"].as<String>() == "US" ? FORMAT_US : FORMAT_EU;
			if (!doc["matrixIntensity"].isNull())
				matrixIntensity = doc["matrixIntensity"].as<uint8_t>();
			if (!doc["scrollSpeed"].isNull())
				scrollSpeed = doc["scrollSpeed"].as<uint8_t>();

			// Apply changes
			timestampStockData = 0;
			writeEEPROM();
			setMatrixIntensity(matrixIntensity);
			setMatrixSpeed(scrollSpeed);

			// Send the JSON object
			request->send(200, "application/json", "{\"status\":\"OK\"}");
			printLogfln("Values settings changed");
		}
	});

	// OPTIONS request for preflighted requests
	server.on("/api/settings", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
		request->send(200);
	});
}