<script setup>
import { computed } from 'vue';
import { KeyRound, Settings, Wifi } from 'lucide-vue-next';
import AppShell from '~/components/ui/AppShell.vue';

const { t } = useI18n();

// Navigation items for the private control panel
const sidebarItems = computed(() => ([{
    id: 'wifi',
    path: '/wifi',
    label: t('nav.wifi.label'),
    description: t('nav.wifi.description'),
    icon: Wifi
}, {
    id: 'settings',
    path: '/settings',
    label: t('nav.settings.label'),
    description: t('nav.settings.description'),
    icon: Settings
}, {
    id: 'api-key',
    path: '/api-key',
    label: t('nav.apiKey.label'),
    description: t('nav.apiKey.description'),
    icon: KeyRound
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