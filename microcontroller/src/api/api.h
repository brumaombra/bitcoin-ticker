#ifndef API_H
#define API_H

#include <ArduinoJson.h>

void setupWebClient();
void createStockDataMessage(JsonDocument marketDoc);
bool getStockDataAPI();
bool callAPI();

#endif