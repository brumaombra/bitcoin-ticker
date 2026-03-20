#include "endpoints.h"
#include "../../config/config.h"
#include "../../utils/utils.h"
#include "../../serial/serial.h"

void setupConnectRoute() {
	// Connect to WiFi
	server.on("/api/connect", HTTP_GET, [](AsyncWebServerRequest *request) {
		// Check required fields
		if (!request->hasParam("ssid") || !request->hasParam("password")) {
			request->send(400, "application/json", "{\"status\":\"KO\"}");
			return;
		}

		// Save the new credentials into temp variables
		stringCopy(wiFiSSID, request->getParam("ssid")->value().c_str(), 35);
		stringCopy(wiFiPassword, request->getParam("password")->value().c_str(), 70);
		wiFiConnectionStatus = WIFI_TRY;

		// Print the new credentials
		printLogfln("SSID: %s, Password: %s\n", wiFiSSID, wiFiPassword);

		// Response
		char jsonResponse[20];
		snprintf(jsonResponse, sizeof(jsonResponse), "{\"status\":\"%d\"}", wiFiConnectionStatus);
		request->send(200, "application/json", jsonResponse);
	});
}