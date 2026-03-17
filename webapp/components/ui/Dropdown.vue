<script setup>
import { computed, onBeforeUnmount, onMounted, ref } from 'vue';

import Card from '~/components/ui/Card.vue';

// Props
const props = defineProps({
    open: { type: Boolean, default: false },
    position: { type: String, default: 'bottom-right', validator: value => ['bottom-right', 'bottom-left', 'top-right', 'top-left'].includes(value) },
    width: { type: String, default: 'w-44' }
});

// Emits
const emits = defineEmits(['update:open', 'close']);

// Dropdown container reference
const dropdownRef = ref(null);

// Resolve the floating panel position classes
const positionClasses = computed(() => {
    switch (props.position) {
        case 'bottom-left':
            return 'left-0 top-full mt-2 origin-top-left';
        case 'top-right':
            return 'right-0 bottom-full mb-2 origin-bottom-right';
        case 'top-left':
            return 'left-0 bottom-full mb-2 origin-bottom-left';
        default:
            return 'right-0 top-full mt-2 origin-top-right';
    }
});

// Close the dropdown and notify listeners
const closeDropdown = () => {
    emits('update:open', false);
    emits('close');
};

// Close the dropdown when clicking outside the container
const handleClickOutside = event => {
    if (dropdownRef.value && !dropdownRef.value.contains(event.target)) {
        closeDropdown();
    }
};

// Close the dropdown when pressing Escape
const handleEscape = event => {
    if (event.key === 'Escape' && props.open) {
        closeDropdown();
    }
};

// Register dropdown listeners on mount
onMounted(() => {
    document.addEventListener('click', handleClickOutside);
    document.addEventListener('keydown', handleEscape);
});

// Clean up listeners on unmount
onBeforeUnmount(() => {
    document.removeEventListener('click', handleClickOutside);
    document.removeEventListener('keydown', handleEscape);
});
</script>

<template>
    <div ref="dropdownRef" class="relative inline-flex">
        <!-- Trigger -->
        <slot name="trigger"></slot>

        <Transition enter-active-class="transition ease-out duration-200"
            enter-from-class="opacity-0 translate-y-1"
            enter-to-class="opacity-100 translate-y-0"
            leave-active-class="transition ease-in duration-150"
            leave-from-class="opacity-100 translate-y-0"
            leave-to-class="opacity-0 translate-y-1">
            <!-- Dropdown panel -->
            <div v-if="props.open" :class="[positionClasses, props.width, 'absolute z-50']">
                <Card class="!p-0 shadow-xl">
                    <slot></slot>
                </Card>
            </div>
        </Transition>
    </div>
</template>