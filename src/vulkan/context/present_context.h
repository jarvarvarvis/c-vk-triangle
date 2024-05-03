#ifndef VKT_VULKAN_PRESENT_CONTEXT_H
#define VKT_VULKAN_PRESENT_CONTEXT_H

#include "context.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

typedef struct {
    VkSurfaceKHR surface;
} VktPresentContext;

int vkt_create_present_context(VktVulkanContext *context, VktPresentContext *present_context, GLFWwindow *window);
void vkt_destroy_present_context(VktVulkanContext *context, VktPresentContext *present_context);

#endif /* VKT_VULKAN_PRESENT_CONTEXT_H */
