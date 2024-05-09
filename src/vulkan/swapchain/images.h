#ifndef VKT_VULKAN_SWAPCHAIN_IMAGES_H
#define VKT_VULKAN_SWAPCHAIN_IMAGES_H

#include "../context/context.h"

typedef struct {
    VkImage *images;
    VkImageView *image_views;

    uint32_t image_count;
} VktSwapchainImages;

int vkt_create_swapchain_images(
    VktVulkanContext *context,
    VkSwapchainKHR swapchain,
    VkFormat format,
    VktSwapchainImages *images
);
void vkt_destroy_swapchain_images(VktVulkanContext *context, VktSwapchainImages *images);

#endif /* VKT_VULKAN_SWAPCHAIN_IMAGES_H */
