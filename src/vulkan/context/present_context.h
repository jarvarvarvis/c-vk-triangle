#ifndef VKT_VULKAN_PRESENT_CONTEXT_H
#define VKT_VULKAN_PRESENT_CONTEXT_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "context.h"

#include "../surface/surface_info.h"

#include "../swapchain/swapchain.h"
#include "../swapchain/images.h"

typedef struct {
    VkSurfaceKHR surface;
    VktSurfaceInfo surface_info;

    VkSwapchainKHR swapchain;
    VktSwapchainImages swapchain_images;

    VkRenderPass render_pass;
} VktPresentContext;

int vkt_create_present_context(VktVulkanContext *context, VktPresentContext *present_context, GLFWwindow *window);
int vkt_create_present_context_swapchain(VktVulkanContext *context, VktPresentContext *present_context, VktSwapchainCreateProps props);
int vkt_create_present_context_render_pass(VktVulkanContext *context, VktPresentContext *present_context);
void vkt_destroy_present_context(VktVulkanContext *context, VktPresentContext *present_context);

#endif /* VKT_VULKAN_PRESENT_CONTEXT_H */
