#ifndef STORAGE_H
#define STORAGE_H

#include <ArduinoJson.h>

void setupEEPROM();
bool readEEPROM(JsonDocument& doc);
void testReadEEPROM();
bool clearEEPROM();
bool writeEEPROM();

#endif