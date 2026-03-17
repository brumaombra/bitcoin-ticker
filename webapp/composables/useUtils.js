import { computed } from 'vue';

import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

// Available theme modes for the webapp
const availableThemes = [
    { id: 'light', label: 'Light' },
    { id: 'dark', label: 'Dark' },
    { id: 'system', label: 'System' }
];

// Shared state for the active theme mode
const getThemeStore = () => useState('theme-mode', () => 'system');

// Apply the resolved theme class to the document root
const applyThemeToDocument = theme => {
    if (!import.meta.client) {
        return;
    }

    const isDarkSystem = window.matchMedia('(prefers-color-scheme: dark)').matches;
    const resolvedTheme = theme === 'system' ? (isDarkSystem ? 'dark' : 'light') : theme;

    document.documentElement.classList.remove('light', 'dark');
    document.documentElement.classList.add(resolvedTheme);
};

// Return the current theme state
export const getCurrentTheme = () => {
    return getThemeStore();
};

// Return the list of supported theme options
export const getThemes = () => {
    return computed(() => availableThemes);
};

// Persist and apply a selected theme
export const setTheme = theme => {
    const currentTheme = getThemeStore();

    currentTheme.value = theme;

    if (import.meta.client) {
        localStorage.setItem('theme', theme);
    }

    applyThemeToDocument(theme);
};

// Initialize the theme from storage and system preference
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

// Toggle the global busy overlay
export const setBusy = busy => {
    const globalStore = useGlobalStore();

    if (globalStore.value.busy === busy) {
        return;
    }

    globalStore.value.busy = busy;
};

// Open the shared message modal
export const showMessage = (type, title, message) => {
    const globalStore = useGlobalStore();

    globalStore.value.messageModal = {
        visible: true,
        type,
        title,
        message
    };
};

// Close the shared message modal
export const closeMessage = () => {
    const globalStore = useGlobalStore();

    globalStore.value.messageModal.visible = false;
};