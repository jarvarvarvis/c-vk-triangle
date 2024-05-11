#ifndef VKT_VULKAN_HELPER_H
#define VKT_VULKAN_HELPER_H

#include <vulkan/vulkan.h>

VkViewport vkt_vulkan_helper_viewport_from_extent(VkExtent2D extent);
VkRect2D vkt_vulkan_helper_rect2d_from_extent(VkExtent2D extent);

#endif /* VKT_VULKAN_HELPER_H */
