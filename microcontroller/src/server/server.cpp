#include "server.h"
#include "../config/config.h"

namespace {
	// Initialize the standard response structure
	void initializeResponse(JsonDocument& doc) {
		doc.clear();
		doc["errorId"] = nullptr;
		doc["errorMessage"] = nullptr;
		doc["data"] = nullptr;
	}

	// Serialize the JSON document and send the response
	void sendResponse(AsyncWebServerRequest* request, uint16_t statusCode, JsonDocument& doc) {
		String jsonResponse;
		serializeJson(doc, jsonResponse);
		request->send(statusCode, "application/json", jsonResponse);
	}
}

// Send a success response with optional data
void sendSuccessResponse(AsyncWebServerRequest* request, uint16_t statusCode, const JsonDocument* doc) {
	JsonDocument responseDoc;
	initializeResponse(responseDoc);

	// Add the data object if provided
	if (doc != nullptr) responseDoc["data"].set(doc->as<JsonVariantConst>());

	// Send the response
	sendResponse(request, statusCode, responseDoc);
}

// Send an error response
void sendErrorResponse(AsyncWebServerRequest* request, uint16_t statusCode, const char* errorId, const char* errorMessage) {
	JsonDocument doc;
	initializeResponse(doc);

	// Populate the error details
	doc["errorId"] = errorId;
	doc["errorMessage"] = errorMessage;

	// Send the response
	sendResponse(request, statusCode, doc);
}

// Setup routes
void setupRoutes() {
	// CORS
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

	// Setup the routes
	setupConnectGetRoute();
	setupCheckConnectionGetRoute();
	setupNetworksGetRoute();
	setupApiKeyPostRoute();
	setupSettingsGetRoute();
	setupSettingsPostRoute();
	setupResetSettingsGetRoute();
	setupCaptivePortalRoute();
}

// Setup server
void setupServer() {
	setupRoutes(); // Setup routes
	server.begin(); // Start the server
}