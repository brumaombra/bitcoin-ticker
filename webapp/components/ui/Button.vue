<script setup>
// Props
const props = defineProps({
    type: { type: String, default: 'secondary', validator: value => ['primary', 'secondary', 'ghost'].includes(value) },
    nativeType: { type: String, default: 'button', validator: value => ['button', 'submit', 'reset'].includes(value) },
    disabled: { type: Boolean, default: false },
    text: { type: String, default: '' }
});

// Emits
const emits = defineEmits(['click']);

// Resolve the button style variant
const getButtonClasses = () => {
    const classes = ['inline-flex min-h-12 items-center justify-center gap-2 rounded px-4 py-3 text-xs sm:text-sm font-medium transition-all duration-300 ease-in-out'];

    // Style variants
    if (props.type === 'primary') {
        classes.push('bg-[var(--button-primary-light)] text-white hover:bg-[var(--button-primary-hover-light)] dark:bg-[var(--button-primary-dark)] dark:text-[var(--text-primary-dark)] dark:hover:bg-[var(--button-primary-hover-dark)]');
    } else if (props.type === 'ghost') {
        classes.push('text-[var(--text-secondary-light)] hover:bg-[var(--bg-selected-light)] hover:text-[var(--text-primary-light)] dark:text-[var(--text-secondary-dark)] dark:hover:bg-[var(--bg-selected-dark)] dark:hover:text-[var(--text-primary-dark)]');
    } else {
        classes.push('border border-[var(--border-light)] bg-[var(--button-secondary-light)] text-[var(--text-primary-light)] hover:border-[var(--border-hover-light)] hover:bg-[var(--bg-selected-light)] dark:border-[var(--border-dark)] dark:bg-[var(--button-secondary-dark)] dark:text-[var(--text-primary-dark)] dark:hover:border-[var(--border-hover-dark)] dark:hover:bg-[var(--bg-selected-dark)]');
    }

    // Disabled state
    if (props.disabled) {
        classes.push('cursor-not-allowed opacity-50');
    } else {
        classes.push('cursor-pointer active:scale-[0.98]');
    }

    // Return the final class list
    return classes;
};

// Emit click events only when the button is enabled
const handleClick = event => {
    if (props.disabled) return;
    emits('click', event);
};
</script>

<template>
    <!-- Button content -->
    <button :type="props.nativeType" :disabled="props.disabled" :class="getButtonClasses()" v-bind="$attrs" @click="handleClick">
        <slot>
            {{ props.text }}
        </slot>
    </button>
</template>