<script setup>
import Card from '~/components/ui/Card.vue';
import CardHeaderWithIcon from '~/components/ui/CardHeaderWithIcon.vue';

// Props
const props = defineProps({
    eyebrow: { type: String, required: true },
    title: { type: String, required: true },
    description: { type: String, required: true }
});
</script>

<template>
    <Card>
        <!-- Header with icon slot -->
        <CardHeaderWithIcon :eyebrow="props.eyebrow" :title="props.title">
            <template #icon>
                <slot name="icon" />
            </template>
        </CardHeaderWithIcon>

        <!-- Description -->
        <p class="text-xs leading-5 text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)] sm:text-sm sm:leading-6">
            {{ props.description }}
        </p>
    </Card>
</template>