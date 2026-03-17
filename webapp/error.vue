<script setup>
import { computed } from 'vue';
import BackgroundGrid from '~/components/ui/BackgroundGrid.vue';
import BrandLogo from '~/components/ui/BrandLogo.vue';
import Button from '~/components/ui/Button.vue';
import Card from '~/components/ui/Card.vue';

// Props
const props = defineProps({
    error: { type: Object, default: () => ({}) }
});

// Resolve the displayed status code
const statusCode = computed(() => {
    return props.error?.statusCode ?? 500;
});

// Resolve the page title
const title = computed(() => {
    if (statusCode.value === 404) {
        return 'Page not found';
    }

    return 'Something went wrong';
});

// Resolve the supporting message
const message = computed(() => {
    if (statusCode.value === 404) {
        return 'The page you requested does not exist or is no longer available on this device.';
    }

    return props.error?.statusMessage || 'An unexpected error interrupted the web interface.';
});

// Clear the error and return to the main setup flow
const handleBackHome = async () => {
    await clearError({ redirect: '/wifi' });
};
</script>

<template>
    <div class="relative min-h-screen bg-[var(--bg-main-light)] text-[var(--text-primary-light)] dark:bg-[var(--bg-main-dark)] dark:text-[var(--text-primary-dark)]">
        <!-- Background grid -->
        <BackgroundGrid />

        <!-- Main content -->
        <main class="relative z-10 mx-auto flex min-h-screen w-full max-w-6xl items-center justify-center px-4 py-10 sm:px-6 lg:px-8">
            <div class="w-full max-w-xl">
                <Card>
                    <!-- Brand -->
                    <BrandLogo brand-name="Bitcoin Ticker" />

                    <!-- Error copy -->
                    <div class="mt-8">
                        <!-- Status code -->
                        <div class="text-sm font-semibold uppercase tracking-[0.24em] text-[var(--button-primary-light)] dark:text-[var(--button-primary-dark)]">
                            Error {{ statusCode }}
                        </div>

                        <!-- Title -->
                        <h1 class="mt-3 text-2xl font-bold">
                            {{ title }}
                        </h1>

                        <!-- Message -->
                        <p class="mt-4 text-sm leading-7 text-[var(--text-secondary-light)] dark:text-[var(--text-secondary-dark)] sm:text-base">
                            {{ message }}
                        </p>
                    </div>

                    <!-- Actions -->
                    <div class="mt-8 flex flex-col gap-3 sm:flex-row">
                        <!-- Back home button -->
                        <Button type="primary" @click="handleBackHome">
                            Back to setup
                        </Button>
                    </div>
                </Card>
            </div>
        </main>
    </div>
</template>