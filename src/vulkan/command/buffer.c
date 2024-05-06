#include "buffer.h"

#include "../common.h"

#include <string.h>

int vkt_allocate_command_buffers_with_level(
    VktVulkanContext *context,
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

    VKT_CHECK(vkAllocateCommandBuffers(context->logical_device.vk_device, &alloc_info, buffers));
    return VKT_GENERIC_SUCCESS;
}

int vkt_allocate_primary_command_buffers(
    VktVulkanContext *context,
    VkCommandPool pool,
    VkCommandBuffer *buffers,
    size_t buffer_count
) {
    return vkt_allocate_command_buffers_with_level(context, pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, buffers, buffer_count);
}

int vkt_reset_and_begin_command_buffer_recording(
    VktVulkanContext *context,
    VkCommandBuffer buffer,
    VkCommandBufferUsageFlags usage_flags
) {
    VKT_CHECK(vkResetCommandBuffer(buffer, 0));

    VkCommandBufferBeginInfo cmd_begin_info;

    cmd_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_begin_info.pNext = NULL;

    cmd_begin_info.pInheritanceInfo = NULL;
    cmd_begin_info.flags = usage_flags;

    VKT_CHECK(vkBeginCommandBuffer(buffer, &cmd_begin_info));
    return VKT_GENERIC_SUCCESS;
}

void vkt_free_command_buffers(
    VktVulkanContext *context,
    VkCommandPool pool,
    VkCommandBuffer *buffers,
    size_t buffer_count
) {
    vkFreeCommandBuffers(context->logical_device.vk_device, pool, buffer_count, buffers);
}
