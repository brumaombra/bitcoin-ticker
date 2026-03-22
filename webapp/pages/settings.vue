<script setup>
import { Settings01Icon } from '@hugeicons/core-free-icons';
import { computed } from 'vue';
import { resetSettings, saveSettings } from '~/composables/useDeviceApi.js';
import { getCryptoThemes, handleBackendErrors, setBusy, setCryptoTheme, showConfirmDialog, showMessage } from '~/composables/useUtils.js';
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
const cryptoThemes = getCryptoThemes();
const { t } = useI18n();

// Reactive settings payload
const settings = computed(() => globalStore.value.settings);

// Reactive crypto theme setting for the accent palette
const selectedCryptoTheme = computed({
    get: () => globalStore.value.cryptoTheme,
    set: value => setCryptoTheme(value)
});

// Slider percentage for matrix intensity
const matrixIntensityPercentage = computed(() => {
    return Math.round((Number(settings.value?.matrixIntensity ?? 0) / 15) * 100);
});

// Slider percentage for scroll speed
const scrollSpeedPercentage = computed(() => {
    return Math.round((Number(settings.value?.scrollSpeed ?? 0) / 15) * 100);
});

// Available thousands separator formats
const separatorOptions = computed(() => ([
    { value: 'US', label: '21,000.00', meta: t('pages.settings.formatType.usMeta') },
    { value: 'EU', label: '21.000,00', meta: t('pages.settings.formatType.euMeta') }
]));

// Available crypto accent themes
const cryptoThemeOptions = computed(() => {
    return cryptoThemes.value.map(theme => ({
        value: theme.id,
        label: t(`pages.settings.cryptoTheme.${theme.id}Label`),
        meta: t(`pages.settings.cryptoTheme.${theme.id}Meta`)
    }));
});

// Save the current settings payload
const saveCurrentSettings = async () => {
    if (!settings.value) {
        return;
    }

    try {
        setBusy(true);
        await saveSettings(settings.value);
        showMessage({
            type: 'Success',
            title: t('dialogs.successTitle'),
            message: t('pages.settings.saveSuccess')
        });
    } catch (error) {
        handleBackendErrors({ error, errorTranslated: t('pages.settings.saveError'), errorMessage: t('pages.settings.saveError'), showDialog: true });
    } finally {
        setBusy(false);
    }
};

// Reset the saved device settings and restart the firmware
const resetSavedSettings = async () => {
    try {
        setBusy(true);
        const result = await resetSettings();
        showMessage({
            type: 'Info',
            title: t('pages.settings.resetInfoTitle'),
            message: result.message || t('pages.settings.resetInfoMessage')
        });
    } catch (error) {
        handleBackendErrors({ error, errorTranslated: t('pages.settings.resetError'), errorMessage: t('pages.settings.resetError'), showDialog: true });
    } finally {
        setBusy(false);
    }
};

// Ask for confirmation before saving the current settings
const handleSavePress = () => {
    showConfirmDialog({
        title: t('pages.settings.saveConfirmTitle'),
        message: t('pages.settings.saveConfirmMessage'),
        confirmButton: {
            text: t('pages.settings.saveAction'),
            type: 'primary'
        },
        cancelButton: {
            text: t('common.keepEditing'),
            type: 'secondary'
        },
        onConfirm: saveCurrentSettings
    });
};

