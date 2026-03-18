<script setup>
import BackgroundGrid from '~/components/ui/BackgroundGrid.vue';
import Navbar from '~/components/ui/navbar/Navbar.vue';

// Props
const props = defineProps({
    brandName: { type: String, required: true },
    sidebarItems: { type: Array, default: () => [] }
});
</script>

<template>
    <div class="relative min-h-screen bg-[var(--bg-main-light)] text-[var(--text-primary-light)] dark:bg-[var(--bg-main-dark)] dark:text-[var(--text-primary-dark)]">
        <!-- Background grid -->
        <BackgroundGrid />

        <!-- Ambient overlay -->
        <div class="pointer-events-none absolute inset-0 bg-[radial-gradient(circle_at_top_left,rgba(247,147,26,0.14),transparent_30%),linear-gradient(180deg,transparent,rgba(0,0,0,0.02))] dark:bg-[radial-gradient(circle_at_top_left,rgba(247,147,26,0.16),transparent_28%),linear-gradient(180deg,transparent,rgba(255,255,255,0.02))]"></div>

        <!-- Top navigation -->
        <Navbar :brand-name="props.brandName"
            :sidebar-items="props.sidebarItems" />

        <!-- Main content -->
        <main class="relative z-10 xl:ps-72">
            <div class="mx-auto flex min-h-[calc(100vh-5rem)] w-full max-w-6xl flex-col px-3 py-6 sm:px-6 lg:px-8 lg:py-10">
                <slot></slot>
            </div>
        </main>
    </div>
</template>