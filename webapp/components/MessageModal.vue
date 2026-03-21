<script setup>
import { HugeiconsIcon } from '@hugeicons/vue';
import { AlertCircleIcon, InformationCircleIcon, Tick01Icon } from '@hugeicons/core-free-icons';
import Button from '~/components/ui/Button.vue';

// Props
const props = defineProps({
    show: { type: Boolean, required: true },
    type: { type: String, required: false, default: 'Info' },
    title: { type: String, required: false, default: 'Message' },
    message: { type: String, required: true }
});

// Emits
const emits = defineEmits(['close']);

// Handle modal close action
const closeModal = () => {
    emits('close');
};
</script>

<template>
    <Transition name="modal">
        <div v-if="props.show" class="relative z-10" aria-labelledby="modal-title" role="dialog" aria-modal="true">
            <!-- Backdrop -->
            <div class="fixed inset-0 bg-black/40 transition-opacity" aria-hidden="true"></div>

            <div class="fixed inset-0 z-10 w-screen overflow-y-auto">
                <div class="flex min-h-full items-center justify-center p-4 text-center sm:p-0">
                    <div class="relative w-[95%] max-w-lg transform overflow-hidden rounded border border-[var(--border-light)] bg-[var(--bg-card-light)] text-left shadow-2xl transition-all dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)]">
                        <!-- Modal body -->
                        <div class="px-5 pb-5 pt-5 sm:p-6 sm:pb-4">
                            <div class="sm:flex sm:items-start">
                                <!-- Error icon -->
                                <div v-if="props.type === 'Error'" class="mx-auto flex size-12 shrink-0 items-center justify-center rounded-full border border-red-200 bg-red-50 sm:mx-0 sm:size-10 dark:border-red-900/50 dark:bg-red-950/30">
                                    <HugeiconsIcon :icon="AlertCircleIcon" :size="24" color="currentColor" :stroke-width="1.8" class="size-6 text-red-600" />
                                </div>

                                <!-- Success icon -->
                                <div v-if="props.type === 'Success'" class="mx-auto flex size-12 shrink-0 items-center justify-center rounded-full border border-green-200 bg-green-50 sm:mx-0 sm:size-10 dark:border-green-900/50 dark:bg-green-950/30">
                                    <HugeiconsIcon :icon="Tick01Icon" :size="24" color="currentColor" :stroke-width="1.8" class="size-6 text-green-600" />
                                </div>

                                <!-- Info icon -->
                                <div v-if="props.type === 'Info'" class="mx-auto flex size-12 shrink-0 items-center justify-center rounded-full border border-[var(--border-light)] bg-[var(--bg-selected-light)] sm:mx-0 sm:size-10 dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)]">
                                    <HugeiconsIcon :icon="InformationCircleIcon" :size="24" color="currentColor" :stroke-width="1.8" class="size-6 text-blue-600" />
                                </div>

                                <!-- Message content -->
                                <div class="mt-3 text-center sm:ml-4 sm:mt-0 sm:text-left">
                                    <h3 id="modal-title" class="text-base font-semibold text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">{{ props.title }}</h3>
                                    <div class="mt-2">
                                        <p class="text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ props.message }}</p>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <!-- Modal footer -->
                        <div class="border-t border-[var(--border-light)] px-5 py-4 dark:border-[var(--border-dark)] sm:flex sm:flex-row-reverse sm:px-6">
                            <Button type="secondary" class="w-full sm:w-auto" @click="closeModal">Close</Button>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </Transition>
</template>

<style scoped>
/* Modal transition */
.modal-enter-active,
.modal-leave-active {
    transition: opacity 0.2s ease;
}

.modal-enter-from,
.modal-leave-to {
    opacity: 0;
}
</style>