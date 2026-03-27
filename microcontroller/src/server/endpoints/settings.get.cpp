#include "../server.h"
#include <ArduinoJson.h>
#include "../../storage/storage.h"

// Get the current settings
void setupSettingsGetRoute() {
	server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		// Get the current config
		const DeviceConfig& config = getDeviceConfig();

		// Build the JSON response
		JsonDocument doc;
		JsonObject data = doc.to<JsonObject>();

		// Add the settings to the JSON object
		data["currentPrice"] = config.currentPriceVisible;
		data["priceChange"] = config.priceChangeVisible;
		data["marketCap"] = config.marketCapVisible;
		data["dailyHighLow"] = config.dailyHighLowVisible;
		data["yearHighLow"] = config.yearHighLowVisible;
		data["openPrice"] = config.openPriceVisible;
		data["volume"] = config.volumeVisible;
		data["cryptoCoin"] = config.cryptoCoin;
		data["formatType"] = config.formatType == FORMAT_US ? "US" : "EU";
		data["matrixIntensity"] = config.matrixIntensity;
		data["scrollSpeed"] = config.scrollSpeed;
		
		// Send the JSON object
		sendSuccessResponse(request, 200, &doc);
	});
}