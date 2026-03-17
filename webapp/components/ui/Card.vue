<script setup>
// Props
const props = defineProps({
    theme: { type: String, default: 'default', validator: value => ['default', 'muted'].includes(value) }
});
</script>

<template>
    <!-- Card surface -->
    <div class="rounded border p-6 transition-colors duration-300 ease-in-out"
        :class="props.theme === 'muted'
            ? 'border-[var(--border-light)] bg-[var(--bg-selected-light)] text-[var(--text-secondary-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-secondary-dark)]'
            : 'border-[var(--border-light)] bg-[var(--bg-card-light)] text-[var(--text-primary-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)] dark:text-[var(--text-primary-dark)]'">
        <slot></slot>
    </div>
</template>