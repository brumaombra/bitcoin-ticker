<script setup>
import { computed, ref } from 'vue';
import { Check, Monitor, Moon, Sun } from 'lucide-vue-next';
import { getThemes, setTheme } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import { Button } from '~/components/shadcn/button';
import { Command, CommandGroup, CommandItem, CommandList } from '~/components/shadcn/command';
import { Popover, PopoverContent, PopoverTrigger } from '~/components/shadcn/popover';

const isOpen = ref(false);
const globalStore = useGlobalStore();
const themes = getThemes();
const { t } = useI18n();

// Theme options for the dropdown menu
const themeOptions = computed(() => {
    return themes.map(theme => ({
        key: theme.id,
        label: t(`theme.${theme.id}`),
        icon: getThemeIcon(theme.id)
    }));
});

// Resolve the icon for each theme mode
const getThemeIcon = theme => {
    if (theme === 'light') {
        return Sun;
    } else if (theme === 'dark') {
        return Moon;
    } else {
        return Monitor;
    }
};

// Resolve the icon for the current theme
const currentThemeIcon = computed(() => {
    return getThemeIcon(globalStore.value.themeMode);
});

// Apply the selected theme and close the menu
const handleSelectTheme = theme => {
    setTheme(theme);
    isOpen.value = false;
};
</script>

<template>
    <Popover v-model:open="isOpen">
        <PopoverTrigger as-child>
            <Button variant="outline" size="icon" :aria-label="t('theme.toggleMenu')">
                <component :is="currentThemeIcon" :stroke-width="1.8" class="h-5 w-5" />
            </Button>
        </PopoverTrigger>

        <!-- Theme options -->
        <PopoverContent side="bottom" align="end" :side-offset="8" class="w-44 !p-0">
            <Command :model-value="globalStore.themeMode">
                <CommandList>
                    <CommandGroup :heading="t('theme.title')">
                        <CommandItem v-for="option in themeOptions" :key="option.key" :value="option.key" @select="handleSelectTheme(option.key)">
                            <component :is="option.icon" :stroke-width="1.8" class="h-4 w-4" />
                            <span class="flex-1">{{ option.label }}</span>
                            <Check :stroke-width="1.8" class="ml-auto h-4 w-4 shrink-0" :class="globalStore.themeMode === option.key ? 'opacity-100' : 'opacity-0'" />
                        </CommandItem>
                    </CommandGroup>
                </CommandList>
            </Command>
        </PopoverContent>
    </Popover>
</template>