<script setup>
import { Info, KeyRound } from 'lucide-vue-next';
import { computed, ref } from 'vue';
import { saveApiKey } from '~/composables/useDeviceApi.js';
import { handleBackendErrors, setBusy, showConfirmDialog, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import { Alert, AlertDescription, AlertTitle } from '~/components/ui/alert';
import { Button } from '~/components/ui/button';
import { Card, CardContent, CardFooter } from '~/components/ui/card';
import { Field, FieldGroup } from '~/components/ui/field';
import { Input } from '~/components/ui/input';
import PageIntroCard from '~/components/PageIntroCard.vue';

const { t } = useI18n();
const globalStore = useGlobalStore();
const apiKey = ref('');

// Require a non-empty API key before submitting
const isFormValid = computed(() => {
    return apiKey.value.trim();
});

// Save the API key on the device
const saveCurrentApiKey = async () => {
    try {
        setBusy(true);
        await saveApiKey(apiKey.value);
        globalStore.value.config.apiKey = apiKey.value;
        showMessage({
            type: 'Success',
            title: t('dialogs.successTitle'),
            message: t('pages.apiKey.saveSuccess')
        });
    } catch (error) {
        handleBackendErrors({ error, defaultMessage: t('pages.apiKey.saveError'), showDialog: true });
    } finally {
        setBusy(false);
        apiKey.value = '';
    }
};

// Ask for confirmation before overwriting the stored API key
const handleSavePress = () => {
    showConfirmDialog({
        title: t('pages.apiKey.confirmTitle'),
        message: t('pages.apiKey.confirmMessage'),
        confirmButton: {
            text: t('pages.apiKey.saveAction'),
            type: 'default'
        },
        cancelButton: {
            text: t('common.keepEditing'),
            type: 'outline'
        },
        onConfirm: saveCurrentApiKey
    });
};

// Page metadata
definePageMeta({
    layout: 'private'
});
</script>

<template>
    <div class="mx-auto flex w-full flex-col gap-6 lg:flex-row">
        <!-- Page intro -->
        <div class="w-full lg:max-w-sm">
            <PageIntroCard :eyebrow="t('pages.apiKey.eyebrow')"
                :title="t('pages.apiKey.title')"
                :description="t('pages.apiKey.description')"
                :icon="KeyRound"
                :icon-label="t('pages.apiKey.label')" />
        </div>

        <!-- API key form -->
        <div class="min-w-0 flex-1">
            <Card>
                <!-- Card content -->
                <CardContent>
                    <form @submit.prevent="handleSavePress">
                        <FieldGroup>
                            <!-- API key input -->
                            <Field>
                                <FieldLabel for="apiKey">{{ t('pages.apiKey.label') }}</FieldLabel>
                                <Input id="apiKey" v-model="apiKey" type="text" :placeholder="t('pages.apiKey.placeholder')" />
                            </Field>

                            <!-- API key guidance -->
                            <Alert>
                                <Info :stroke-width="1.8" />
                                <AlertTitle>{{ t('common.note') }}</AlertTitle>
                                <AlertDescription>
                                    {{ t('pages.apiKey.note') }}
                                </AlertDescription>
                            </Alert>
                        </FieldGroup>
                    </form>
                </CardContent>

                <!-- Card footer -->
                <CardFooter>
                    <Button variant="default" type="button" class="w-full" :disabled="!isFormValid" @click="handleSavePress">{{ t('pages.apiKey.saveAction') }}</Button>
                </CardFooter>
            </Card>
        </div>
    </div>
</template>
