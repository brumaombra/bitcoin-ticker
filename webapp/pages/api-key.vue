<script setup>
import { HugeiconsIcon } from '@hugeicons/vue';
import { Key01Icon } from '@hugeicons/core-free-icons';
import { computed, ref } from 'vue';
import { saveApiKey } from '~/composables/useDeviceApi.js';
import { handleBackendErrors, setBusy, showConfirmDialog, showMessage } from '~/composables/useUtils.js';
import Button from '~/components/ui/Button.vue';
import Card from '~/components/ui/Card.vue';
import InfoBox from '~/components/ui/InfoBox.vue';
import Label from '~/components/ui/Label.vue';
import Input from '~/components/ui/Input.vue';
import PageIntroCard from '~/components/ui/PageIntroCard.vue';

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
        showMessage({
            type: 'Success',
            title: 'Success',
            message: 'The API key has been saved successfully!'
        });
    } catch (error) {
        handleBackendErrors({ error, errorTranslated: 'An error occurred while saving the API key', errorMessage: 'An error occurred while saving the API key', showDialog: true });
    } finally {
        setBusy(false);
        apiKey.value = '';
    }
};

// Ask for confirmation before overwriting the stored API key
const handleSavePress = () => {
    showConfirmDialog({
        title: 'Save API key',
        message: 'Store this API key on the device and replace the current value?',
        confirmButton: {
            text: 'Save API key',
            type: 'primary'
        },
        cancelButton: {
            text: 'Keep editing',
            type: 'secondary'
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
            <PageIntroCard eyebrow="Credentials" title="API Key" description="Store the market data API key used by the ticker firmware. The value is sent directly to the device configuration endpoint.">
                <template #icon>
                    <HugeiconsIcon :icon="Key01Icon" :size="28" color="currentColor" :stroke-width="1.8" aria-label="API key" role="img" class="h-7 w-7" />
                </template>
            </PageIntroCard>
        </div>

        <!-- API key form -->
        <div class="min-w-0 flex-1">
            <Card>
                <form class="space-y-5" @submit.prevent="handleSavePress">
                    <!-- API key input -->
                    <div class="space-y-2">
                        <Label for="apiKey">API key</Label>
                        <Input id="apiKey" v-model="apiKey" type="text" placeholder="Paste your API key" />
                    </div>

                    <!-- API key guidance -->
                    <InfoBox>
                        Use a key with enough quota for frequent polling. The current firmware expects the API key as a plain string.
                    </InfoBox>

                    <!-- Submit button -->
                    <Button type="primary" native-type="submit" class="w-full" :disabled="!isFormValid">Save API key</Button>
                </form>
            </Card>
        </div>
    </div>
</template>