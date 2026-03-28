<script setup>
import { Settings } from 'lucide-vue-next';
import { computed } from 'vue';
import { resetSettings, saveSettings } from '~/composables/useDeviceApi.js';
import { getCryptoCoins, handleBackendErrors, setBusy, setCryptoCoin, showConfirmDialog, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import { Button } from '~/components/shadcn/button';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '~/components/shadcn/card';
import FormInfoText from '~/components/ui/FormInfoText.vue';
import PageIntroCard from '~/components/ui/PageIntroCard.vue';
import SettingToggleItem from '~/components/ui/SettingToggleItem.vue';
import { Slider } from '~/components/shadcn/slider';
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from '~/components/shadcn/select';
import { Label } from '~/components/shadcn/label';

const globalStore = useGlobalStore();
const cryptoCoins = getCryptoCoins();
const { t } = useI18n();

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
const separatorOptions = computed(() => ([
    { value: 'US', label: '21,000.00', meta: t('pages.settings.formatType.usMeta') },
    { value: 'EU', label: '21.000,00', meta: t('pages.settings.formatType.euMeta') }
]));

// Available crypto coin choices
const cryptoCoinOptions = computed(() => {
    return cryptoCoins.map(coin => ({
        value: coin.id,
        label: t(`pages.settings.cryptoCoin.${coin.id}Label`),
        meta: t(`pages.settings.cryptoCoin.${coin.id}Meta`)
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
        handleBackendErrors({ error, defaultMessage: t('pages.settings.saveError'), showDialog: true });
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
        handleBackendErrors({ error, defaultMessage: t('pages.settings.resetError'), showDialog: true });
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
            type: 'default'
        },
        cancelButton: {
            text: t('common.keepEditing'),
            type: 'outline'
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
            type: 'default'
        },
        cancelButton: {
            text: t('common.cancel'),
            type: 'outline'
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
    <div class="mx-auto flex w-full min-w-0 flex-col gap-6">
        <div class="grid min-w-0 gap-6 xl:grid-cols-[minmax(0,320px),minmax(0,1fr)]">
            <!-- Page intro -->
            <PageIntroCard class="min-w-0"
                :eyebrow="t('pages.settings.eyebrow')"
                :title="t('pages.settings.title')"
                :description="t('pages.settings.description')"
                :icon="Settings"
                :icon-label="t('nav.settings.label')" />

            <!-- Settings form -->
            <form class="min-w-0 space-y-6" @submit.prevent="handleSavePress">
                <!-- Visibility card -->
                <Card>
                    <!-- Visibility section -->
                    <CardHeader>
                        <CardTitle>{{ t('pages.settings.visibilityTitle') }}</CardTitle>
                        <CardDescription>{{ t('pages.settings.visibilityDescription') }}</CardDescription>
                    </CardHeader>

                    <!-- Visibility toggles -->
                    <CardContent class="space-y-3">
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
                    </CardContent>
                </Card>

                <!-- Formatting & motion card -->
                <Card>
                    <!-- Formatting section -->
                    <CardHeader>
                        <CardTitle>{{ t('pages.settings.formattingTitle') }}</CardTitle>
                        <CardDescription>{{ t('pages.settings.formattingDescription') }}</CardDescription>
                    </CardHeader>

                    <!-- Formatting controls -->
                    <CardContent class="space-y-6">
                        <!-- Crypto coin -->
                        <div class="space-y-2">
                            <Label for="cryptoCoin">{{ t('pages.settings.cryptoCoin.label') }}</Label>
                            <Select v-model="settings.cryptoCoin" @update:modelValue="setCryptoCoin">
                                <SelectTrigger id="cryptoCoin" class="w-full">
                                    <SelectValue :placeholder="t('pages.settings.cryptoCoin.placeholder')" />
                                </SelectTrigger>
                                <SelectContent>
                                    <SelectItem v-for="option in cryptoCoinOptions" :key="option.value" :value="option.value" :text-value="option.label">
                                        <div class="flex flex-col">
                                            <span>{{ option.label }}</span>
                                            <span class="text-xs text-muted-foreground">{{ option.meta }}</span>
                                        </div>
                                    </SelectItem>
                                </SelectContent>
                            </Select>
                            <FormInfoText>{{ t('pages.settings.cryptoCoin.note') }}</FormInfoText>
                        </div>

                        <!-- Thousands separator format -->
                        <div class="space-y-2">
                            <Label for="selectFormatType">{{ t('pages.settings.formatType.label') }}</Label>
                            <Select v-model="settings.formatType">
                                <SelectTrigger id="selectFormatType" class="w-full">
                                    <SelectValue :placeholder="t('pages.settings.formatType.placeholder')" />
                                </SelectTrigger>
                                <SelectContent>
                                    <SelectItem v-for="option in separatorOptions" :key="option.value" :value="option.value" :text-value="option.label">
                                        <div class="flex flex-col">
                                            <span>{{ option.label }}</span>
                                            <span class="text-xs text-muted-foreground">{{ option.meta }}</span>
                                        </div>
                                    </SelectItem>
                                </SelectContent>
                            </Select>
                            <FormInfoText>{{ t('pages.settings.formatType.note') }}</FormInfoText>
                        </div>

                        <!-- Matrix intensity -->
                        <div class="space-y-2">
                            <div class="flex items-center justify-between">
                                <Label for="matrixIntensity">{{ t('pages.settings.matrixIntensity.label') }}</Label>
                                <span class="text-sm font-semibold text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ matrixIntensityPercentage }}%</span>
                            </div>
                            <Slider :model-value="[settings.matrixIntensity]" :min="0" :max="15" @update:model-value="val => settings.matrixIntensity = val[0]" />
                            <FormInfoText class="mt-2">{{ t('pages.settings.matrixIntensity.note') }}</FormInfoText>
                        </div>

                        <!-- Scroll speed -->
                        <div class="space-y-2">
                            <div class="flex items-center justify-between">
                                <Label for="scrollSpeed">{{ t('pages.settings.scrollSpeed.label') }}</Label>
                                <span class="text-sm font-semibold text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">{{ scrollSpeedPercentage }}%</span>
                            </div>
                            <Slider :model-value="[settings.scrollSpeed]" :min="0" :max="15" @update:model-value="val => settings.scrollSpeed = val[0]" />
                            <FormInfoText class="mt-2">{{ t('pages.settings.scrollSpeed.note') }}</FormInfoText>
                        </div>
                    </CardContent>
                </Card>

                <!-- Action buttons -->
                <div class="flex flex-col gap-3 md:flex-row md:justify-end">
                    <Button variant="default" type="submit" class="w-full md:w-auto">{{ t('pages.settings.saveAction') }}</Button>
                    <Button variant="outline" type="button" class="w-full md:w-auto" @click="handleResetPress">{{ t('pages.settings.resetAction') }}</Button>
                </div>
            </form>
        </div>
    </div>
</template>