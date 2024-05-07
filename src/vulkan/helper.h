#ifndef VKT_UTIL_VK_HELPER_H
#define VKT_UTIL_VK_HELPER_H

#include <vulkan/vulkan.h>

VkViewport vkt_helper_viewport_from_extent(VkExtent2D extent);
VkRect2D vkt_helper_rect2d_from_extent(VkExtent2D extent);

#endif /* VKT_UTIL_VK_HELPER_H */
