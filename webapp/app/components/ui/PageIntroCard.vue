<script setup>
import Card from '~/components/ui/Card.vue';
import CardHeaderWithIcon from '~/components/ui/CardHeaderWithIcon.vue';

// Props
const props = defineProps({
    eyebrow: { type: String, required: true },
    title: { type: String, required: true },
    description: { type: String, required: true },
    icon: { type: Object, required: true },
    iconLabel: { type: String, default: '' }
});
</script>

<template>
    <Card>
        <!-- Header with icon -->
        <CardHeaderWithIcon :eyebrow="props.eyebrow"
            :title="props.title"
            :icon="props.icon"
            :icon-label="props.iconLabel" />

        <!-- Description -->
        <p class="text-xs leading-5 text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)] sm:text-sm sm:leading-6">
            {{ props.description }}
        </p>
    </Card>
</template>