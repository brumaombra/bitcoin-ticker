// WiFi connection status codes returned by the device
const connectionStatus = Object.freeze({
    WIFI_TRY: 2,
    WIFI_OK: 1,
    WIFI_KO: 0
});

// Build a device endpoint URL with an optional query string
const buildDeviceUrl = (baseUrl, path, query = null) => {
    const normalizedBaseUrl = baseUrl.endsWith('/') ? baseUrl.slice(0, -1) : baseUrl;
    const searchParams = query ? `?${new URLSearchParams(query).toString()}` : '';
    return `${normalizedBaseUrl}${path}${searchParams}`;
};

// Read the device base URL from runtime config
const getDeviceBaseUrl = () => {
    const runtimeConfig = useRuntimeConfig();
    return runtimeConfig.public.espBaseUrl;
};

// Fetch the list of visible WiFi networks
export const getNetworks = async () => {
    const baseUrl = getDeviceBaseUrl();

    try {
        const data = await $fetch(buildDeviceUrl(baseUrl, '/networks'));
        return Array.isArray(data.networks) ? data.networks : [];
    } catch (error) {
        console.error(error);
        return [];
    }
};

// Fetch the saved device settings
export const getSettings = async () => {
    const baseUrl = getDeviceBaseUrl();

    try {
        const data = await $fetch(buildDeviceUrl(baseUrl, '/settings'));
        return data || {};
    } catch (error) {
        console.error(error);
        return {};
    }
};

// Persist the current device settings
export const saveSettings = async settings => {
    const baseUrl = getDeviceBaseUrl();

    try {
        const data = await $fetch(buildDeviceUrl(baseUrl, '/settings'), {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: settings
        });

        return data.status === 'OK';
    } catch (error) {
        console.error(error);
        throw new Error('An error occurred while saving the settings');
    }
};

// Save the market data API key on the device
export const saveApiKey = async apiKey => {
    const baseUrl = getDeviceBaseUrl();

    try {
        const data = await $fetch(buildDeviceUrl(baseUrl, '/apiKey', { apiKey }));
        return data.status === 'OK';
    } catch (error) {
        console.error(error);
        throw new Error('An error occurred while saving the API key');
    }
};

// Poll the device until the WiFi connection attempt completes
const checkWiFiConnectionPolling = async () => {
    const baseUrl = getDeviceBaseUrl();

    return new Promise((resolve, reject) => {
        const maxAttempts = 30;
        let attempts = 0;
        const polling = window.setInterval(async () => {
            try {
                attempts += 1;

                if (attempts > maxAttempts) {
                    window.clearInterval(polling);
                    reject(new Error('Connection timeout'));
                    return;
                }

                const data = await $fetch(buildDeviceUrl(baseUrl, '/checkConnection'));
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
    const baseUrl = getDeviceBaseUrl();

    try {
        const data = await $fetch(buildDeviceUrl(baseUrl, '/connect', { ssid, password }));

        if (data.status === connectionStatus.WIFI_TRY) {
            return await checkWiFiConnectionPolling();
        }

        throw new Error('Failed to connect to WiFi');
    } catch (error) {
        console.error(error);
        throw new Error('An error occurred while connecting to the Wi-Fi network');
    }
};