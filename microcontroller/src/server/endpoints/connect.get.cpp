#include "../server.h"
#include <cstring>
#include "../../config/config.h"
#include "../../utils/utils.h"
#include "../../serial/serial.h"

namespace {
	constexpr size_t MAX_WIFI_SSID_LENGTH = WIFI_SSID_SIZE - 1;
	constexpr size_t MAX_WIFI_PASSWORD_LENGTH = WIFI_PASSWORD_SIZE - 1;

	// Validate the credential parameters
	bool validateCredentialParam(AsyncWebServerRequest* request, const char* fieldName, const String& value, size_t maxLength, bool allowEmpty) {
		// Null or empty check
		if (!allowEmpty && value.length() == 0) {
			printLogfln("Missing required %s value", fieldName);
			sendErrorResponse(request, 400, "missing_required_fields", fieldName);
			return false;
		}

		// Length check
		if (value.length() > maxLength) {
			printLogfln("%s is too long: %d", fieldName, value.length());
			sendErrorResponse(request, 400, "invalid_connection_field", fieldName);
			return false;
		}

		// Valid parameter
		return true;
	}
}

// Connect to WiFi with the provided credentials
void setupConnectGetRoute() {
	server.on("/api/connect", HTTP_GET, [](AsyncWebServerRequest *request) {
		// Check required fields
		if (!request->hasParam("ssid") || !request->hasParam("password")) {
			sendErrorResponse(request, 400, "missing_required_fields", "Missing required ssid or password");
			return;
		}

		// Get the parameters
		const String ssidValue = request->getParam("ssid")->value();
		const String passwordValue = request->getParam("password")->value();

		// Validate the parameters
		if (!validateCredentialParam(request, "ssid", ssidValue, MAX_WIFI_SSID_LENGTH, false)
			|| !validateCredentialParam(request, "password", passwordValue, MAX_WIFI_PASSWORD_LENGTH, true)) {
			return;
		}

		// Save the new credentials into the runtime config
		DeviceConfig config = getDeviceConfig();
		stringCopy(config.ssid, ssidValue.c_str(), sizeof(config.ssid));
		stringCopy(config.password, passwordValue.c_str(), sizeof(config.password));
		setDeviceConfig(config);
		wiFiConnectionStatus = WIFI_TRY;

		// Print the new credentials
		printLogfln("SSID: %s, Password: %s\n", config.ssid, config.password);

		// Response
		JsonDocument doc;
		JsonObject data = doc.to<JsonObject>();
		data["connectionStatus"] = static_cast<int>(wiFiConnectionStatus);
		sendSuccessResponse(request, 200, &doc);
	});
}