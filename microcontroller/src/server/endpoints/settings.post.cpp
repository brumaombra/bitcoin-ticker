#include "../server.h"
#include <ArduinoJson.h>
#include "../../config/config.h"
#include "../../utils/utils.h"
#include "../../storage/storage.h"
#include "../../matrix/matrix.h"
#include "../../serial/serial.h"

// Save the settings
void setupSettingsPostRoute() {
	// POST to update the settings
	server.on("/api/settings", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		// Accumulate the request body
		const RequestBodyResult body = accumulateRequestBody(request, data, len, index, total);
		if (body.state == RequestBodyState::InProgress) {
			return;
		}

		// If error, send error response
		if (body.state == RequestBodyState::Error) {
			request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to read request body\"}");
			return;
		}
		
		// Print the complete request body
		printLogfln("Complete POST body: %s", body.data);

		// Parse the JSON object and release the request body memory
		JsonDocument doc;
		DeserializationError error = deserializeJson(doc, body.data, body.length + 1);
		releaseRequestBody(request);

		// If error, send error response
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

		// Send success response
		request->send(200, "application/json", "{\"status\":\"OK\"}");
		printLogfln("Values settings changed");
	});

	// OPTIONS request for preflighted requests
	server.on("/api/settings", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
		request->send(200);
	});
}