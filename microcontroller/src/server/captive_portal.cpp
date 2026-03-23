#include "server.h"
#include "../config/config.h"
#include "../webui/generated_assets.h"

namespace {
	// Normalize the request path by removing trailing slashes
	String normalizeAssetPath(const String& requestPath) {
		if (requestPath.length() <= 1 || !requestPath.endsWith("/")) return requestPath;
		return requestPath.substring(0, requestPath.length() - 1);
	}

	// Find the embedded asset that matches the request path
	const EmbeddedAsset* findEmbeddedAsset(const String& requestPath) {
		const String normalizedPath = normalizeAssetPath(requestPath);

		// Loop all embedded assets
		for (size_t i = 0; i < EMBEDDED_ASSET_COUNT; i++) {
			if (normalizedPath.equals(EMBEDDED_ASSETS[i].routePath)) {
				return &EMBEDDED_ASSETS[i];
			}
		}

		// No matching asset found
		return NULL;
	}

	// Send the embedded asset
	bool sendEmbeddedAsset(AsyncWebServerRequest *request, const String& requestPath) {
		// Find the asset for the requested path
		const EmbeddedAsset* asset = findEmbeddedAsset(requestPath);
		if (asset == NULL) {
			return false;
		}

		// Send the asset data as the response
		AsyncWebServerResponse *response = request->beginResponse_P(200, asset->contentType, asset->data, asset->size);
		if (asset->gzip) {
			response->addHeader("Content-Encoding", "gzip");
		}

		// Send the response
		request->send(response);
		return true;
	}

	// Determine if the request should be redirected to the setup page in AP mode
	bool shouldRedirectToSetupPage(const String& requestPath) {
		if (!accessPointEnabled) return false;
		if (requestPath == "/ap-setup" || requestPath.startsWith("/_nuxt/") || requestPath.startsWith("/api/")) return false;
		return true;
	}
}

// Setup the captive portal route to handle all unmatched requests
void setupCaptivePortalRoute() {
	server.onNotFound([](AsyncWebServerRequest *request) {
		// Redirect to Wi-Fi setup page if in AP mode
		if ((request->method() == HTTP_GET || request->method() == HTTP_HEAD) && shouldRedirectToSetupPage(request->url())) {
			request->redirect("/ap-setup");
			return;
		}

		// Serve embedded assets
		if (request->method() == HTTP_GET && sendEmbeddedAsset(request, request->url())) {
			return;
		}

		// If no route matched, return 404
		request->send(404);
	});
}