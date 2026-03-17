<script setup>
import AppShell from '~/components/ui/AppShell.vue';
import { useAppUi } from '~/composables/useAppUi.js';
import { useDeviceApi } from '~/composables/useDeviceApi.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

const globalStore = useGlobalStore();
const { getNetworks, getSettings } = useDeviceApi();
const { setBusy, showMessage } = useAppUi();

const sidebarItems = [
    {
        id: 'wifi',
        path: '/app/wifi',
        label: 'WiFi',
        description: 'Scan networks and connect the device',
        iconSrc: '/svg/wifi.svg'
    },
    {
        id: 'settings',
        path: '/app/settings',
        label: 'Settings',
        description: 'Adjust ticker visibility and behavior',
        iconSrc: '/svg/settings.svg'
    },
    {
        id: 'api-key',
        path: '/app/apiKey',
        label: 'API Key',
        description: 'Provide the market data credential',
        iconSrc: '/svg/key.svg'
    }
];

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
    <AppShell brand-name="Bitcoin Ticker" :sidebar-items="sidebarItems">
        <div class="pb-10">
            <slot />
        </div>
    </AppShell>
</template>