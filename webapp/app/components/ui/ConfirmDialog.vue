<script setup>
import { computed } from 'vue';
import { Info } from 'lucide-vue-next';
import { AlertDialog, AlertDialogContent, AlertDialogDescription, AlertDialogFooter, AlertDialogHeader, AlertDialogTitle } from '~/components/shadcn/alert-dialog';
import { Button } from '~/components/shadcn/button';

// Props
const props = defineProps({
    show: { type: Boolean, required: true },
    title: { type: String, default: '' },
    message: { type: String, required: true },
    confirmText: { type: String, default: '' },
    cancelText: { type: String, default: '' },
    confirmButtonType: { type: String, default: 'default', validator: value => ['default', 'outline', 'ghost'].includes(value) },
    cancelButtonType: { type: String, default: 'outline', validator: value => ['default', 'outline', 'ghost'].includes(value) },
    icon: { type: [Object, Function], default: () => Info }
});

// Emits
const emits = defineEmits(['confirm', 'cancel', 'close', 'update:show']);

const { t } = useI18n();
const dialogTitle = computed(() => props.title || t('dialogs.confirmTitle'));
const confirmLabel = computed(() => props.confirmText || t('dialogs.confirm'));
const cancelLabel = computed(() => props.cancelText || t('common.cancel'));

// Sync the controlled alert dialog open state back to the caller
const handleOpenChange = value => {
    if (!value) {
        emits('update:show', false);
    }
};

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
    <AlertDialog :open="props.show" @update:open="handleOpenChange">
        <AlertDialogContent>
            <div class="px-5 pb-5 pt-5 sm:px-6 sm:pb-4 sm:pt-6">
                <!-- Dialog body -->
                <div class="flex flex-col gap-4 sm:flex-row sm:items-start">
                    <!-- Icon -->
                    <div class="mx-auto flex size-12 shrink-0 items-center justify-center rounded border border-[var(--border-light)] bg-[var(--bg-surface-light)] sm:mx-0 sm:size-10 dark:border-[var(--border-dark)] dark:bg-[var(--bg-surface-dark)]">
                        <component :is="props.icon" :stroke-width="1.8" class="size-6 text-[var(--button-primary-light)] dark:text-[var(--button-primary-dark)]" />
                    </div>

                    <!-- Message content -->
                    <AlertDialogHeader class="sm:pt-0">
                        <AlertDialogTitle>{{ dialogTitle }}</AlertDialogTitle>
                        <AlertDialogDescription>{{ props.message }}</AlertDialogDescription>
                    </AlertDialogHeader>
                </div>
            </div>

            <!-- Footer -->
            <AlertDialogFooter>
                <Button :variant="props.cancelButtonType" class="w-full sm:w-auto" @click="handleCancelPress">{{ cancelLabel }}</Button>
                <Button :variant="props.confirmButtonType" class="w-full sm:w-auto" @click="handleConfirmPress">{{ confirmLabel }}</Button>
            </AlertDialogFooter>
        </AlertDialogContent>
    </AlertDialog>
</template>