<script setup>
import { useRoute } from 'vue-router';
import BrandLogo from '~/components/ui/BrandLogo.vue';

const props = defineProps({
    brandName: { type: String, required: true },
    isOpen: { type: Boolean, required: true },
    sidebarItems: { type: Array, default: () => [] }
});

const emits = defineEmits(['close']);
const route = useRoute();

const isActiveItem = path => {
    return route.path === path;
};

const handleClose = () => {
    emits('close');
};
</script>

<template>
    <div>
        <Transition name="fade">
            <div v-if="props.isOpen" class="fixed inset-0 z-35 bg-black/45 xl:hidden" @click="handleClose"></div>
        </Transition>

        <aside :class="['fixed left-0 top-0 z-40 flex h-screen w-72 flex-col transition-transform duration-300', props.isOpen ? 'translate-x-0' : '-translate-x-full xl:translate-x-0']">
            <div class="flex h-full flex-col border-r border-[var(--border-light)] bg-[var(--bg-card-light)] dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)]">
                <div class="flex h-20 items-center border-b border-[var(--border-light)] px-6 dark:border-[var(--border-dark)]">
                    <NuxtLink to="/app/wifi" @click="handleClose">
                        <BrandLogo :brand-name="props.brandName" />
                    </NuxtLink>
                </div>

                <nav class="flex-1 overflow-y-auto px-4 py-6">
                    <div class="mb-4 px-2 text-xs font-semibold uppercase tracking-[0.24em] text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
                        Control Panel
                    </div>

                    <ul class="space-y-3">
                        <li v-for="item in props.sidebarItems" :key="item.id">
                            <NuxtLink :to="item.path"
                                class="group flex items-center gap-4 rounded border px-4 py-3 text-sm font-medium transition-all duration-200"
                                :class="isActiveItem(item.path)
                                    ? 'border-[var(--border-hover-light)] bg-[var(--bg-selected-light)] text-[var(--text-primary-light)] dark:border-[var(--border-hover-dark)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-primary-dark)]'
                                    : 'border-[var(--border-light)] text-[var(--text-secondary-light)] hover:border-[var(--border-hover-light)] hover:bg-[var(--bg-selected-light)] hover:text-[var(--text-primary-light)] dark:border-[var(--border-dark)] dark:text-[var(--text-secondary-dark)] dark:hover:border-[var(--border-hover-dark)] dark:hover:bg-[var(--bg-selected-dark)] dark:hover:text-[var(--text-primary-dark)]'"
                                @click="handleClose">
                                <div class="flex h-10 w-10 items-center justify-center rounded border border-[var(--border-light)] bg-[var(--button-secondary-light)] dark:border-[var(--border-dark)] dark:bg-[var(--button-secondary-dark)]">
                                    <img :src="item.iconSrc" :alt="item.label" class="h-5 w-5 opacity-90">
                                </div>

                                <div class="min-w-0">
                                    <div class="truncate font-semibold">{{ item.label }}</div>
                                    <div v-if="item.description" class="mt-1 text-xs text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)]">
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
.fade-enter-active,
.fade-leave-active {
    transition: opacity 0.2s ease;
}

.fade-enter-from,
.fade-leave-to {
    opacity: 0;
}
</style>