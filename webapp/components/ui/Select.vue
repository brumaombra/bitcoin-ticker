<script setup>
import { computed, nextTick, onBeforeUnmount, ref } from 'vue';

const props = defineProps({
    modelValue: { type: [String, Number], default: '' },
    id: { type: String, default: '' },
    disabled: { type: Boolean, default: false },
    placeholder: { type: String, default: '' },
    optionList: { type: Array, default: () => [] }
});

const emits = defineEmits(['update:modelValue', 'change']);

const isOpen = ref(false);
const selectRef = ref(null);
const selectButtonRef = ref(null);
const dropdownPosition = ref({ top: 0, left: 0, width: 0 });

const options = computed(() => {
    return props.optionList.map(option => {
        const isObject = typeof option === 'object' && option !== null;
        return {
            value: isObject ? option.value : option,
            label: isObject ? option.label : option,
            disabled: isObject ? option.disabled || false : false,
            meta: isObject ? option.meta || '' : ''
        };
    });
});

const selectedOption = computed(() => {
    return options.value.find(option => option.value === props.modelValue) || null;
});

const calculateDropdownPosition = () => {
    if (!selectButtonRef.value) {
        return;
    }

    const buttonRect = selectButtonRef.value.getBoundingClientRect();
    const viewportHeight = window.innerHeight;
    const dropdownHeight = Math.min(280, Math.max(1, options.value.length) * 64);

    let top = buttonRect.bottom + 4;
    if (top + dropdownHeight > viewportHeight) {
        top = buttonRect.top - dropdownHeight - 4;
    }

    dropdownPosition.value = {
        top,
        left: buttonRect.left,
        width: buttonRect.width
    };
};

const openDropdown = () => {
    if (props.disabled || isOpen.value) {
        return;
    }

    isOpen.value = true;

    nextTick(() => {
        calculateDropdownPosition();
        document.addEventListener('click', handleClickOutside);
        window.addEventListener('scroll', calculateDropdownPosition, { passive: true });
        window.addEventListener('resize', calculateDropdownPosition, { passive: true });
    });
};

const closeDropdown = () => {
    isOpen.value = false;
    document.removeEventListener('click', handleClickOutside);
    window.removeEventListener('scroll', calculateDropdownPosition);
    window.removeEventListener('resize', calculateDropdownPosition);
};

const toggleDropdown = () => {
    if (isOpen.value) {
        closeDropdown();
        return;
    }

    openDropdown();
};

const handleClickOutside = event => {
    if (selectRef.value && !selectRef.value.contains(event.target)) {
        closeDropdown();
    }
};

const selectOption = option => {
    if (option.disabled) {
        return;
    }

    emits('update:modelValue', option.value);
    emits('change', option.value);
    closeDropdown();
};

const getDropdownStyles = computed(() => {
    return {
        top: `${dropdownPosition.value.top}px`,
        left: `${dropdownPosition.value.left}px`,
        width: `${dropdownPosition.value.width}px`,
        zIndex: 9999
    };
});

onBeforeUnmount(() => {
    closeDropdown();
});
</script>

<template>
    <div ref="selectRef" class="relative">
        <select :id="props.id" :value="props.modelValue" class="hidden" aria-hidden="true">
            <option v-for="option in options" :key="option.value" :value="option.value">
                {{ option.label }}
            </option>
        </select>

        <button ref="selectButtonRef"
            type="button"
            :disabled="props.disabled"
            class="flex h-[52px] w-full items-center justify-between rounded border border-[var(--border-light)] bg-[var(--button-secondary-light)] px-4 py-3 text-left text-sm font-medium text-[var(--text-primary-light)] transition-all duration-300 ease-in-out focus:border-[var(--border-hover-light)] focus:outline-none disabled:cursor-not-allowed disabled:opacity-60 dark:border-[var(--border-dark)] dark:bg-[var(--button-secondary-dark)] dark:text-[var(--text-primary-dark)] dark:focus:border-[var(--border-hover-dark)]"
            :class="{ 'border-[var(--border-hover-light)] dark:border-[var(--border-hover-dark)]': isOpen }"
            @click="toggleDropdown">
            <span :class="selectedOption ? '' : 'text-[var(--text-secondary-light)] opacity-70 dark:text-[var(--text-secondary-dark)]'">
                {{ selectedOption?.label || props.placeholder }}
            </span>

            <svg class="h-4 w-4 text-[var(--text-secondary-light)] transition-transform duration-300 dark:text-[var(--text-secondary-dark)]" :class="{ 'rotate-180': isOpen }" viewBox="0 0 20 20" fill="currentColor" aria-hidden="true">
                <path fill-rule="evenodd" d="M5.23 7.21a.75.75 0 011.06.02L10 11.168l3.71-3.938a.75.75 0 111.08 1.04l-4.25 4.5a.75.75 0 01-1.08 0l-4.25-4.5a.75.75 0 01.02-1.06Z" clip-rule="evenodd" />
            </svg>
        </button>

        <Teleport to="body">
            <Transition enter-active-class="transition ease-out duration-200" enter-from-class="opacity-0 translate-y-1" enter-to-class="opacity-100 translate-y-0" leave-active-class="transition ease-in duration-150" leave-from-class="opacity-100 translate-y-0" leave-to-class="opacity-0 translate-y-1">
                <div v-show="isOpen"
                    class="fixed max-h-70 overflow-y-auto rounded border border-[var(--border-light)] bg-[var(--bg-card-light)] shadow-xl dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)]"
                    :style="getDropdownStyles">
                    <button v-for="option in options"
                        :key="option.value"
                        type="button"
                        class="flex w-full items-start justify-between gap-3 px-4 py-3 text-left transition-colors duration-200"
                        :class="option.value === props.modelValue
                            ? 'bg-[var(--bg-selected-light)] text-[var(--text-primary-light)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-primary-dark)]'
                            : 'text-[var(--text-primary-light)] hover:bg-[var(--bg-selected-light)] dark:text-[var(--text-primary-dark)] dark:hover:bg-[var(--bg-selected-dark)]'"
                        @click="selectOption(option)">
                        <div>
                            <div class="text-sm font-medium">{{ option.label }}</div>
                            <div v-if="option.meta" class="mt-1 text-xs text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ option.meta }}</div>
                        </div>

                        <svg v-if="option.value === props.modelValue" class="mt-0.5 h-4 w-4 shrink-0 text-[var(--button-primary-light)] dark:text-[var(--text-primary-dark)]" viewBox="0 0 20 20" fill="currentColor" aria-hidden="true">
                            <path fill-rule="evenodd" d="M16.704 5.29a1 1 0 010 1.42l-7.2 7.2a1 1 0 01-1.415 0l-3-3a1 1 0 111.414-1.42l2.293 2.294 6.493-6.494a1 1 0 011.415 0Z" clip-rule="evenodd" />
                        </svg>
                    </button>
                </div>
            </Transition>
        </Teleport>
    </div>
</template>