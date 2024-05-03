#ifndef VKT_VULKAN_COMMAND_POOL_H
#define VKT_VULKAN_COMMAND_POOL_H

#include <vulkan/vulkan.h>

#include "../context/context.h"

int vkt_create_command_pool(VktVulkanContext *ctx, VkCommandPool *pool);
void vkt_destroy_command_pool(VktVulkanContext *ctx, VkCommandPool pool);

#endif /* VKT_VULKAN_COMMAND_POOL_H */
