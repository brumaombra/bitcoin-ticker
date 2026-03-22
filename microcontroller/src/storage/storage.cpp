#include "storage.h"
#include <EEPROM.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>
#include <cstring>
#include "../config/config.h"
#include "../utils/utils.h"
#include "../serial/serial.h"

const int EEPROM_SIZE = 512; // EEPROM size

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

    // Add the selected crypto coin to the JSON object
    doc["cryptoCoin"] = cryptoCoin;

    // Add the visibility settings to the JSON object
    doc["currentPrice"] = currentPriceVisible;
    doc["priceChange"] = priceChangeVisible;
    doc["marketCap"] = marketCapVisible;
    doc["dailyHighLow"] = dailyHighLowVisible;
    doc["yearHighLow"] = yearHighLowVisible;
    doc["openPrice"] = openPriceVisible;
    doc["volume"] = volumeVisible;

    // Add the various settings to the JSON object
    doc["formatType"] = formatType == FORMAT_US ? "US" : "EU";
    doc["matrixIntensity"] = matrixIntensity;
    doc["scrollSpeed"] = scrollSpeed;

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

// Load the settings from the EEPROM
void loadSettingFromEEPROM() {
    JsonDocument doc; // JSON object

    // Read the EEPROM
	if (!readEEPROM(doc)) {
		printLogfln("EEPROM is empty or invalid, writing default settings...");
		writeEEPROM();
		return;
    }

    // Load the API key
    if (!doc["apiKey"].isNull()) {
        stringCopy(apiKey, doc["apiKey"], sizeof(apiKey));
    }

    // Load the selected crypto coin
    if (!doc["cryptoCoin"].isNull()) {
        stringCopy(cryptoCoin, doc["cryptoCoin"].as<const char*>(), sizeof(cryptoCoin));
    }
    
    // Load the WiFi credentials
    if (!doc["ssid"].isNull()) {
        stringCopy(wiFiSSID, doc["ssid"], sizeof(wiFiSSID));
    }
    if (!doc["password"].isNull()) {
        stringCopy(wiFiPassword, doc["password"], sizeof(wiFiPassword));
    }
    
    // Load the visibility settings
    if (!doc["currentPrice"].isNull()) {
        currentPriceVisible = doc["currentPrice"].as<bool>();
    }
    if (!doc["priceChange"].isNull()) {
        priceChangeVisible = doc["priceChange"].as<bool>();
    }
    if (!doc["marketCap"].isNull()) {
        marketCapVisible = doc["marketCap"].as<bool>();
    }
    if (!doc["dailyHighLow"].isNull()) {
        dailyHighLowVisible = doc["dailyHighLow"].as<bool>();
    }
    if (!doc["yearHighLow"].isNull()) {
        yearHighLowVisible = doc["yearHighLow"].as<bool>();
    }
    if (!doc["openPrice"].isNull()) {
        openPriceVisible = doc["openPrice"].as<bool>();
    }
    if (!doc["volume"].isNull()) {
        volumeVisible = doc["volume"].as<bool>();
    }

    // Load the various settings
    if (!doc["formatType"].isNull()) {
        formatType = strcmp(doc["formatType"].as<const char*>(), "US") == 0 ? FORMAT_US : FORMAT_EU;
    }
    if (!doc["matrixIntensity"].isNull()) {
        matrixIntensity = doc["matrixIntensity"].as<uint8_t>();
    }
    if (!doc["scrollSpeed"].isNull()) {
        scrollSpeed = doc["scrollSpeed"].as<uint8_t>();
    }
    
	printLogfln("EEPROM data loaded successfully");
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
    loadSettingFromEEPROM(); // Load the settings from the EEPROM
}