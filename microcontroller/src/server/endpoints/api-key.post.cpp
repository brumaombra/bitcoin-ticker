#include "../server.h"
#include <ArduinoJson.h>
#include "../../config/config.h"
#include "../../utils/utils.h"
#include "../../storage/storage.h"
#include "../../serial/serial.h"

namespace {
	String apiKeyPostData = "";
}

// Save the API key
void setupApiKeyPostRoute() {
	// POST to update the API key
	server.on("/api/api-key", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		// Accumulate chunks (ESP8266 may split the body into multiple chunks)
		if (index == 0) apiKeyPostData = "";
		for (size_t i = 0; i < len; i++) {
			apiKeyPostData += static_cast<char>(data[i]);
		}

		// Process only when all chunks have been received
		if (index + len == total) {
			JsonDocument doc;
			DeserializationError error = deserializeJson(doc, apiKeyPostData);
			if (error || doc["apiKey"].isNull()) {
				request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
				return;
			}

			stringCopy(apiKey, doc["apiKey"].as<const char*>(), sizeof(apiKey));
			writeEEPROM();

			request->send(200, "application/json", "{\"status\":\"OK\"}");
			printLogfln("API key changed");
		}
	});

    // OPTIONS request for preflighted requests
	server.on("/api/api-key", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
		request->send(200);
	});
}