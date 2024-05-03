#ifndef VKT_VULKAN_DEVICE_PHYSICAL_H
#define VKT_VULKAN_DEVICE_PHYSICAL_H

#include "../instance/instance.h"

typedef struct {
    VkQueueFlagBits queue_family_bits;
} VktFindPhysicalDeviceProps;

typedef struct {
    VkPhysicalDevice vk_device;
    int32_t queue_family_index;
} VktPhysicalDevice;

int vkt_find_physical_device(VktVulkanInstance *instance, VktFindPhysicalDeviceProps props, VktPhysicalDevice *physical_device);

#endif /* VKT_VULKAN_DEVICE_PHYSICAL_H */
