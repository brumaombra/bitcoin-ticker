<script setup>
import { computed, ref } from 'vue';
import { Info, Wifi } from 'lucide-vue-next';
import { connectToWiFi, getNetworks } from '~/composables/useDeviceApi.js';
import { handleBackendErrors, setBusy, showConfirmDialog, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import BackgroundGrid from '~/components/ui/BackgroundGrid.vue';
import BrandLogo from '~/components/ui/BrandLogo.vue';
import { Alert, AlertDescription, AlertTitle } from '~/components/shadcn/alert';
import { Button } from '~/components/shadcn/button';
import { Card, CardContent, CardHeader, CardFooter } from '~/components/shadcn/card';
import { Input } from '~/components/shadcn/input';
import { Label } from '~/components/shadcn/label';
import { Select, SelectContent, SelectItem, SelectTrigger, SelectValue } from '~/components/shadcn/select';
import CardHeaderWithIcon from '~/components/ui/CardHeaderWithIcon.vue';
import LanguageSelector from '~/components/ui/LanguageSelector.vue';
import ThemeSelector from '~/components/ui/ThemeSelector.vue';

const globalStore = useGlobalStore();
const ssid = ref('');
const password = ref('');
const isLoading = ref(false);
const { t } = useI18n();

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

// Ask for confirmation before disabling AP mode and joining the selected WiFi network
const handleConnectPress = () => {
    showConfirmDialog({
        title: t('pages.apSetup.connectConfirmTitle'),
        message: t('pages.apSetup.connectConfirmMessage', { ssid: ssid.value }),
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
</script>

<template>
    <div class="relative min-h-screen bg-[var(--bg-main-light)] text-[var(--text-primary-light)] dark:bg-[var(--bg-main-dark)] dark:text-[var(--text-primary-dark)]">
        <BackgroundGrid />

        <!-- Main content -->
        <main class="relative z-10 mx-auto flex min-h-screen w-full max-w-2xl items-center justify-center px-4 py-10 sm:px-6 lg:px-8">
            <div class="w-full space-y-6">
                <!-- Toolbar -->
                <div class="flex justify-end gap-3">
                    <LanguageSelector />
                    <ThemeSelector />
                </div>

                <!-- Brand -->
                <BrandLogo :brand-name="t('app.title')" />

                <!-- Setup card -->
                <Card>
                    <!-- Header with icon -->
                    <CardHeader>
                        <CardHeaderWithIcon :eyebrow="t('pages.apSetup.eyebrow')"
                            :title="t('pages.apSetup.title')"
                            :icon="Wifi"
                            :icon-label="t('nav.wifi.label')" />
                    </CardHeader>

                    <!-- Card content -->
                    <CardContent class="pt-0">
                        <form class="space-y-5" @submit.prevent="handleConnectPress">
                            <!-- Network selector -->
                            <div class="space-y-3">
                                <!-- Network header -->
                                <div class="flex items-end justify-between gap-3">
                                    <!-- Label -->
                                    <Label for="ssid">{{ t('pages.wifi.availableNetworks') }}</Label>

                                    <!-- Refresh button -->
                                    <Button type="button" variant="outline" :disabled="isLoading" @click="refreshSSIDList">
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
                            </div>

                            <!-- Password input -->
                            <div class="space-y-3">
                                <Label for="password">{{ t('pages.wifi.password') }}</Label>
                                <Input id="password" v-model="password" type="password" :placeholder="t('pages.wifi.passwordPlaceholder')" />
                            </div>

                            <!-- Connection note -->
                            <Alert>
                                <Info :stroke-width="1.8" />
                                <AlertTitle>{{ t('common.note') }}</AlertTitle>
                                <AlertDescription>
                                    {{ t('pages.wifi.note') }}
                                </AlertDescription>
                            </Alert>
                        </form>
                    </CardContent>

                    <!-- Card footer -->
                    <CardFooter>
                        <Button variant="default" type="button" class="w-full" :disabled="!ssid || !password" @click="handleConnectPress">{{ t('pages.wifi.connectAction') }}</Button>
                    </CardFooter>
                </Card>
            </div>
        </main>
    </div>
</template>