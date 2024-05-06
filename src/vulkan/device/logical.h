#ifndef VKT_VULKAN_DEVICE_LOGICAL_H
#define VKT_VULKAN_DEVICE_LOGICAL_H

#include "physical.h"

typedef struct {
    VkDevice vk_device;
    int32_t queue_family_index;
} VktLogicalDevice;

int vkt_create_logical_device(VktLogicalDevice *device, VktPhysicalDevice physical_device);

VkQueue vkt_get_logical_device_queue(VktLogicalDevice *device, uint32_t index);

void vkt_destroy_logical_device(VktLogicalDevice *device);

#endif /* VKT_VULKAN_DEVICE_LOGICAL_H */

