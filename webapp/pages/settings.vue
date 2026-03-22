<script setup>
import { HugeiconsIcon } from '@hugeicons/vue';
import { Settings01Icon } from '@hugeicons/core-free-icons';
import { computed } from 'vue';
import { resetSettings, saveSettings } from '~/composables/useDeviceApi.js';
import { handleBackendErrors, setBusy, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import Button from '~/components/ui/Button.vue';
import Card from '~/components/ui/Card.vue';
import CardHeader from '~/components/ui/CardHeader.vue';
import FormInfoText from '~/components/ui/FormInfoText.vue';
import PageIntroCard from '~/components/ui/PageIntroCard.vue';
import SettingToggleItem from '~/components/ui/SettingToggleItem.vue';
import SliderInput from '~/components/ui/SliderInput.vue';
import Select from '~/components/ui/Select.vue';
import Label from '~/components/ui/Label.vue';

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
        showMessage({
            type: 'Success',
            title: 'Success',
            message: 'The settings have been saved successfully!'
        });
    } catch (error) {
        handleBackendErrors({ error, errorTranslated: 'An error occurred while saving the settings', errorMessage: 'An error occurred while saving the settings', showDialog: true });
    } finally {
        setBusy(false);
    }
};

// Reset the saved device settings and restart the firmware
const handleResetPress = async () => {
    try {
        setBusy(true);
        const result = await resetSettings();
        showMessage({
            type: 'Info',
            title: 'Device restarting',
            message: result.message || 'The device settings were cleared and the firmware is restarting.'
        });
    } catch (error) {
        handleBackendErrors({ error, errorTranslated: 'An error occurred while resetting the settings', errorMessage: 'An error occurred while resetting the settings', showDialog: true });
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
    <div class="mx-auto flex w-full flex-col gap-6">
        <div class="grid gap-6 xl:grid-cols-[minmax(0,320px),minmax(0,1fr)]">
            <!-- Page intro -->
            <PageIntroCard eyebrow="Display" title="Ticker Settings" description="Fine-tune what the LED matrix shows and how aggressively it scrolls through the market data.">
                <template #icon>
                    <HugeiconsIcon :icon="Settings01Icon" :size="28" color="currentColor" :stroke-width="1.8" aria-label="Settings" role="img" class="h-7 w-7" />
                </template>
            </PageIntroCard>

            <!-- Settings form -->
            <form class="space-y-6" @submit.prevent="handleSavePress">
                <!-- Visibility card -->
                <Card>
                    <!-- Visibility section -->
                    <CardHeader title="Visibility"
                        description="Choose which market metrics rotate on the matrix." />

                    <!-- Visibility toggles -->
                    <div class="space-y-3">
                        <!-- Current price -->
                        <SettingToggleItem id="currentPrice"
                            v-model="settings.currentPrice"
                            label="Current price"
                            description="The live Bitcoin price currently fetched from the API." />

                        <!-- Price change -->
                        <SettingToggleItem id="priceChange"
                            v-model="settings.priceChange"
                            label="Price change"
                            description="How much the price moved compared to the previous period." />

                        <!-- Market cap -->
                        <SettingToggleItem id="marketCap"
                            v-model="settings.marketCap"
                            label="Market cap"
                            description="The total market value of Bitcoin." />

                        <!-- 24h volume -->
                        <SettingToggleItem id="dailyHighLow"
                            v-model="settings.dailyHighLow"
                            label="Daily high / low"
                            description="The highest and lowest price reached today." />

                        <!-- 24h high / low -->
                        <SettingToggleItem id="yearHighLow"
                            v-model="settings.yearHighLow"
                            label="Year high / low"
                            description="The highest and lowest price reached over the last 52 weeks." />

                        <!-- Open price -->
                        <SettingToggleItem id="openPrice"
                            v-model="settings.openPrice"
                            label="Open price"
                            description="The price when the current period opened." />

                        <!-- Volume -->
                        <SettingToggleItem id="volume"
                            v-model="settings.volume"
                            label="Volume"
                            description="The trading volume for the current period." />
                    </div>
                </Card>

                <!-- Formatting & motion card -->
                <Card>
                    <!-- Formatting section -->
                    <CardHeader title="Formatting & Motion"
                        description="Tweak the number formatting and playback intensity for the matrix." />

                    <!-- Formatting controls -->
                    <div class="space-y-6">
                        <!-- Thousands separator format -->
                        <div class="space-y-2">
                            <Label for="selectFormatType">Thousands separator</Label>
                            <Select id="selectFormatType" v-model="settings.formatType" :option-list="separatorOptions" placeholder="Choose a number format" />
                            <FormInfoText>The formatting updates on the next API refresh.</FormInfoText>
                        </div>

                        <!-- Matrix intensity -->
                        <div class="space-y-2">
                            <div class="flex items-center justify-between">
                                <Label for="matrixIntensity">Matrix intensity</Label>
                                <span class="text-sm font-semibold text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ matrixIntensityPercentage }}%</span>
                            </div>
                            <SliderInput id="matrixIntensity" v-model="settings.matrixIntensity" :min="0" :max="15" />
                            <FormInfoText class="mt-2">Controls the brightness of the matrix modules.</FormInfoText>
                        </div>

                        <!-- Scroll speed -->
                        <div class="space-y-2">
                            <div class="flex items-center justify-between">
                                <Label for="scrollSpeed">Scroll speed</Label>
                                <span class="text-sm font-semibold text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ scrollSpeedPercentage }}%</span>
                            </div>
                            <SliderInput id="scrollSpeed" v-model="settings.scrollSpeed" :min="0" :max="15" />
                            <FormInfoText class="mt-2">Controls how fast each text block moves across the display.</FormInfoText>
                        </div>
                    </div>
                </Card>

                <!-- Action buttons -->
                <div class="flex flex-col gap-3 md:flex-row md:justify-end">
                    <Button type="primary" native-type="submit" class="w-full md:w-auto">Save settings</Button>
                    <Button type="secondary" class="w-full md:w-auto" @click="handleResetPress">Reset saved settings</Button>
                </div>
            </form>
        </div>
    </div>
</template>