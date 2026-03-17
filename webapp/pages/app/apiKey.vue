<script setup>
import Button from '~/components/ui/Button.vue';
import Card from '~/components/ui/Card.vue';
import Input from '~/components/ui/Input.vue';
import { saveApiKey } from '~/composables/useDeviceApi.js';
import { setBusy, showMessage } from '~/composables/useUtils.js';

// Page metadata
definePageMeta({
    layout: 'private'
});

// Form state
const apiKey = ref('');

// Require a non-empty API key before submitting
const isFormValid = computed(() => {
    return apiKey.value.trim();
});

// Save the API key on the device
const handleSavePress = async () => {
    try {
        setBusy(true);
        await saveApiKey(apiKey.value);
        showMessage('Success', 'Success', 'The API key has been saved successfully!');
    } catch (error) {
        console.error(error);
        showMessage('Error', 'Error', 'An error occurred while saving the API key');
    } finally {
        setBusy(false);
        apiKey.value = '';
    }
};
</script>

<template>
    <div class="mx-auto flex w-full max-w-5xl flex-col gap-6 lg:flex-row">
        <!-- Page intro -->
        <div class="w-full lg:max-w-sm">
            <Card>
                <div class="mb-6 flex items-center gap-4">
                    <div class="flex h-14 w-14 items-center justify-center rounded border border-[var(--border-light)] bg-[var(--bg-selected-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)]">
                        <img src="/svg/key.svg" alt="API key icon" class="h-7 w-7">
                    </div>
                    <div>
                        <div class="text-xs font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Credentials</div>
                        <h1 class="mt-1 text-2xl font-bold">API Key</h1>
                    </div>
                </div>

                <p class="text-sm leading-6 text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
                    Store the market data API key used by the ticker firmware. The value is sent directly to the device configuration endpoint.
                </p>
            </Card>
        </div>

        <!-- API key form -->
        <div class="min-w-0 flex-1">
            <Card>
                <form class="space-y-5" @submit.prevent="handleSavePress">
                    <!-- API key input -->
                    <div>
                        <label for="apiKey" class="mb-2 block text-sm font-medium">API key</label>
                        <Input id="apiKey" v-model="apiKey" type="text" placeholder="Paste your API key" />
                    </div>

                    <!-- API key guidance -->
                    <div class="rounded border border-[var(--border-light)] bg-[var(--bg-selected-light)] px-4 py-3 text-sm text-[var(--text-secondary-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-secondary-dark)]">
                        Use a key with enough quota for frequent polling. The current firmware expects the API key as a plain string.
                    </div>

                    <!-- Submit button -->
                    <Button type="primary" class="w-full" :disabled="!isFormValid">Save API Key</Button>
                </form>
            </Card>
        </div>
    </div>
</template>