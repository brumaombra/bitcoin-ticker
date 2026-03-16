<script setup>
import ToggleSwitch from '~/components/ToggleSwitch.vue';
import { useAppUi } from '~/composables/useAppUi.js';
import { useDeviceApi } from '~/composables/useDeviceApi.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

const globalStore = useGlobalStore();
const { saveSettings } = useDeviceApi();
const { setBusy, showMessage } = useAppUi();

const settings = computed(() => globalStore.value.settings);

const matrixIntensityPercentage = computed(() => {
    return Math.round((Number(settings.value?.matrixIntensity ?? 0) / 15) * 100);
});

const scrollSpeedPercentage = computed(() => {
    return Math.round((Number(settings.value?.scrollSpeed ?? 0) / 15) * 100);
});

const handleSavePress = async () => {
    if (!settings.value) {
        return;
    }

    try {
        setBusy(true);
        await saveSettings(settings.value);
        showMessage('Success', 'Success', 'The settings have been saved successfully!');
    } catch (error) {
        console.error(error);
        showMessage('Error', 'Error', 'An error occurred while saving the settings');
    } finally {
        setBusy(false);
    }
};
</script>

<template>
    <div class="flex justify-center">
        <div v-if="settings" class="w-full max-w-md rounded-lg bg-white p-8 shadow-lg">
            <div class="mb-6">
                <div class="mb-6 flex justify-center">
                    <img src="/settings.svg" alt="Bitcoin logo" class="h-16 w-16">
                </div>
                <h1 class="text-center text-3xl font-bold text-gray-800">Ticker settings</h1>
            </div>

            <form @submit.prevent="handleSavePress">
                <div>
                    <h2 class="mb-5 text-center text-xl font-bold text-gray-800">Visibility</h2>

                    <div class="mb-4 flex items-center justify-between">
                        <label for="currentPrice" class="text-sm font-medium text-gray-700">Current price</label>
                        <ToggleSwitch v-model="settings.currentPrice" />
                    </div>

                    <div class="mb-4 flex items-center justify-between">
                        <label for="priceChange" class="text-sm font-medium text-gray-700">Price change</label>
                        <ToggleSwitch v-model="settings.priceChange" />
                    </div>

                    <div class="mb-4 flex items-center justify-between">
                        <label for="marketCap" class="text-sm font-medium text-gray-700">Market cap</label>
                        <ToggleSwitch v-model="settings.marketCap" />
                    </div>

                    <div class="mb-4 flex items-center justify-between">
                        <label for="dailyHighLow" class="text-sm font-medium text-gray-700">Daily high/low</label>
                        <ToggleSwitch v-model="settings.dailyHighLow" />
                    </div>

                    <div class="mb-4 flex items-center justify-between">
                        <label for="yearHighLow" class="text-sm font-medium text-gray-700">Year high/low</label>
                        <ToggleSwitch v-model="settings.yearHighLow" />
                    </div>

                    <div class="mb-4 flex items-center justify-between">
                        <label for="openPrice" class="text-sm font-medium text-gray-700">Open price</label>
                        <ToggleSwitch v-model="settings.openPrice" />
                    </div>

                    <div class="mb-4 flex items-center justify-between">
                        <label for="volume" class="text-sm font-medium text-gray-700">Volume</label>
                        <ToggleSwitch v-model="settings.volume" />
                    </div>
                </div>

                <hr class="my-6">

                <div class="mb-6">
                    <h2 class="mb-5 text-center text-xl font-bold text-gray-800">Various</h2>

                    <div class="mb-5">
                        <label for="selectFormatType" class="mb-2 block text-sm font-medium text-gray-700">Thousands separator</label>
                        <select id="selectFormatType" v-model="settings.formatType" class="block w-full appearance-none rounded-lg border border-gray-300 bg-white bg-size-[20px] bg-position-[right_12px_center] bg-no-repeat px-4 py-3 pr-10 text-gray-900 hover:cursor-pointer focus:border-gray-500 focus:outline-none focus:ring-2 focus:ring-gray-500" style="background-image: url(&quot;data:image/svg+xml;charset=US-ASCII,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%2220%22%20height%3D%2220%22%20viewBox%3D%220%200%2020%2020%22%3E%3Cpath%20fill%3D%22%23666%22%20d%3D%22M5.293%208.293a1%201%200%20011.414%200L10%2011.586l3.293-3.293a1%201%200%20111.414%201.414l-4%204a1%201%200%2001-1.414%200l-4-4a1%201%200%20010-1.414z%22%2F%3E%3C%2Fsvg%3E&quot;)">
                            <option value="US">21,000.00</option>
                            <option value="EU">21.000,00</option>
                        </select>
                        <div class="mt-2 text-sm text-gray-500">
                            The formatting will be updated on the next API call
                        </div>
                    </div>

                    <div class="mb-5">
                        <div class="mb-2 flex items-center justify-between">
                            <label for="matrixIntensity" class="text-sm font-medium text-gray-700">Matrix intensity</label>
                            <span class="text-base font-semibold text-gray-500">{{ matrixIntensityPercentage }}%</span>
                        </div>
                        <input id="matrixIntensity" v-model="settings.matrixIntensity" type="range" min="0" max="15" class="w-full cursor-pointer appearance-none rounded-lg bg-gray-200 accent-gray-800 dark:bg-gray-700">
                        <div class="mt-2 text-sm text-gray-500">
                            The light intensity of the matrix display
                        </div>
                    </div>

                    <div>
                        <div class="mb-2 flex items-center justify-between">
                            <label for="scrollSpeed" class="text-sm font-medium text-gray-700">Scroll speed</label>
                            <span class="text-base font-semibold text-gray-500">{{ scrollSpeedPercentage }}%</span>
                        </div>
                        <input id="scrollSpeed" v-model="settings.scrollSpeed" type="range" min="0" max="15" class="w-full cursor-pointer appearance-none rounded-lg bg-gray-200 accent-gray-800 dark:bg-gray-700">
                        <div class="mt-2 text-sm text-gray-500">
                            The speed of the scrolling text
                        </div>
                    </div>
                </div>

                <button type="submit" class="w-full rounded-lg border border-transparent bg-gray-800 px-4 py-3 text-sm font-medium text-white hover:bg-gray-600">
                    Save
                </button>
            </form>
        </div>

        <div v-else class="rounded-lg bg-white px-8 py-10 text-gray-500 shadow-lg">Loading settings...</div>
    </div>
</template>