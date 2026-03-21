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
import Input from '~/components/ui/Input.vue';
import Select from '~/components/ui/Select.vue';

const globalStore = useGlobalStore();
const ssid = ref('');
const password = ref('');
const isLoading = ref(false);

// Available network options
const networkOptions = computed(() => {
    return globalStore.value.networksList.map(network => ({
        value: network.ssid,
        label: network.ssid,
        meta: `${network.signal} dBm`
    }));
});

// Submit WiFi credentials to the device
const handleConnectPress = async () => {
    try {
        setBusy(true);
        await connectToWiFi(ssid.value, password.value);
        showMessage('Success', 'Success', 'You have successfully connected the device to the Wi-Fi network! The access point will be disabled. Enjoy the Bitcoin ticker!');
    } catch (error) {
        handleBackendErrors({ error, errorTranslated: 'An error occurred while connecting to the Wi-Fi network', errorMessage: 'An error occurred while connecting to the Wi-Fi network', showDialog: true });
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
                    <!-- Intro header -->
                    <div class="mb-6 flex items-center gap-4">
                        <!-- WiFi icon -->
                        <div class="flex h-14 w-14 items-center justify-center rounded border border-[var(--border-light)] bg-[var(--bg-selected-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)]">
                            <HugeiconsIcon :icon="Wifi01Icon" :size="28" color="currentColor" :stroke-width="1.8" aria-label="WiFi" role="img" class="h-7 w-7" />
                        </div>
                        <!-- Title copy -->
                        <div>
                            <div class="text-xs font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Access point mode</div>
                            <h1 class="mt-1 text-2xl font-bold">Connect to WiFi</h1>
                        </div>
                    </div>

                    <!-- Main form -->
                    <form class="mt-8 space-y-5" @submit.prevent="handleConnectPress">
                        <!-- Network selector -->
                        <div>
                            <!-- Network header -->
                            <div class="mb-2 flex items-center justify-between gap-3">
                                <!-- Label -->
                                <label for="ssid" class="text-sm font-medium">Available networks</label>

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
                        <div>
                            <label for="password" class="mb-2 block text-sm font-medium">Password</label>
                            <Input id="password" v-model="password" type="password" placeholder="Enter the WiFi password" />
                        </div>

                        <!-- Connection note -->
                        <div class="rounded border border-[var(--border-light)] bg-[var(--bg-selected-light)] px-4 py-3 text-sm text-[var(--text-secondary-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-secondary-dark)]">
                            The device will try the credentials immediately and disable its temporary hotspot after a successful connection.
                        </div>

                        <!-- Submit button -->
                        <Button type="primary" class="w-full" :disabled="!ssid || !password" @click="handleConnectPress">Connect Device</Button>
                    </form>
                </Card>
            </div>
        </main>
    </div>
</template>