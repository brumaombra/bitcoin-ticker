<script setup>
import { Key01Icon, Settings01Icon, Wifi01Icon } from '@hugeicons/core-free-icons';
import AppShell from '~/components/ui/AppShell.vue';

// Navigation items for the private control panel
const sidebarItems = [{
    id: 'wifi',
    path: '/wifi',
    label: 'WiFi',
    description: 'Scan networks and connect the device',
    icon: Wifi01Icon
}, {
    id: 'settings',
    path: '/settings',
    label: 'Settings',
    description: 'Adjust ticker visibility and behavior',
    icon: Settings01Icon
}, {
    id: 'api-key',
    path: '/api-key',
    label: 'API Key',
    description: 'Provide the market data credential',
    icon: Key01Icon
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