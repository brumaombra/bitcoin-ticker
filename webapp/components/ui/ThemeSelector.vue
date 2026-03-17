<script setup>
import { useTheme } from '~/composables/useTheme.js';
import IconButton from '~/components/ui/IconButton.vue';

const { currentTheme, setTheme, themes } = useTheme();

const isOpen = ref(false);

const currentThemeLabel = computed(() => {
    return themes.value.find(theme => theme.id === currentTheme.value)?.label || 'Theme';
});

const toggleDropdown = () => {
    isOpen.value = !isOpen.value;
};

const handleSelectTheme = theme => {
    setTheme(theme);
    isOpen.value = false;
};
</script>

<template>
    <div class="relative">
        <IconButton aria-label="Toggle theme menu" @click="toggleDropdown">
            <svg v-if="currentTheme === 'light'" class="h-5 w-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8" aria-hidden="true">
                <circle cx="12" cy="12" r="4"></circle>
                <path d="M12 2v2.5M12 19.5V22M4.93 4.93l1.77 1.77M17.3 17.3l1.77 1.77M2 12h2.5M19.5 12H22M4.93 19.07l1.77-1.77M17.3 6.7l1.77-1.77"></path>
            </svg>
            <svg v-else-if="currentTheme === 'dark'" class="h-5 w-5" viewBox="0 0 24 24" fill="currentColor" aria-hidden="true">
                <path d="M21 14.25A8.25 8.25 0 1111.75 3a6.75 6.75 0 109.25 11.25Z"></path>
            </svg>
            <svg v-else class="h-5 w-5" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8" aria-hidden="true">
                <rect x="3" y="4" width="18" height="12" rx="2"></rect>
                <path d="M8 20h8"></path>
            </svg>
        </IconButton>

        <Transition enter-active-class="transition ease-out duration-200" enter-from-class="opacity-0 translate-y-1" enter-to-class="opacity-100 translate-y-0" leave-active-class="transition ease-in duration-150" leave-from-class="opacity-100 translate-y-0" leave-to-class="opacity-0 translate-y-1">
            <div v-if="isOpen" class="absolute right-0 top-[calc(100%+0.5rem)] z-50 w-44 rounded border border-[var(--border-light)] bg-[var(--bg-card-light)] p-2 shadow-xl dark:border-[var(--border-dark)] dark:bg-[var(--bg-card-dark)]">
                <button v-for="theme in themes"
                    :key="theme.id"
                    type="button"
                    class="flex w-full items-center justify-between rounded px-3 py-2 text-sm transition-colors duration-200"
                    :class="theme.id === currentTheme
                        ? 'bg-[var(--bg-selected-light)] text-[var(--text-primary-light)] dark:bg-[var(--bg-selected-dark)] dark:text-[var(--text-primary-dark)]'
                        : 'text-[var(--text-secondary-light)] hover:bg-[var(--bg-selected-light)] hover:text-[var(--text-primary-light)] dark:text-[var(--text-secondary-dark)] dark:hover:bg-[var(--bg-selected-dark)] dark:hover:text-[var(--text-primary-dark)]'"
                    @click="handleSelectTheme(theme.id)">
                    <span>{{ theme.label }}</span>
                    <span v-if="theme.id === currentTheme" class="text-xs uppercase tracking-[0.18em]">On</span>
                </button>
            </div>
        </Transition>
    </div>
</template>