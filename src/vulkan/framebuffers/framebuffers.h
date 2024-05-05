#ifndef VKT_VULKAN_FRAMEBUFFERS_H
#define VKT_VULKAN_FRAMEBUFFERS_H

#include "../context/context.h"

#include "../swapchain/images.h"

typedef struct {
    VkFramebuffer *framebuffers;
    uint32_t framebuffer_count;
} VktFramebuffers;

int vkt_create_framebuffers(
    VktVulkanContext *context,
    VktSwapchainImages *swapchain_images,
    VkRenderPass render_pass,
    VkExtent2D image_extent,
    VktFramebuffers *framebuffers
);

void vkt_destroy_framebuffers(VktVulkanContext *context, VktFramebuffers *framebuffers);

#endif /* VKT_VULKAN_FRAMEBUFFERS_H */
