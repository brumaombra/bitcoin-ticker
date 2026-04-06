<script setup>
import { onMounted } from 'vue';
import { Busy } from 'theme-vintage/busy';
import { ConfirmDialog } from 'theme-vintage/confirm-dialog';
import { MessageDialog } from 'theme-vintage/message-dialog';
import { getConfig, getNetworks } from '~/composables/useDeviceApi.js';
import { handleBackendErrors, initializeCryptoCoin, initializeTheme, setBusy, setCryptoCoin } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

const globalStore = useGlobalStore();
const { t } = useI18n();

// Load the initial device data once for the app shell
const loadInitialData = async () => {
    try {
        setBusy(true); // Busy on

        // Load the networks if needed
        if (!globalStore.value.networksList?.length) {
            const networksData = await getNetworks();
            globalStore.value.networksList = networksData.networks;
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

            // Mark config as loaded
            globalStore.value.configLoaded = true;
        }

        // Set the crypto coin in the global store
        setCryptoCoin(globalStore.value.config?.cryptoCoin || 'bitcoin');
    } catch (error) {
        handleBackendErrors({ error, defaultMessage: t('app.loadError'), showDialog: true });
    } finally {
        setBusy(false); // Busy off
    }
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

        <!-- Confirm dialog -->
        <ConfirmDialog />

        <!-- Message dialog -->
        <MessageDialog />

        <!-- Global busy overlay -->
        <Busy :show="globalStore.busy" :label="t('common.syncingDevice')" />
    </div>
</template>