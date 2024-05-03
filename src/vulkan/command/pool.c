#include "pool.h"

#include "../common.h"

#include <string.h>

int vkt_create_command_pool(VktVulkanContext *ctx, VkCommandPool *pool) {
    VkCommandPoolCreateInfo cmd_pool_info;
    memset(&cmd_pool_info, 0, sizeof(VkCommandPoolCreateInfo));

    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    cmd_pool_info.queueFamilyIndex = ctx->physical_device.queue_family_index;
    cmd_pool_info.flags = 0;

    VKT_CHECK(vkCreateCommandPool(ctx->logical_device.vk_device, &cmd_pool_info, NULL, pool));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_command_pool(VktVulkanContext *ctx, VkCommandPool pool) {
    vkDestroyCommandPool(ctx->logical_device.vk_device, pool, NULL);
}
