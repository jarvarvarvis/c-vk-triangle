#ifndef VKT_VULKAN_PRESENT_CONTEXT_H
#define VKT_VULKAN_PRESENT_CONTEXT_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "context.h"

typedef struct {
    VkSurfaceKHR surface;
} VktPresentContext;

int vkt_create_present_context(VktVulkanContext *context, VktPresentContext *present_context, GLFWwindow *window);

int vkt_query_supported_surface_formats(
    VktVulkanContext *context,
    VktPresentContext *present_context,
    VkSurfaceFormatKHR **formats,
    uint32_t *format_count
);

int vkt_query_surface_present_modes(
    VktVulkanContext *context,
    VktPresentContext *present_context,
    VkPresentModeKHR **present_modes,
    uint32_t *present_mode_count
);

int vkt_query_surface_capabilities(
    VktVulkanContext *context,
    VktPresentContext *present_context,
    VkSurfaceCapabilitiesKHR *capabilities
);

void vkt_destroy_present_context(VktVulkanContext *context, VktPresentContext *present_context);

#endif /* VKT_VULKAN_PRESENT_CONTEXT_H */
