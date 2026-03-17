<script setup>
import { computed, nextTick, onBeforeUnmount, ref } from 'vue';

import CustomIcon from '~/components/ui/CustomIcon.vue';

// Props
const props = defineProps({
    modelValue: { type: [String, Number], default: '' },
    id: { type: String, default: '' },
    disabled: { type: Boolean, default: false },
    placeholder: { type: String, default: '' },
    optionList: { type: Array, default: () => [] }
});

// Emits
const emits = defineEmits(['update:modelValue', 'change']);

// Select state
const isOpen = ref(false);
const selectRef = ref(null);
const selectButtonRef = ref(null);
const dropdownPosition = ref({ top: 0, left: 0, width: 0 });

// Normalize the provided option list
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

// Resolve the selected option object
const selectedOption = computed(() => {
    return options.value.find(option => option.value === props.modelValue) || null;
});

// Position the floating dropdown relative to the trigger
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

// Open the dropdown and register positioning listeners
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

// Close the dropdown and remove listeners
const closeDropdown = () => {
    isOpen.value = false;
    document.removeEventListener('click', handleClickOutside);
    window.removeEventListener('scroll', calculateDropdownPosition);
    window.removeEventListener('resize', calculateDropdownPosition);
};

// Toggle the dropdown visibility
const toggleDropdown = () => {
    if (isOpen.value) {
        closeDropdown();
        return;
    }

    openDropdown();
};

// Close the dropdown when clicking outside the control
const handleClickOutside = event => {
    if (selectRef.value && !selectRef.value.contains(event.target)) {
        closeDropdown();
    }
};

// Select an option and emit the updated value
const selectOption = option => {
    if (option.disabled) {
        return;
    }

    emits('update:modelValue', option.value);
    emits('change', option.value);
    closeDropdown();
};

// Resolve the trigger text color when nothing is selected
const getSelectedValueClasses = () => {
    if (selectedOption.value) {
        return '';
    }

    return 'text-[var(--text-secondary-light)] opacity-70 dark:text-[var(--text-secondary-dark)]';
};

// Resolve the option item classes
const getOptionClasses = option => {
    if (option.value === props.modelValue) {
        return 'bg-[var(--bg-selected-light)] text-[var(--text-primary-light)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-primary-dark)]';
    }

    return 'text-[var(--text-primary-light)] hover:bg-[var(--bg-selected-light)] dark:text-[var(--text-primary-dark)] dark:hover:bg-[var(--bg-selected-dark)]';
};

// Inline styles for the teleported dropdown panel
const getDropdownStyles = computed(() => {
    return {
        top: `${dropdownPosition.value.top}px`,
        left: `${dropdownPosition.value.left}px`,
        width: `${dropdownPosition.value.width}px`,
        zIndex: 9999
    };
});

// Clean up any pending listeners when the component unmounts
onBeforeUnmount(() => {
    closeDropdown();
});
</script>

<template>
    <div ref="selectRef" class="relative">
        <!-- Native select fallback -->
        <select :id="props.id" :value="props.modelValue" class="hidden" aria-hidden="true">
            <option v-for="option in options" :key="option.value" :value="option.value">
                {{ option.label }}
            </option>
        </select>

        <!-- Trigger button -->
        <button ref="selectButtonRef" type="button" :disabled="props.disabled" class="flex h-[52px] w-full items-center justify-between rounded border border-[var(--border-light)] bg-[var(--button-secondary-light)] px-4 py-3 text-left text-sm font-medium text-[var(--text-primary-light)] transition-all duration-300 ease-in-out focus:border-[var(--border-hover-light)] focus:outline-none disabled:cursor-not-allowed disabled:opacity-60 dark:border-[var(--border-dark)] dark:bg-[var(--button-secondary-dark)] dark:text-[var(--text-primary-dark)] dark:focus:border-[var(--border-hover-dark)]" :class="{ 'border-[var(--border-hover-light)] dark:border-[var(--border-hover-dark)]': isOpen }" @click="toggleDropdown">
            <span :class="getSelectedValueClasses()">
                {{ selectedOption?.label || props.placeholder }}
            </span>

            <!-- Dropdown indicator -->
            <CustomIcon icon="chevron-down" class="h-4 w-4 text-[var(--text-secondary-light)] transition-transform duration-300 dark:text-[var(--text-secondary-dark)]" :class="{ 'rotate-180': isOpen }" />
        </button>

        <Teleport to="body">
            <Transition enter-active-class="transition ease-out duration-200" enter-from-class="opacity-0 translate-y-1" enter-to-class="opacity-100 translate-y-0" leave-active-class="transition ease-in duration-150" leave-from-class="opacity-100 translate-y-0" leave-to-class="opacity-0 translate-y-1">
                <!-- Dropdown list -->
                <div v-show="isOpen" class="fixed max-h-70 overflow-y-auto rounded border border-[var(--border-light)] bg-[var(--bg-card-light)] shadow-xl dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)]" :style="getDropdownStyles">
                    <button v-for="option in options" :key="option.value" type="button" class="flex w-full items-start justify-between gap-3 px-4 py-3 text-left transition-colors duration-200" :class="getOptionClasses(option)" @click="selectOption(option)">
                        <!-- Option content -->
                        <div>
                            <div class="text-sm font-medium">{{ option.label }}</div>
                            <div v-if="option.meta" class="mt-1 text-xs text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ option.meta }}</div>
                        </div>

                        <!-- Selected indicator -->
                        <CustomIcon v-if="option.value === props.modelValue" icon="check" class="mt-0.5 h-4 w-4 shrink-0 text-[var(--button-primary-light)] dark:text-[var(--text-primary-dark)]" />
                    </button>
                </div>
            </Transition>
        </Teleport>
    </div>
</template>