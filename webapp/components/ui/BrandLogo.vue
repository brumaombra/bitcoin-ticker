<script setup>
// Props
const props = defineProps({
    brandName: { type: String, required: true },
    compact: { type: Boolean, default: false }
});
</script>

<template>
    <div class="flex items-center gap-3">
        <!-- Brand mark -->
        <img src="/svg/bitcoin.svg" alt="Bitcoin logo" class="h-9 w-9">

        <!-- Brand text -->
        <div v-if="!props.compact">
            <!-- Brand name -->
            <div class="text-sm font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
                Device UI
            </div>

            <!-- Brand name -->
            <div class="text-lg font-bold text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">
                {{ props.brandName }}
            </div>
        </div>
    </div>
</template>