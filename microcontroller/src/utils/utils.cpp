#include "utils.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "../config/config.h"

// Custom string copy function
void stringCopy(char* destination, const char* text, size_t length) {
	if (length <= 0) return; // If length is 0, do nothing
    strncpy(destination, text, length - 1); // Copy the string
    destination[length - 1] = '\0'; // Add the terminating character
}

// Format a currency - Inspired by the Currency library made by RobTillaart
char* addThousandsSeparators(double value, int decimals, char decimalSeparator, char thousandSeparator, char symbol) {
	static char tmp[30]; // Temporary buffer to store the formatted string
	uint8_t index = 0; // Index for placing characters in tmp
	int64_t v = (int64_t)value; // Convert the value to an integer
	bool negative = v < 0; // Flag for negative numbers
	if (negative) v = -v; // Make v positive for processing
	int pos = -decimals; // Tracks the position relative to the decimal point
	while ((pos < 1) || (v > 0)) { // Loop until we've processed all digits
		if ((pos == 0) && (decimals > 0)) tmp[index++] = decimalSeparator; // Add decimal separator
		if ((pos > 0) && (pos % 3 == 0)) tmp[index++] = thousandSeparator; // Add thousand separator
		pos++;
		tmp[index++] = (v % 10) + '0'; // Extract and store the last digit of v
		v /= 10; // Remove the last digit from v
	}
	if (negative) tmp[index++] = '-'; // Add negative sign if necessary
	if (symbol != ' ') {
		tmp[index++] = ' '; // Add space
		tmp[index++] = symbol; // Add the currency symbol if there is
	}
	tmp[index] = '\0'; // Null-terminate the string
	for (uint8_t i = 0, j = index - 1; i < index / 2; i++, j--) { // Reverse the string since it was built backwards
		char c = tmp[i];
		tmp[i] = tmp[j];
		tmp[j] = c;
	}
	return tmp; // Return the pointer to the formatted string
}

// Format currency
void formatCurrency(double value, char* output, const byte length) {
	const DeviceConfig& config = getDeviceConfig();
	if (config.formatType == FORMAT_US) {
		stringCopy(output, addThousandsSeparators(value * 100, 2, '.', ','), length);
	} else {
		stringCopy(output, addThousandsSeparators(value * 100, 2, ',', '.'), length);
	}
}

// Accumulate the request body for POST requests
RequestBodyResult accumulateRequestBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	// If it's the first chunk, allocate memory for the entire body
	if (index == 0) {
		releaseRequestBody(request);
		request->_tempObject = malloc(total + 1);
		if (request->_tempObject == NULL) {
			return { RequestBodyState::Error, NULL, 0 };
		}
	}

	// If memory allocation failed, return an error
	if (request->_tempObject == NULL) {
		return { RequestBodyState::Error, NULL, 0 };
	}

	// Copy the current chunk into the allocated memory
	memcpy(static_cast<uint8_t*>(request->_tempObject) + index, data, len);

	// If still receiving data, wait for the next call
	if (index + len != total) {
		return { RequestBodyState::InProgress, NULL, 0 };
	}

	// All data received, return the complete body
	char* body = static_cast<char*>(request->_tempObject);
	body[total] = '\0';
	return { RequestBodyState::Ready, body, total };
}

// Release the memory allocated for the request body
void releaseRequestBody(AsyncWebServerRequest *request) {
	if (request->_tempObject == NULL) return;
	free(request->_tempObject);
	request->_tempObject = NULL;
}