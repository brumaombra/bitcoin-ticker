<script setup>
import { onMounted } from 'vue';

// Redirect the root page to the WiFi screen
onMounted(() => {
    navigateTo('/wifi', { replace: true });
});

// Page metadata
definePageMeta({
    layout: 'private'
});
</script>

<template>
    <div class="flex min-h-[50vh] items-center justify-center text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Redirecting to WiFi setup...</div>
</template>