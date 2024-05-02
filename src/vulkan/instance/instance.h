#ifndef VKT_VULKAN_INSTANCE_H
#define VKT_VULKAN_INSTANCE_H

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

static const char *const VKT_ENABLED_LAYERS[] = {
#ifdef BUILD_MODE_DEBUG
    "VK_LAYER_KHRONOS_validation"
#endif
};

static const char *const VKT_EXTRA_ENABLED_EXTENSIONS[] = {
    // Enable VK_EXT_debug_utils in debug build mode
#ifdef BUILD_MODE_DEBUG
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
};

typedef struct {
    VkInstance vk_instance;

#ifdef BUILD_MODE_DEBUG
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} VktVulkanInstance;

int vkt_create_vulkan_instance(VktVulkanInstance *instance, GLFWwindow *window);
void vkt_delete_vulkan_instance(VktVulkanInstance *instance);

#endif /* VKT_VULKAN_INSTANCE_H */
