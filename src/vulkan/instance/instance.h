#ifndef VKT_VULKAN_INSTANCE_H
#define VKT_VULKAN_INSTANCE_H

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

typedef struct {
    VkInstance vk_instance;

#ifdef BUILD_MODE_DEBUG
    VkDebugUtilsMessengerEXT debug_messenger;
#endif
} VktVulkanInstance;

int vkt_create_vulkan_instance(VktVulkanInstance *instance, GLFWwindow *window);
void vkt_destroy_vulkan_instance(VktVulkanInstance *instance);

#endif /* VKT_VULKAN_INSTANCE_H */
