<script setup>
// Props
defineProps({
    modelValue: { type: Boolean, required: true }
});

// Emits
defineEmits(['update:modelValue']);
</script>

<template>
    <div class="flex items-center">
        <label class="relative inline-flex cursor-pointer items-center">
            <input type="checkbox" class="peer sr-only" :checked="modelValue" @change="$emit('update:modelValue', $event.target.checked)">

            <div class="h-7 w-12 rounded-full border border-[var(--border-light)] bg-[var(--bg-selected-light)] transition duration-200 peer-checked:border-[var(--button-primary-light)] peer-checked:bg-[var(--button-primary-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)] dark:peer-checked:border-[var(--button-primary-dark)] dark:peer-checked:bg-[var(--button-primary-dark)]"></div>

            <div class="absolute left-1 top-1 h-5 w-5 rounded-full bg-white shadow transition-transform duration-200 peer-checked:translate-x-5 dark:bg-[var(--bg-card-dark)]"></div>
        </label>
    </div>
</template>