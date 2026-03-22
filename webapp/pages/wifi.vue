<script setup>
import { computed, ref } from 'vue';
import { HugeiconsIcon } from '@hugeicons/vue';
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
const connectDeviceToWiFi = async () => {
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

// Ask for confirmation before replacing the device WiFi connection
const handleConnectPress = () => {
    showConfirmDialog({
        title: 'Connect device to WiFi',
        message: `Send the selected credentials for ${ssid.value} to the device and switch it to that network?`,
        confirmButton: {
            text: 'Connect device',
            type: 'primary'
        },
        cancelButton: {
            text: 'Cancel',
            type: 'secondary'
        },
        onConfirm: connectDeviceToWiFi
    });
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

// Page metadata
definePageMeta({
    layout: 'private'
});
</script>

<template>
    <div class="mx-auto flex w-full flex-col gap-6 lg:flex-row">
        <!-- Page intro -->
        <div class="w-full lg:max-w-sm">
            <PageIntroCard eyebrow="Setup" title="Connect to WiFi" description="Join the device to your local network so it can fetch market data and keep the LED matrix updated.">
                <template #icon>
                    <HugeiconsIcon :icon="Wifi01Icon" :size="28" color="currentColor" :stroke-width="1.8" aria-label="WiFi" role="img" class="h-7 w-7" />
                </template>
            </PageIntroCard>
        </div>

        <!-- Connection form -->
        <div class="min-w-0 flex-1">
            <Card>
                <form class="space-y-5" @submit.prevent="handleConnectPress">
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
    </div>
</template>