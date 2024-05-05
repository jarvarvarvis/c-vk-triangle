#ifndef VKT_VULKAN_ENGINE_SYNC_H
#define VKT_VULKAN_ENGINE_SYNC_H

#include "../context/context.h"

typedef struct {
    VkFence render_fence;

    VkSemaphore render_semaphore;
    VkSemaphore present_semaphore;
} VktEngineSyncStructures;

int vkt_create_engine_sync_structures(VktVulkanContext *context, VktEngineSyncStructures *sync_structures);
void vkt_destroy_engine_sync_structures(VktVulkanContext *context, VktEngineSyncStructures *sync_structures);

#endif /* VKT_VULKAN_ENGINE_SYNC_H */
