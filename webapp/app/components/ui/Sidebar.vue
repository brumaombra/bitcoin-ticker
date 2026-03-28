<script setup>
import { useRoute } from 'vue-router';
import {
    Sidebar as ShadcnSidebar,
    SidebarContent,
    SidebarGroup,
    SidebarGroupLabel,
    SidebarHeader,
    SidebarMenu,
    SidebarMenuButton,
    SidebarMenuItem
} from '~/components/shadcn/sidebar';
import BrandLogo from '~/components/ui/BrandLogo.vue';

const props = defineProps({
    brandName: { type: String, required: true },
    sidebarItems: { type: Array, default: () => [] }
});

const route = useRoute();
const localePath = useLocalePath();
const { t } = useI18n();

const isActiveItem = path => {
    return route.path === localePath(path);
};
</script>

<template>
    <ShadcnSidebar>
        <SidebarHeader class="border-b border-sidebar-border px-4 py-4 sm:px-6">
            <NuxtLink :to="localePath('/wifi')" class="flex min-h-12 items-center">
                <BrandLogo :brand-name="props.brandName" />
            </NuxtLink>
        </SidebarHeader>

        <SidebarContent class="py-4 sm:py-6">
            <SidebarGroup>
                <SidebarGroupLabel class="px-2 text-[0.65rem] font-semibold uppercase tracking-[0.24em] text-sidebar-foreground/60 sm:text-xs">
                    {{ t('common.controlPanel') }}
                </SidebarGroupLabel>

                <SidebarMenu class="mt-2 gap-2">
                    <SidebarMenuItem v-for="item in props.sidebarItems" :key="item.id">
                        <SidebarMenuButton as-child :is-active="isActiveItem(item.path)" class="h-auto min-h-12 items-start px-3 py-2.5 sm:px-4 sm:py-3">
                            <NuxtLink :to="localePath(item.path)">
                                <component :is="item.icon" :stroke-width="1.8" :aria-label="item.label" role="img" class="mt-0.5 h-4 w-4 shrink-0 sm:h-5 sm:w-5" />

                                <div class="grid min-w-0 flex-1 text-left leading-tight">
                                    <span class="truncate font-semibold">{{ item.label }}</span>
                                    <span v-if="item.description" class="mt-0.5 truncate text-xs text-sidebar-foreground/65">
                                        {{ item.description }}
                                    </span>
                                </div>
                            </NuxtLink>
                        </SidebarMenuButton>
                    </SidebarMenuItem>
                </SidebarMenu>
            </SidebarGroup>
        </SidebarContent>
    </ShadcnSidebar>
</template>