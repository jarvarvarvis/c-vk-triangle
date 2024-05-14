#ifndef VKT_VULKAN_RENDERPASS_H
#define VKT_VULKAN_RENDERPASS_H

#include "../context/context.h"

int vkt_create_default_renderpass(
    VktVulkanContext *context,
    VkFormat color_format,
    VkFormat depth_format,
    VkRenderPass *renderpass
);

void vkt_destroy_renderpass(VktVulkanContext *context, VkRenderPass render_pass);

#endif /* VKT_VULKAN_RENDERPASS_H */
