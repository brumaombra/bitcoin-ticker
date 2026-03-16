<script setup>
const { saveApiKey } = useDeviceApi();
const { setBusy, showMessage } = useAppUi();

const apiKey = ref('');

const isFormValid = computed(() => {
    return apiKey.value.trim();
});

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
    <div class="flex justify-center">
        <div class="w-full max-w-md rounded-lg bg-white p-8 shadow-lg">
            <div class="mb-6">
                <div class="mb-6 flex justify-center">
                    <img src="/key.svg" alt="Bitcoin logo" class="h-16 w-16">
                </div>
                <h1 class="text-center text-3xl font-bold text-gray-800">Insert the API key</h1>
            </div>

            <form @submit.prevent="handleSavePress">
                <div class="mb-4">
                    <label for="apiKey" class="mb-2 block text-sm font-medium text-gray-700">API Key</label>
                    <input id="apiKey" v-model="apiKey" type="text" name="apiKey" class="block w-full rounded-lg border border-gray-300 px-4 py-3 focus:border-gray-500 focus:outline-none focus:ring-2 focus:ring-gray-500" placeholder="Enter your API key">
                </div>

                <button type="submit" :disabled="!isFormValid" class="w-full rounded-lg border border-transparent bg-gray-800 px-4 py-3 text-sm font-medium text-white hover:bg-gray-600 disabled:cursor-not-allowed disabled:opacity-50">
                    Save
                </button>
            </form>
        </div>
    </div>
</template>