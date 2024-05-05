#ifndef VKT_VULKAN_SWAPCHAIN_H
#define VKT_VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.h>

#include "../context/context.h"

typedef struct {
    VkPresentModeKHR desired_present_mode;
} VktSwapchainCreateProps;

int vkt_create_swapchain(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VktSwapchainCreateProps props,
    VkSwapchainKHR *swapchain
);

void vkt_destroy_swapchain(VktVulkanContext *context, VkSwapchainKHR swapchain);

#endif /* VKT_VULKAN_SWAPCHAIN_H */
