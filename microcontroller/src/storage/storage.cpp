#include "storage.h"
#include <EEPROM.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>
#include <cstring>
#include "../config/config.h"
#include "../utils/utils.h"
#include "../serial/serial.h"

const int EEPROM_SIZE = 512; // EEPROM size

namespace {
    // Append the settings to a JSON document
    void appendSettingsToJson(JsonDocument& doc, const Settings& settings) {
        doc["currentPrice"] = settings.currentPriceVisible;
        doc["priceChange"] = settings.priceChangeVisible;
        doc["marketCap"] = settings.marketCapVisible;
        doc["dailyHighLow"] = settings.dailyHighLowVisible;
        doc["yearHighLow"] = settings.yearHighLowVisible;
        doc["openPrice"] = settings.openPriceVisible;
        doc["volume"] = settings.volumeVisible;
        doc["cryptoCoin"] = settings.cryptoCoin;
        doc["formatType"] = settings.formatType == FORMAT_US ? "US" : "EU";
        doc["matrixIntensity"] = settings.matrixIntensity;
        doc["scrollSpeed"] = settings.scrollSpeed;
    }

    // Read the settings from a JSON document and apply them to a settings snapshot
    void readSettingsFromJson(JsonDocument& doc, Settings& settings) {
        if (!doc["currentPrice"].isNull())
            settings.currentPriceVisible = doc["currentPrice"].as<bool>();
        if (!doc["priceChange"].isNull())
            settings.priceChangeVisible = doc["priceChange"].as<bool>();
        if (!doc["marketCap"].isNull())
            settings.marketCapVisible = doc["marketCap"].as<bool>();
        if (!doc["dailyHighLow"].isNull())
            settings.dailyHighLowVisible = doc["dailyHighLow"].as<bool>();
        if (!doc["yearHighLow"].isNull())
            settings.yearHighLowVisible = doc["yearHighLow"].as<bool>();
        if (!doc["openPrice"].isNull())
            settings.openPriceVisible = doc["openPrice"].as<bool>();
        if (!doc["volume"].isNull())
            settings.volumeVisible = doc["volume"].as<bool>();
        if (!doc["cryptoCoin"].isNull())
            stringCopy(settings.cryptoCoin, doc["cryptoCoin"].as<const char*>(), sizeof(settings.cryptoCoin));
        if (!doc["formatType"].isNull())
            settings.formatType = strcmp(doc["formatType"].as<const char*>(), "US") == 0 ? FORMAT_US : FORMAT_EU;
        if (!doc["matrixIntensity"].isNull())
            settings.matrixIntensity = doc["matrixIntensity"].as<uint8_t>();
        if (!doc["scrollSpeed"].isNull())
            settings.scrollSpeed = doc["scrollSpeed"].as<uint8_t>();
    }

    // Read the credentials from a JSON document
    void readCredentialsFromJson(JsonDocument& doc) {
        if (!doc["apiKey"].isNull())
            stringCopy(apiKey, doc["apiKey"], sizeof(apiKey));
        if (!doc["ssid"].isNull())
            stringCopy(wiFiSSID, doc["ssid"], sizeof(wiFiSSID));
        if (!doc["password"].isNull())
            stringCopy(wiFiPassword, doc["password"], sizeof(wiFiPassword));
    }
}

// Read a JSON document from EEPROM by extracting the stored JSON payload.
bool readEEPROM(JsonDocument& doc) {
    char eepromData[EEPROM_SIZE + 1];
    size_t jsonStart = EEPROM_SIZE;
    size_t jsonEnd = 0;

    for (size_t i = 0; i < EEPROM_SIZE; i++) {
        const char currentByte = static_cast<char>(EEPROM.read(i));
        eepromData[i] = currentByte;

        if (jsonStart == EEPROM_SIZE && currentByte == '{') {
            jsonStart = i;
        }

        if (jsonStart != EEPROM_SIZE && currentByte == '}') {
            jsonEnd = i;
        }

        if (currentByte == '\0') {
            break;
        }
    }

    if (jsonStart == EEPROM_SIZE || jsonEnd < jsonStart) {
        return false;
    }

    const size_t jsonLength = jsonEnd - jsonStart + 1;
    char jsonData[EEPROM_SIZE + 1];
    memcpy(jsonData, eepromData + jsonStart, jsonLength);
    jsonData[jsonLength] = '\0';

	DeserializationError error = deserializeJson(doc, jsonData);
    if (error) {
        return false;
    }

    return true;
}

// Write data to the EEPROM
bool writeEEPROM() {
    return saveSettingsToEEPROM(getSettings());
}

// Load the settings snapshot from EEPROM without mutating runtime state
bool loadSettingsFromEEPROM(Settings& settings) {
	JsonDocument doc; // JSON object
    settings = getSettings();

    if (!readEEPROM(doc)) {
        return false;
    }

    readSettingsFromJson(doc, settings);
    return true;
}

// Save a settings snapshot to EEPROM while preserving credentials and API key from runtime state
bool saveSettingsToEEPROM(const Settings& settings) {
    JsonDocument doc; // JSON object

    // Add the WiFi credentials to the JSON object
    if (strlen(wiFiSSID) != 0 && strlen(wiFiPassword) != 0) {
        doc["ssid"] = wiFiSSID;
        doc["password"] = wiFiPassword;
    }

    // Add the API key to the JSON object
    if (strlen(apiKey) != 0) {
        doc["apiKey"] = apiKey;
    }

    // Add the settings to the JSON object
    appendSettingsToJson(doc, settings);

    // Write data to EEPROM
    EepromStream eepromStream(0, EEPROM_SIZE);
    const size_t jsonSize = serializeJson(doc, eepromStream);
    if (jsonSize == 0) {
        return false; // Error while writing on EEPROM
    }

    if (jsonSize < EEPROM_SIZE) {
        EEPROM.write(jsonSize, '\0');
    }

    // Commit changes
	if (!EEPROM.commit()) {
 		return false; // Error while committing changes
    }

    // Mark as saved
	printLogfln("Data saved on EEPROM");
	return true; // Write success
}

// Clear all saved data from the EEPROM
bool clearEEPROM() {
    // Write 0xFF to all EEPROM addresses
    for (int i = 0; i < EEPROM_SIZE; i++) {
        EEPROM.write(i, 0xFF);
    }

    // Commit changes
    if (!EEPROM.commit()) {
        return false;
    }

    // Mark as cleared
    printLogfln("EEPROM cleared");
    return true;
}

// Test read EEPROM
void testReadEEPROM() {
    printLogfln("EEPROM size: %d", EEPROM.length()); // Print the size of the EEPROM
    
    // Print the content of the EEPROM
    printLog("EEPROM data: ");
    for (int i = 0; i < EEPROM_SIZE; i++) {
        printLog(char(EEPROM.read(i)));
    }
    printLogln("");
}

// Setup EEPROM
void setupEEPROM() {
	EEPROM.begin(EEPROM_SIZE); // Start the EEPROM

    JsonDocument doc; // JSON object
    if (!readEEPROM(doc)) {
        printLogfln("EEPROM is empty or invalid, writing default settings...");
        writeEEPROM();
        return;
    }

    readCredentialsFromJson(doc);
    Settings settings = getSettings();
    readSettingsFromJson(doc, settings);
    setSettings(settings);
    printLogfln("EEPROM data loaded successfully");
}