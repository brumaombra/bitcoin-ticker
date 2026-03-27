#ifndef STORAGE_H
#define STORAGE_H

#include <ArduinoJson.h>
#include "../config/config.h"

void setupEEPROM();
bool readEEPROM(JsonDocument& doc);
void testReadEEPROM();
bool clearEEPROM();
bool loadDeviceConfigFromEEPROM(DeviceConfig& config);
bool saveDeviceConfigToEEPROM(const DeviceConfig& config);
bool writeEEPROM();

#endif