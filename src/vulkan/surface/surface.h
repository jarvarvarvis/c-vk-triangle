#ifndef VKT_VULKAN_SURFACE_H
#define VKT_VULKAN_SURFACE_H

#include "../context/context.h"

int vkt_query_supported_surface_formats(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VkSurfaceFormatKHR **formats,
    uint32_t *format_count
);

int vkt_query_surface_present_modes(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VkPresentModeKHR **present_modes,
    uint32_t *present_mode_count
);

int vkt_query_surface_capabilities(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VkSurfaceCapabilitiesKHR *capabilities
);

#endif /* VKT_VULKAN_SURFACE_H */
