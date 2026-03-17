<script setup>
import { computed } from 'vue';

const props = defineProps({
    icon: { type: String, required: true, validator: value => ['check', 'chevron-down', 'menu', 'theme-dark', 'theme-light', 'theme-system'].includes(value) },
    ariaLabel: { type: String, default: '' },
    decorative: { type: Boolean, default: true },
    strokeWidth: { type: [Number, String], default: 1.8 }
});

const viewBox = computed(() => {
    if (['check', 'chevron-down'].includes(props.icon)) {
        return '0 0 20 20';
    }

    return '0 0 24 24';
});

const fill = computed(() => {
    if (['check', 'chevron-down', 'theme-dark'].includes(props.icon)) {
        return 'currentColor';
    }

    return 'none';
});

const stroke = computed(() => {
    if (['check', 'chevron-down', 'theme-dark'].includes(props.icon)) {
        return 'none';
    }

    return 'currentColor';
});

const isDecorative = computed(() => {
    return props.decorative && !props.ariaLabel;
});
</script>

<template>
    <svg :viewBox="viewBox" :fill="fill" :stroke="stroke" :stroke-width="strokeWidth" :aria-hidden="isDecorative ? 'true' : undefined" :aria-label="!isDecorative ? props.ariaLabel : undefined" :role="!isDecorative ? 'img' : undefined">
        <template v-if="props.icon === 'theme-light'">
            <circle cx="12" cy="12" r="4"></circle>
            <path d="M12 2v2.5M12 19.5V22M4.93 4.93l1.77 1.77M17.3 17.3l1.77 1.77M2 12h2.5M19.5 12H22M4.93 19.07l1.77-1.77M17.3 6.7l1.77-1.77"></path>
        </template>

        <path v-else-if="props.icon === 'theme-dark'" d="M21 14.25A8.25 8.25 0 1111.75 3a6.75 6.75 0 109.25 11.25Z"></path>

        <template v-else-if="props.icon === 'theme-system'">
            <rect x="3" y="4" width="18" height="12" rx="2"></rect>
            <path d="M8 20h8"></path>
        </template>

        <path v-else-if="props.icon === 'menu'" stroke-linecap="round" stroke-linejoin="round" d="M3.75 6.75h16.5M3.75 12h16.5m-16.5 5.25h16.5"></path>

        <path v-else-if="props.icon === 'check'" fill-rule="evenodd" d="M16.704 5.29a1 1 0 010 1.42l-7.2 7.2a1 1 0 01-1.415 0l-3-3a1 1 0 111.414-1.42l2.293 2.294 6.493-6.494a1 1 0 011.415 0Z" clip-rule="evenodd"></path>

        <path v-else-if="props.icon === 'chevron-down'" fill-rule="evenodd" d="M5.23 7.21a.75.75 0 011.06.02L10 11.168l3.71-3.938a.75.75 0 111.08 1.04l-4.25 4.5a.75.75 0 01-1.08 0l-4.25-4.5a.75.75 0 01.02-1.06Z" clip-rule="evenodd"></path>
    </svg>
</template>