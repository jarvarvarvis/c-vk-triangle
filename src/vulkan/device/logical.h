#ifndef VKT_VULKAN_DEVICE_LOGICAL_H
#define VKT_VULKAN_DEVICE_LOGICAL_H

#include "physical.h"

typedef struct {
    VkDevice vk_device;
} VktLogicalDevice;

int vkt_create_logical_device(
    VktLogicalDevice *device,
    VktPhysicalDevice physical_device
);

void vkt_destroy_logical_device(VktLogicalDevice *device);

#endif /* VKT_VULKAN_DEVICE_LOGICAL_H */

