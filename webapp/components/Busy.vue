<script setup>
import CryptoLogo from '~/components/ui/crypto/CryptoLogo.vue';

// Props
const props = defineProps({
    show: { type: Boolean, default: false }
});

const { t } = useI18n();
</script>

<template>
    <Transition name="fade">
        <div v-if="props.show" class="fixed inset-0 z-50 flex items-center justify-center bg-black/35 backdrop-blur-sm">
            <div class="rounded border border-[var(--border-light)] bg-[color:color-mix(in_srgb,var(--bg-card-light)_92%,transparent)] px-8 py-7 shadow-2xl dark:border-[var(--border-dark)] dark:bg-[color:color-mix(in_srgb,var(--bg-card-dark)_92%,transparent)]">
                <!-- Busy state content -->
                <div class="flex flex-col items-center">
                    <CryptoLogo class="h-14 w-14 animate-spin" style="animation-duration: 2s;" />
                    <span class="mt-4 text-sm font-medium text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">{{ t('common.syncingDevice') }}</span>
                </div>
            </div>
        </div>
    </Transition>
</template>

<style scoped>
/* Fade transition */
.fade-enter-active,
.fade-leave-active {
    transition: opacity 0.1s ease;
}

.fade-enter-from,
.fade-leave-to {
    opacity: 0;
}
</style>