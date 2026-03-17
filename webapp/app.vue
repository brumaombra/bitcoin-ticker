<script setup>
import Busy from '~/components/Busy.vue';
import MessageModal from '~/components/MessageModal.vue';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';
import { closeMessage, initializeTheme } from '~/composables/useUtils.js';

const globalStore = useGlobalStore();

onMounted(() => {
    initializeTheme();
});
</script>

<template>
    <div>
        <NuxtLayout>
            <NuxtPage />
        </NuxtLayout>

        <MessageModal :show="globalStore.messageModal.visible"
            :type="globalStore.messageModal.type"
            :title="globalStore.messageModal.title"
            :message="globalStore.messageModal.message"
            @close="closeMessage" />

        <Busy :show="globalStore.busy" />
    </div>
</template>