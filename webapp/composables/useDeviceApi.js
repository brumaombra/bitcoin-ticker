import { delay, translate } from '~/composables/useUtils.js';

// WiFi connection status codes returned by the device
const connectionStatus = Object.freeze({
    WIFI_TRY: 2,
    WIFI_OK: 1,
    WIFI_KO: 0
});

// Build a device endpoint URL with an optional query string
const buildDeviceUrl = (path, query = null) => {
    const runtimeConfig = useRuntimeConfig();
    const baseUrl = runtimeConfig.public.espBaseUrl;
    const normalizedBaseUrl = baseUrl.endsWith('/') ? baseUrl.slice(0, -1) : baseUrl;
    const searchParams = query ? `?${new URLSearchParams(query).toString()}` : '';
    return `${normalizedBaseUrl}${path}${searchParams}`;
};

// Fetch the list of visible WiFi networks
export const getNetworks = async () => {
    const url = buildDeviceUrl('/api/networks');
    const response = await $fetch(url);
    return response.data?.networks || [];
};

// Fetch the saved device settings
export const getSettings = async () => {
    const url = buildDeviceUrl('/api/settings');
    const response = await $fetch(url);
    return response.data || {};
};

// Persist the current device settings
export const saveSettings = async settings => {
    // Call the device API to save the settings
    const url = buildDeviceUrl('/api/settings');
    await $fetch(url, {
        method: 'POST',
        body: settings
    });

    // Return result status
    return true;
};

// Reset the saved device settings and restart the firmware
export const resetSettings = async () => {
    const url = buildDeviceUrl('/api/reset-settings');
    const response = await $fetch(url);
    return response.data || {};
};

// Save the market data API key on the device
export const saveApiKey = async apiKey => {
    // Call the device API to save the API key
    const url = buildDeviceUrl('/api/api-key');
    await $fetch(url, {
        method: 'POST',
        body: {
            apiKey
        }
    });

    // Return result status
    return true;
};

// Poll the device until the WiFi connection attempt completes
const checkWiFiConnectionPolling = async () => {
    const maxAttempts = 45;

    for (let attempt = 1; attempt <= maxAttempts; attempt += 1) {
        try {
            const url = buildDeviceUrl('/api/check-connection');
            const response = await $fetch(url, { retry: 0 });
            const status = Number(response.data?.connectionStatus);

            switch (status) {
                case connectionStatus.WIFI_TRY:
                    break;
                case connectionStatus.WIFI_OK:
                    return {
                        ssid: response.data?.ssid || '',
                        ip: response.data?.ip || '',
                        hostname: response.data?.hostname || ''
                    };
                case connectionStatus.WIFI_KO:
                    throw new Error(translate('api.wifiWrongCredentials'));
                default:
                    throw new Error(translate('api.wifiUnknownStatus'));
            }
        } catch (error) {
            if (error instanceof Error && error.message !== 'fetch failed') {
                throw error;
            }
        }

        await delay(1000);
    }

    throw new Error(translate('api.wifiTimeout'));
};

// Send WiFi credentials and wait for the final connection result
export const connectToWiFi = async (ssid, password) => {
    try {
        // Call the device API to start the WiFi connection process
        const url = buildDeviceUrl('/api/connect', { ssid, password });
        const response = await $fetch(url);

        // If the device is trying to connect, start polling for the result
        if (Number(response.data?.connectionStatus) === connectionStatus.WIFI_TRY) {
            return await checkWiFiConnectionPolling();
        }

        // If the device returned an immediate failure, throw an error
        throw new Error(translate('api.wifiFailed'));
    } catch (error) {
        if (error instanceof Error) {
            throw error;
        }

        throw new Error(translate('api.wifiConnectError'));
    }
};