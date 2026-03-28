<script setup>
import { SidebarTrigger } from '~/components/shadcn/sidebar';
import BrandLogo from '~/components/ui/BrandLogo.vue';
import LanguageSelector from '~/components/ui/LanguageSelector.vue';
import ThemeSelector from '~/components/ui/ThemeSelector.vue';

const props = defineProps({
    brandName: { type: String, required: true },
    sidebarItems: { type: Array, default: () => [] }
});

const { t } = useI18n();
</script>

<template>
    <header class="sticky inset-x-0 top-0 z-30 h-20 border-b border-[var(--border-light)] bg-[color:color-mix(in_srgb,var(--bg-card-light)_84%,transparent)] backdrop-blur dark:border-[var(--border-dark)] dark:bg-[color:color-mix(in_srgb,var(--bg-card-dark)_84%,transparent)]">
        <nav class="mx-auto flex h-full max-w-6xl items-center justify-between px-3 sm:px-6 lg:px-8">
            <div class="flex items-center gap-3">
                <SidebarTrigger class="md:hidden" />

                <div class="flex flex-1 justify-start md:justify-center">
                    <BrandLogo :brand-name="props.brandName" compact />
                </div>
            </div>

            <div class="flex items-center gap-3">
                <LanguageSelector />
                <ThemeSelector />
            </div>
        </nav>
    </header>
</template>