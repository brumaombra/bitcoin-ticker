<script setup>
import { useAppUi } from '~/composables/useAppUi.js';
import { useDeviceApi } from '~/composables/useDeviceApi.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

const globalStore = useGlobalStore();
const { connectToWiFi, getNetworks } = useDeviceApi();
const { setBusy, showMessage } = useAppUi();

const ssid = ref('');
const password = ref('');
const isLoading = ref(false);

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
    <div class="flex justify-center">
        <div class="w-full max-w-md rounded-lg bg-white p-8 shadow-lg">
            <div class="mb-6">
                <div class="mb-6 flex justify-center">
                    <img src="/wifi.svg" alt="Bitcoin logo" class="h-16 w-16">
                </div>
                <h1 class="text-center text-3xl font-bold text-gray-800">Connect to WiFi</h1>
            </div>

            <form @submit.prevent="handleConnectPress">
                <div class="mb-4">
                    <label for="ssid" class="mb-2 block text-sm font-medium text-gray-700">SSID</label>
                    <div class="flex items-center">
                        <select id="ssid" v-model="ssid" name="ssid" class="block w-full appearance-none rounded-lg border border-gray-300 bg-white bg-size-[20px] bg-position-[right_12px_center] bg-no-repeat px-4 py-3 pr-10 text-gray-900 hover:cursor-pointer focus:border-gray-500 focus:outline-none focus:ring-2 focus:ring-gray-500" style="background-image: url(&quot;data:image/svg+xml;charset=US-ASCII,%3Csvg%20xmlns%3D%22http%3A%2F%2Fwww.w3.org%2F2000%2Fsvg%22%20width%3D%2220%22%20height%3D%2220%22%20viewBox%3D%220%200%2020%2020%22%3E%3Cpath%20fill%3D%22%23666%22%20d%3D%22M5.293%208.293a1%201%200%20011.414%200L10%2011.586l3.293-3.293a1%201%200%20111.414%201.414l-4%204a1%201%200%2001-1.414%200l-4-4a1%201%200%20010-1.414z%22%2F%3E%3C%2Fsvg%3E&quot;)">
                            <option value="">Select a network</option>
                            <option v-for="network in globalStore.networksList" :key="network.ssid" :value="network.ssid">
                                {{ network.ssid }} ({{ network.signal }} dBm)
                            </option>
                        </select>

                        <button type="button" class="ml-2 rounded-lg border border-transparent bg-gray-800 px-4 py-3 text-sm font-medium text-white hover:bg-gray-600" @click="refreshSSIDList">
                            <div v-if="isLoading">
                                <svg class="h-5 w-5 animate-spin text-white" xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24">
                                    <circle class="opacity-25" cx="12" cy="12" r="10" stroke="currentColor" stroke-width="4"></circle>
                                    <path class="opacity-75" fill="currentColor" d="M4 12a8 8 0 018-8V0C5.373 0 0 5.373 0 12h4zm2 5.291A7.962 7.962 0 014 12H0c0 3.042 1.135 5.824 3 7.938l3-2.647z"></path>
                                </svg>
                            </div>
                            <span v-else>Refresh</span>
                        </button>
                    </div>
                </div>

                <div class="mb-4">
                    <label for="password" class="mb-2 block text-sm font-medium text-gray-700">Password</label>
                    <input id="password" v-model="password" type="password" name="password" class="block w-full rounded-lg border border-gray-300 px-4 py-3 focus:border-gray-500 focus:outline-none focus:ring-2 focus:ring-gray-500" placeholder="Enter your WiFi password">
                </div>

                <button type="submit" class="w-full rounded-lg border border-transparent bg-gray-800 px-4 py-3 text-sm font-medium text-white hover:bg-gray-600">
                    Connect
                </button>
            </form>
        </div>
    </div>
</template>