<script setup>
import { onMounted } from 'vue';

// Redirect the section root to the WiFi page
onMounted(async () => {
    await navigateTo('/app/wifi', { replace: true });
});

// Page metadata
definePageMeta({
    layout: 'private'
});
</script>

<template>
    <div class="flex min-h-[50vh] items-center justify-center text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Redirecting to WiFi setup...</div>
</template>