<script setup>
import { computed } from 'vue';
import { saveSettings } from '~/composables/useDeviceApi.js';
import { setBusy, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import Button from '~/components/ui/Button.vue';
import Card from '~/components/ui/Card.vue';
import Select from '~/components/ui/Select.vue';
import ToggleSwitch from '~/components/ui/ToggleSwitch.vue';

// Shared device settings
const globalStore = useGlobalStore();

// Reactive settings payload
const settings = computed(() => globalStore.value.settings);

// Slider percentage for matrix intensity
const matrixIntensityPercentage = computed(() => {
    return Math.round((Number(settings.value?.matrixIntensity ?? 0) / 15) * 100);
});

// Slider percentage for scroll speed
const scrollSpeedPercentage = computed(() => {
    return Math.round((Number(settings.value?.scrollSpeed ?? 0) / 15) * 100);
});

// Available thousands separator formats
const separatorOptions = [
    { value: 'US', label: '21,000.00', meta: 'Comma thousands, dot decimals' },
    { value: 'EU', label: '21.000,00', meta: 'Dot thousands, comma decimals' }
];

// Save the current settings payload
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

// Page metadata
definePageMeta({
    layout: 'private'
});
</script>

<template>
    <div v-if="settings" class="mx-auto flex w-full flex-col gap-6">
        <div class="grid gap-6 xl:grid-cols-[minmax(0,320px),minmax(0,1fr)]">
            <!-- Page intro -->
            <Card>
                <div class="mb-6 flex items-center gap-4">
                    <div class="flex h-14 w-14 items-center justify-center rounded border border-[var(--border-light)] bg-[var(--bg-selected-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)]">
                        <img src="/svg/settings.svg" alt="Settings icon" class="h-7 w-7">
                    </div>
                    <div>
                        <div class="text-xs font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Display</div>
                        <h1 class="mt-1 text-2xl font-bold">Ticker Settings</h1>
                    </div>
                </div>

                <p class="text-sm leading-6 text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
                    Fine-tune what the LED matrix shows and how aggressively it scrolls through the market data.
                </p>
            </Card>

            <!-- Settings form -->
            <form class="space-y-6" @submit.prevent="handleSavePress">
                <Card>
                    <!-- Visibility section -->
                    <div class="mb-5 flex items-center justify-between">
                        <div>
                            <h2 class="text-lg font-bold">Visibility</h2>
                            <p class="mt-1 text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Choose which market metrics rotate on the matrix.</p>
                        </div>
                    </div>

                    <!-- Visibility toggles -->
                    <div class="space-y-3">
                        <div class="flex items-center justify-between rounded border border-[var(--border-light)] px-4 py-3 dark:border-[var(--border-dark)]">
                            <label for="currentPrice" class="text-sm font-medium">Current price</label>
                            <ToggleSwitch v-model="settings.currentPrice" />
                        </div>
                        <div class="flex items-center justify-between rounded border border-[var(--border-light)] px-4 py-3 dark:border-[var(--border-dark)]">
                            <label for="priceChange" class="text-sm font-medium">Price change</label>
                            <ToggleSwitch v-model="settings.priceChange" />
                        </div>
                        <div class="flex items-center justify-between rounded border border-[var(--border-light)] px-4 py-3 dark:border-[var(--border-dark)]">
                            <label for="marketCap" class="text-sm font-medium">Market cap</label>
                            <ToggleSwitch v-model="settings.marketCap" />
                        </div>
                        <div class="flex items-center justify-between rounded border border-[var(--border-light)] px-4 py-3 dark:border-[var(--border-dark)]">
                            <label for="dailyHighLow" class="text-sm font-medium">Daily high / low</label>
                            <ToggleSwitch v-model="settings.dailyHighLow" />
                        </div>
                        <div class="flex items-center justify-between rounded border border-[var(--border-light)] px-4 py-3 dark:border-[var(--border-dark)]">
                            <label for="yearHighLow" class="text-sm font-medium">Year high / low</label>
                            <ToggleSwitch v-model="settings.yearHighLow" />
                        </div>
                        <div class="flex items-center justify-between rounded border border-[var(--border-light)] px-4 py-3 dark:border-[var(--border-dark)]">
                            <label for="openPrice" class="text-sm font-medium">Open price</label>
                            <ToggleSwitch v-model="settings.openPrice" />
                        </div>
                        <div class="flex items-center justify-between rounded border border-[var(--border-light)] px-4 py-3 dark:border-[var(--border-dark)]">
                            <label for="volume" class="text-sm font-medium">Volume</label>
                            <ToggleSwitch v-model="settings.volume" />
                        </div>
                    </div>
                </Card>

                <Card>
                    <!-- Formatting section -->
                    <div class="mb-5">
                        <h2 class="text-lg font-bold">Formatting & Motion</h2>
                        <p class="mt-1 text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Tweak the number formatting and playback intensity for the matrix.</p>
                    </div>

                    <!-- Formatting controls -->
                    <div class="space-y-6">
                        <div>
                            <label for="selectFormatType" class="mb-2 block text-sm font-medium">Thousands separator</label>
                            <Select id="selectFormatType" v-model="settings.formatType" :option-list="separatorOptions" placeholder="Choose a number format" />
                            <p class="mt-2 text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">The formatting updates on the next API refresh.</p>
                        </div>

                        <div>
                            <div class="mb-2 flex items-center justify-between">
                                <label for="matrixIntensity" class="text-sm font-medium">Matrix intensity</label>
                                <span class="text-sm font-semibold text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ matrixIntensityPercentage }}%</span>
                            </div>
                            <input id="matrixIntensity" v-model="settings.matrixIntensity" type="range" min="0" max="15" class="h-2 w-full cursor-pointer appearance-none rounded-full bg-[var(--bg-selected-light)] accent-[var(--button-primary-light)] dark:bg-[var(--bg-selected-dark)] dark:accent-[var(--button-primary-dark)]">
                            <p class="mt-2 text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Controls the brightness of the matrix modules.</p>
                        </div>

                        <div>
                            <div class="mb-2 flex items-center justify-between">
                                <label for="scrollSpeed" class="text-sm font-medium">Scroll speed</label>
                                <span class="text-sm font-semibold text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ scrollSpeedPercentage }}%</span>
                            </div>
                            <input id="scrollSpeed" v-model="settings.scrollSpeed" type="range" min="0" max="15" class="h-2 w-full cursor-pointer appearance-none rounded-full bg-[var(--bg-selected-light)] accent-[var(--button-primary-light)] dark:bg-[var(--bg-selected-dark)] dark:accent-[var(--button-primary-dark)]">
                            <p class="mt-2 text-sm text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Controls how fast each text block moves across the display.</p>
                        </div>
                    </div>
                </Card>

                <!-- Submit button -->
                <Button type="primary" class="w-full md:w-auto">Save Settings</Button>
            </form>
        </div>
    </div>

    <!-- Loading state -->
    <div v-else class="mx-auto w-full max-w-xl">
        <Card>Loading settings...</Card>
    </div>
</template>