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
        shortLabel: language.code.toUpperCase()
    }));
});

// Currently selected language label
const currentLanguageLabel = computed(() => {
    const currentLanguage = languageOptions.value.find(language => language.key === locale.value);
    return currentLanguage?.shortLabel || locale.value.toUpperCase();
});

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
        <template #trigger>
            <IconButton :aria-label="t('language.toggleMenu')" @click="toggleDropdown">
                <span class="text-xs font-semibold uppercase tracking-[0.18em]">{{ currentLanguageLabel }}</span>
            </IconButton>
        </template>

        <DropdownMenu :title="t('language.title')" :options="languageOptions" :current="locale" @select="handleSelectLanguage" />
    </Dropdown>
</template>