#include "../server.h"
#include <ArduinoJson.h>
#include "../../config/config.h"
#include "../../utils/utils.h"
#include "../../storage/storage.h"
#include "../../serial/serial.h"

// Save the API key
void setupApiKeyPostRoute() {
	// POST to update the API key
	server.on("/api/api-key", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
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

		// Check required fields
		if (doc["apiKey"].isNull()) {
			request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing apiKey field\"}");
			return;
		}

		// Save the new API key and write to EEPROM
		stringCopy(apiKey, doc["apiKey"].as<const char*>(), sizeof(apiKey));
		writeEEPROM();

		// Send success response
		request->send(200, "application/json", "{\"status\":\"OK\"}");
		printLogfln("API key changed");
	});

    // OPTIONS request for preflighted requests
	server.on("/api/api-key", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
		request->send(200);
	});
}