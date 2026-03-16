<script setup>
onMounted(async () => {
    await navigateTo('/app/wifi', { replace: true });
});
</script>

<template>
    <div class="flex min-h-[50vh] items-center justify-center text-gray-500">Redirecting...</div>
</template>