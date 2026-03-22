<script setup>
import { HugeiconsIcon } from '@hugeicons/vue';
import { AlertCircleIcon, InformationCircleIcon, Tick01Icon } from '@hugeicons/core-free-icons';
import Button from '~/components/ui/Button.vue';
import Dialog from '~/components/ui/Dialog.vue';

// Props
const props = defineProps({
    show: { type: Boolean, required: true },
    type: { type: String, required: false, default: 'Info' },
    title: { type: String, required: false, default: 'Message' },
    message: { type: String, required: true }
});

// Emits
const emits = defineEmits(['close']);

// Handle dialog close action
const closeDialog = () => {
    emits('close');
};
</script>

<template>
    <Dialog :show="props.show" title-id="message-dialog-title" size="md">
        <!-- Dialog content -->
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
                <h3 id="message-dialog-title" class="text-base font-semibold text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">{{ props.title }}</h3>
                <div class="mt-2">
                    <p class="text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ props.message }}</p>
                </div>
            </div>
        </div>

        <!-- Dialog footer -->
        <template #footer>
            <div class="sm:flex sm:justify-end">
                <Button type="secondary" class="w-full sm:w-auto" @click="closeDialog">Close</Button>
            </div>
        </template>
    </Dialog>
</template>