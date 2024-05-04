#ifndef VKT_VULKAN_SWAPCHAIN_H
#define VKT_VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.h>

#include "../context/context.h"
#include "../context/present_context.h"

int vkt_create_swapchain(VktVulkanContext *context, VktPresentContext *present_context, VkSwapchainKHR *swapchain);
void vkt_destroy_swapchain(VktVulkanContext *context, VkSwapchainKHR swapchain);

#endif /* VKT_VULKAN_SWAPCHAIN_H */
