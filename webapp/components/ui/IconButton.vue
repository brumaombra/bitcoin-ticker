<script setup>
// Props
const props = defineProps({
    ariaLabel: { type: String, default: '' },
    disabled: { type: Boolean, default: false }
});

// Emits
const emits = defineEmits(['click']);

// Emit clicks only when the control is enabled
const handleClick = event => {
    if (props.disabled) {
        return;
    }

    emits('click', event);
};
</script>

<template>
    <!-- Icon button -->
    <button type="button"
        :aria-label="props.ariaLabel"
        :disabled="props.disabled"
        class="relative flex h-12 w-12 items-center justify-center rounded border border-[var(--border-light)] bg-[var(--bg-card-light)] text-[var(--text-primary-light)] transition duration-200 ease-in-out hover:border-[var(--border-hover-light)] hover:bg-[var(--bg-selected-light)] disabled:cursor-not-allowed disabled:opacity-50 dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)] dark:text-[var(--text-primary-dark)] dark:hover:border-[var(--border-hover-dark)] dark:hover:bg-[var(--bg-selected-dark)]"
        @click="handleClick">
        <slot></slot>
    </button>
</template>