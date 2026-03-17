// Shared device and UI state for the webapp
export const useGlobalStore = () => useState('global', () => ({
    // Busy overlay state
    busy: false,

    // Cached WiFi scan results
    networksList: [],

    // Cached device settings payload
    settings: {
        currentPrice: false,
        priceChange: false,
        marketCap: false,
        dailyHighLow: false,
        yearHighLow: false,
        openPrice: false,
        volume: false,
        formatType: 'US',
        matrixIntensity: 0,
        scrollSpeed: 0
    },

    // Flag to track whether the settings have been fetched at least once
    settingsLoaded: false,

    // Shared modal message state
    messageModal: {
        visible: false,
        type: 'Info',
        title: 'Message',
        message: ''
    }
}));