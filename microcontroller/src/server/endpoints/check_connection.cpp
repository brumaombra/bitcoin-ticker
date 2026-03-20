#include "endpoints.h"
#include "../../config/config.h"

void setupCheckConnectionRoute() {
	// Check the WiFi connection status
	server.on("/api/check-connection", HTTP_GET, [](AsyncWebServerRequest *request) {
		// JSON response
		char jsonResponse[20];
		snprintf(jsonResponse, sizeof(jsonResponse), "{\"status\":\"%d\"}", wiFiConnectionStatus);

		switch (wiFiConnectionStatus) {
			case WIFI_TRY:
				// Response
				request->send(200, "application/json", jsonResponse);
				break;
			case WIFI_OK:
				// I need to disable the access point
				disableAccessPoint = true;
				// Response
				request->send(200, "application/json", jsonResponse);
				break;
			case WIFI_KO:
				// Response
				request->send(400, "application/json", jsonResponse);
				break;
		}
	});
}