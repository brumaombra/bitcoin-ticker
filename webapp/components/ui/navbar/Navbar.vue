<script setup>
import IconButton from '~/components/ui/IconButton.vue';
import ThemeSelector from '~/components/ui/ThemeSelector.vue';
import Sidebar from '~/components/ui/navbar/Sidebar.vue';

const props = defineProps({
    brandName: { type: String, required: true },
    sidebarItems: { type: Array, default: () => [] }
});

const isSidebarOpen = ref(false);

const handleToggleSidebar = () => {
    isSidebarOpen.value = !isSidebarOpen.value;
};

const handleCloseSidebar = () => {
    isSidebarOpen.value = false;
};
</script>

<template>
    <Sidebar :brand-name="props.brandName" :is-open="isSidebarOpen" :sidebar-items="props.sidebarItems" @close="handleCloseSidebar" />

    <header class="sticky inset-x-0 top-0 z-30 h-20 border-b border-[var(--border-light)] bg-[color:color-mix(in_srgb,var(--bg-card-light)_84%,transparent)] backdrop-blur xl:ps-72 dark:border-[var(--border-dark)] dark:bg-[color:color-mix(in_srgb,var(--bg-card-dark)_84%,transparent)]">
        <nav class="mx-auto flex h-full max-w-6xl items-center justify-between px-3 sm:px-6 lg:px-8">
            <div class="xl:hidden">
                <IconButton aria-label="Open navigation" @click="handleToggleSidebar">
                    <svg class="h-5 w-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8" aria-hidden="true">
                        <path stroke-linecap="round" stroke-linejoin="round" d="M3.75 6.75h16.5M3.75 12h16.5m-16.5 5.25h16.5" />
                    </svg>
                </IconButton>
            </div>

            <div class="min-w-0">
                <div class="text-xs font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
                    Bitcoin Ticker
                </div>
                <div class="mt-1 text-sm font-medium text-[var(--text-primary-light)] dark:text-[var(--text-primary-dark)]">
                    Configure your LED matrix device
                </div>
            </div>

            <div class="flex items-center gap-3">
                <ThemeSelector />
            </div>
        </nav>
    </header>
</template>