<script setup>
import { ref } from 'vue';
import { Menu } from 'lucide-vue-next';
import IconButton from '~/components/ui/IconButton.vue';
import BrandLogo from '~/components/ui/BrandLogo.vue';
import LanguageSelector from '~/components/ui/LanguageSelector.vue';
import ThemeSelector from '~/components/ui/ThemeSelector.vue';
import Sidebar from '~/components/ui/navbar/Sidebar.vue';

// Props
const props = defineProps({
    brandName: { type: String, required: true },
    sidebarItems: { type: Array, default: () => [] }
});

const { t } = useI18n();
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
    <Sidebar :brand-name="props.brandName"
        :is-open="isSidebarOpen"
        :sidebar-items="props.sidebarItems"
        @close="handleCloseSidebar" />

    <!-- Top bar -->
    <header class="sticky inset-x-0 top-0 z-30 h-20 border-b border-[var(--border-light)] bg-[color:color-mix(in_srgb,var(--bg-card-light)_84%,transparent)] backdrop-blur xl:ps-72 dark:border-[var(--border-dark)] dark:bg-[color:color-mix(in_srgb,var(--bg-card-dark)_84%,transparent)]">
        <nav class="mx-auto flex h-full max-w-6xl items-center justify-between px-3 sm:px-6 lg:px-8">
            <!-- Left side -->
            <div class="flex items-center gap-3">
                <!-- Mobile sidebar toggle -->
                <div class="xl:hidden">
                    <IconButton :aria-label="t('nav.openNavigation')" @click="handleToggleSidebar">
                        <Menu :stroke-width="1.8" class="h-5 w-5" />
                    </IconButton>
                </div>

                <!-- Compact logo -->
                <div class="flex flex-1 justify-start xl:justify-center">
                    <BrandLogo :brand-name="props.brandName" compact />
                </div>
            </div>

            <!-- Right side -->
            <div class="flex items-center gap-3">
                <LanguageSelector />
                <ThemeSelector />
            </div>
        </nav>
    </header>
</template>