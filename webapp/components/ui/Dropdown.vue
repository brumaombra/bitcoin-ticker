<script setup>
import { computed, onBeforeUnmount, onMounted, ref } from 'vue';

import Card from '~/components/ui/Card.vue';

const props = defineProps({
    open: { type: Boolean, default: false },
    position: { type: String, default: 'bottom-right', validator: value => ['bottom-right', 'bottom-left', 'top-right', 'top-left'].includes(value) },
    width: { type: String, default: 'w-44' }
});

const emits = defineEmits(['update:open', 'close']);

const dropdownRef = ref(null);

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

const closeDropdown = () => {
    emits('update:open', false);
    emits('close');
};

const handleClickOutside = event => {
    if (dropdownRef.value && !dropdownRef.value.contains(event.target)) {
        closeDropdown();
    }
};

const handleEscape = event => {
    if (event.key === 'Escape' && props.open) {
        closeDropdown();
    }
};

onMounted(() => {
    document.addEventListener('click', handleClickOutside);
    document.addEventListener('keydown', handleEscape);
});

onBeforeUnmount(() => {
    document.removeEventListener('click', handleClickOutside);
    document.removeEventListener('keydown', handleEscape);
});
</script>

<template>
    <div ref="dropdownRef" class="relative inline-flex">
        <slot name="trigger"></slot>

        <Transition enter-active-class="transition ease-out duration-200"
            enter-from-class="opacity-0 translate-y-1"
            enter-to-class="opacity-100 translate-y-0"
            leave-active-class="transition ease-in duration-150"
            leave-from-class="opacity-100 translate-y-0"
            leave-to-class="opacity-0 translate-y-1">
            <div v-if="props.open" :class="[positionClasses, props.width, 'absolute z-50']">
                <Card class="!p-0 shadow-xl">
                    <slot></slot>
                </Card>
            </div>
        </Transition>
    </div>
</template>