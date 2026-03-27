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
    // Append the config to a JSON document
    void appendConfigToJson(JsonDocument& doc, const DeviceConfig& config) {
        // Add the config fields to the JSON object
        doc["currentPrice"] = config.currentPriceVisible;
        doc["priceChange"] = config.priceChangeVisible;
        doc["marketCap"] = config.marketCapVisible;
        doc["dailyHighLow"] = config.dailyHighLowVisible;
        doc["yearHighLow"] = config.yearHighLowVisible;
        doc["openPrice"] = config.openPriceVisible;
        doc["volume"] = config.volumeVisible;
        doc["cryptoCoin"] = config.cryptoCoin;
        doc["formatType"] = config.formatType == FORMAT_US ? "US" : "EU";
        doc["matrixIntensity"] = config.matrixIntensity;
        doc["scrollSpeed"] = config.scrollSpeed;

        // Add WiFi credentials if present
        if (strlen(config.ssid) != 0 && strlen(config.password) != 0) {
            doc["ssid"] = config.ssid;
            doc["password"] = config.password;
        }

        // Add API key if present
        if (strlen(config.apiKey) != 0) {
            doc["apiKey"] = config.apiKey;
        }
    }

    // Read the config from a JSON document and apply it to a config snapshot
    void readConfigFromJson(JsonDocument& doc, DeviceConfig& config) {
        if (!doc["currentPrice"].isNull())
            config.currentPriceVisible = doc["currentPrice"].as<bool>();
        if (!doc["priceChange"].isNull())
            config.priceChangeVisible = doc["priceChange"].as<bool>();
        if (!doc["marketCap"].isNull())
            config.marketCapVisible = doc["marketCap"].as<bool>();
        if (!doc["dailyHighLow"].isNull())
            config.dailyHighLowVisible = doc["dailyHighLow"].as<bool>();
        if (!doc["yearHighLow"].isNull())
            config.yearHighLowVisible = doc["yearHighLow"].as<bool>();
        if (!doc["openPrice"].isNull())
            config.openPriceVisible = doc["openPrice"].as<bool>();
        if (!doc["volume"].isNull())
            config.volumeVisible = doc["volume"].as<bool>();
        if (!doc["cryptoCoin"].isNull())
            stringCopy(config.cryptoCoin, doc["cryptoCoin"].as<const char*>(), sizeof(config.cryptoCoin));
        if (!doc["formatType"].isNull())
            config.formatType = strcmp(doc["formatType"].as<const char*>(), "US") == 0 ? FORMAT_US : FORMAT_EU;
        if (!doc["matrixIntensity"].isNull())
            config.matrixIntensity = doc["matrixIntensity"].as<uint8_t>();
        if (!doc["scrollSpeed"].isNull())
            config.scrollSpeed = doc["scrollSpeed"].as<uint8_t>();
        if (!doc["apiKey"].isNull())
            stringCopy(config.apiKey, doc["apiKey"], sizeof(config.apiKey));
        if (!doc["ssid"].isNull())
            stringCopy(config.ssid, doc["ssid"], sizeof(config.ssid));
        if (!doc["password"].isNull())
            stringCopy(config.password, doc["password"], sizeof(config.password));
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
    const DeviceConfig& config = getDeviceConfig();
    return saveDeviceConfigToEEPROM(config);
}

// Load the config snapshot from EEPROM without mutating runtime state
bool loadDeviceConfigFromEEPROM(DeviceConfig& config) {
    // Get the current config
    config = getDeviceConfig();

    // Read the config from EEPROM
    JsonDocument doc;
    if (!readEEPROM(doc)) {
        return false;
    }

    // Apply the read config to the provided config snapshot
    readConfigFromJson(doc, config);
    return true;
}

// Save the config snapshot to EEPROM
bool saveDeviceConfigToEEPROM(const DeviceConfig& config) {
    JsonDocument doc; // JSON object

    // Add the config to the JSON object
    appendConfigToJson(doc, config);

    // Write data to EEPROM
    EepromStream eepromStream(0, EEPROM_SIZE);
    const size_t jsonSize = serializeJson(doc, eepromStream);
    if (jsonSize == 0) {
        return false; // Error while writing on EEPROM
    }

    // Add a null terminator if there is space
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
    // Start the EEPROM
	EEPROM.begin(EEPROM_SIZE);

    // Read the config from EEPROM
    JsonDocument doc;
    if (!readEEPROM(doc)) {
        printLogfln("EEPROM is empty or invalid, writing default settings...");
        writeEEPROM();
        return;
    }

    // Apply the read config to the runtime state
    DeviceConfig config = getDeviceConfig();
    readConfigFromJson(doc, config);
    setDeviceConfig(config);
    printLogfln("EEPROM data loaded successfully");
}