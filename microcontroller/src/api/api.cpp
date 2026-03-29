#include "api.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <StreamUtils.h>
#include <WiFiClientSecure.h>
#include "../config/config.h"
#include "../matrix/matrix.h"
#include "../serial/serial.h"

WiFiClientSecure clientSecure; // HTTPS client
HTTPClient http; // HTTP object
const char marketApiUrlTemplate[] = "https://api.coingecko.com/api/v3/coins/markets?vs_currency=usd&ids=%s&price_change_percentage=24h"; // Market API URL template
const char apiKeyHeaderName[] = "x-cg-demo-api-key"; // CoinGecko demo key header

namespace {
	MarketTickerData cachedMarketData = { "BTC", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	bool hasCachedMarketData = false;
}

// Get the selected crypto coin API ID
static const char* getSelectedCryptoApiId() {
	const DeviceConfig& config = getDeviceConfig();
	return strcmp(config.cryptoCoin, "kaspa") == 0 ? "kaspa" : "bitcoin";
}

// Get the selected crypto coin ticker label
static const char* getSelectedCryptoTickerLabel() {
	const DeviceConfig& config = getDeviceConfig();
	return strcmp(config.cryptoCoin, "kaspa") == 0 ? "KAS" : "BTC";
}

// Get the main market data used by the ticker
static bool fetchMarketData(JsonDocument &doc) {
	char url[220];

	// Filter just the needed data to save memory and speed up parsing
	JsonDocument filter;
	filter[0]["current_price"] = true;
	filter[0]["price_change_percentage_24h_in_currency"] = true;
	filter[0]["price_change_24h"] = true;
	filter[0]["market_cap"] = true;
	filter[0]["high_24h"] = true;
	filter[0]["low_24h"] = true;
	filter[0]["total_volume"] = true;
	filter[0]["ath"] = true;
	filter[0]["atl"] = true;

	// Prepare the HTTP request
	snprintf(url, sizeof(url), marketApiUrlTemplate, getSelectedCryptoApiId());
	printLogfln("Requesting URL: %s", url);
	http.begin(clientSecure, url);
	http.addHeader("Accept", "application/json");
	http.addHeader(apiKeyHeaderName, getDeviceConfig().apiKey);

	// Send the request
	const int httpCode = http.GET();
	if (httpCode != HTTP_CODE_OK) {
		printLogfln("HTTP call error: %d", httpCode);
		http.end();
		return false;
	}

	// CoinGecko can reply using chunked transfer encoding, decode it before JSON parsing
	ChunkDecodingStream decodedStream(http.getStream());
	DeserializationError error = deserializeJson(doc, decodedStream, DeserializationOption::Filter(filter));
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

// Parse the fetched market data into the runtime model
static bool parseMarketData(JsonDocument& marketDoc, MarketTickerData& marketData) {
	JsonVariant marketDataJson = marketDoc[0];

	// Check if the market data is present
	if (marketDataJson.isNull()) {
		printLogfln("Missing CoinGecko market data item");
		return false;
	}

	// Parse the market data into the output model
	marketData.tickerLabel = getSelectedCryptoTickerLabel();
	marketData.currentPrice = marketDataJson["current_price"].as<double>();
	marketData.priceChangePercentage24h = marketDataJson["price_change_percentage_24h_in_currency"].as<double>();
	marketData.priceChange24h = marketDataJson["price_change_24h"].as<double>();
	marketData.marketCap = marketDataJson["market_cap"].as<double>();
	marketData.dailyHigh = marketDataJson["high_24h"].as<double>();
	marketData.dailyLow = marketDataJson["low_24h"].as<double>();
	marketData.yearHigh = marketDataJson["ath"].as<double>();
	marketData.yearLow = marketDataJson["atl"].as<double>();
	marketData.openPrice = marketData.currentPrice - marketData.priceChange24h;
	marketData.volume = marketDataJson["total_volume"].as<double>();

	// Mark as successful
	return true;
}

// Fetch and parse the market data
static bool getStockDataAPI(MarketTickerData& marketData) {
	JsonDocument marketDoc;

	// Fetch the market data
	if (!fetchMarketData(marketDoc)) {
		return false;
	}

	// Parse the result into the output model
	return parseMarketData(marketDoc, marketData);
}

// Call the API to get the data
bool callAPI(MarketTickerData& marketData) {
	const unsigned long currentMillis = millis();

	// Call the API every 6 minutes (To limit usage)
	if (currentMillis - timestampStockData > 360000 || timestampStockData == 0) {
		// Check if the API key is present
		if (getDeviceConfig().apiKey[0] == '\0') {
			const char errorMessageApi[] = "API key is not present. Use the web page to insert the key and try again.";
			printLogfln(errorMessageApi);
			printOnLedMatrix(errorMessageApi, sizeof(errorMessageApi)); // Print the message on the matrix
			return false; // If error, return false
		}

		// Get the data
		printLogfln("Calling the API...");
		MarketTickerData refreshedMarketData;

		// If error, print the message on the matrix and return false
		if (!getStockDataAPI(refreshedMarketData)) {
			const char errorMessageServer[] = "Error while calling the API. Retrying...";
			printLogfln(errorMessageServer);
			printOnLedMatrix(errorMessageServer, sizeof(errorMessageServer)); // Print the message on the matrix
			return false; // If error, return false
		}

		// Update the cache with the refreshed data
		cachedMarketData = refreshedMarketData;
		hasCachedMarketData = true;

		// Update the timestamp
		timestampStockData = currentMillis;
	}

	// If no cached data, return false
	if (!hasCachedMarketData) {
		return false;
	}

	// Return the cached data
	marketData = cachedMarketData;

	// If no error, return true
	return true;
}