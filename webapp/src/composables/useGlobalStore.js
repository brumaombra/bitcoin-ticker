const initialState = () => ({
    busy: false,
    networksList: [],
    settings: null,
    messageModal: {
        visible: false,
        type: 'Info',
        title: 'Message',
        message: ''
    }
});

export const useGlobalStore = () => useState('globalStore', initialState);

export const resetGlobalStore = () => {
    const globalStore = useGlobalStore();
    globalStore.value = initialState();
};