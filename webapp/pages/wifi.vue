<script setup>
import { computed, ref } from 'vue';
import { Wifi01Icon } from '@hugeicons/core-free-icons';
import { connectToWiFi, getNetworks } from '~/composables/useDeviceApi.js';
import { handleBackendErrors, setBusy, showConfirmDialog, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import Button from '~/components/ui/Button.vue';
import Card from '~/components/ui/Card.vue';
import InfoBox from '~/components/ui/InfoBox.vue';
import Input from '~/components/ui/Input.vue';
import Label from '~/components/ui/Label.vue';
import Select from '~/components/ui/Select.vue';
import PageIntroCard from '~/components/ui/PageIntroCard.vue';
import WifiStatusPanel from '~/components/WifiStatusPanel.vue';

const globalStore = useGlobalStore();
const ssid = ref('');
const password = ref('');
const isLoading = ref(false);
const { t } = useI18n();

// Device WiFi status summary for the card header
const currentNetworkLabel = computed(() => {
    return globalStore.value.currentNetworkSsid || t('pages.wifi.notConnected');
});

// Number of scanned networks reported by the ESP
const scannedNetworksCount = computed(() => {
    return Number(globalStore.value.networksCount || 0);
});

// Available network options
const networkOptions = computed(() => {
    const networks = Array.isArray(globalStore.value?.networksList) ? globalStore.value.networksList : [];
    return networks.map(network => ({
        value: network.ssid,
        label: network.ssid,
        meta: `${network.signal} dBm · Ch ${network.channel} · ${network.quality}% · ${network.secured ? t('pages.wifi.secured') : t('pages.wifi.open')}`
    }));
});

// Submit WiFi credentials to the device
const connectDeviceToWiFi = async () => {
    try {
        setBusy(true);
        const result = await connectToWiFi(ssid.value, password.value);
        showMessage({
            type: 'Info',
            title: t('pages.wifi.connectedTitle'),
            message: t('pages.wifi.connectedMessage', { ssid: result.ssid || ssid.value, hostname: result.hostname, ip: result.ip })
        });
    } catch (error) {
        handleBackendErrors({ error, defaultMessage: t('pages.wifi.connectError'), showDialog: true });
    } finally {
        setBusy(false);
        password.value = '';
    }
};

// Ask for confirmation before replacing the device WiFi connection
const handleConnectPress = () => {
    showConfirmDialog({
        title: t('pages.wifi.connectConfirmTitle'),
        message: t('pages.wifi.connectConfirmMessage', { ssid: ssid.value }),
        confirmButton: {
            text: t('pages.wifi.connectAction'),
            type: 'primary'
        },
        cancelButton: {
            text: t('common.cancel'),
            type: 'secondary'
        },
        onConfirm: connectDeviceToWiFi
    });
};

// Refresh the detected SSID list
const refreshSSIDList = async () => {
    isLoading.value = true;

    try {
        const networksData = await getNetworks();
        globalStore.value.networksList = networksData.networks;
        globalStore.value.networksCount = networksData.count;
        globalStore.value.currentNetworkSsid = networksData.currentSsid;
    } catch (error) {
        handleBackendErrors({ error, defaultMessage: t('pages.wifi.refreshError'), showDialog: true });
    } finally {
        isLoading.value = false;
    }
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
            <PageIntroCard :eyebrow="t('pages.wifi.eyebrow')"
                :title="t('pages.wifi.title')"
                :description="t('pages.wifi.description')"
                :icon="Wifi01Icon"
                :icon-label="t('nav.wifi.label')" />
        </div>

        <!-- Connection form -->
        <div class="min-w-0 flex-1">
            <Card>
                <form class="space-y-5" @submit.prevent="handleConnectPress">
                    <!-- Current network summary -->
                    <WifiStatusPanel :current-network-label="currentNetworkLabel"
                        :scanned-networks-count="scannedNetworksCount" />

                    <!-- Network selector -->
                    <div class="space-y-2">
                        <!-- Network header -->
                        <div class="flex items-center justify-between gap-3">
                            <!-- Label -->
                            <Label for="ssid">{{ t('pages.wifi.availableNetworks') }}</Label>

                            <!-- Refresh button -->
                            <Button type="secondary" :disabled="isLoading" @click="refreshSSIDList">
                                <span v-if="isLoading">{{ t('common.refreshing') }}</span>
                                <span v-else>{{ t('common.refresh') }}</span>
                            </Button>
                        </div>

                        <!-- Network select -->
                        <Select id="ssid" v-model="ssid" :placeholder="t('pages.wifi.networkPlaceholder')" :option-list="networkOptions" />
                    </div>

                    <!-- Password input -->
                    <div class="space-y-2">
                        <Label for="password">{{ t('pages.wifi.password') }}</Label>
                        <Input id="password" v-model="password" type="password" :placeholder="t('pages.wifi.passwordPlaceholder')" />
                    </div>

                    <!-- Connection note -->
                    <InfoBox>
                        {{ t('pages.wifi.note') }}
                    </InfoBox>

                    <!-- Submit button -->
                    <Button type="primary" native-type="submit" class="w-full" :disabled="!ssid || !password">{{ t('pages.wifi.connectAction') }}</Button>
                </form>
            </Card>
        </div>
    </div>
</template>