const connectionStatus = Object.freeze({
    WIFI_TRY: 2,
    WIFI_OK: 1,
    WIFI_KO: 0
});

const buildDeviceUrl = (baseUrl, path, query = null) => {
    const normalizedBaseUrl = baseUrl.endsWith('/') ? baseUrl.slice(0, -1) : baseUrl;
    const searchParams = query ? `?${new URLSearchParams(query).toString()}` : '';
    return `${normalizedBaseUrl}${path}${searchParams}`;
};

export const useDeviceApi = () => {
    const runtimeConfig = useRuntimeConfig();
    const baseUrl = runtimeConfig.public.espBaseUrl;

    const getNetworks = async () => {
        try {
            const data = await $fetch(buildDeviceUrl(baseUrl, '/networks'));
            return Array.isArray(data.networks) ? data.networks : [];
        } catch (error) {
            console.error(error);
            return [];
        }
    };

    const getSettings = async () => {
        try {
            const data = await $fetch(buildDeviceUrl(baseUrl, '/settings'));
            return data || {};
        } catch (error) {
            console.error(error);
            return {};
        }
    };

    const saveSettings = async settings => {
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

    const saveApiKey = async apiKey => {
        try {
            const data = await $fetch(buildDeviceUrl(baseUrl, '/apiKey', { apiKey }));
            return data.status === 'OK';
        } catch (error) {
            console.error(error);
            throw new Error('An error occurred while saving the API key');
        }
    };

    const checkWiFiConnectionPolling = async () => {
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

    const connectToWiFi = async (ssid, password) => {
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

    return {
        connectToWiFi,
        getNetworks,
        getSettings,
        saveApiKey,
        saveSettings
    };
};