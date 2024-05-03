#include "context.h"

#include "../common.h"

#include <string.h>
#include <c_log/c_log.h>

int vkt_create_basic_graphics_context(VktVulkanContext *context, GLFWwindow *window) {
    memset(context, 0, sizeof(VktVulkanContext));

    // Create instance
    VktVulkanInstance instance;
    if (vkt_create_vulkan_instance(&instance, window) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create Vulkan instance!");
        return VKT_GENERIC_FAILURE;
    }

    // Physical device discovery
    VktFindPhysicalDeviceResult phys_device_result;
    VktFindPhysicalDeviceProps find_phys_device_props;
    find_phys_device_props.queue_family_bits = VK_QUEUE_GRAPHICS_BIT;
    if (vkt_find_physical_device(&instance, find_phys_device_props, &phys_device_result) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to find physical device!");
        return VKT_GENERIC_FAILURE;
    }

    // Create logical device
    VktLogicalDevice logical_device;
    if (vkt_create_logical_device(&logical_device, phys_device_result) != VKT_GENERIC_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to create logical device!");
        return VKT_GENERIC_FAILURE;
    }

    // Create window surface
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance.vk_instance, window, NULL, &surface) != VK_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create window surface!");
        return VKT_GENERIC_FAILURE;
    }

    context->instance = instance;
    context->phys_device_result = phys_device_result;
    context->logical_device = logical_device;
    context->surface = surface;

    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_context(VktVulkanContext *context) {
    vkDestroySurfaceKHR(context->instance.vk_instance, context->surface, NULL);
    vkt_destroy_logical_device(&context->logical_device);
    vkt_destroy_vulkan_instance(&context->instance);
}
