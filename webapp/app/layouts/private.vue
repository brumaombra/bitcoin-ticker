<script setup>
import { computed } from 'vue';
import { Key01Icon, Settings01Icon, Wifi01Icon } from '@hugeicons/core-free-icons';
import AppShell from '~/components/ui/AppShell.vue';

const { t } = useI18n();

// Navigation items for the private control panel
const sidebarItems = computed(() => ([{
    id: 'wifi',
    path: '/wifi',
    label: t('nav.wifi.label'),
    description: t('nav.wifi.description'),
    icon: Wifi01Icon
}, {
    id: 'settings',
    path: '/settings',
    label: t('nav.settings.label'),
    description: t('nav.settings.description'),
    icon: Settings01Icon
}, {
    id: 'api-key',
    path: '/api-key',
    label: t('nav.apiKey.label'),
    description: t('nav.apiKey.description'),
    icon: Key01Icon
}]));
</script>

<template>
    <AppShell :brand-name="t('app.title')" :sidebar-items="sidebarItems">
        <!-- Routed page content -->
        <div class="pb-10">
            <slot />
        </div>
    </AppShell>
</template>