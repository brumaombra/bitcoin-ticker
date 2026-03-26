#include "../server.h"
#include <ArduinoJson.h>
#include "../../storage/storage.h"

// Get the current settings
void setupSettingsGetRoute() {
	server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		// Get the current settings
		const Settings& settings = getSettings();

		// Build the JSON response
		JsonDocument doc;
		JsonObject data = doc.to<JsonObject>();

		// Add the settings to the JSON object
		data["currentPrice"] = settings.currentPriceVisible;
		data["priceChange"] = settings.priceChangeVisible;
		data["marketCap"] = settings.marketCapVisible;
		data["dailyHighLow"] = settings.dailyHighLowVisible;
		data["yearHighLow"] = settings.yearHighLowVisible;
		data["openPrice"] = settings.openPriceVisible;
		data["volume"] = settings.volumeVisible;
		data["cryptoCoin"] = settings.cryptoCoin;
		data["formatType"] = settings.formatType == FORMAT_US ? "US" : "EU";
		data["matrixIntensity"] = settings.matrixIntensity;
		data["scrollSpeed"] = settings.scrollSpeed;
		
		// Send the JSON object
		sendSuccessResponse(request, 200, &doc);
	});
}