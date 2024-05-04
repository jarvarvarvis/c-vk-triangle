#include "present_context.h"

#include "../common.h"

#include <c_log/c_log.h>

int vkt_create_present_context(VktVulkanContext *context, VktPresentContext *present_context, GLFWwindow *window) {
    memset(present_context, 0, sizeof(VktPresentContext));

    // Create window surface
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(context->instance.vk_instance, window, NULL, &surface) != VK_SUCCESS) {
        c_log(C_LOG_SEVERITY_ERROR, "Failed to create window surface!");
        return VKT_GENERIC_FAILURE;
    }

    // Validate that the physical device supports the surface
    VkBool32 surface_supported_by_device_and_queue = VK_TRUE;
    vkGetPhysicalDeviceSurfaceSupportKHR(
        context->physical_device.vk_device,
        context->physical_device.queue_family_index,
        surface,
        &surface_supported_by_device_and_queue
    );
    if (!surface_supported_by_device_and_queue) {
        c_log(C_LOG_SEVERITY_ERROR, "Window surface created by GLFW not supported by selected physical device and queue!");
        return VKT_GENERIC_FAILURE;
    }

    present_context->surface = surface;

    return VKT_GENERIC_SUCCESS;
}

int vkt_query_supported_surface_formats(
    VktVulkanContext *context,
    VktPresentContext *present_context,
    VkSurfaceFormatKHR **formats,
    uint32_t *format_count
) {
    VKT_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physical_device.vk_device, present_context->surface, format_count, NULL));

    // Allocate buffer to store formats
    *formats = malloc(sizeof(VkSurfaceFormatKHR) * (*format_count));
    VKT_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physical_device.vk_device, present_context->surface, format_count, *formats));

    return VKT_GENERIC_SUCCESS;
}

int vkt_query_surface_present_modes(
    VktVulkanContext *context,
    VktPresentContext *present_context,
    VkPresentModeKHR **present_modes,
    uint32_t *present_mode_count
) {
    VKT_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(context->physical_device.vk_device, present_context->surface, present_mode_count, NULL));

    // Allocate buffer to store present modes
    *present_modes = malloc(sizeof(VkSurfaceFormatKHR) * (*present_mode_count));
    VKT_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(context->physical_device.vk_device, present_context->surface, present_mode_count, *present_modes));

    return VKT_GENERIC_SUCCESS;
}

int vkt_query_surface_capabilities(
    VktVulkanContext *context,
    VktPresentContext *present_context,
    VkSurfaceCapabilitiesKHR *capabilities
) {
    VKT_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->physical_device.vk_device, present_context->surface, capabilities));
    return VKT_GENERIC_SUCCESS;
}

void vkt_destroy_present_context(VktVulkanContext *context, VktPresentContext *present_context) {
    vkDestroySurfaceKHR(context->instance.vk_instance, present_context->surface, NULL);
}
