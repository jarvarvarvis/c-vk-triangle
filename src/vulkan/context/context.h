#ifndef VKT_VULKAN_CONTEXT_H
#define VKT_VULKAN_CONTEXT_H

#include "../instance/instance.h"
#include "../device/physical.h"
#include "../device/logical.h"

typedef struct {
    VktVulkanInstance instance;
    VktPhysicalDevice physical_device;
    VktLogicalDevice logical_device;
} VktVulkanContext;

int vkt_create_basic_graphics_context(VktVulkanContext *context, char *app_name);
void vkt_destroy_context(VktVulkanContext *context);

#endif /* VKT_VULKAN_CONTEXT_H */
