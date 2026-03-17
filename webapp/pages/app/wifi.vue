<script setup>
import Button from '~/components/ui/Button.vue';
import Card from '~/components/ui/Card.vue';
import Input from '~/components/ui/Input.vue';
import Select from '~/components/ui/Select.vue';
import { useAppUi } from '~/composables/useAppUi.js';
import { useDeviceApi } from '~/composables/useDeviceApi.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

definePageMeta({
    layout: 'private'
});

const globalStore = useGlobalStore();
const { connectToWiFi, getNetworks } = useDeviceApi();
const { setBusy, showMessage } = useAppUi();

const ssid = ref('');
const password = ref('');
const isLoading = ref(false);

const networkOptions = computed(() => {
    return globalStore.value.networksList.map(network => ({
        value: network.ssid,
        label: network.ssid,
        meta: `${network.signal} dBm`
    }));
});

const handleConnectPress = async () => {
    try {
        setBusy(true);
        await connectToWiFi(ssid.value, password.value);
        showMessage('Success', 'Success', 'You have successfully connected the device to the Wi-Fi network! The access point will be disabled. Enjoy the Bitcoin ticker!');
    } catch (error) {
        console.error(error);
        showMessage('Error', 'Error', 'An error occurred while connecting to the Wi-Fi network, try with a different network or check the password');
    } finally {
        setBusy(false);
        password.value = '';
    }
};

const refreshSSIDList = async () => {
    isLoading.value = true;

    try {
        globalStore.value.networksList = await getNetworks();
    } catch (error) {
        console.error(error);
        showMessage('Error', 'Error', 'An error occurred while refreshing the SSID list');
    } finally {
        isLoading.value = false;
    }
};
</script>

<template>
    <div class="mx-auto flex w-full max-w-5xl flex-col gap-6 lg:flex-row">
        <div class="w-full lg:max-w-sm">
            <Card>
                <div class="mb-6 flex items-center gap-4">
                    <div class="flex h-14 w-14 items-center justify-center rounded border border-[var(--border-light)] bg-[var(--bg-selected-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)]">
                        <img src="/svg/wifi.svg" alt="WiFi icon" class="h-7 w-7">
                    </div>
                    <div>
                        <div class="text-xs font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">Setup</div>
                        <h1 class="mt-1 text-2xl font-bold">Connect to WiFi</h1>
                    </div>
                </div>

                <p class="text-sm leading-6 text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
                    Join the device to your local network so it can fetch market data and keep the LED matrix updated.
                </p>
            </Card>
        </div>

        <div class="min-w-0 flex-1">
            <Card>
                <form class="space-y-5" @submit.prevent="handleConnectPress">
                    <div>
                        <div class="mb-2 flex items-center justify-between gap-3">
                            <label for="ssid" class="text-sm font-medium">Available networks</label>
                            <Button type="secondary" :disabled="isLoading" @click="refreshSSIDList">
                                <span v-if="isLoading">Refreshing...</span>
                                <span v-else>Refresh</span>
                            </Button>
                        </div>

                        <Select id="ssid" v-model="ssid" placeholder="Select a network" :option-list="networkOptions" />
                    </div>

                    <div>
                        <label for="password" class="mb-2 block text-sm font-medium">Password</label>
                        <Input id="password" v-model="password" type="password" placeholder="Enter the WiFi password" />
                    </div>

                    <div class="rounded border border-[var(--border-light)] bg-[var(--bg-selected-light)] px-4 py-3 text-sm text-[var(--text-secondary-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-secondary-dark)]">
                        The device will try the credentials immediately and disable its temporary hotspot after a successful connection.
                    </div>

                    <Button type="primary" class="w-full" :disabled="!ssid || !password">Connect Device</Button>
                </form>
            </Card>
        </div>
    </div>
</template>