<script setup>
import { onMounted } from 'vue';
import { getNetworks, getSettings } from '~/composables/useDeviceApi.js';
import { closeConfirmDialog, closeMessage, initializeCryptoCoin, initializeTheme, setBusy, setCryptoCoin, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import Busy from '~/components/Busy.vue';
import ConfirmDialog from '~/components/ConfirmDialog.vue';
import MessageDialog from '~/components/MessageDialog.vue';

const globalStore = useGlobalStore();
const { t } = useI18n();

// Load the initial device data once for the app shell
const loadInitialData = async () => {
    // Exit if already loaded
    if (globalStore.value.settingsLoaded && globalStore.value.networksList.length > 0) {
        setCryptoCoin(globalStore.value.settings.cryptoCoin || 'bitcoin');
        return;
    }

    try {
        setBusy(true); // Busy on

        // Load the networks if needed
        if (!globalStore.value.networksList.length) {
            globalStore.value.networksList = await getNetworks();
        }

        // Load the settings if needed
        if (!globalStore.value.settingsLoaded) {
            // Get the settings
            const settings = await getSettings();

            // Merge with the existing settings
            globalStore.value.settings = {
                ...globalStore.value.settings,
                ...settings
            };

            // Set the crypto coin in the global store
            setCryptoCoin(globalStore.value.settings.cryptoCoin || 'bitcoin');
            globalStore.value.settingsLoaded = true;
        }
    } catch (error) {
        console.error(error);
        showMessage({ type: 'Error', title: t('dialogs.errorTitle'), message: t('app.loadError') });
    } finally {
        setBusy(false); // Busy off
    }
};

// Confirm the active global dialog action
const handleConfirmDialogConfirm = () => {
    const onConfirm = globalStore.value.confirmDialog.onConfirm;
    closeConfirmDialog();
    if (typeof onConfirm === 'function') onConfirm(); // Execute the confirm callback if it exists
};

// Cancel the active global dialog action
const handleConfirmDialogCancel = () => {
    const onCancel = globalStore.value.confirmDialog.onCancel;
    closeConfirmDialog();
    if (typeof onCancel === 'function') onCancel(); // Execute the cancel callback if it exists
};

// Initialize the theme and preload device data once the app mounts
onMounted(async () => {
    initializeTheme();
    initializeCryptoCoin();
    await loadInitialData();
});
</script>

<template>
    <div>
        <!-- Route content -->
        <NuxtLayout>
            <NuxtPage />
        </NuxtLayout>

        <!-- Global message dialog -->
        <MessageDialog :show="globalStore.messageDialog.visible"
            :type="globalStore.messageDialog.type"
            :title="globalStore.messageDialog.title"
            :message="globalStore.messageDialog.message"
            @close="closeMessage" />

        <!-- Global confirm dialog -->
        <ConfirmDialog :show="globalStore.confirmDialog.visible"
            :title="globalStore.confirmDialog.title"
            :message="globalStore.confirmDialog.message"
            :confirm-text="globalStore.confirmDialog.confirmButton.text"
            :cancel-text="globalStore.confirmDialog.cancelButton.text"
            :confirm-button-type="globalStore.confirmDialog.confirmButton.type"
            :cancel-button-type="globalStore.confirmDialog.cancelButton.type"
            :icon="globalStore.confirmDialog.icon || undefined"
            @confirm="handleConfirmDialogConfirm"
            @cancel="handleConfirmDialogCancel"
            @close="closeConfirmDialog"
            @update:show="value => { if (!value) closeConfirmDialog(); }" />

        <!-- Global busy overlay -->
        <Busy :show="globalStore.busy" />
    </div>
</template>