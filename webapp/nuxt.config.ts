import tailwindcss from '@tailwindcss/vite';

export default defineNuxtConfig({
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

    vite: {
        plugins: [
            tailwindcss()
        ]
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