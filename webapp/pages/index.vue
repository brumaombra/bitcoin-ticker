<script setup>
import Card from '~/components/ui/Card.vue';
import BackgroundGrid from '~/components/ui/BackgroundGrid.vue';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import { getNetworks, getSettings } from '~/composables/useDeviceApi.js';
import { showMessage } from '~/composables/useUtils.js';

// Shared device data cache
const globalStore = useGlobalStore();

// Load the initial device data before redirecting into the app
const initializeApp = async () => {
    globalStore.value.networksList = await getNetworks();
    globalStore.value.settings = await getSettings();
};

// Show a short splash screen while fetching initial data
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
    <div class="relative flex min-h-screen items-center justify-center overflow-hidden bg-[var(--bg-main-light)] px-4 text-[var(--text-primary-light)] dark:bg-[var(--bg-main-dark)] dark:text-[var(--text-primary-dark)]">
        <!-- Background grid -->
        <BackgroundGrid />

        <!-- Ambient overlay -->
        <div class="pointer-events-none absolute inset-0 bg-[radial-gradient(circle_at_top_left,rgba(247,147,26,0.14),transparent_30%),linear-gradient(180deg,transparent,rgba(0,0,0,0.02))] dark:bg-[radial-gradient(circle_at_top_left,rgba(247,147,26,0.16),transparent_28%),linear-gradient(180deg,transparent,rgba(255,255,255,0.02))]"></div>

        <!-- Splash card -->
        <Card class="relative z-10 w-full max-w-md text-center">
            <img src="/svg/bitcoin.svg" alt="Bitcoin logo" class="mx-auto mb-5 h-20 w-20 animate-spin">
            <div class="text-xs font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Initializing</div>
            <h1 class="mt-2 text-3xl font-bold">Preparing the device panel</h1>
            <p class="mt-3 text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Fetching the current WiFi list and saved device settings.</p>
        </Card>
    </div>
</template>

<style scoped>
/* Spinner animation */
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