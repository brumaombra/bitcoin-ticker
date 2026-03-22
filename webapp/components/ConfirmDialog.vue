<script setup>
import { HugeiconsIcon } from '@hugeicons/vue';
import { InformationCircleIcon } from '@hugeicons/core-free-icons';
import Button from '~/components/ui/Button.vue';
import Dialog from '~/components/ui/Dialog.vue';

// Props
const props = defineProps({
    show: { type: Boolean, required: true },
    title: { type: String, default: 'Confirm action' },
    message: { type: String, required: true },
    confirmText: { type: String, default: 'Confirm' },
    cancelText: { type: String, default: 'Cancel' },
    confirmButtonType: { type: String, default: 'primary', validator: value => ['primary', 'secondary', 'ghost'].includes(value) },
    cancelButtonType: { type: String, default: 'secondary', validator: value => ['primary', 'secondary', 'ghost'].includes(value) },
    icon: { type: Object, default: () => InformationCircleIcon }
});

// Emits
const emits = defineEmits(['confirm', 'cancel', 'close', 'update:show']);

// Handle cancel press
const handleCancelPress = () => {
    emits('cancel');
    emits('close');
    emits('update:show', false);
};

// Handle confirm press
const handleConfirmPress = () => {
    emits('confirm');
    emits('update:show', false);
};
</script>

<template>
    <Dialog :show="props.show" title-id="confirm-dialog-title" size="md" close-on-backdrop @close="handleCancelPress" @update:show="value => emits('update:show', value)">
        <div class="sm:flex sm:items-start">
            <!-- Icon -->
            <div class="mx-auto flex size-12 shrink-0 items-center justify-center rounded-full border border-[var(--border-light)] bg-[var(--bg-selected-light)] sm:mx-0 sm:size-10 dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)]">
                <HugeiconsIcon :icon="props.icon" :size="24" color="currentColor" :stroke-width="1.8" class="size-6 text-[var(--button-primary-light)] dark:text-[var(--button-primary-dark)]" />
            </div>

            <!-- Message content -->
            <div class="mt-3 text-center sm:ml-4 sm:mt-0 sm:text-left">
                <!-- Title -->
                <h3 id="confirm-dialog-title" class="text-base font-semibold text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">{{ props.title }}</h3>

                <!-- Message -->
                <div class="mt-2">
                    <p class="text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ props.message }}</p>
                </div>
            </div>
        </div>

        <template #footer>
            <div class="flex w-full flex-col gap-3 sm:w-auto sm:ml-auto sm:flex-row-reverse">
                <Button :type="props.confirmButtonType" class="w-full sm:w-auto" @click="handleConfirmPress">{{ props.confirmText }}</Button>
                <Button :type="props.cancelButtonType" class="w-full sm:w-auto" @click="handleCancelPress">{{ props.cancelText }}</Button>
            </div>
        </template>
    </Dialog>
</template>