// Ask for confirmation before resetting the saved settings
const handleResetPress = () => {
    showConfirmDialog({
        title: t('pages.settings.resetConfirmTitle'),
        message: t('pages.settings.resetConfirmMessage'),
        confirmButton: {
            text: t('pages.settings.resetAction'),
            type: 'primary'
        },
        cancelButton: {
            text: t('common.cancel'),
            type: 'secondary'
        },
        onConfirm: resetSavedSettings
    });
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
            <PageIntroCard :eyebrow="t('pages.settings.eyebrow')"
                :title="t('pages.settings.title')"
                :description="t('pages.settings.description')"
                :icon="Settings01Icon"
                :icon-label="t('nav.settings.label')" />

            <!-- Settings form -->
            <form class="space-y-6" @submit.prevent="handleSavePress">
                <!-- Visibility card -->
                <Card>
                    <!-- Visibility section -->
                    <CardHeader :title="t('pages.settings.visibilityTitle')"
                        :description="t('pages.settings.visibilityDescription')" />

                    <!-- Visibility toggles -->
                    <div class="space-y-3">
                        <!-- Current price -->
                        <SettingToggleItem id="currentPrice"
                            v-model="settings.currentPrice"
                            :label="t('pages.settings.visibility.currentPrice.label')"
                            :description="t('pages.settings.visibility.currentPrice.description')" />

                        <!-- Price change -->
                        <SettingToggleItem id="priceChange"
                            v-model="settings.priceChange"
                            :label="t('pages.settings.visibility.priceChange.label')"
                            :description="t('pages.settings.visibility.priceChange.description')" />

                        <!-- Market cap -->
                        <SettingToggleItem id="marketCap"
                            v-model="settings.marketCap"
                            :label="t('pages.settings.visibility.marketCap.label')"
                            :description="t('pages.settings.visibility.marketCap.description')" />

                        <!-- 24h volume -->
                        <SettingToggleItem id="dailyHighLow"
                            v-model="settings.dailyHighLow"
                            :label="t('pages.settings.visibility.dailyHighLow.label')"
                            :description="t('pages.settings.visibility.dailyHighLow.description')" />

                        <!-- 24h high / low -->
                        <SettingToggleItem id="yearHighLow"
                            v-model="settings.yearHighLow"
                            :label="t('pages.settings.visibility.yearHighLow.label')"
                            :description="t('pages.settings.visibility.yearHighLow.description')" />

                        <!-- Open price -->
                        <SettingToggleItem id="openPrice"
                            v-model="settings.openPrice"
                            :label="t('pages.settings.visibility.openPrice.label')"
                            :description="t('pages.settings.visibility.openPrice.description')" />

                        <!-- Volume -->
                        <SettingToggleItem id="volume"
                            v-model="settings.volume"
                            :label="t('pages.settings.visibility.volume.label')"
                            :description="t('pages.settings.visibility.volume.description')" />
                    </div>
                </Card>

                <!-- Formatting & motion card -->
                <Card>
                    <!-- Formatting section -->
                    <CardHeader :title="t('pages.settings.formattingTitle')"
                        :description="t('pages.settings.formattingDescription')" />

                    <!-- Formatting controls -->
                    <div class="space-y-6">
                        <!-- Crypto theme -->
                        <div class="space-y-2">
                            <Label for="cryptoTheme">{{ t('pages.settings.cryptoTheme.label') }}</Label>
                            <Select id="cryptoTheme" v-model="selectedCryptoTheme" :option-list="cryptoThemeOptions" :placeholder="t('pages.settings.cryptoTheme.placeholder')" />
                            <FormInfoText>{{ t('pages.settings.cryptoTheme.note') }}</FormInfoText>
                        </div>

                        <!-- Thousands separator format -->
                        <div class="space-y-2">
                            <Label for="selectFormatType">{{ t('pages.settings.formatType.label') }}</Label>
                            <Select id="selectFormatType" v-model="settings.formatType" :option-list="separatorOptions" :placeholder="t('pages.settings.formatType.placeholder')" />
                            <FormInfoText>{{ t('pages.settings.formatType.note') }}</FormInfoText>
                        </div>

                        <!-- Matrix intensity -->
                        <div class="space-y-2">
                            <div class="flex items-center justify-between">
                                <Label for="matrixIntensity">{{ t('pages.settings.matrixIntensity.label') }}</Label>
                                <span class="text-sm font-semibold text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ matrixIntensityPercentage }}%</span>
                            </div>
                            <SliderInput id="matrixIntensity" v-model="settings.matrixIntensity" :min="0" :max="15" />
                            <FormInfoText class="mt-2">{{ t('pages.settings.matrixIntensity.note') }}</FormInfoText>
                        </div>

                        <!-- Scroll speed -->
                        <div class="space-y-2">
                            <div class="flex items-center justify-between">
                                <Label for="scrollSpeed">{{ t('pages.settings.scrollSpeed.label') }}</Label>
                                <span class="text-sm font-semibold text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ scrollSpeedPercentage }}%</span>
                            </div>
                            <SliderInput id="scrollSpeed" v-model="settings.scrollSpeed" :min="0" :max="15" />
                            <FormInfoText class="mt-2">{{ t('pages.settings.scrollSpeed.note') }}</FormInfoText>
                        </div>
                    </div>
                </Card>

                <!-- Action buttons -->
                <div class="flex flex-col gap-3 md:flex-row md:justify-end">
                    <Button type="primary" native-type="submit" class="w-full md:w-auto">{{ t('pages.settings.saveAction') }}</Button>
                    <Button type="secondary" class="w-full md:w-auto" @click="handleResetPress">{{ t('pages.settings.resetAction') }}</Button>
                </div>
            </form>
        </div>
    </div>
</template>