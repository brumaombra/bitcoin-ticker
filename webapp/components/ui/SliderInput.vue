<script setup>
// Props
const props = defineProps({
    id: { type: String, required: true },
    modelValue: { type: [String, Number], default: '' },
    min: { type: [String, Number], default: 0 },
    max: { type: [String, Number], default: 100 },
    step: { type: [String, Number], default: 1 }
});

// Emits
const emits = defineEmits(['update:modelValue']);

// Update the model value when the slider input changes
const updateValue = event => {
    emits('update:modelValue', event.target.value);
};
</script>

<template>
    <input :id="props.id"
        :value="props.modelValue"
        :min="props.min"
        :max="props.max"
        :step="props.step"
        type="range"
        class="h-2 w-full cursor-pointer appearance-none rounded-full bg-[var(--bg-selected-light)] accent-[var(--button-primary-light)] dark:bg-[var(--bg-selected-dark)] dark:accent-[var(--button-primary-dark)]"
        @input="updateValue">
</template>