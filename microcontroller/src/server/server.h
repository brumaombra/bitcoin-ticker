#ifndef SERVER_H
#define SERVER_H

#include <ArduinoJson.h>
#include "../config/config.h"

// Standard API responses
void sendSuccessResponse(AsyncWebServerRequest* request, uint16_t statusCode = 200, const JsonDocument* dataDoc = nullptr);
void sendErrorResponse(AsyncWebServerRequest* request, uint16_t statusCode, const char* errorId, const char* errorMessage);

// Web server routes
void setupConnectGetRoute();
void setupCheckConnectionGetRoute();
void setupNetworksGetRoute();
void setupApiKeyPostRoute();
void setupSettingsGetRoute();
void setupSettingsPostRoute();
void setupResetSettingsGetRoute();
void setupCaptivePortalRoute();

// Server setup
void setupRoutes();
void setupServer();

#endif