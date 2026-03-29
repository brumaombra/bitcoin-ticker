#ifndef WIFI_H
#define WIFI_H

void initializeWiFiStack();
bool connectToWiFi();
bool setupAccessPoint();
bool manageWiFiConnection();
bool checkWifiConnection();

#endif