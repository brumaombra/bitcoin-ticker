export default defineNuxtConfig({
    compatibilityDate: '2026-03-16',
    devtools: {
        enabled: true
    },
    srcDir: 'src/',
    dir: {
        public: '../public'
    },
    ssr: false,
    modules: ['@nuxtjs/tailwindcss'],
    css: ['~/assets/main.css'],
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
    nitro: {
        prerender: {
            routes: ['/', '/app', '/app/wifi', '/app/settings', '/app/apiKey']
        }
    }
});