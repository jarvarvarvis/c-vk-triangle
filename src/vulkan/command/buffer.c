#include "buffer.h"

#include "../common.h"

#include <string.h>

int vkt_allocate_command_buffers_with_level(
    VktVulkanContext *ctx,
    VkCommandPool pool,
    VkCommandBufferLevel level,
    VkCommandBuffer *buffers,
    size_t buffer_count
) {
    VkCommandBufferAllocateInfo alloc_info;
    memset(&alloc_info, 0, sizeof(VkCommandBufferAllocateInfo));

    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.pNext = NULL;
    alloc_info.commandPool = pool;
    alloc_info.level = level;
    alloc_info.commandBufferCount = buffer_count;

    VKT_CHECK(vkAllocateCommandBuffers(ctx->logical_device.vk_device, &alloc_info, buffers));
    return VKT_GENERIC_SUCCESS;
}

int vkt_allocate_primary_command_buffers(
    VktVulkanContext *ctx,
    VkCommandPool pool,
    VkCommandBuffer *buffers,
    size_t buffer_count
) {
    return vkt_allocate_command_buffers_with_level(ctx, pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, buffers, buffer_count);
}

void vkt_free_command_buffers(
    VktVulkanContext *ctx,
    VkCommandPool pool,
    VkCommandBuffer *buffers,
    size_t buffer_count
) {
    vkFreeCommandBuffers(ctx->logical_device.vk_device, pool, buffer_count, buffers);
}
