export const useAppUi = () => {
    const globalStore = useGlobalStore();

    const setBusy = busy => {
        if (globalStore.value.busy === busy) {
            return;
        }

        globalStore.value.busy = busy;
    };

    const showMessage = (type, title, message) => {
        globalStore.value.messageModal = {
            visible: true,
            type,
            title,
            message
        };
    };

    const closeMessage = () => {
        globalStore.value.messageModal.visible = false;
    };

    return {
        closeMessage,
        setBusy,
        showMessage
    };
};