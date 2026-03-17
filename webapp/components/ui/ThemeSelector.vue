<script setup>
import { computed, ref } from 'vue';

import CustomIcon from '~/components/ui/CustomIcon.vue';
import Dropdown from '~/components/ui/Dropdown.vue';
import DropdownMenu from '~/components/ui/DropdownMenu.vue';
import IconButton from '~/components/ui/IconButton.vue';
import { getCurrentTheme, getThemes, setTheme } from '~/composables/useUtils.js';

const isOpen = ref(false);
const currentTheme = getCurrentTheme();
const themes = getThemes();

const themeOptions = computed(() => {
    return themes.value.map(theme => ({
        key: theme.id,
        label: theme.label,
        icon: getThemeIcon(theme.id)
    }));
});

const getThemeIcon = theme => {
    if (theme === 'light') {
        return 'theme-light';
    }

    if (theme === 'dark') {
        return 'theme-dark';
    }

    return 'theme-system';
};

const currentThemeIcon = computed(() => {
    return getThemeIcon(currentTheme.value);
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
    <Dropdown v-model:open="isOpen" width="w-44">
        <template #trigger>
            <IconButton aria-label="Toggle theme menu" @click="toggleDropdown">
                <CustomIcon :icon="currentThemeIcon" class="h-5 w-5" />
            </IconButton>
        </template>

        <DropdownMenu title="Theme" :options="themeOptions" :current="currentTheme" @select="handleSelectTheme">
            <template #option-leading="{ option }">
                <CustomIcon :icon="option.icon" class="h-4 w-4" />
            </template>
        </DropdownMenu>
    </Dropdown>
</template>