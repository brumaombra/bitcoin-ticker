#include "../server.h"
#include "../../config/config.h"
#include "../../utils/utils.h"
#include "../../serial/serial.h"

// Connect to WiFi with the provided credentials
void setupConnectGetRoute() {
	server.on("/api/connect", HTTP_GET, [](AsyncWebServerRequest *request) {
		// Check required fields
		if (!request->hasParam("ssid") || !request->hasParam("password")) {
			sendErrorResponse(request, 400, "missing_required_fields", "Missing required ssid or password");
			return;
		}

		// Save the new credentials into temp variables
		stringCopy(wiFiSSID, request->getParam("ssid")->value().c_str(), 35);
		stringCopy(wiFiPassword, request->getParam("password")->value().c_str(), 70);
		wiFiConnectionStatus = WIFI_TRY;

		// Print the new credentials
		printLogfln("SSID: %s, Password: %s\n", wiFiSSID, wiFiPassword);

		// Response
		JsonDocument doc;
		JsonObject data = doc.to<JsonObject>();
		data["connectionStatus"] = static_cast<int>(wiFiConnectionStatus);
		sendSuccessResponse(request, 200, &doc);
	});
}