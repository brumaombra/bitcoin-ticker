import { computed } from 'vue';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

// Available theme modes for the webapp
const availableThemes = [
    { id: 'light' },
    { id: 'dark' },
    { id: 'system' }
];

// Available crypto accent themes for the webapp
const availableCryptoThemes = [
    { id: 'bitcoin', shortLabel: 'BTC', accentLight: '#f7931a', accentDark: '#f7a23c' },
    { id: 'kaspa', shortLabel: 'KAS', accentLight: '#49eacb', accentDark: '#5cefd4' }
];

// Translate a key through the active i18n instance when available
const translate = (key, params = {}) => {
    const nuxtApp = useNuxtApp();

    if (typeof nuxtApp?.$i18n?.t === 'function') {
        return nuxtApp.$i18n.t(key, params);
    }

    return key;
};

// Apply the resolved theme class to the document root
const applyThemeToDocument = theme => {
    if (!import.meta.client) return;
    const isDarkSystem = window.matchMedia('(prefers-color-scheme: dark)').matches;
    const resolvedTheme = theme === 'system' ? (isDarkSystem ? 'dark' : 'light') : theme;
    document.documentElement.classList.remove('light', 'dark');
    document.documentElement.classList.add(resolvedTheme);
};

// Apply the active crypto accent theme to the document root
const applyCryptoThemeToDocument = cryptoTheme => {
    if (!import.meta.client) return;
    document.documentElement.dataset.cryptoTheme = cryptoTheme;
};

// Return the list of supported theme options
export const getThemes = () => {
    return computed(() => availableThemes);
};

// Return the list of supported crypto accent themes
export const getCryptoThemes = () => {
    return computed(() => availableCryptoThemes);
};

// Persist and apply a selected theme
export const setTheme = theme => {
    const globalStore = useGlobalStore();
    globalStore.value.themeMode = theme;

    // Save the preference in localStorage for persistence across sessions
    if (import.meta.client) {
        localStorage.setItem('theme', theme);
    }

    // Apply the theme to the document root to update the UI
    applyThemeToDocument(theme);
};

// Persist and apply a selected crypto accent theme
export const setCryptoTheme = cryptoTheme => {
    const globalStore = useGlobalStore();
    globalStore.value.cryptoTheme = cryptoTheme;

    if (import.meta.client) {
        localStorage.setItem('crypto-theme', cryptoTheme);
    }

    applyCryptoThemeToDocument(cryptoTheme);
};

// Initialize the theme from storage and system preference
export const initializeTheme = () => {
    const globalStore = useGlobalStore();
    if (!import.meta.client) return;

    // Check localStorage for a saved theme preference
    const storedTheme = localStorage.getItem('theme') || 'system';
    globalStore.value.themeMode = storedTheme;
    applyThemeToDocument(storedTheme);

    // Listen for changes in the system theme preference
    window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', () => {
        if (globalStore.value.themeMode === 'system') {
            applyThemeToDocument('system');
        }
    });
};

// Initialize the crypto accent theme from storage
export const initializeCryptoTheme = () => {
    const globalStore = useGlobalStore();
    if (!import.meta.client) return;

    const storedCryptoTheme = localStorage.getItem('crypto-theme') || 'bitcoin';
    const isSupportedCryptoTheme = availableCryptoThemes.some(theme => theme.id === storedCryptoTheme);
    globalStore.value.cryptoTheme = isSupportedCryptoTheme ? storedCryptoTheme : 'bitcoin';
    applyCryptoThemeToDocument(globalStore.value.cryptoTheme);
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
export const showMessage = ({ type = 'Error', title = '', message = '' }) => {
    const globalStore = useGlobalStore();
    const defaultTitles = {
        Error: translate('dialogs.errorTitle'),
        Success: translate('dialogs.successTitle'),
        Info: translate('dialogs.infoTitle')
    };

    globalStore.value.messageDialog = {
        visible: true,
        type,
        title: title || defaultTitles[type] || translate('dialogs.messageTitle'),
        message: message || translate('common.genericError')
    };
};

// Close the shared message dialog
export const closeMessage = () => {
    const globalStore = useGlobalStore();
    globalStore.value.messageDialog.visible = false;
};

// Open the shared confirm dialog
export const showConfirmDialog = ({
    title = '',
    message = '',
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
        title: title || translate('dialogs.confirmTitle'),
        message: message || translate('dialogs.confirmMessage'),
        icon,
        onConfirm,
        onCancel,
        confirmButton: {
            ...globalStore.value.confirmDialog.confirmButton,
            text: translate('dialogs.confirm'),
            ...confirmButton
        },
        cancelButton: {
            ...globalStore.value.confirmDialog.cancelButton,
            text: translate('common.cancel'),
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
    console.error(errorMessage || translate('common.genericError'), error);

    // Show a user-friendly error dialog if requested
    if (showDialog) {
        showMessage({
            type: 'Error',
            title: translate('dialogs.errorTitle'),
            message: errorTranslated || errorMessage || translate('common.genericError')
        });
    }

    // If the error contains a status code, rethrow it for upstream handling
    if (error?.statusCode || error?.response?.status) {
        throw error;
    }

    // For other types of errors, throw a generic error with the translated message if available
    throw new Error(errorTranslated || translate('common.genericError'));
};