<script setup>
import { computed, ref } from 'vue';
import { HugeiconsIcon } from '@hugeicons/vue';
import { ComputerIcon, Moon02Icon, Sun01Icon } from '@hugeicons/core-free-icons';
import { getCurrentTheme, getThemes, setTheme } from '~/composables/useUtils.js';
import Dropdown from '~/components/ui/Dropdown.vue';
import DropdownMenu from '~/components/ui/DropdownMenu.vue';
import IconButton from '~/components/ui/IconButton.vue';

const isOpen = ref(false);
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
        return Sun01Icon;
    } else if (theme === 'dark') {
        return Moon02Icon;
    } else {
        return ComputerIcon;
    }
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
                <HugeiconsIcon :icon="currentThemeIcon" :size="20" color="currentColor" :stroke-width="1.8" class="h-5 w-5" />
            </IconButton>
        </template>

        <!-- Theme options -->
        <DropdownMenu title="Theme" :options="themeOptions" :current="currentTheme" @select="handleSelectTheme">
            <template #option-leading="{ option }">
                <HugeiconsIcon :icon="option.icon" :size="16" color="currentColor" :stroke-width="1.8" class="h-4 w-4" />
            </template>
        </DropdownMenu>
    </Dropdown>
</template>