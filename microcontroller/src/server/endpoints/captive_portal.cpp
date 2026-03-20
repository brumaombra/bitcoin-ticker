#include "endpoints.h"
#include "../../config/config.h"
#include "../../webui/generated_assets.h"

namespace {
	String normalizeAssetPath(const String& requestPath) {
		if (requestPath.length() <= 1 || !requestPath.endsWith("/")) {
			return requestPath;
		}

		return requestPath.substring(0, requestPath.length() - 1);
	}

	const EmbeddedAsset* findEmbeddedAsset(const String& requestPath) {
		const String normalizedPath = normalizeAssetPath(requestPath);

		for (size_t i = 0; i < EMBEDDED_ASSET_COUNT; i++) {
			if (normalizedPath.equals(EMBEDDED_ASSETS[i].routePath)) {
				return &EMBEDDED_ASSETS[i];
			}
		}

		return NULL;
	}

	bool sendEmbeddedAsset(AsyncWebServerRequest *request, const String& requestPath) {
		const EmbeddedAsset* asset = findEmbeddedAsset(requestPath);
		if (asset == NULL) {
			return false;
		}

		AsyncWebServerResponse *response = request->beginResponse_P(200, asset->contentType, asset->data, asset->size);
		if (asset->gzip) {
			response->addHeader("Content-Encoding", "gzip");
		}

		request->send(response);
		return true;
	}

	bool shouldRedirectToWifiPage(const String& requestPath) {
		if (!accessPointEnabled) {
			return false;
		}

		if (requestPath == "/wifi" || requestPath.startsWith("/_nuxt/") || requestPath.startsWith("/api/")) {
			return false;
		}

		return true;
	}
}

void setupCaptivePortalRoute() {
	// Embedded Nuxt routes and generated assets
	server.onNotFound([](AsyncWebServerRequest *request) {
		// Redirect to Wi-Fi setup page if in AP mode
		if ((request->method() == HTTP_GET || request->method() == HTTP_HEAD) && shouldRedirectToWifiPage(request->url())) {
			request->redirect("/wifi");
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