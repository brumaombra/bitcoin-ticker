<script setup>
// Props
const props = defineProps({
    for: { type: String, default: '' }
});
</script>

<template>
    <label :for="props.for" class="block text-sm font-medium text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">
        <slot />
    </label>
</template>