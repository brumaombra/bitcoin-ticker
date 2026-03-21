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
    return new Promise((resolve, reject) => {
        const maxAttempts = 30;
        let attempts = 0;
        const polling = window.setInterval(async () => {
            try {
                attempts += 1;

                // If we've exceeded the maximum number of attempts, stop polling and reject
                if (attempts > maxAttempts) {
                    window.clearInterval(polling);
                    reject(new Error('Connection timeout'));
                    return;
                }

                // Call the device API to check the connection status
                const url = buildDeviceUrl('/api/check-connection');
                const data = await $fetch(url);
                switch (data.status) {
                    case connectionStatus.WIFI_TRY:
                        return;
                    case connectionStatus.WIFI_OK:
                        window.clearInterval(polling);
                        resolve(true);
                        return;
                    case connectionStatus.WIFI_KO:
                        window.clearInterval(polling);
                        reject(new Error('Connection failed'));
                        return;
                    default:
                        throw new Error('Unknown status');
                }
            } catch (error) {
                window.clearInterval(polling);
                reject(new Error('An error occurred while checking the WiFi connection'));
            }
        }, 1000);
    });
};

// Send WiFi credentials and wait for the final connection result
export const connectToWiFi = async (ssid, password) => {
    try {
        // Call the device API to start the WiFi connection process
        const url = buildDeviceUrl('/api/connect', { ssid, password });
        const data = await $fetch(url);

        // If the device is trying to connect, start polling for the result
        if (data.status === connectionStatus.WIFI_TRY) {
            return await checkWiFiConnectionPolling();
        }

        // If the device returned an immediate failure, throw an error
        throw new Error('Failed to connect to WiFi');
    } catch (error) {
        throw new Error('An error occurred while connecting to the Wi-Fi network');
    }
};