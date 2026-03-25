<script setup>
import { HugeiconsIcon } from '@hugeicons/vue';
import { InformationCircleIcon, Wifi01Icon } from '@hugeicons/core-free-icons';

// Props
const props = defineProps({
    currentNetworkLabel: { type: String, required: true },
    scannedNetworksCount: { type: Number, required: true }
});

const { t } = useI18n();
</script>

<template>
    <div class="overflow-hidden rounded border border-[var(--border-light)] bg-[linear-gradient(135deg,color-mix(in_srgb,var(--bg-selected-light)_86%,white),var(--bg-card-light))] dark:border-[var(--border-dark)] dark:bg-[linear-gradient(135deg,color-mix(in_srgb,var(--bg-selected-dark)_88%,black),var(--bg-card-dark))]">
        <div class="grid gap-px bg-[var(--border-light)] sm:grid-cols-2 dark:bg-[var(--border-dark)]">
            <!-- Current network status panel -->
            <div class="bg-[color:color-mix(in_srgb,var(--bg-card-light)_92%,transparent)] p-4 dark:bg-[color:color-mix(in_srgb,var(--bg-card-dark)_92%,transparent)]">
                <div class="flex items-start gap-3">
                    <!-- WiFi status icon -->
                    <div class="flex h-11 w-11 shrink-0 items-center justify-center rounded-full bg-[color:color-mix(in_srgb,var(--button-primary-light)_14%,white)] text-[var(--button-primary-light)] dark:bg-[color:color-mix(in_srgb,var(--button-primary-dark)_18%,black)] dark:text-[var(--button-primary-dark)]">
                        <HugeiconsIcon :icon="Wifi01Icon" :size="18" color="currentColor" :stroke-width="1.8" class="h-[18px] w-[18px]" />
                    </div>

                    <!-- Current network info -->
                    <div class="min-w-0 flex-1">
                        <div class="text-[11px] font-semibold uppercase tracking-[0.14em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ t('pages.wifi.currentNetworkLabel') }}</div>
                        <div class="mt-1 truncate text-base font-semibold text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">{{ props.currentNetworkLabel }}</div>
                    </div>
                </div>
            </div>

            <!-- Detected networks count panel -->
            <div class="bg-[color:color-mix(in_srgb,var(--bg-card-light)_92%,transparent)] p-4 dark:bg-[color:color-mix(in_srgb,var(--bg-card-dark)_92%,transparent)]">
                <div class="flex items-start gap-3">
                    <!-- Networks count icon -->
                    <div class="flex h-11 w-11 shrink-0 items-center justify-center rounded-full bg-[color:color-mix(in_srgb,var(--bg-selected-light)_72%,white)] text-[var(--text-primary-light)] dark:bg-[color:color-mix(in_srgb,var(--bg-selected-dark)_78%,black)] dark:text-[var(--text-primary-dark)]">
                        <HugeiconsIcon :icon="InformationCircleIcon" :size="18" color="currentColor" :stroke-width="1.8" class="h-[18px] w-[18px]" />
                    </div>

                    <!-- Networks count info -->
                    <div class="min-w-0 flex-1">
                        <div class="text-[11px] font-semibold uppercase tracking-[0.14em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ t('pages.wifi.networkCountLabel') }}</div>
                        <div class="mt-1 text-2xl font-bold leading-none text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">{{ props.scannedNetworksCount }}</div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>