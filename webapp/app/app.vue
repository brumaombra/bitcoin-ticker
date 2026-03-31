<script setup>
import { onMounted } from 'vue';
import { getConfig, getNetworks } from '~/composables/useDeviceApi.js';
import { closeConfirmDialog, closeMessage, handleBackendErrors, initializeCryptoCoin, initializeTheme, setBusy, setCryptoCoin } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import { Busy } from '~/components/ui/busy';
import ConfirmDialog from '~/components/ConfirmDialog.vue';
import MessageDialog from '~/components/MessageDialog.vue';

const globalStore = useGlobalStore();
const { t } = useI18n();

// Load the initial device data once for the app shell
const loadInitialData = async () => {
    // Exit if already loaded
    if (globalStore.value.configLoaded && globalStore.value.networksList?.length > 0) {
        setCryptoCoin(globalStore.value.config.cryptoCoin || 'bitcoin');
        return;
    }

    try {
        setBusy(true); // Busy on

        // Load the networks if needed
        if (!globalStore.value.networksList?.length) {
            const networksData = await getNetworks();
            globalStore.value.networksList = networksData.networks;
            globalStore.value.networksCount = networksData.count;
        }

        // Load the config if needed
        if (!globalStore.value.configLoaded) {
            // Get the full device config
            const config = await getConfig();

            // Merge with the existing config
            globalStore.value.config = {
                ...globalStore.value.config,
                ...config
            };

            // Set the crypto coin in the global store
            setCryptoCoin(globalStore.value.config.cryptoCoin || 'bitcoin');
            globalStore.value.configLoaded = true;
        }
    } catch (error) {
        handleBackendErrors({ error, defaultMessage: t('app.loadError'), showDialog: true });
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
        <Busy :show="globalStore.busy" :label="t('common.syncingDevice')" />
    </div>
</template>
