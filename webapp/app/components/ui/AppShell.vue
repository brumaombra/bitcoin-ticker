<script setup>
import { SidebarInset, SidebarProvider } from '~/components/shadcn/sidebar';
import BackgroundGrid from '~/components/ui/BackgroundGrid.vue';
import Navbar from '~/components/ui/Navbar.vue';
import Sidebar from '~/components/ui/Sidebar.vue';

const props = defineProps({
    brandName: { type: String, required: true },
    sidebarItems: { type: Array, default: () => [] }
});
</script>

<template>
    <SidebarProvider>
        <Sidebar :brand-name="props.brandName" :sidebar-items="props.sidebarItems" />

        <SidebarInset class="relative min-h-screen min-w-0 overflow-x-clip bg-[var(--bg-main-light)] text-[var(--text-primary-light)] dark:bg-[var(--bg-main-dark)] dark:text-[var(--text-primary-dark)]">
            <BackgroundGrid />

            <Navbar :brand-name="props.brandName"
                :sidebar-items="props.sidebarItems" />

            <main class="relative z-10 min-w-0 flex-1">
                <div class="mx-auto flex min-h-[calc(100vh-5rem)] w-full min-w-0 max-w-6xl flex-col px-3 py-6 sm:px-6 lg:px-8 lg:py-10">
                    <slot></slot>
                </div>
            </main>
        </SidebarInset>
    </SidebarProvider>
</template>