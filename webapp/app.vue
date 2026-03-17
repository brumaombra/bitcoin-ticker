<script setup>
import { onMounted } from 'vue';
import { getNetworks, getSettings } from '~/composables/useDeviceApi.js';
import { closeMessage, initializeTheme, setBusy, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import Busy from '~/components/Busy.vue';
import MessageModal from '~/components/MessageModal.vue';

// Shared global UI state
const globalStore = useGlobalStore();

// Load the initial device data once for the app shell
const ensureDeviceData = async () => {
    if (globalStore.value.settingsLoaded && globalStore.value.networksList.length > 0) {
        return;
    }

    try {
        setBusy(true);

        if (!globalStore.value.networksList.length) {
            globalStore.value.networksList = await getNetworks();
        }

        if (!globalStore.value.settingsLoaded) {
            globalStore.value.settings = {
                ...globalStore.value.settings,
                ...await getSettings()
            };
            globalStore.value.settingsLoaded = true;
        }
    } catch (error) {
        console.error(error);
        showMessage('Error', 'Error', 'An error occurred while loading the data');
    } finally {
        setBusy(false);
    }
};

// Initialize the theme and preload device data once the app mounts
onMounted(async () => {
    initializeTheme();
    await ensureDeviceData();
});
</script>

<template>
    <div>
        <!-- Route content -->
        <NuxtLayout>
            <NuxtPage />
        </NuxtLayout>

        <!-- Global message modal -->
        <MessageModal :show="globalStore.messageModal.visible"
            :type="globalStore.messageModal.type"
            :title="globalStore.messageModal.title"
            :message="globalStore.messageModal.message"
            @close="closeMessage" />

        <!-- Global busy overlay -->
        <Busy :show="globalStore.busy" />
    </div>
</template>