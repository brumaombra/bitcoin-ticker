<script setup>
import Busy from '~/components/Busy.vue';
import MessageModal from '~/components/MessageModal.vue';
import { useTheme } from '~/composables/useTheme.js';
import { useAppUi } from '~/composables/useAppUi.js';
import { useGlobalStore } from '~/composables/stores/useGlobalStore.js';

const globalStore = useGlobalStore();
const { closeMessage } = useAppUi();
const { initializeTheme } = useTheme();

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