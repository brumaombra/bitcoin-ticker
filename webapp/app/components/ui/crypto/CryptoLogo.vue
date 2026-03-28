<script setup>
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import BitcoinLogo from '~/components/ui/crypto/BitcoinLogo.vue';
import KaspaLogo from '~/components/ui/crypto/KaspaLogo.vue';

const globalStore = useGlobalStore();
</script>

<template>
    <KaspaLogo v-if="globalStore.settings.cryptoCoin === 'kaspa'" v-bind="$attrs" />
    <BitcoinLogo v-else v-bind="$attrs" />
</template>