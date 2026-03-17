<script setup>
import { ref } from 'vue';
import CustomIcon from '~/components/ui/CustomIcon.vue';
import IconButton from '~/components/ui/IconButton.vue';
import BrandLogo from '~/components/ui/BrandLogo.vue';
import ThemeSelector from '~/components/ui/ThemeSelector.vue';
import Sidebar from '~/components/ui/navbar/Sidebar.vue';

// Props
const props = defineProps({
    brandName: { type: String, required: true },
    sidebarItems: { type: Array, default: () => [] }
});

// Mobile sidebar state
const isSidebarOpen = ref(false);

// Toggle the mobile sidebar
const handleToggleSidebar = () => {
    isSidebarOpen.value = !isSidebarOpen.value;
};

// Close the mobile sidebar
const handleCloseSidebar = () => {
    isSidebarOpen.value = false;
};
</script>

<template>
    <!-- Sidebar -->
    <Sidebar :brand-name="props.brandName" :is-open="isSidebarOpen" :sidebar-items="props.sidebarItems" @close="handleCloseSidebar" />

    <!-- Top bar -->
    <header class="sticky inset-x-0 top-0 z-30 h-20 border-b border-[var(--border-light)] bg-[color:color-mix(in_srgb,var(--bg-card-light)_84%,transparent)] backdrop-blur xl:ps-72 dark:border-[var(--border-dark)] dark:bg-[color:color-mix(in_srgb,var(--bg-card-dark)_84%,transparent)]">
        <nav class="mx-auto flex h-full max-w-6xl items-center justify-between px-3 sm:px-6 lg:px-8">
            <!-- Mobile sidebar toggle -->
            <div class="xl:hidden">
                <IconButton aria-label="Open navigation" @click="handleToggleSidebar">
                    <CustomIcon icon="menu" class="h-5 w-5" />
                </IconButton>
            </div>

            <!-- Compact logo -->
            <div class="flex flex-1 justify-center">
                <BrandLogo :brand-name="props.brandName" compact />
            </div>

            <!-- Toolbar actions -->
            <div class="flex items-center gap-3">
                <ThemeSelector />
            </div>
        </nav>
    </header>
</template>