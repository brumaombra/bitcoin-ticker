#include "../server.h"
#include <Arduino.h>
#include "../../config/config.h"
#include "../../storage/storage.h"

void setupResetSettingsGetRoute() {
	// Clear all saved settings from EEPROM and restart
	server.on("/api/reset-settings", HTTP_GET, [](AsyncWebServerRequest *request) {
		// Clear EEPROM
		if (!clearEEPROM()) {
			request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"Failed to clear EEPROM\"}");
			return;
		}

		// Send response and restart
		request->send(200, "application/json", "{\"status\":\"OK\",\"message\":\"EEPROM cleared. Restarting...\"}");
		delay(500);
		ESP.restart();
	});
}