#ifndef VKT_VULKAN_DEBUG_H
#define VKT_VULKAN_DEBUG_H

#include <vulkan/vulkan.h>

#include <c_log/c_log.h>

VkResult vkt_debug_make_messenger(
    VkInstance instance,
	VkDebugUtilsMessengerEXT *debug_messenger
);
VkResult vkt_debug_destroy_messenger(
    VkInstance instance,
	VkDebugUtilsMessengerEXT debug_messenger
);

#endif /* VKT_VULKAN_DEBUG_H */
