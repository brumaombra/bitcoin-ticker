import { computed } from 'vue';

import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

const availableThemes = [
    { id: 'light', label: 'Light' },
    { id: 'dark', label: 'Dark' },
    { id: 'system', label: 'System' }
];

const getThemeStore = () => useState('theme-mode', () => 'system');

const applyThemeToDocument = theme => {
    if (!import.meta.client) {
        return;
    }

    const isDarkSystem = window.matchMedia('(prefers-color-scheme: dark)').matches;
    const resolvedTheme = theme === 'system' ? (isDarkSystem ? 'dark' : 'light') : theme;

    document.documentElement.classList.remove('light', 'dark');
    document.documentElement.classList.add(resolvedTheme);
};

export const getCurrentTheme = () => {
    return getThemeStore();
};

export const getThemes = () => {
    return computed(() => availableThemes);
};

export const setTheme = theme => {
    const currentTheme = getThemeStore();

    currentTheme.value = theme;

    if (import.meta.client) {
        localStorage.setItem('theme', theme);
    }

    applyThemeToDocument(theme);
};

export const initializeTheme = () => {
    const currentTheme = getThemeStore();

    if (!import.meta.client) {
        return;
    }

    const storedTheme = localStorage.getItem('theme') || 'system';
    currentTheme.value = storedTheme;
    applyThemeToDocument(storedTheme);

    window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', () => {
        if (currentTheme.value === 'system') {
            applyThemeToDocument('system');
        }
    });
};

export const setBusy = busy => {
    const globalStore = useGlobalStore();

    if (globalStore.value.busy === busy) {
        return;
    }

    globalStore.value.busy = busy;
};

export const showMessage = (type, title, message) => {
    const globalStore = useGlobalStore();

    globalStore.value.messageModal = {
        visible: true,
        type,
        title,
        message
    };
};

export const closeMessage = () => {
    const globalStore = useGlobalStore();

    globalStore.value.messageModal.visible = false;
};