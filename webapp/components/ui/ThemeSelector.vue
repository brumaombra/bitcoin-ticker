<script setup>
import { computed, ref } from 'vue';

import CustomIcon from '~/components/ui/CustomIcon.vue';
import Dropdown from '~/components/ui/Dropdown.vue';
import DropdownMenu from '~/components/ui/DropdownMenu.vue';
import IconButton from '~/components/ui/IconButton.vue';
import { getCurrentTheme, getThemes, setTheme } from '~/composables/useUtils.js';

// Dropdown state
const isOpen = ref(false);

// Shared theme state
const currentTheme = getCurrentTheme();
const themes = getThemes();

// Theme options for the dropdown menu
const themeOptions = computed(() => {
    return themes.value.map(theme => ({
        key: theme.id,
        label: theme.label,
        icon: getThemeIcon(theme.id)
    }));
});

// Resolve the icon for each theme mode
const getThemeIcon = theme => {
    if (theme === 'light') {
        return 'theme-light';
    }

    if (theme === 'dark') {
        return 'theme-dark';
    }

    return 'theme-system';
};

// Resolve the icon for the current theme
const currentThemeIcon = computed(() => {
    return getThemeIcon(currentTheme.value);
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
            <IconButton aria-label="Toggle theme menu" @click="toggleDropdown">
                <CustomIcon :icon="currentThemeIcon" class="h-5 w-5" />
            </IconButton>
        </template>

        <!-- Theme options -->
        <DropdownMenu title="Theme" :options="themeOptions" :current="currentTheme" @select="handleSelectTheme">
            <template #option-leading="{ option }">
                <CustomIcon :icon="option.icon" class="h-4 w-4" />
            </template>
        </DropdownMenu>
    </Dropdown>
</template>