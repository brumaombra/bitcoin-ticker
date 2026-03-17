<script setup>
import { computed, useSlots } from 'vue';

const props = defineProps({
    modelValue: { type: [String, Number], default: '' },
    id: { type: String, default: '' },
    type: { type: String, default: 'text' },
    placeholder: { type: String, default: '' },
    disabled: { type: Boolean, default: false },
    maxlength: { type: [String, Number], default: null }
});

const emits = defineEmits(['update:modelValue']);
const slots = useSlots();

const inputClasses = computed(() => {
    const classes = ['h-[52px] w-full rounded border border-[var(--border-light)] bg-[var(--button-secondary-light)] px-4 py-3 text-sm font-medium text-[var(--text-primary-light)] outline-none transition-all duration-300 ease-in-out placeholder:text-[var(--text-secondary-light)] placeholder:opacity-60 focus:border-[var(--border-hover-light)] dark:border-[var(--border-dark)] dark:bg-[var(--button-secondary-dark)] dark:text-[var(--text-primary-dark)] dark:placeholder:text-[var(--text-secondary-dark)] dark:focus:border-[var(--border-hover-dark)] disabled:cursor-not-allowed disabled:opacity-60'];

    if (slots.suffix) {
        classes.push('pr-12');
    }

    return classes;
});

const updateValue = event => {
    emits('update:modelValue', event.target.value);
};
</script>

<template>
    <div class="relative w-full">
        <input :id="props.id"
            :type="props.type"
            :value="props.modelValue"
            :placeholder="props.placeholder"
            :disabled="props.disabled"
            :maxlength="props.maxlength"
            :class="inputClasses"
            @input="updateValue">

        <div v-if="slots.suffix" class="pointer-events-none absolute inset-y-0 right-4 flex items-center text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
            <slot name="suffix"></slot>
        </div>
    </div>
</template>