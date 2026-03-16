import tailwindcss from '@tailwindcss/vite';

export default defineNuxtConfig({
    ssr: false,

    app: {
        head: {
            title: 'Bitcoin Ticker',
            meta: [
                {
                    name: 'viewport',
                    content: 'width=device-width, initial-scale=1.0'
                }
            ],
            link: [
                {
                    rel: 'icon',
                    href: '/bitcoin.svg',
                    type: 'image/svg+xml'
                }
            ]
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

    compatibilityDate: '2026-03-16',

    nitro: {
        prerender: {
            routes: ['/', '/app', '/app/wifi', '/app/settings', '/app/apiKey']
        }
    }
});