<script setup>
// Props
const props = defineProps({
    modelValue: { type: Boolean, required: true },
    id: { type: String, default: '' }
});

// Emits
const emits = defineEmits(['update:modelValue']);
</script>

<template>
    <div class="flex items-center">
        <label class="relative inline-flex cursor-pointer items-center">
            <!-- Hidden checkbox -->
            <input :id="props.id" type="checkbox" class="peer sr-only" :checked="props.modelValue" @change="emits('update:modelValue', $event.target.checked)">

            <!-- Track -->
            <div class="h-7 w-12 rounded border border-[var(--border-light)] bg-[var(--button-secondary-light)] transition-all duration-200 peer-checked:border-[var(--button-primary-light)] peer-checked:bg-[var(--bg-selected-light)] dark:border-[var(--border-dark)] dark:bg-[var(--button-secondary-dark)] dark:peer-checked:border-[var(--button-primary-dark)] dark:peer-checked:bg-[var(--bg-selected-dark)]"></div>

            <!-- Thumb -->
            <div class="absolute left-[4px] top-1/2 h-4 w-4 -translate-y-1/2 rounded-[0.2rem] bg-[var(--text-secondary-light)] transition-all duration-200 peer-checked:translate-x-6 peer-checked:bg-[var(--button-primary-light)] dark:bg-[var(--text-secondary-dark)] dark:peer-checked:bg-[var(--button-primary-dark)]"></div>
        </label>
    </div>
</template>