<script setup>
import Label from '~/components/ui/Label.vue';
import ToggleSwitch from '~/components/ui/ToggleSwitch.vue';

// Props
const props = defineProps({
    id: { type: String, required: true },
    label: { type: String, required: true },
    description: { type: String, required: true },
    modelValue: { type: Boolean, required: true }
});

// Emits
const emits = defineEmits(['update:modelValue']);
</script>

<template>
    <div class="rounded border border-[var(--border-light)] px-4 py-3 dark:border-[var(--border-dark)]">
        <div class="flex items-center justify-between gap-4">
            <!-- Label -->
            <Label :for="props.id">{{ props.label }}</Label>

            <!-- Toggle switch -->
            <ToggleSwitch :id="props.id"
                :model-value="props.modelValue"
                @update:modelValue="emits('update:modelValue', $event)" />
        </div>

        <!-- Description -->
        <p class="mt-2 text-xs leading-5 text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
            {{ props.description }}
        </p>
    </div>
</template>