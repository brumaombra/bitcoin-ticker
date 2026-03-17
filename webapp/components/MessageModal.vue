<script setup>
import Button from '~/components/ui/Button.vue';

// Props
const props = defineProps({
    show: { type: Boolean, required: true },
    type: { type: String, required: false, default: 'Info' },
    title: { type: String, required: false, default: 'Message' },
    message: { type: String, required: true }
});

// Emits
const emit = defineEmits(['close']);
</script>

<template>
    <Transition name="modal">
        <div v-if="show" class="relative z-10" aria-labelledby="modal-title" role="dialog" aria-modal="true">
            <!-- Backdrop -->
            <div class="fixed inset-0 bg-black/40 transition-opacity" aria-hidden="true"></div>

            <div class="fixed inset-0 z-10 w-screen overflow-y-auto">
                <div class="flex min-h-full items-center justify-center p-4 text-center sm:p-0">
                    <div class="relative w-[95%] max-w-lg transform overflow-hidden rounded border border-[var(--border-light)] bg-[var(--bg-card-light)] text-left shadow-2xl transition-all dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)]">
                        <!-- Modal body -->
                        <div class="px-5 pb-5 pt-5 sm:p-6 sm:pb-4">
                            <div class="sm:flex sm:items-start">
                                <!-- Error icon -->
                                <div v-if="type === 'Error'" class="mx-auto flex size-12 shrink-0 items-center justify-center rounded-full border border-red-200 bg-red-50 sm:mx-0 sm:size-10 dark:border-red-900/50 dark:bg-red-950/30">
                                    <svg class="size-6 text-red-600" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" aria-hidden="true" data-slot="icon">
                                        <path stroke-linecap="round" stroke-linejoin="round" d="M12 9v3.75m-9.303 3.376c-.866 1.5.217 3.374 1.948 3.374h14.71c1.73 0 2.813-1.874 1.948-3.374L13.949 3.378c-.866-1.5-3.032-1.5-3.898 0L2.697 16.126ZM12 15.75h.007v.008H12v-.008Z" />
                                    </svg>
                                </div>

                                <!-- Success icon -->
                                <div v-if="type === 'Success'" class="mx-auto flex size-12 shrink-0 items-center justify-center rounded-full border border-green-200 bg-green-50 sm:mx-0 sm:size-10 dark:border-green-900/50 dark:bg-green-950/30">
                                    <svg class="size-6 text-green-600" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" aria-hidden="true" data-slot="icon">
                                        <path stroke-linecap="round" stroke-linejoin="round" d="M5 13l4 4L19 7" />
                                    </svg>
                                </div>

                                <!-- Info icon -->
                                <div v-if="type === 'Info'" class="mx-auto flex size-12 shrink-0 items-center justify-center rounded-full border border-[var(--border-light)] bg-[var(--bg-selected-light)] sm:mx-0 sm:size-10 dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)]">
                                    <svg class="size-6 text-blue-600" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" aria-hidden="true" data-slot="icon">
                                        <path stroke-linecap="round" stroke-linejoin="round" d="M12 8v4m0 4v.01" />
                                    </svg>
                                </div>

                                <!-- Message content -->
                                <div class="mt-3 text-center sm:ml-4 sm:mt-0 sm:text-left">
                                    <h3 id="modal-title" class="text-base font-semibold text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">{{ title }}</h3>
                                    <div class="mt-2">
                                        <p class="text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ message }}</p>
                                    </div>
                                </div>
                            </div>
                        </div>

                        <!-- Modal footer -->
                        <div class="border-t border-[var(--border-light)] px-5 py-4 dark:border-[var(--border-dark)] sm:flex sm:flex-row-reverse sm:px-6">
                            <Button type="secondary" class="w-full sm:w-auto" @click="$emit('close')">Close</Button>
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