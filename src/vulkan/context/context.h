#ifndef VKT_VULKAN_CONTEXT_H
#define VKT_VULKAN_CONTEXT_H

#include "../instance/instance.h"
#include "../device/physical.h"
#include "../device/logical.h"

typedef struct {
    VktVulkanInstance instance;
    VktFindPhysicalDeviceResult phys_device_result;
    VktLogicalDevice logical_device;

    VkSurfaceKHR surface;
} VktVulkanContext;

int vkt_create_basic_graphics_context(VktVulkanContext *context, GLFWwindow *window);
void vkt_destroy_context(VktVulkanContext *context);

#endif /* VKT_VULKAN_CONTEXT_H */
