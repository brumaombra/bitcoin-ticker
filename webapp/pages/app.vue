<script setup>
import Navbar from '~/components/Navbar.vue';
import { useAppUi } from '~/composables/useAppUi.js';
import { useDeviceApi } from '~/composables/useDeviceApi.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

const globalStore = useGlobalStore();
const { getNetworks, getSettings } = useDeviceApi();
const { setBusy, showMessage } = useAppUi();

const ensureDeviceData = async () => {
    if (globalStore.value.settings && globalStore.value.networksList.length > 0) {
        return;
    }

    try {
        setBusy(true);

        if (!globalStore.value.networksList.length) {
            globalStore.value.networksList = await getNetworks();
        }

        if (!globalStore.value.settings) {
            globalStore.value.settings = await getSettings();
        }
    } catch (error) {
        console.error(error);
        showMessage('Error', 'Error', 'An error occurred while loading the data');
    } finally {
        setBusy(false);
    }
};

onMounted(async () => {
    await ensureDeviceData();
});
</script>

<template>
    <div class="flex h-screen flex-col overflow-hidden bg-gray-100">
        <Navbar />

        <div class="mt-16 flex-1 overflow-y-auto px-5 py-5 md:px-0 md:pt-12">
            <NuxtPage />
        </div>
    </div>
</template>