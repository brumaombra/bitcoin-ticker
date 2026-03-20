#include "endpoints.h"
#include "../../config/config.h"
#include "../../utils/utils.h"
#include "../../storage/storage.h"
#include "../../serial/serial.h"

void setupApiKeyRoute() {
	// Save the API key
	server.on("/api/api-key", HTTP_GET, [](AsyncWebServerRequest *request) {
		// Check if the API key is missing
		if (!request->hasParam("apiKey")) {
			request->send(400, "application/json", "{\"status\":\"KO\"}");
			return;
		}

		// Save the new API key
		stringCopy(apiKey, request->getParam("apiKey")->value().c_str(), 35);
		writeEEPROM();

		// Send the JSON object
		request->send(200, "application/json", "{\"status\":\"OK\"}");
		printLogfln("API key changed");
	});
}