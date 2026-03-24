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
			sendErrorResponse(request, 500, "request_body_read_failed", "Failed to read request body");
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
			sendErrorResponse(request, 400, "invalid_json", "Invalid JSON");
			return;
		}

		// Check required fields
		if (doc["apiKey"].isNull()) {
			sendErrorResponse(request, 400, "missing_api_key", "Missing apiKey field");
			return;
		}

		// Save the new API key and write to EEPROM
		stringCopy(apiKey, doc["apiKey"].as<const char*>(), sizeof(apiKey));
		writeEEPROM();

		// Send success response
		sendSuccessResponse(request);
		printLogfln("API key changed");
	});

    // OPTIONS request for preflighted requests
	server.on("/api/api-key", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
		request->send(200);
	});
}