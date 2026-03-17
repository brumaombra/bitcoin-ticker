<script setup>
const props = defineProps({
    brandName: { type: String, required: true }
});
</script>

<template>
    <div class="flex items-center gap-3">
        <div class="flex h-10 w-10 items-center justify-center rounded border border-[var(--border-light)] bg-[var(--bg-card-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)]">
            <img src="/svg/bitcoin.svg" alt="Bitcoin logo" class="h-6 w-6">
        </div>

        <div>
            <div class="text-sm font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
                Device UI
            </div>
            <div class="text-lg font-bold text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">
                {{ props.brandName }}
            </div>
        </div>
    </div>
</template>