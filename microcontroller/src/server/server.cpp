#include "server.h"
#include "../config/config.h"

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