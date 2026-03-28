<script setup>
// Props
const props = defineProps({
    show: { type: Boolean, required: true },
    titleId: { type: String, default: '' },
    size: { type: String, default: 'lg', validator: value => ['sm', 'md', 'lg', 'xl'].includes(value) },
    closeOnBackdrop: { type: Boolean, default: false }
});

// Emits
const emits = defineEmits(['close', 'update:show']);

// Resolve panel width by size preset
const getPanelWidthClass = () => {
    // Map of size presets
    const widthMap = {
        sm: 'max-w-md',
        md: 'max-w-lg',
        lg: 'max-w-xl',
        xl: 'max-w-2xl'
    };

    // Return the corresponding width class
    return widthMap[props.size] || widthMap.lg;
};

// Handle backdrop clicks when the dialog is dismissible
const handleBackdropClick = () => {
    if (!props.closeOnBackdrop) return;
    emits('close');
    emits('update:show', false);
};
</script>

<template>
    <Transition name="modal">
        <div v-if="props.show" class="fixed inset-0 z-50" :aria-labelledby="props.titleId || undefined" role="dialog" aria-modal="true">
            <!-- Backdrop -->
            <div class="absolute inset-0 bg-black/40 transition-opacity" aria-hidden="true" @click="handleBackdropClick"></div>

            <!-- Modal panel -->
            <div class="absolute inset-0 z-10 w-screen overflow-y-auto">
                <div class="flex min-h-full items-center justify-center p-4 text-center sm:p-0">
                    <div :class="['relative w-[95%] transform overflow-hidden rounded border border-[var(--border-light)] bg-[var(--bg-card-light)] text-left shadow-2xl transition-all dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)]', getPanelWidthClass()]">
                        <!-- Modal body -->
                        <div class="px-5 pb-5 pt-5 sm:p-6 sm:pb-4">
                            <slot></slot>
                        </div>

                        <!-- Modal footer -->
                        <div v-if="$slots.footer" class="border-t border-[var(--border-light)] px-5 py-4 dark:border-[var(--border-dark)] sm:px-6">
                            <slot name="footer"></slot>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </Transition>
</template>

<style scoped>
.modal-enter-active,
.modal-leave-active {
    transition: opacity 0.2s ease;
}

.modal-enter-from,
.modal-leave-to {
    opacity: 0;
}
</style>