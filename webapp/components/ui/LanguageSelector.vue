<script setup>
import { computed, ref } from 'vue';
import Dropdown from '~/components/ui/Dropdown.vue';
import DropdownMenu from '~/components/ui/DropdownMenu.vue';
import IconButton from '~/components/ui/IconButton.vue';
import EnglishFlag from '~/components/ui/flags/EnglishFlag.vue';
import ItalianFlag from '~/components/ui/flags/ItalianFlag.vue';

const isOpen = ref(false);
const { locale, locales, setLocale, t } = useI18n();

// Available language options for the selector
const languageOptions = computed(() => {
    return locales.value.map(language => ({
        key: language.code,
        label: language.name,
        flagCode: language.code === 'en' ? 'gb' : language.code
    }));
});

// Currently selected language metadata
const currentLanguage = computed(() => {
    const currentLanguage = languageOptions.value.find(language => language.key === locale.value);
    return currentLanguage || null;
});

// Get the appropriate flag component based on the language code
const getFlagComponent = flagCode => {
    // Check the flag code
    if (flagCode === 'it') {
        return ItalianFlag;
    } else {
        return EnglishFlag;
    }
};

// Toggle the language menu
const toggleDropdown = () => {
    isOpen.value = !isOpen.value;
};

// Apply a selected language and close the menu
const handleSelectLanguage = async language => {
    await setLocale(language);
    isOpen.value = false;
};
</script>

<template>
    <Dropdown v-model:open="isOpen" width="w-44">
        <!-- Trigger -->
        <template #trigger>
            <IconButton :aria-label="t('language.toggleMenu')" @click="toggleDropdown">
                <component :is="getFlagComponent(currentLanguage?.flagCode)" class="h-4 w-[1.35rem] rounded-[2px] border border-[var(--border-light)] shadow-sm dark:border-[var(--border-dark)]" />
            </IconButton>
        </template>

        <!-- Menu -->
        <DropdownMenu :title="t('language.title')" :options="languageOptions" :current="locale" @select="handleSelectLanguage">
            <template #option-leading="{ option }">
                <component :is="getFlagComponent(option.flagCode)" class="h-4 w-[1.35rem] rounded-[2px] border border-[var(--border-light)] shadow-sm dark:border-[var(--border-dark)]" />
            </template>
        </DropdownMenu>
    </Dropdown>
</template>