#include "api.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include "../config/config.h"
#include "../utils/utils.h"
#include "../storage/storage.h"
#include "../matrix/matrix.h"
#include "../serial/serial.h"

WiFiClientSecure clientSecure; // HTTPS client
HTTPClient http; // HTTP object
const char marketApiUrlTemplate[] = "https://api.coingecko.com/api/v3/coins/%s?localization=false&tickers=false&market_data=true&community_data=false&developer_data=false&sparkline=false"; // Market API URL template
const char apiKeyHeaderName[] = "x-cg-demo-api-key"; // CoinGecko demo key header

// Get the selected crypto coin API ID
static const char* getSelectedCryptoApiId() {
	return strcmp(cryptoCoin, "kaspa") == 0 ? "kaspa" : "bitcoin";
}

// Get the selected crypto coin ticker label
static const char* getSelectedCryptoTickerLabel() {
	return strcmp(cryptoCoin, "kaspa") == 0 ? "KAS" : "BTC";
}

// Get the main market data used by the ticker
static bool fetchMarketData(JsonDocument &doc) {
	char url[220];

	// Filter just the needed data to save memory and speed up parsing
	JsonDocument filter;
	filter["market_data"]["current_price"]["usd"] = true;
	filter["market_data"]["price_change_percentage_24h"] = true;
	filter["market_data"]["price_change_24h"] = true;
	filter["market_data"]["market_cap"]["usd"] = true;
	filter["market_data"]["high_24h"]["usd"] = true;
	filter["market_data"]["low_24h"]["usd"] = true;
	filter["market_data"]["total_volume"]["usd"] = true;
	filter["market_data"]["ath"]["usd"] = true;
	filter["market_data"]["atl"]["usd"] = true;

	// Prepare the HTTP request
	snprintf(url, sizeof(url), marketApiUrlTemplate, getSelectedCryptoApiId());
	printLogfln("Requesting URL: %s", url);
	http.begin(clientSecure, url);
	http.addHeader("Accept", "application/json");
	http.addHeader(apiKeyHeaderName, apiKey);

	// Send the request
	const int httpCode = http.GET();
	if (httpCode != HTTP_CODE_OK) {
		printLogfln("HTTP call error: %d", httpCode);
		http.end();
		return false;
	}

	// Parse the response
	DeserializationError error = deserializeJson(doc, http.getStream(), DeserializationOption::Filter(filter));
	if (error) {
		printLogfln("Error while parsing the JSON: %s", error.c_str());
		http.end();
		return false;
	}

	// End the HTTP connection
	http.end();
	return true;
}

// Setup the web client
void setupWebClient() {
    clientSecure.setInsecure(); // Disable SSL certificate verification
    http.setTimeout(5000); // Set timeout
}

// Create the scrolling message
void createStockDataMessage(JsonDocument marketDoc) {
	printLogfln("Creating message...");
	const byte MAX_NUMBER_SIZE = 30; // Max length for the numbers
	char tempString[MAX_NUMBER_SIZE]; // Temporary string 1
	char tempString2[MAX_NUMBER_SIZE]; // Temporary string 2
	double tempVal; // Temporary variable 1
	double tempVal2; // Temporary variable 2
	JsonVariant marketData = marketDoc["market_data"];

	// Check if the market data is present
	if (marketData.isNull()) {
		printLogfln("Missing CoinGecko market data");
		return;
	}

	// Price
	tempVal = marketData["current_price"]["usd"].as<double>();
	tempVal2 = marketData["price_change_percentage_24h"].as<double>();
	formatCurrency(tempVal, tempString, MAX_NUMBER_SIZE);
	formatCurrency(tempVal2, tempString2, MAX_NUMBER_SIZE);
	snprintf(stripMessagePrice, BUF_SIZE, " %s $ %s  (%s%%)", getSelectedCryptoTickerLabel(), tempString, tempString2);

	// Daily Change
	tempVal = marketData["price_change_24h"].as<double>();
	formatCurrency(tempVal, tempString, MAX_NUMBER_SIZE);
	snprintf(stripMessageDailyChange, BUF_SIZE, "Daily Change: $ %s", tempString);

	// Market cap
	tempVal = marketData["market_cap"]["usd"].as<double>();
	formatCurrency(tempVal, tempString, MAX_NUMBER_SIZE);
	snprintf(stripMessageMarketCap, BUF_SIZE, "Market Cap: $ %s", tempString);

	// Daily High/Low
	tempVal = marketData["high_24h"]["usd"].as<double>();
	tempVal2 = marketData["low_24h"]["usd"].as<double>();
	formatCurrency(tempVal, tempString, MAX_NUMBER_SIZE);
	formatCurrency(tempVal2, tempString2, MAX_NUMBER_SIZE);
	snprintf(stripMessageDailyHighLow, BUF_SIZE, "Daily High: $ %s  -  Daily Low: $ %s", tempString, tempString2);

	// Year High/Low
	tempVal = marketData["ath"]["usd"].as<double>();
	tempVal2 = marketData["atl"]["usd"].as<double>();
	formatCurrency(tempVal, tempString, MAX_NUMBER_SIZE);
	formatCurrency(tempVal2, tempString2, MAX_NUMBER_SIZE);
	snprintf(stripMessageYearHighLow, BUF_SIZE, "Year High: $ %s  -  Year Low: $ %s", tempString, tempString2);

	// Open
	tempVal = marketData["current_price"]["usd"].as<double>() - marketData["price_change_24h"].as<double>();
	formatCurrency(tempVal, tempString, MAX_NUMBER_SIZE);
	snprintf(stripMessageOpen, BUF_SIZE, "Open: $ %s", tempString);

	// Volume
	tempVal = marketData["total_volume"]["usd"].as<double>();
	formatCurrency(tempVal, tempString, MAX_NUMBER_SIZE);
	snprintf(stripMessageVolume, BUF_SIZE, "Volume: $ %s", tempString);
}

// Getting Bitcoin data
bool getStockDataAPI() {
	JsonDocument marketDoc;

	// Fetch the market data
	if (!fetchMarketData(marketDoc)) {
		return false;
	}

	// Create the scrolling message
	createStockDataMessage(marketDoc);

	// All OK
	return true;
}

// Call the API to get the data
bool callAPI() {
	currentMillis = millis();

	// Call the API every 6 minutes (To limit usage)
	if (currentMillis - timestampStockData > 360000 || timestampStockData == 0) {
		// Check if the API key is present
		if (apiKey[0] == '\0') {
			const char errorMessageApi[] = "API key is not present. Use the web page to insert the key and try again.";
			printLogfln(errorMessageApi);
			printOnLedMatrix(errorMessageApi, sizeof(errorMessageApi)); // Print the message on the matrix
			return false; // If error, return false
		}

		// Get the data
		printLogfln("Calling the API...");
		if (!getStockDataAPI()) {
			const char errorMessageServer[] = "Error while calling the API. Retrying...";
			printLogfln(errorMessageServer);
			printOnLedMatrix(errorMessageServer, sizeof(errorMessageServer)); // Print the message on the matrix
			return false; // If error, return false
		}

		// Update the timestamp
		timestampStockData = currentMillis;
	}

	// If no error, return true
	return true
}