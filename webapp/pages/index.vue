<script setup>
import { useAppUi } from '~/composables/useAppUi.js';
import { useDeviceApi } from '~/composables/useDeviceApi.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

const globalStore = useGlobalStore();
const { getNetworks, getSettings } = useDeviceApi();
const { showMessage } = useAppUi();

const initializeApp = async () => {
    globalStore.value.networksList = await getNetworks();
    globalStore.value.settings = await getSettings();
};

onMounted(() => {
    window.setTimeout(async () => {
        try {
            await initializeApp();
            await navigateTo('/app/wifi');
        } catch (error) {
            console.error(error);
            showMessage('Error', 'Error', 'An error occurred while loading the data');
        }
    }, 3000);
});
</script>

<template>
    <div class="flex min-h-screen items-center justify-center bg-gray-800">
        <div class="text-center">
            <img src="/bitcoin.svg" alt="Bitcoin logo" class="mx-auto mb-4 h-24 w-24 animate-spin">
            <h1 class="text-3xl font-bold text-white">Loading...</h1>
        </div>
    </div>
</template>

<style scoped>
@keyframes spin {
    0% {
        transform: rotate(0deg);
    }

    100% {
        transform: rotate(360deg);
    }
}

.animate-spin {
    animation: spin 1s linear infinite;
}
</style>