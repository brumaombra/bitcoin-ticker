<script setup>
import { computed } from 'vue';
import { useRoute } from 'vue-router';
import { KeyRound, Settings, Wifi } from 'lucide-vue-next';
import { Separator } from '~/components/shadcn/separator';
import {
    Sidebar as ShadcnSidebar,
    SidebarContent,
    SidebarFooter,
    SidebarGroup,
    SidebarGroupContent,
    SidebarGroupLabel,
    SidebarHeader,
    SidebarInset,
    SidebarMenu,
    SidebarMenuButton,
    SidebarMenuItem,
    SidebarProvider,
    SidebarRail,
    SidebarTrigger
} from '~/components/shadcn/sidebar';
import CryptoLogo from '~/components/ui/crypto/CryptoLogo.vue';
import LanguageSelector from '~/components/ui/LanguageSelector.vue';
import ThemeSelector from '~/components/ui/ThemeSelector.vue';

const { t } = useI18n();
const route = useRoute();
const localePath = useLocalePath();

// Navigation items for the private control panel
const sidebarItems = computed(() => ([{
    id: 'wifi',
    path: '/wifi',
    label: t('nav.wifi.label'),
    description: t('nav.wifi.description'),
    icon: Wifi
}, {
    id: 'settings',
    path: '/settings',
    label: t('nav.settings.label'),
    description: t('nav.settings.description'),
    icon: Settings
}, {
    id: 'api-key',
    path: '/api-key',
    label: t('nav.apiKey.label'),
    description: t('nav.apiKey.description'),
    icon: KeyRound
}]));

const activeItem = computed(() => {
    return sidebarItems.value.find(item => route.path === localePath(item.path)) || null;
});
</script>

<template>
    <SidebarProvider>
        <ShadcnSidebar collapsible="offcanvas">
            <SidebarHeader class="h-16 justify-center border-b border-sidebar-border">
                <SidebarMenu>
                    <SidebarMenuItem>
                        <SidebarMenuButton as-child size="lg" class="rounded">
                            <NuxtLink :to="localePath('/')">
                                <div class="bg-sidebar-primary text-sidebar-primary-foreground flex aspect-square size-8 items-center justify-center rounded">
                                    <CryptoLogo class="size-5 rounded" />
                                </div>

                                <div class="grid min-w-0 flex-1 text-left text-sm leading-tight">
                                    <span class="truncate font-semibold">{{ t('app.title') }}</span>
                                    <span class="truncate text-xs text-sidebar-foreground/70">{{ t('common.deviceUi') }}</span>
                                </div>
                            </NuxtLink>
                        </SidebarMenuButton>
                    </SidebarMenuItem>
                </SidebarMenu>
            </SidebarHeader>

            <SidebarContent>
                <SidebarGroup>
                    <SidebarGroupLabel>{{ t('common.controlPanel') }}</SidebarGroupLabel>

                    <SidebarGroupContent>
                        <SidebarMenu class="gap-3">
                            <SidebarMenuItem v-for="item in sidebarItems" :key="item.id">
                                <SidebarMenuButton as-child :is-active="route.path === localePath(item.path)" class="group h-auto min-h-12 items-start rounded border border-[var(--border-light)] px-3 py-2.5 text-[var(--text-secondary-light)] transition-all duration-200 hover:border-[var(--border-hover-light)] hover:bg-[var(--bg-selected-light)] hover:text-[var(--text-primary-light)] data-[active=true]:border-[var(--border-hover-light)] data-[active=true]:bg-[var(--bg-selected-light)] data-[active=true]:text-[var(--text-primary-light)] dark:border-[var(--border-dark)] dark:text-[var(--text-secondary-dark)] dark:hover:border-[var(--border-hover-dark)] dark:hover:bg-[var(--bg-selected-dark)] dark:hover:text-[var(--text-primary-dark)] dark:data-[active=true]:border-[var(--border-hover-dark)] dark:data-[active=true]:bg-[var(--bg-selected-dark)] dark:data-[active=true]:text-[var(--text-primary-dark)] sm:px-4 sm:py-3">
                                    <NuxtLink :to="localePath(item.path)">
                                        <component :is="item.icon" :stroke-width="1.8" class="mt-0.5 h-4 w-4 shrink-0 opacity-90 transition-opacity duration-200 group-hover:opacity-100 sm:h-5 sm:w-5" />

                                        <div class="grid min-w-0 flex-1 text-left leading-tight">
                                            <span class="truncate font-semibold">{{ item.label }}</span>
                                            <span v-if="item.description" class="mt-0.5 text-[11px] leading-4 text-sidebar-foreground/65">
                                                {{ item.description }}
                                            </span>
                                        </div>
                                    </NuxtLink>
                                </SidebarMenuButton>
                            </SidebarMenuItem>
                        </SidebarMenu>
                    </SidebarGroupContent>
                </SidebarGroup>
            </SidebarContent>

            <SidebarFooter class="border-t border-sidebar-border">
                <div class="flex items-center gap-2 px-2">
                    <LanguageSelector />
                    <ThemeSelector />
                </div>
            </SidebarFooter>

            <SidebarRail />
        </ShadcnSidebar>

        <SidebarInset class="min-h-screen min-w-0 bg-background text-foreground">
            <header class="sticky top-0 z-20 flex h-16 shrink-0 items-center gap-2 border-b bg-background/80 px-4 backdrop-blur">
                <SidebarTrigger class="-ml-1" />
                <Separator orientation="vertical" class="mr-2 data-[orientation=vertical]:h-4" />

                <div class="min-w-0 flex-1">
                    <div class="truncate text-sm font-medium">
                        {{ activeItem?.label || t('common.controlPanel') }}
                    </div>
                    <div v-if="activeItem?.description" class="truncate text-xs text-muted-foreground">
                        {{ activeItem.description }}
                    </div>
                </div>
            </header>

            <main class="min-w-0 flex-1">
                <div class="mx-auto flex w-full min-w-0 max-w-6xl flex-col gap-6 p-4 sm:p-6 lg:p-8">
                    <div class="pb-10">
                        <slot />
                    </div>
                </div>
            </main>
        </SidebarInset>
    </SidebarProvider>
</template>