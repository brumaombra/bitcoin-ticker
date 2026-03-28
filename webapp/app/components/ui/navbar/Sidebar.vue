<script setup>
import { useRoute } from 'vue-router';
import BrandLogo from '~/components/ui/BrandLogo.vue';

// Props
const props = defineProps({
    brandName: { type: String, required: true },
    isOpen: { type: Boolean, required: true },
    sidebarItems: { type: Array, default: () => [] }
});

// Emits
const emits = defineEmits(['close']);

const route = useRoute();
const localePath = useLocalePath();
const { t } = useI18n();

// Check if a navigation item matches the current route
const isActiveItem = path => {
    return route.path === path;
};

// Resolve the style classes for each navigation item
const getNavigationItemClasses = path => {
    if (isActiveItem(path)) {
        return 'border-[var(--border-hover-light)] bg-[var(--bg-selected-light)] text-[var(--text-primary-light)] dark:border-[var(--border-hover-dark)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-primary-dark)]';
    } else {
        return 'border-[var(--border-light)] text-[var(--text-secondary-light)] hover:border-[var(--border-hover-light)] hover:bg-[var(--bg-selected-light)] hover:text-[var(--text-primary-light)] dark:border-[var(--border-dark)] dark:text-[var(--text-secondary-dark)] dark:hover:border-[var(--border-hover-dark)] dark:hover:bg-[var(--bg-selected-dark)] dark:hover:text-[var(--text-primary-dark)]';
    }
};

// Emit a close request for the mobile sidebar
const handleClose = () => {
    emits('close');
};
</script>

<template>
    <div>
        <Transition name="fade">
            <!-- Mobile backdrop -->
            <div v-if="props.isOpen" class="fixed inset-0 z-35 bg-black/45 xl:hidden" @click="handleClose"></div>
        </Transition>

        <!-- Sidebar panel -->
        <aside :class="['fixed left-0 top-0 z-40 flex h-screen w-64 flex-col transition-transform duration-300 sm:w-72', props.isOpen ? 'translate-x-0' : '-translate-x-full xl:translate-x-0']">
            <div class="flex h-full flex-col border-r border-[var(--border-light)] bg-[var(--bg-card-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)]">
                <!-- Sidebar header -->
                <div class="flex h-16 items-center border-b border-[var(--border-light)] px-4 my-2 sm:my-0 sm:h-20 sm:px-6 dark:border-[var(--border-dark)]">
                    <NuxtLink :to="localePath('/wifi')" @click="handleClose">
                        <BrandLogo :brand-name="props.brandName" />
                    </NuxtLink>
                </div>

                <!-- Sidebar navigation -->
                <nav class="flex-1 overflow-y-auto px-3 py-4 sm:px-4 sm:py-6">
                    <!-- Navigation section title -->
                    <div class="mb-3 px-2 text-[0.65rem] font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)] sm:mb-4 sm:text-xs">
                        {{ t('common.controlPanel') }}
                    </div>

                    <!-- Navigation items -->
                    <ul class="space-y-3">
                        <li v-for="item in props.sidebarItems" :key="item.id">
                            <NuxtLink :to="localePath(item.path)" class="group flex items-center gap-3 rounded border px-3 py-2.5 text-sm font-medium transition-all duration-200 sm:gap-4 sm:px-4 sm:py-3" :class="getNavigationItemClasses(item.path)" @click="handleClose">
                                <!-- Navigation icon -->
                                <component :is="item.icon" :stroke-width="1.8" :aria-label="item.label" role="img" class="h-4 w-4 shrink-0 opacity-90 transition-opacity duration-200 group-hover:opacity-100 sm:h-5 sm:w-5" />

                                <!-- Navigation text -->
                                <div class="min-w-0">
                                    <div class="truncate font-semibold">{{ item.label }}</div>
                                    <div v-if="item.description" class="mt-0.5 text-[0.7rem] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)] sm:mt-1 sm:text-xs">
                                        {{ item.description }}
                                    </div>
                                </div>
                            </NuxtLink>
                        </li>
                    </ul>
                </nav>
            </div>
        </aside>
    </div>
</template>

<style scoped>
/* Fade transition */
.fade-enter-active,
.fade-leave-active {
    transition: opacity 0.2s ease;
}

.fade-enter-from,
.fade-leave-to {
    opacity: 0;
}
</style>