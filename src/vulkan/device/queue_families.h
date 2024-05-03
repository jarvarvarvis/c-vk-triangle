#ifndef VKT_VULKAN_DEVICE_QUEUE_FAMILIES_H
#define VKT_VULKAN_DEVICE_QUEUE_FAMILIES_H

#include <vulkan/vulkan.h>

int vkt_find_queue_families_for_device(
    VkPhysicalDevice device,
    VkQueueFlagBits *families_to_find_flags,
    int32_t *families_to_find_indices_out,
    size_t families_to_find_count
);

#endif /* VKT_VULKAN_DEVICE_QUEUE_FAMILIES_H */
