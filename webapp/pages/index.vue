<script setup>
import Card from '~/components/ui/Card.vue';
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
    <div class="flex min-h-screen items-center justify-center px-4">
        <Card class="w-full max-w-md text-center">
            <img src="/svg/bitcoin.svg" alt="Bitcoin logo" class="mx-auto mb-5 h-20 w-20 animate-spin">
            <div class="text-xs font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Initializing</div>
            <h1 class="mt-2 text-3xl font-bold">Preparing the device panel</h1>
            <p class="mt-3 text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Fetching the current WiFi list and saved device settings.</p>
        </Card>
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