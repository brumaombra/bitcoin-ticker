<script setup>
import { computed, ref } from 'vue';
import Dropdown from '~/components/ui/Dropdown.vue';
import DropdownMenu from '~/components/ui/DropdownMenu.vue';
import IconButton from '~/components/ui/IconButton.vue';

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

// Resolve a flag-icons class name
const getFlagClass = flagCode => {
    return `fi fi-${flagCode}`;
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
                <span :class="getFlagClass(currentLanguage?.flagCode || 'gb')" class="inline-block h-4 w-5 overflow-hidden rounded-[2px]"></span>
            </IconButton>
        </template>

        <!-- Menu -->
        <DropdownMenu :title="t('language.title')"
            :options="languageOptions"
            :current="locale"
            @select="handleSelectLanguage">
            <template #option-leading="{ option }">
                <span :class="getFlagClass(option.flagCode)" class="inline-block h-4 w-5 overflow-hidden rounded-[2px]"></span>
            </template>
        </DropdownMenu>
    </Dropdown>
</template>