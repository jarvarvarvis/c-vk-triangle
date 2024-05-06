#include "sync.h"

#include "../common.h"

int vkt_create_engine_sync_structures(VktVulkanContext *context, VktEngineSyncStructures *sync_structures) {
    // Create the render fence
    VkFenceCreateInfo fence_create_info;
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.pNext = NULL;

    // Create the fence with the CREATE_SIGNALED flag, so we can wait on it before using it on a GPU command (for the first frame)
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VKT_CHECK(vkCreateFence(context->logical_device.vk_device, &fence_create_info, NULL, &sync_structures->render_fence));

    // Create present and render semaphores
    VkSemaphoreCreateInfo semaphore_create_info;
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = NULL;
    semaphore_create_info.flags = 0;

    VKT_CHECK(vkCreateSemaphore(context->logical_device.vk_device, &semaphore_create_info, NULL, &sync_structures->render_semaphore));
    VKT_CHECK(vkCreateSemaphore(context->logical_device.vk_device, &semaphore_create_info, NULL, &sync_structures->present_semaphore));

    return VKT_GENERIC_SUCCESS;
}

int vkt_wait_for_sync_structures_render_fence(VktVulkanContext *context, VktEngineSyncStructures *sync_structures) {
    // Wait until the GPU has finished rendering the last frame
    VKT_CHECK(vkWaitForFences(
        context->logical_device.vk_device,
        1,
        &sync_structures->render_fence,
        true,
        UINT64_MAX
    ));

    VKT_CHECK(vkResetFences(context->logical_device.vk_device, 1, &sync_structures->render_fence));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_engine_sync_structures(VktVulkanContext *context, VktEngineSyncStructures *sync_structures) {
    vkDestroyFence(context->logical_device.vk_device, sync_structures->render_fence, NULL);
    vkDestroySemaphore(context->logical_device.vk_device, sync_structures->render_semaphore, NULL);
    vkDestroySemaphore(context->logical_device.vk_device, sync_structures->present_semaphore, NULL);
}
