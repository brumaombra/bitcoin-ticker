<script setup>
import CustomIcon from '~/components/ui/CustomIcon.vue';

// Props
const props = defineProps({
    title: { type: String, default: '' },
    options: { type: Array, required: true },
    current: { type: [String, Number], default: '' }
});

// Emits
const emits = defineEmits(['select']);

// Check if an option is the current selection
const isCurrentOption = option => {
    return props.current === option.key;
};

// Resolve the style classes for each option
const getOptionClasses = option => {
    if (isCurrentOption(option)) {
        return 'bg-[var(--bg-selected-light)] text-[var(--text-primary-light)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-primary-dark)]';
    }

    return 'text-[var(--text-secondary-light)] hover:bg-[var(--bg-selected-light)] hover:text-[var(--text-primary-light)] dark:text-[var(--text-secondary-dark)] dark:hover:bg-[var(--bg-selected-dark)] dark:hover:text-[var(--text-primary-dark)]';
};

// Emit the selected option key
const handleSelect = optionKey => {
    emits('select', optionKey);
};
</script>

<template>
    <div>
        <!-- Menu title -->
        <div v-if="props.title" class="border-b border-[var(--border-light)] px-3 py-2 text-xs font-semibold uppercase tracking-[0.16em] text-[var(--text-secondary-light)] dark:border-[var(--border-dark)] dark:text-[var(--text-secondary-dark)]">
            {{ props.title }}
        </div>

        <!-- Menu options -->
        <div class="p-2">
            <button v-for="option in props.options"
                :key="option.key"
                type="button"
                class="flex w-full items-center gap-3 rounded px-3 py-2 text-left text-sm font-medium transition-colors duration-200"
                :class="getOptionClasses(option)"
                :aria-label="option.label"
                @click="handleSelect(option.key)">
                <!-- Leading slot -->
                <span v-if="$slots['option-leading']" class="flex h-5 w-5 shrink-0 items-center justify-center">
                    <slot name="option-leading" :option="option" :is-current="isCurrentOption(option)"></slot>
                </span>

                <!-- Option label -->
                <span class="flex-1">{{ option.label }}</span>

                <!-- Selected indicator -->
                <CustomIcon v-if="isCurrentOption(option)" icon="check" class="h-4 w-4 shrink-0 text-[var(--button-primary-light)] dark:text-[var(--text-primary-dark)]" />
            </button>
        </div>
    </div>
</template>