#ifndef STORAGE_H
#define STORAGE_H

#include <ArduinoJson.h>
#include "../config/config.h"

void setupEEPROM();
bool readEEPROM(JsonDocument& doc);
void testReadEEPROM();
bool clearEEPROM();
bool loadSettingsFromEEPROM(Settings& settings);
bool saveSettingsToEEPROM(const Settings& settings);
bool writeEEPROM();

#endif