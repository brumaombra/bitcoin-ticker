import { delay } from '~/composables/useUtils.js';

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
    try {
        const url = buildDeviceUrl('/api/networks');
        const data = await $fetch(url);
        return data.networks || [];
    } catch (error) {
        throw new Error('An error occurred while loading the Wi-Fi networks');
    }
};

// Fetch the saved device settings
export const getSettings = async () => {
    try {
        const url = buildDeviceUrl('/api/settings');
        const data = await $fetch(url);
        return data || {};
    } catch (error) {
        throw new Error('An error occurred while loading the settings');
    }
};

// Persist the current device settings
export const saveSettings = async settings => {
    try {
        // Call the device API to save the settings
        const url = buildDeviceUrl('/api/settings');
        const data = await $fetch(url, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: settings
        });

        // Return if successful
        return data.status === 'OK';
    } catch (error) {
        throw new Error('An error occurred while saving the settings');
    }
};

// Reset the saved device settings and restart the firmware
export const resetSettings = async () => {
    try {
        const url = buildDeviceUrl('/api/reset-settings');
        const data = await $fetch(url);
        return data || {};
    } catch (error) {
        throw new Error('An error occurred while resetting the settings');
    }
};

// Save the market data API key on the device
export const saveApiKey = async apiKey => {
    try {
        const url = buildDeviceUrl('/api/api-key', { apiKey });
        const data = await $fetch(url);
        return data.status === 'OK';
    } catch (error) {
        throw new Error('An error occurred while saving the API key');
    }
};

// Poll the device until the WiFi connection attempt completes
const checkWiFiConnectionPolling = async () => {
    const maxAttempts = 45;

    for (let attempt = 1; attempt <= maxAttempts; attempt += 1) {
        try {
            const url = buildDeviceUrl('/api/check-connection');
            const data = await $fetch(url, { retry: 0 });
            const status = Number(data?.status);

            switch (status) {
                case connectionStatus.WIFI_TRY:
                    break;
                case connectionStatus.WIFI_OK:
                    return {
                        ssid: data?.ssid || '',
                        ip: data?.ip || ''
                    };
                case connectionStatus.WIFI_KO:
                    throw new Error('The Wi-Fi credentials are wrong or the network is unavailable.');
                default:
                    throw new Error('The device returned an unknown Wi-Fi connection status.');
            }
        } catch (error) {
            if (error instanceof Error && error.message !== 'fetch failed') {
                throw error;
            }
        }

        await delay(1000);
    }

    throw new Error('The device did not finish connecting to Wi-Fi in time.');
};

// Send WiFi credentials and wait for the final connection result
export const connectToWiFi = async (ssid, password) => {
    try {
        // Call the device API to start the WiFi connection process
        const url = buildDeviceUrl('/api/connect', { ssid, password });
        const data = await $fetch(url);

        // If the device is trying to connect, start polling for the result
        if (Number(data.status) === connectionStatus.WIFI_TRY) {
            return await checkWiFiConnectionPolling();
        }

        // If the device returned an immediate failure, throw an error
        throw new Error('Failed to connect to WiFi');
    } catch (error) {
        if (error instanceof Error) {
            throw error;
        }

        throw new Error('An error occurred while connecting to the Wi-Fi network');
    }
};