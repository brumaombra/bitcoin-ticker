<script setup>
import { computed, ref } from 'vue';
import { Monitor, Moon, Sun } from 'lucide-vue-next';
import { getThemes, setTheme } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import Dropdown from '~/components/ui/Dropdown.vue';
import DropdownMenu from '~/components/ui/DropdownMenu.vue';
import IconButton from '~/components/ui/IconButton.vue';

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

// Toggle the theme menu
const toggleDropdown = () => {
    isOpen.value = !isOpen.value;
};

// Apply the selected theme and close the menu
const handleSelectTheme = theme => {
    setTheme(theme);
    isOpen.value = false;
};
</script>

<template>
    <Dropdown v-model:open="isOpen" width="w-44">
        <!-- Trigger -->
        <template #trigger>
            <IconButton :aria-label="t('theme.toggleMenu')" @click="toggleDropdown">
                <component :is="currentThemeIcon" :stroke-width="1.8" class="h-5 w-5" />
            </IconButton>
        </template>

        <!-- Theme options -->
        <DropdownMenu :title="t('theme.title')" :options="themeOptions" :current="globalStore.themeMode" @select="handleSelectTheme">
            <template #option-leading="{ option }">
                <component :is="option.icon" :stroke-width="1.8" class="h-4 w-4" />
            </template>
        </DropdownMenu>
    </Dropdown>
</template>