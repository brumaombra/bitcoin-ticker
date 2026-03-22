<script setup>
import { computed, ref } from 'vue';
import { HugeiconsIcon } from '@hugeicons/vue';
import { Wifi01Icon } from '@hugeicons/core-free-icons';
import { connectToWiFi, getNetworks } from '~/composables/useDeviceApi.js';
import { handleBackendErrors, setBusy, showMessage } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import BackgroundGrid from '~/components/ui/BackgroundGrid.vue';
import BrandLogo from '~/components/ui/BrandLogo.vue';
import Button from '~/components/ui/Button.vue';
import Card from '~/components/ui/Card.vue';
import InfoBox from '~/components/ui/InfoBox.vue';
import Input from '~/components/ui/Input.vue';
import Label from '~/components/ui/Label.vue';
import Select from '~/components/ui/Select.vue';
import CardHeaderWithIcon from '~/components/ui/CardHeaderWithIcon.vue';

const globalStore = useGlobalStore();
const ssid = ref('');
const password = ref('');
const isLoading = ref(false);

// Available network options
const networkOptions = computed(() => {
    return globalStore.value.networksList.map(network => ({
        value: network.ssid,
        label: network.ssid,
        meta: `${network.signal} dBm · Ch ${network.channel} · ${network.quality}% · ${network.secured ? 'Secured' : 'Open'}`
    }));
});

// Submit WiFi credentials to the device
const handleConnectPress = async () => {
    try {
        setBusy(true);
        const result = await connectToWiFi(ssid.value, password.value);
        showMessage({
            type: 'Info',
            title: 'Device connected',
            message: `The microcontroller connected to ${result.ssid || ssid.value} and is available at ${result.hostname} and ${result.ip}.`
        });
    } catch (error) {
        handleBackendErrors({
            error,
            errorTranslated: error instanceof Error ? error.message : 'An error occurred while connecting to the Wi-Fi network',
            errorMessage: error instanceof Error ? error.message : 'An error occurred while connecting to the Wi-Fi network',
            showDialog: true
        });
    } finally {
        setBusy(false);
        password.value = '';
    }
};

// Refresh the detected SSID list
const refreshSSIDList = async () => {
    isLoading.value = true;

    try {
        globalStore.value.networksList = await getNetworks();
    } catch (error) {
        handleBackendErrors({ error, errorTranslated: 'An error occurred while refreshing the SSID list', errorMessage: 'An error occurred while refreshing the SSID list', showDialog: true });
    } finally {
        isLoading.value = false;
    }
};
</script>

<template>
    <div class="relative min-h-screen bg-[var(--bg-main-light)] text-[var(--text-primary-light)] dark:bg-[var(--bg-main-dark)] dark:text-[var(--text-primary-dark)]">
        <!-- Background grid -->
        <BackgroundGrid />

        <!-- Main content -->
        <main class="relative z-10 mx-auto flex min-h-screen w-full max-w-2xl items-center justify-center px-4 py-10 sm:px-6 lg:px-8">
            <div class="w-full space-y-6">
                <!-- Brand -->
                <BrandLogo brand-name="Bitcoin Ticker" />

                <!-- Setup card -->
                <Card>
                    <!-- Header with icon -->
                    <CardHeaderWithIcon eyebrow="Access point mode" title="Connect to WiFi">
                        <template #icon>
                            <HugeiconsIcon :icon="Wifi01Icon" :size="28" color="currentColor" :stroke-width="1.8" aria-label="WiFi" role="img" class="h-7 w-7" />
                        </template>
                    </CardHeaderWithIcon>

                    <!-- Main form -->
                    <form class="mt-8 space-y-5" @submit.prevent="handleConnectPress">
                        <!-- Network selector -->
                        <div class="space-y-2">
                            <!-- Network header -->
                            <div class="flex items-center justify-between gap-3">
                                <!-- Label -->
                                <Label for="ssid">Available networks</Label>

                                <!-- Refresh button -->
                                <Button type="secondary" :disabled="isLoading" @click="refreshSSIDList">
                                    <span v-if="isLoading">Refreshing...</span>
                                    <span v-else>Refresh</span>
                                </Button>
                            </div>

                            <!-- Network select -->
                            <Select id="ssid" v-model="ssid" placeholder="Select a network" :option-list="networkOptions" />
                        </div>

                        <!-- Password input -->
                        <div class="space-y-2">
                            <Label for="password">Password</Label>
                            <Input id="password" v-model="password" type="password" placeholder="Enter the WiFi password" />
                        </div>

                        <!-- Connection note -->
                        <InfoBox>
                            The device will try the credentials immediately and disable its temporary hotspot after a successful connection.
                        </InfoBox>

                        <!-- Submit button -->
                        <Button type="primary" native-type="submit" class="w-full" :disabled="!ssid || !password">Connect device</Button>
                    </form>
                </Card>
            </div>
        </main>
    </div>
</template>