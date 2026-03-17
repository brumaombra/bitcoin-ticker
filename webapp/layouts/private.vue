<script setup>
import AppShell from '~/components/ui/AppShell.vue';

// Navigation items for the private control panel
const sidebarItems = [{
    id: 'wifi',
    path: '/wifi',
    label: 'WiFi',
    description: 'Scan networks and connect the device',
    iconSrc: '/svg/wifi.svg'
}, {
    id: 'settings',
    path: '/settings',
    label: 'Settings',
    description: 'Adjust ticker visibility and behavior',
    iconSrc: '/svg/settings.svg'
}, {
    id: 'api-key',
    path: '/api-key',
    label: 'API Key',
    description: 'Provide the market data credential',
    iconSrc: '/svg/key.svg'
}];
</script>

<template>
    <AppShell brand-name="Bitcoin Ticker" :sidebar-items="sidebarItems">
        <!-- Routed page content -->
        <div class="pb-10">
            <slot />
        </div>
    </AppShell>
</template>
