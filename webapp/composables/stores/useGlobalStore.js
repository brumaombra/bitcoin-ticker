// Shared device and UI state for the webapp
export const useGlobalStore = () => useState('global', () => ({
    // Busy overlay state
    busy: false,

    // Cached WiFi scan results
    networksList: [],

    // Cached device settings payload
    settings: null,

    // Shared modal message state
    messageModal: {
        visible: false,
        type: 'Info',
        title: 'Message',
        message: ''
    }
}));