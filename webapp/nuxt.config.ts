import tailwindcss from '@tailwindcss/vite';

export default defineNuxtConfig({
    srcDir: 'app/',

    ssr: false,

    app: {
        head: {
            title: 'Bitcoin Ticker'
        }
    },

    runtimeConfig: {
        public: {
            espBaseUrl: ''
        }
    },

    css: [
        '~/assets/main.css'
    ],

    modules: [
        '@nuxtjs/i18n',
        'shadcn-nuxt'
    ],

    i18n: {
        defaultLocale: 'en',
        strategy: 'no_prefix',
        langDir: 'locales',
        detectBrowserLanguage: {
            useCookie: true,
            cookieKey: 'bitcoin_ticker_locale',
            alwaysRedirect: false,
            redirectOn: 'root',
            fallbackLocale: 'en'
        },
        locales: [
            { code: 'en', name: 'English', language: 'en-US', file: 'en.json' },
            { code: 'it', name: 'Italiano', language: 'it-IT', file: 'it.json' }
        ]
    },

    shadcn: {
        prefix: '',
        componentDir: './app/components/shadcn'
    },

    vite: {
        plugins: [
            tailwindcss()
        ],
        optimizeDeps: {
            include: [
                '@hugeicons/vue',
                '@hugeicons/core-free-icons',
                '@vueuse/core',
                'clsx',
                'tailwind-merge',
                'lucide-vue-next',
                'reka-ui'
            ]
        }
    },

    devtools: {
        enabled: false
    },

    nitro: {
        prerender: {
            routes: [
                '/',
                '/ap-setup',
                '/wifi',
                '/settings',
                '/api-key'
            ]
        }
    },

    compatibilityDate: '2026-03-16'
});