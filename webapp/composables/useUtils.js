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
    if (!import.meta.client) return;
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

    // Save the preference in localStorage for persistence across sessions
    if (import.meta.client) {
        localStorage.setItem('theme', theme);
    }

    // Apply the theme to the document root to update the UI
    applyThemeToDocument(theme);
};

// Initialize the theme from storage and system preference
export const initializeTheme = () => {
    const currentTheme = getThemeStore();
    if (!import.meta.client) return;

    // Check localStorage for a saved theme preference
    const storedTheme = localStorage.getItem('theme') || 'system';
    currentTheme.value = storedTheme;
    applyThemeToDocument(storedTheme);

    // Listen for changes in the system theme preference
    window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', () => {
        if (currentTheme.value === 'system') {
            applyThemeToDocument('system');
        }
    });
};

// Toggle the global busy overlay
export const setBusy = busy => {
    const globalStore = useGlobalStore();
    if (globalStore.value.busy === busy) return; // If already in the desired state, do nothing
    globalStore.value.busy = busy;
};

// Pause execution for a short delay
export const delay = timeout => {
    return new Promise(resolve => setTimeout(resolve, timeout));
};

// Open the shared message dialog
export const showMessage = ({ type = 'Error', title = 'Error', message = 'Internal server error' }) => {
    const globalStore = useGlobalStore();
    globalStore.value.messageDialog = {
        visible: true,
        type,
        title,
        message
    };
};

// Close the shared message dialog
export const closeMessage = () => {
    const globalStore = useGlobalStore();
    globalStore.value.messageDialog.visible = false;
};

// Open the shared confirm dialog
export const showConfirmDialog = ({
    title = 'Confirm action',
    message = 'Are you sure?',
    icon = null,
    onConfirm = null,
    onCancel = null,
    confirmButton = {},
    cancelButton = {}
}) => {
    const globalStore = useGlobalStore();
    globalStore.value.confirmDialog = {
        ...globalStore.value.confirmDialog,
        visible: true,
        title,
        message,
        icon,
        onConfirm,
        onCancel,
        confirmButton: {
            ...globalStore.value.confirmDialog.confirmButton,
            ...confirmButton
        },
        cancelButton: {
            ...globalStore.value.confirmDialog.cancelButton,
            ...cancelButton
        }
    };
};

// Close the shared confirm dialog
export const closeConfirmDialog = () => {
    const globalStore = useGlobalStore();
    globalStore.value.confirmDialog.visible = false;
    globalStore.value.confirmDialog.onConfirm = null;
    globalStore.value.confirmDialog.onCancel = null;
};

// Handle backend errors with a shared message strategy
export const handleBackendErrors = ({ error, errorTranslated = '', errorMessage = '', showDialog = false }) => {
    console.error(errorMessage || 'An error occurred:', error);

    // Show a user-friendly error dialog if requested
    if (showDialog) {
        showMessage({
            type: 'Error',
            title: 'Error',
            message: errorTranslated || errorMessage || 'Internal server error'
        });
    }

    // If the error contains a status code, rethrow it for upstream handling
    if (error?.statusCode || error?.response?.status) {
        throw error;
    }

    // For other types of errors, throw a generic error with the translated message if available
    throw new Error(errorTranslated || 'Internal server error');
};