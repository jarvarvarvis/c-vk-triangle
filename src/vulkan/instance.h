#ifndef VKT_VULKAN_INSTANCE_H
#define VKT_VULKAN_INSTANCE_H

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

typedef struct {
    VkInstance vk_instance;
} VktVulkanInstance;

int vkt_create_vulkan_instance(VktVulkanInstance *instance, GLFWwindow *window);
void vkt_delete_vulkan_instance(VktVulkanInstance *instance);

#endif /* VKT_VULKAN_INSTANCE_H */
