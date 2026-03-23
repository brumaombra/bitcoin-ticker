#include "../server.h"
#include <ArduinoJson.h>
#include "../../config/config.h"

void setupSettingsGetRoute() {
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
		doc["cryptoCoin"] = cryptoCoin;
		doc["formatType"] = formatType == FORMAT_US ? "US" : "EU";
		doc["matrixIntensity"] = matrixIntensity;
		doc["scrollSpeed"] = scrollSpeed;

		size_t jsonLength = measureJson(doc) + 1;
		char json[jsonLength];
		serializeJson(doc, json, jsonLength);
		// Send the JSON object
		request->send(200, "application/json", json);
	});
}