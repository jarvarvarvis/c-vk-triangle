#ifndef VKT_VULKAN_SURFACE_INFO_H
#define VKT_VULKAN_SURFACE_INFO_H

#include "../context/context.h"

typedef struct {
    VkSurfaceFormatKHR *surface_formats;
    uint32_t surface_format_count;

    VkPresentModeKHR *present_modes;
    uint32_t present_mode_count;

    VkSurfaceCapabilitiesKHR surface_capabilities;
} VktSurfaceInfo;

int vkt_query_surface_info(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VktSurfaceInfo *info
);

void vkt_destroy_surface_info(VktSurfaceInfo *info);

#endif /* VKT_VULKAN_SURFACE_INFO_H */
