#ifndef VKT_VULKAN_COMMAND_BUFFER_H
#define VKT_VULKAN_COMMAND_BUFFER_H

#include <vulkan/vulkan.h>

#include "../context/context.h"

int vkt_allocate_command_buffers_with_level(
    VktVulkanContext *context,
    VkCommandPool pool,
    VkCommandBufferLevel level,
    VkCommandBuffer *buffers,
    size_t buffer_count
);

int vkt_allocate_primary_command_buffers(
    VktVulkanContext *context,
    VkCommandPool pool,
    VkCommandBuffer *buffers,
    size_t buffer_count
);

int vkt_reset_and_begin_command_buffer_recording(
    VktVulkanContext *context,
    VkCommandBuffer buffer,
    VkCommandBufferUsageFlags usage_flags
);

void vkt_free_command_buffers(
    VktVulkanContext *context,
    VkCommandPool pool,
    VkCommandBuffer *buffers,
    size_t buffer_count
);

#endif /* VKT_VULKAN_COMMAND_BUFFER_H */
