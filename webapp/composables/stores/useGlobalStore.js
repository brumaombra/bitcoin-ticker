export const useGlobalStore = () => useState('global', () => ({
    busy: false,
    networksList: [],
    settings: null,
    messageModal: {
        visible: false,
        type: 'Info',
        title: 'Message',
        message: ''
    }
}));