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

    // Shared message dialog state
    messageDialog: {
        visible: false,
        type: 'Info',
        title: 'Message',
        message: ''
    },

    // Shared confirm dialog state
    confirmDialog: {
        visible: false,
        title: 'Confirm action',
        message: '',
        icon: null,
        onConfirm: null,
        onCancel: null,
        confirmButton: {
            type: 'primary',
            text: 'Confirm'
        },
        cancelButton: {
            type: 'secondary',
            text: 'Cancel'
        }
    }
}));