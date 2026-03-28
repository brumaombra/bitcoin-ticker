// Shared device and UI state for the webapp
export const useGlobalStore = () => useState('global', () => ({
    // Busy overlay state
    busy: false,

    // Shared UI theme mode
    themeMode: 'system',

    // Cached WiFi scan results
    networksList: [],
    networksCount: 0,
    currentNetworkSsid: '',

    // Cached device settings payload
    settings: {
        currentPrice: false,
        priceChange: false,
        marketCap: false,
        dailyHighLow: false,
        yearHighLow: false,
        openPrice: false,
        volume: false,
        cryptoCoin: 'bitcoin',
        formatType: 'US',
        matrixIntensity: 0,
        scrollSpeed: 0
    },

    // Flag to track whether the settings have been fetched at least once
    settingsLoaded: false,

    // Shared message dialog state
    messageDialog: {
        visible: false,
        type: 'Info',
        title: '',
        message: ''
    },

    // Shared confirm dialog state
    confirmDialog: {
        visible: false,
        title: '',
        message: '',
        icon: null,
        onConfirm: null,
        onCancel: null,
        confirmButton: {
            type: 'primary',
            text: ''
        },
        cancelButton: {
            type: 'secondary',
            text: ''
        }
    }
}));