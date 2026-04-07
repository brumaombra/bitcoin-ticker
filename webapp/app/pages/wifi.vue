<script setup>
import { computed, ref } from 'vue';
import { CheckCircle2, Info, RefreshCw, Wifi, X } from 'lucide-vue-next';
import { Alert, AlertDescription, AlertTitle } from 'theme-vintage/alert';
import { setBusy } from 'theme-vintage/busy-indicator';
import { Button } from 'theme-vintage/button';
import { Card, CardContent, CardFooter } from 'theme-vintage/card';
import { showConfirmDialog } from 'theme-vintage/confirm-dialog';
import { showMessageDialog } from 'theme-vintage/message-dialog';
import { Field, FieldGroup, FieldLabel } from 'theme-vintage/field';
import { Input } from 'theme-vintage/input';
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from 'theme-vintage/select';
import { connectToWiFi, getNetworks } from '~/composables/useDeviceApi.js';
import { handleBackendErrors } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import PageIntroCard from '~/components/PageIntroCard.vue';
import WifiStatusPanel from '~/components/WifiStatusPanel.vue';

const globalStore = useGlobalStore();
const ssid = ref('');
const password = ref('');
const isLoading = ref(false);
const { t } = useI18n();

// Device WiFi status summary for the card header
const currentNetworkLabel = computed(() => {
    return globalStore.value.config.ssid || t('pages.wifi.notConnected');
});

// Number of scanned networks reported by the ESP
const scannedNetworksCount = computed(() => {
    return Array.isArray(globalStore.value.networksList) ? globalStore.value.networksList.length : 0;
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
        globalStore.value.config.ssid = result.ssid || ssid.value;
        showMessageDialog({
            type: 'Info',
            icon: CheckCircle2,
            title: t('pages.wifi.connectedTitle'),
            message: t('pages.wifi.connectedMessage', { ssid: result.ssid || ssid.value, hostname: result.hostname, ip: result.ip }),
            closeText: t('common.close'),
            closeButtonIcon: X
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
        icon: Wifi,
        title: t('pages.wifi.connectConfirmTitle'),
        message: t('pages.wifi.connectConfirmMessage', { ssid: ssid.value }),
        confirmText: t('pages.wifi.connectAction'),
        cancelText: t('common.cancel'),
        confirmButtonType: 'default',
        cancelButtonType: 'outline',
        confirmButtonIcon: Wifi,
        cancelButtonIcon: X,
        onConfirm: connectDeviceToWiFi
    });
};

// Refresh the detected SSID list
const refreshSSIDList = async () => {
    isLoading.value = true;

    try {
        const networksData = await getNetworks();
        globalStore.value.networksList = networksData.networks;
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
            <PageIntroCard :eyebrow="t('pages.wifi.eyebrow')" :title="t('pages.wifi.title')" :description="t('pages.wifi.description')" :icon="Wifi" :icon-label="t('nav.wifi.label')" />
        </div>

        <!-- Connection form -->
        <div class="min-w-0 flex-1">
            <Card>
                <!-- Card content -->
                <CardContent>
                    <form @submit.prevent="handleConnectPress">
                        <FieldGroup>
                            <!-- Current network summary -->
                            <WifiStatusPanel :current-network-label="currentNetworkLabel"
                                :scanned-networks-count="scannedNetworksCount" />

                            <!-- Network selector -->
                            <Field>
                                <!-- Network header -->
                                <div class="flex items-end justify-between gap-3">
                                    <!-- Label -->
                                    <FieldLabel for="ssid">{{ t('pages.wifi.availableNetworks') }}</FieldLabel>

                                    <!-- Refresh button -->
                                    <Button variant="outline" :disabled="isLoading" @click="refreshSSIDList">
                                        <RefreshCw :stroke-width="1.8" class="size-4" :class="isLoading ? 'animate-spin' : ''" />
                                        <span v-if="isLoading">{{ t('common.refreshing') }}</span>
                                        <span v-else>{{ t('common.refresh') }}</span>
                                    </Button>
                                </div>

                                <!-- Network select -->
                                <Select v-model="ssid">
                                    <SelectTrigger id="ssid" class="w-full">
                                        <SelectValue :placeholder="t('pages.wifi.networkPlaceholder')" />
                                    </SelectTrigger>
                                    <SelectContent>
                                        <SelectItem v-for="option in networkOptions" :key="option.value" :value="option.value" :text-value="option.label">
                                            <div class="flex flex-col">
                                                <span>{{ option.label }}</span>
                                                <span class="text-xs text-muted-foreground">{{ option.meta }}</span>
                                            </div>
                                        </SelectItem>
                                    </SelectContent>
                                </Select>
                            </Field>

                            <!-- Password input -->
                            <Field>
                                <FieldLabel for="password">{{ t('pages.wifi.password') }}</FieldLabel>
                                <Input id="password" v-model="password" type="password" :placeholder="t('pages.wifi.passwordPlaceholder')" />
                            </Field>

                            <!-- Connection note -->
                            <Alert>
                                <Info :stroke-width="1.8" />
                                <AlertTitle>{{ t('common.note') }}</AlertTitle>
                                <AlertDescription>
                                    {{ t('pages.wifi.note') }}
                                </AlertDescription>
                            </Alert>
                        </FieldGroup>
                    </form>
                </CardContent>

                <!-- Card footer -->
                <CardFooter>
                    <!-- Connect button -->
                    <Button variant="default" type="button" class="w-full" :disabled="!ssid || !password" @click="handleConnectPress">
                        <Wifi :stroke-width="1.8" class="size-4" />
                        {{ t('pages.wifi.connectAction') }}
                    </Button>
                </CardFooter>
            </Card>
        </div>
    </div>
</template>