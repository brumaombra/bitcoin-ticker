#ifndef SERVER_H
#define SERVER_H

// Web server routes
void setupConnectGetRoute();
void setupCheckConnectionGetRoute();
void setupNetworksGetRoute();
void setupApiKeyPostRoute();
void setupSettingsGetRoute();
void setupSettingsPostRoute();
void setupResetSettingsGetRoute();
void setupCaptivePortalRoute();

// Server setup
void setupRoutes();
void setupServer();

#endif