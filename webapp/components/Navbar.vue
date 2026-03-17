<script setup>
import { ref } from 'vue';

// Mobile menu state
const isOpen = ref(false);

// Toggle the mobile navigation
const toggleMenu = () => {
    isOpen.value = !isOpen.value;
};
</script>

<template>
    <nav class="fixed top-0 w-full z-50 bg-gray-800 shadow-lg">
        <div class="mx-auto max-w-7xl px-2 sm:px-6 lg:px-8">
            <div class="relative flex h-16 items-center justify-between">
                <!-- Mobile menu button -->
                <div class="absolute inset-y-0 left-0 flex items-center sm:hidden">
                    <!-- Toggle button -->
                    <button type="button" class="relative inline-flex items-center justify-center rounded-md p-2 text-gray-400" aria-controls="mobile-menu" aria-expanded="false" @click="toggleMenu">
                        <span class="absolute -inset-0.5"></span>
                        <span class="sr-only">Open main menu</span>
                        <svg class="block size-6" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" aria-hidden="true" data-slot="icon">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 6.75h16.5M3.75 12h16.5m-16.5 5.25h16.5" />
                        </svg>
                        <svg class="hidden size-6" fill="none" viewBox="0 0 24 24" stroke-width="1.5" stroke="currentColor" aria-hidden="true" data-slot="icon">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M6 18 18 6M6 6l12 12" />
                        </svg>
                    </button>
                </div>

                <!-- Desktop menu -->
                <div class="flex flex-1 items-center justify-center sm:items-stretch sm:justify-start">
                    <!-- Brand -->
                    <div class="flex shrink-0 items-center">
                        <img class="h-8 w-auto" src="/svg/bitcoin.svg" alt="Bitcoin logo">
                    </div>
                    <!-- Navigation links -->
                    <div class="hidden sm:ml-6 sm:block">
                        <div class="flex space-x-4">
                            <NuxtLink to="/settings" class="rounded-md px-3 py-2 text-sm font-medium text-gray-300 hover:bg-gray-700 hover:text-white" active-class="bg-gray-700 text-white font-bold">Settings</NuxtLink>
                            <NuxtLink to="/wifi" class="rounded-md px-3 py-2 text-sm font-medium text-gray-300 hover:bg-gray-700 hover:text-white" active-class="bg-gray-700 text-white font-bold">WiFi</NuxtLink>
                            <NuxtLink to="/api-key" class="rounded-md px-3 py-2 text-sm font-medium text-gray-300 hover:bg-gray-700 hover:text-white" active-class="bg-gray-700 text-white font-bold">API Key</NuxtLink>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <!-- Mobile menu -->
        <div id="mobile-menu" class="sm:hidden" :class="{'block': isOpen, 'hidden': !isOpen}">
            <!-- Navigation links -->
            <div class="space-y-1 px-2 pb-3 pt-2">
                <NuxtLink to="/settings" class="block rounded-md px-3 py-2 text-base font-medium text-gray-300" active-class="text-white !font-extrabold" @click="toggleMenu">Settings</NuxtLink>
                <NuxtLink to="/wifi" class="block rounded-md px-3 py-2 text-base font-medium text-gray-300" active-class="text-white !font-extrabold" @click="toggleMenu">WiFi</NuxtLink>
                <NuxtLink to="/api-key" class="block rounded-md px-3 py-2 text-base font-medium text-gray-300" active-class="text-white !font-extrabold" @click="toggleMenu">API Key</NuxtLink>
            </div>
        </div>
    </nav>
</template>