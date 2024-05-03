#ifndef VKT_VULKAN_DEVICE_PHYSICAL_H
#define VKT_VULKAN_DEVICE_PHYSICAL_H

#include <vulkan/vulkan.h>

#include "../instance/instance.h"

typedef struct {
    VkQueueFlagBits queue_family_bits;
} VktFindPhysicalDeviceProps;

typedef struct {
    VkPhysicalDevice physical_device;
    int32_t queue_family_index;
} VktFindPhysicalDeviceResult;

int vkt_find_physical_device(VktVulkanInstance *instance, VktFindPhysicalDeviceProps props, VktFindPhysicalDeviceResult *result);

#endif /* VKT_VULKAN_DEVICE_PHYSICAL_H */
