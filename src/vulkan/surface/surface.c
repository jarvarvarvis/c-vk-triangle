#include "surface.h"

#include "../common.h"

int vkt_query_supported_surface_formats(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VkSurfaceFormatKHR **formats,
    uint32_t *format_count
) {
    VKT_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physical_device.vk_device, surface, format_count, NULL));

    // Check if there are any supported surface formats at all
    if (*format_count == 0) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to query any supported surface formats for the surface!");
        return VKT_GENERIC_FAILURE;
    }

    // Allocate buffer to store formats
    *formats = malloc(sizeof(VkSurfaceFormatKHR) * (*format_count));
    VKT_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(context->physical_device.vk_device, surface, format_count, *formats));

    return VKT_GENERIC_SUCCESS;
}

int vkt_query_surface_present_modes(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VkPresentModeKHR **present_modes,
    uint32_t *present_mode_count
) {
    VKT_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(context->physical_device.vk_device, surface, present_mode_count, NULL));

    // Check if there are any supported present modes at all
    if (*present_mode_count == 0) {
        c_log(C_LOG_SEVERITY_ERROR, "Unable to query any supported surface present modes for the surface!");
        return VKT_GENERIC_FAILURE;
    }

    // Allocate buffer to store present modes
    *present_modes = malloc(sizeof(VkSurfaceFormatKHR) * (*present_mode_count));
    VKT_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(context->physical_device.vk_device, surface, present_mode_count, *present_modes));

    return VKT_GENERIC_SUCCESS;
}

int vkt_query_surface_capabilities(
    VktVulkanContext *context,
    VkSurfaceKHR surface,
    VkSurfaceCapabilitiesKHR *capabilities
) {
    VKT_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context->physical_device.vk_device, surface, capabilities));
    return VKT_GENERIC_SUCCESS;
}

