<script setup>
import BitcoinLogo from '~/components/ui/BitcoinLogo.vue';

// Props
const props = defineProps({
    brandName: { type: String, required: true },
    compact: { type: Boolean, default: false }
});

const { t } = useI18n();
</script>

<template>
    <div class="flex items-center gap-2 sm:gap-3">
        <!-- Brand mark -->
        <BitcoinLogo class="h-8 w-8 sm:h-9 sm:w-9" />

        <!-- Brand text -->
        <div v-if="!props.compact">
            <!-- Brand name -->
            <div class="text-[0.65rem] font-semibold uppercase tracking-[0.22em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)] sm:text-sm sm:tracking-[0.24em]">
                {{ t('common.deviceUi') }}
            </div>

            <!-- Brand name -->
            <div class="text-base font-bold text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)] sm:text-lg">
                {{ props.brandName }}
            </div>
        </div>
    </div>
</template>