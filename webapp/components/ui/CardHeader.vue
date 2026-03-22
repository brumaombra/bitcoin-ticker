<script setup>
// Props
const props = defineProps({
    title: { type: String, required: true },
    description: { type: String, default: '' }
});
</script>

<template>
    <div class="mb-4 sm:mb-5">
        <!-- Title -->
        <h2 class="text-base font-bold sm:text-lg">{{ props.title }}</h2>

        <!-- Description -->
        <p v-if="props.description" class="mt-1 text-xs text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)] sm:text-sm">
            {{ props.description }}
        </p>
    </div>
</template>