<script setup>
import { computed } from 'vue';
import { Check, CircleAlert, Info } from 'lucide-vue-next';
import { AlertDialog, AlertDialogAction, AlertDialogContent, AlertDialogDescription, AlertDialogFooter, AlertDialogHeader, AlertDialogTitle } from '~/components/shadcn/alert-dialog';
import { Button } from '~/components/shadcn/button';

// Props
const props = defineProps({
    show: { type: Boolean, required: true },
    type: { type: String, required: false, default: 'Info' },
    title: { type: String, required: false, default: '' },
    message: { type: String, required: true }
});

// Emits
const emits = defineEmits(['close']);

const { t } = useI18n();

// Resolve a translated fallback title by dialog type
const dialogTitle = computed(() => {
    if (props.title) {
        return props.title;
    }

    const titleMap = {
        Error: t('dialogs.errorTitle'),
        Success: t('dialogs.successTitle'),
        Info: t('dialogs.infoTitle')
    };

    return titleMap[props.type] || t('dialogs.messageTitle');
});

// Handle dialog close action
const closeDialog = () => {
    emits('close');
};
</script>

<template>
    <AlertDialog :open="props.show" @update:open="value => { if (!value) closeDialog(); }">
        <AlertDialogContent>
            <div class="px-5 pb-5 pt-5 sm:px-6 sm:pb-4 sm:pt-6">
                <!-- Dialog body -->
                <div class="flex flex-col gap-4 sm:flex-row sm:items-start">
                    <!-- Error icon -->
                    <div v-if="props.type === 'Error'" class="mx-auto flex size-12 shrink-0 items-center justify-center rounded border border-red-200 bg-red-50 sm:mx-0 sm:size-10 dark:border-red-900/50 dark:bg-red-950/30">
                        <CircleAlert :stroke-width="1.8" class="size-6 text-red-600" />
                    </div>

                    <!-- Success icon -->
                    <div v-if="props.type === 'Success'" class="mx-auto flex size-12 shrink-0 items-center justify-center rounded border border-green-200 bg-green-50 sm:mx-0 sm:size-10 dark:border-green-900/50 dark:bg-green-950/30">
                        <Check :stroke-width="1.8" class="size-6 text-green-600" />
                    </div>

                    <!-- Info icon -->
                    <div v-if="props.type === 'Info'" class="mx-auto flex size-12 shrink-0 items-center justify-center rounded border border-[var(--border-light)] bg-[var(--bg-surface-light)] sm:mx-0 sm:size-10 dark:border-[var(--border-dark)] dark:bg-[var(--bg-surface-dark)]">
                        <Info :stroke-width="1.8" class="size-6 text-blue-600" />
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
                <AlertDialogAction as-child>
                    <Button variant="outline" class="w-full sm:w-auto">{{ t('common.close') }}</Button>
                </AlertDialogAction>
            </AlertDialogFooter>
        </AlertDialogContent>
    </AlertDialog>
</template>