#ifndef VKT_VULKAN_DEBUG_H
#define VKT_VULKAN_DEBUG_H

#include <vulkan/vulkan.h>

VkResult vkt_create_debug_messenger(
    VkInstance instance,
	VkDebugUtilsMessengerEXT *debug_messenger
);
VkResult vkt_destroy_debug_messenger(
    VkInstance instance,
	VkDebugUtilsMessengerEXT debug_messenger
);

#endif /* VKT_VULKAN_DEBUG_H */
