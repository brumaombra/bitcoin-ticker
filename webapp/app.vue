<script setup>
import { onMounted } from 'vue';
import { closeMessage, initializeTheme } from '~/composables/useUtils.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import Busy from '~/components/Busy.vue';
import MessageModal from '~/components/MessageModal.vue';

// Shared global UI state
const globalStore = useGlobalStore();

// Initialize the saved theme once the app mounts
onMounted(() => {
    initializeTheme();
});
</script>

<template>
    <div>
        <!-- Route content -->
        <NuxtLayout>
            <NuxtPage />
        </NuxtLayout>

        <!-- Global message modal -->
        <MessageModal :show="globalStore.messageModal.visible"
            :type="globalStore.messageModal.type"
            :title="globalStore.messageModal.title"
            :message="globalStore.messageModal.message"
            @close="closeMessage" />

        <!-- Global busy overlay -->
        <Busy :show="globalStore.busy" />
    </div>
</template>