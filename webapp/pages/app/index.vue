<script setup>
definePageMeta({
    layout: 'private'
});

onMounted(async () => {
    await navigateTo('/app/wifi', { replace: true });
});
</script>

<template>
    <div class="flex min-h-[50vh] items-center justify-center text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Redirecting to WiFi setup...</div>
</template>