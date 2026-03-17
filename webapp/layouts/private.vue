<script setup>
import { onMounted } from 'vue';
import { getNetworks, getSettings } from '~/composables/useDeviceApi.js';
import { setBusy, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import AppShell from '~/components/ui/AppShell.vue';

const globalStore = useGlobalStore();

// Navigation items for the private control panel
const sidebarItems = [{
    id: 'wifi',
    path: '/app/wifi',
    label: 'WiFi',
    description: 'Scan networks and connect the device',
    iconSrc: '/svg/wifi.svg'
}, {
    id: 'settings',
    path: '/app/settings',
    label: 'Settings',
    description: 'Adjust ticker visibility and behavior',
    iconSrc: '/svg/settings.svg'
}, {
    id: 'api-key',
    path: '/app/apiKey',
    label: 'API Key',
    description: 'Provide the market data credential',
    iconSrc: '/svg/key.svg'
}];

// Load device data once for the private area
const ensureDeviceData = async () => {
    if (globalStore.value.settingsLoaded && globalStore.value.networksList.length > 0) {
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

// Prime the layout with the device data on first mount
onMounted(async () => {
    await ensureDeviceData();
});
</script>

<template>
    <AppShell brand-name="Bitcoin Ticker" :sidebar-items="sidebarItems">
        <!-- Routed page content -->
        <div class="pb-10">
            <slot />
        </div>
    </AppShell>
</template>