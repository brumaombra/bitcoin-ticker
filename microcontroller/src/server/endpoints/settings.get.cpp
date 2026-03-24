#include "../server.h"
#include <ArduinoJson.h>
#include "../../config/config.h"

// Get the current settings
void setupSettingsGetRoute() {
	server.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		JsonDocument doc;
		JsonObject data = doc.to<JsonObject>();

		// Add the settings to the JSON object
		data["currentPrice"] = currentPriceVisible;
		data["priceChange"] = priceChangeVisible;
		data["marketCap"] = marketCapVisible;
		data["dailyHighLow"] = dailyHighLowVisible;
		data["yearHighLow"] = yearHighLowVisible;
		data["openPrice"] = openPriceVisible;
		data["volume"] = volumeVisible;
		data["cryptoCoin"] = cryptoCoin;
		data["formatType"] = formatType == FORMAT_US ? "US" : "EU";
		data["matrixIntensity"] = matrixIntensity;
		data["scrollSpeed"] = scrollSpeed;
		
		// Send the JSON object
		sendSuccessResponse(request, 200, &doc);
	});
}