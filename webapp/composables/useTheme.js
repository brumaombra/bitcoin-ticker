const availableThemes = [
    { id: 'light', label: 'Light' },
    { id: 'dark', label: 'Dark' },
    { id: 'system', label: 'System' }
];

const applyThemeToDocument = theme => {
    if (!import.meta.client) {
        return;
    }

    const isDarkSystem = window.matchMedia('(prefers-color-scheme: dark)').matches;
    const resolvedTheme = theme === 'system' ? (isDarkSystem ? 'dark' : 'light') : theme;

    document.documentElement.classList.remove('light', 'dark');
    document.documentElement.classList.add(resolvedTheme);
};

export const useTheme = () => {
    const currentTheme = useState('theme-mode', () => 'system');

    const themes = computed(() => availableThemes);

    const setTheme = theme => {
        currentTheme.value = theme;

        if (import.meta.client) {
            localStorage.setItem('theme', theme);
        }

        applyThemeToDocument(theme);
    };

    const initializeTheme = () => {
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

    return {
        currentTheme,
        initializeTheme,
        setTheme,
        themes
    };
};