#ifndef VKT_VULKAN_SWAPCHAIN_H
#define VKT_VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.h>

#include "../context/context.h"
#include "../surface/surface_info.h"

typedef struct {
    VkPresentModeKHR desired_present_mode;
    VkExtent2D image_extent;
} VktSwapchainCreateProps;

int vkt_create_swapchain(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VktSurfaceInfo *surface_info,
    VktSwapchainCreateProps props,
    VkSwapchainKHR old_swapchain,
    VkSwapchainKHR *swapchain
);

void vkt_destroy_swapchain(VktVulkanContext *context, VkSwapchainKHR swapchain);

#endif /* VKT_VULKAN_SWAPCHAIN_H */
