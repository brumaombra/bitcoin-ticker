<script setup>
import { Label } from '~/components/shadcn/label';
import { Switch } from '~/components/shadcn/switch';

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
        <div class="flex items-start justify-between gap-4">
            <!-- Label -->
            <Label :for="props.id" class="font-bold">
                {{ props.label }}
            </Label>

            <!-- Toggle switch -->
            <Switch :id="props.id"
                :model-value="props.modelValue"
                @update:modelValue="emits('update:modelValue', $event)" />
        </div>

        <!-- Description -->
        <p class="mt-2 text-xs leading-5 text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
            {{ props.description }}
        </p>
    </div>
</template>